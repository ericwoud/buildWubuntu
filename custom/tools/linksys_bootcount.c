/*
 * mtd - simple memory technology device manipulation tool
 *
 * Copyright (C) 2005      Waldemar Brodkorb <wbx@dass-it.de>,
 * Copyright (C) 2005-2009 Felix Fietkau <nbd@nbd.name>
 *
 * Linksys boot counter reset code for mtd
 *
 * Copyright (C) 2013 Jonas Gorski <jogo@openwrt.org>
 *
 * Code edited so Linksys boot counter reset code an be used seperately.
 * Original versions from lede project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License v2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */
#define _GNU_SOURCE
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <endian.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>

#define BOOTCOUNT_MAGIC	0x20110811

struct bootcounter {
	uint32_t magic;
	uint32_t count;
	uint32_t checksum;
};

static char page[2048];
int mtdsize = 0;
int erasesize = 0;
int mtdtype = 0;

int mtd_open(const char *mtd, bool block)
{
	FILE *fp;
	char dev[PATH_MAX];
	int i;
	int ret;
	int flags = O_RDWR | O_SYNC;
	char name[PATH_MAX];
	snprintf(name, sizeof(name), "\"%s\"", mtd);
	if ((fp = fopen("/proc/mtd", "r"))) {
		while (fgets(dev, sizeof(dev), fp)) {
			if (sscanf(dev, "mtd%d:", &i) && strstr(dev, name)) {
				snprintf(dev, sizeof(dev), "/dev/mtd%s/%d", (block ? "block" : ""), i);
				if ((ret=open(dev, flags))<0) {
					snprintf(dev, sizeof(dev), "/dev/mtd%s%d", (block ? "block" : ""), i);
					ret=open(dev, flags);
				}
				fclose(fp);
				return ret;
			}
		}
		fclose(fp);
	}
	return open(mtd, flags);
}

int mtd_check_open(const char *mtd)
{
	struct mtd_info_user mtdInfo;
	int fd;
	fd = mtd_open(mtd, false);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		return -1;
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		close(fd);
		return -1;
	}
	mtdsize = mtdInfo.size;
	erasesize = mtdInfo.erasesize;
	mtdtype = mtdInfo.type;
	return fd;
}

int mtd_resetbc(int new_count)
{
	struct mtd_info_user mtd_info;
	struct bootcounter *curr = (struct bootcounter *)page;
	unsigned int i;
	int last_count = 0;
	int num_bc;
	int fd;
	int ret;
	fd = mtd_check_open("s_env");
	if (ioctl(fd, MEMGETINFO, &mtd_info) < 0) {
		fprintf(stderr, "failed to get mtd info!\n");
		return -1;
	}
	num_bc = mtd_info.size / mtd_info.writesize;
	for (i = 0; i < num_bc; i++) {
		pread(fd, curr, sizeof(*curr), i * mtd_info.writesize);
		if (curr->magic != BOOTCOUNT_MAGIC && curr->magic != 0xffffffff) {
			fprintf(stderr, "unexpected magic %08x, bailing out\n", curr->magic);
			goto out;
		}
		if (curr->magic == 0xffffffff)
			break;
		last_count = curr->count;
	}
	if (new_count < 0) {
		fprintf(stdout, "%i\n", last_count);
		goto out;
	}
	if (last_count == new_count)
		goto out;
	if (i == num_bc) {
		struct erase_info_user erase_info;
		erase_info.start = 0;
		erase_info.length = mtd_info.size;
		ret = ioctl(fd, MEMERASE, &erase_info);
		if (ret < 0) {
			fprintf(stderr, "failed to erase block: %i\n", ret);
			return -1;
		}
		i = 0;
	}
	memset(curr, 0xff, mtd_info.writesize);
	curr->magic = BOOTCOUNT_MAGIC;
	curr->count = new_count;
	curr->checksum = BOOTCOUNT_MAGIC;
	ret = pwrite(fd, curr, mtd_info.writesize, i * mtd_info.writesize);
	if (ret < 0)
		fprintf(stderr, "failed to write: %i\n", ret);
	sync();
out:
	close(fd);
	return 0;
}


static void usage(void)
{
	fprintf(stderr, "Usage: <command>\n"
	"linksys_bootcount recognizes these commands:\n"
	"        resetbc        reset the uboot boot counter\n"
	"        printbc        print the uboot boot counter\n");
	exit(1);
}

int main (int argc, char **argv)
{
	if (argc < 2)
		usage();
	if ((strcmp(argv[1], "printbc") == 0) && (argc == 2)) {
		mtd_resetbc(-1);
	} else if ((strcmp(argv[1], "resetbc") == 0) && (argc == 2)) {
		mtd_resetbc(0);
	} else {
		usage();
	}
	return 0;
}

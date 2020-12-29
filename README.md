# buildWubuntu

Install a minimal Ubuntu on WRT1900ACS or WRT3200ACM from scratch. 

USE AT YOUR OWN RISC!!!

## Getting Started

You need:

  - WRT1900ACS or WRT3200ACM
  - USB Stick

### Prerequisites

Format the USB stick as EXT4 and label it WUBUNTU. With this label it will be recognized by the script. Make sure it is inserted when you start building.

### Installing


Clone from Git

```
git clone https://github.com/ericwoud/buildWubuntu.git
```

Change directory

```
cd buildWubuntu
```

When connecting multiple disks, we need to boot using the PARTUUID, otherwise the firmware does not know from which disk to boot. Change the ID number of the usb stick to the predefined number, using the following command. Skipping this command, the script will use the random number the stick already has. The number will be hard-coded into the firmware. If you do not want to use the PARTUUID number at all then delete the bash.rootpartuuid file.

```
./build -uU
```

Now build the root filesystem and kernel.

```
./build
```

The firmware file we need to flash can now also be found in the buildWubuntu directory named uImageDtb.bin. For the first time, preferably, flash it from the original firmware so auto_recovery is enabled. WUBUNTU will reset the bootcounter after a succesfull boot. If not succesfull, try booting 3 times and after that you will boot from the original firmware again.
Once running succesfully, one can flash the firmware from the ssh terminal with the /root/buildWubuntu/custom/tools/firmware_write script.


## Deployment

After flashing the firmware, insert the usb stick, connect the WRT with a LAN port to your network and boot the WRT.

To connect to WUBUNTU, password admin:

```
ssh root@192.168.5.1
```

Wifi is enabled, so this should also work. See files in /etc/hostapd/ for details.

IPforward is on, the system is setup as router. Also see https://github.com/ericwoud/bridgefdbd to make AP work nicely.

After this, you are on your own. It is supposed to be a minimal installation of Ubuntu.

When you need to build in-tree/out-of-tree kernel modules, first execute the following on the WRT:

```
./build -akp
```
It helps set up the build scripts correctly (as armhf executable instead of x86 executable).

## Setup as Access Point

When using a second or third Wubuntu as Access Point, and connecting router-lan to AP-lan, do the following: 

Setup the lan ports which connect router and AP as lan-trunk port on both router and AP. 

On the AP, remove vet3/eth3, disable IpForwarding (br0.network), disable isc-dhcp-server. Add the gateway address on the bridge with the address of the router. Change ip address of br0, stay in the same subnet. 

Most important: use a fix like [FDB Deamon](https://github.com/ericwoud/bridgefdbd) or [Mc Spoof](https://github.com/ericwoud/mcspoof). This is necessairy because there is a problem in the DSA driver.


## TODO:

* Test all new changes in build from scratch
* Check building on wrt
* Test 802.11i 802.11k 802.11r 802.11v

## Latest updates (29-Dec-2020)

* Kernel v5.10 lts
* Guest WIFI
* 802.11i 802.11k 802.11r 802.11v
* Using veth device instead of vlan devices. Now hostapd authentication works in combination with a vlan aware bridge. When attaching br0.2 and br0.3 to br0, hostapd cannot communicate over br0 anymore.
* Patch cpu port. After applying this patch, any vlan group within 2000-2020 will not also be added to the dsa cpu port. It does not seem to be necessary to apply this patch, but traffic does not need to go the cpu port when forwarding traffic through eth0 <-> aux <-> wan.

## Updates (5-Dec-2020)

* Use DSA port 6 and call it "aux".
* Setup VLAN.
* Setup as router.
* Setup dhcpd.
* Setup WAN connection on eth0. Now outbound traffic goes through et0-aux-wan ports, as they should.


## Updates (1-Nov-2020)

* Ubuntu Focal
* Kernel V5.4.69
* Much faster startup due to changing from ifupdown (/etc/network/interfaces)
  to systemd-networkd.service (/etc/systemd/network).
* Option to choose different cross compiler version by specifying tarball to download.
* Add 8 digit md5 code to firmware.bin to easily check if the contents changed. 
  This helps to determine if you need to flash a new firmware.
* Improved firmware_write script.
* Uses diffdef_config to easily change to newer kernel.


## Features

* Files copied from custom kernel and rootfs directory.
* Enable custom services installed from custom/rootfs/etc/systemd/system
* Optional scripts in the custom kernel and rootfs directory. Files starting with "bash." 
  Environment and variables from main script can be used.
* Optional patches in the custom kernel and rootfs directory. Files starting with "patch."
* Change MBR ID, usefull when boot using PARTUUID. Use ./build -uU
* Creation of archives to save time next time building. Use -t to create -T to delete.
* Install necessairy packages. Use -a
* Build RootFS. Use -r
* Build Kernel. Use -k
* Deletion of RootFS. Use -R
* Deletion of Kernel. Use -K
* Building on USB Stick. Use -u
* Make modules_prepare omly. Use -p together with -k
* Default options when no options entered -r -k -a -t -u
* build script can run on WRT also, to compile a new kernel.
* Adding extra packages to install. See extrapackages= at top of build script.
* Other variables to tweak also at top of build script. Try building a different release or kernel version.
* Adding kernel options from custom script (see example T230C2)

Custom kernel directory:

* Support for network and wifi
* Cake support, not tested
* MyGica T230C2 DVB-T and DVB-C support

Custom rootfs directory:

* Support for network and wifi


## Acknowledgments

* [McDebian](https://github.com/Chadster766/McDebian)
* [UbuntuWRT](https://github.com/cilix-lab/ubuntu-wrt)
* [CrazyCat Media Build](https://github.com/crazycat69/linux_media)
* [MwlWifi](https://github.com/kaloz/mwlwifi)
* [Cake](https://github.com/dtaht/sch_cake)



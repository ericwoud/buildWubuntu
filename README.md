# buildWubuntu

Install a minimal Ubuntu on WRT1900ACS or WRT3200ACM from scratch. 

USE AT YOUR OWN RISK!!!

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
ssh root@192.168.1.2
```

Wifi is enabled, so this should also work. See files in /etc/hostapd/ for details.

IPforward is off, the system is setup as AP. Also see https://github.com/ericwoud/bridgefdbd to make AP work nicely.

After this, you are on your own. It is supposed to be a minimal installation of Ubuntu.


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
* Default options when no options entered -r -k -a -t -u
* build script can run on WRT also, to compile a new kernel.
* Adding extra packages to install. See extrapackages= at top of build script.
* Other variables to tweak also at top of build script. Try building a different release or kernel version.
* Adding kernel options from custom script (see example T230C2)

Custom kernel directory:

* Support for network and wifi
* Cake support, not tested
* MyGica T230C2 DVB-T and DVB-C support, not fully tested yet

Custom rootfs directory:

* Support for network and wifi


## Acknowledgments

* [McDebian](https://github.com/Chadster766/McDebian)
* [UbuntuWRT](https://github.com/cilix-lab/ubuntu-wrt)
* [CrazyCat Media Build](https://github.com/crazycat69/linux_media)
* [MwlWifi](https://github.com/kaloz/mwlwifi)
* [Cake](https://github.com/dtaht/sch_cake)



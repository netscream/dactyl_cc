#!/bin/sh
make clean

rm -rf keyboards/dactyl_cc/
cp -r /mnt/c/Users/hlynu/Documents/GitHub/qmk_firmware/keyboards/dactyl_cc/ keyboards/ 
rm -rf lib/chibios-contrib/os/hal/ports/MIMXRT1062/LLD/I2Cv1/ 
cp -r /mnt/c/Users/hlynu/Documents/GitHub/qmk_firmware/keyboards/dactyl_cc/teensy40/I2Cv1/ lib/chibios-contrib/os/hal/ports/MIMXRT1062/LLD/
cp -r keyboards/dactyl_cc/teensy40/MIMXRT1062/platform.mk lib/chibios-contrib/os/hal/ports/MIMXRT1062/MIMXRT1062/platform.mk

make -j 16 dactyl_cc:netscream

cp -r .build/dactyl_cc_teensy40_netscream.hex /mnt/c/Users/hlynu/Desktop/ 

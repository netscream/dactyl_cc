#!/bin/sh

KEYMAP=netscream

pr () {
	printf "##### ${1} #####\n"
}

QMK () {
	pr "Fetching QMK from git"
	git clone https://github.com/qmk/qmk_firmware
}

QMK_SUBMODULE () {
	pr "Fetching submodules"
	make git-submodule	
}

QMK_DEV () {
	cd qmk_firmware
	IS_DEV=`git branch | grep -io "develop"`

	if [ ! "${IS_DEV}" = "develop" ]
	then
		pr "Changing QMK to 'develop' branch"
		git checkout develop
		QMK_SUBMODULE
	else
		pr "QMK is in develop branch"
	fi

	cd ../
}

COPY_KEYBOARD () {
	if [ ! -d "qmk_firmware/keyboards/dactyl_cc" ]
	then
		pr "Copying dactyl_cc to keyboards"
		cp -r keyboards/dactyl_cc qmk_firmware/keyboards
	else
		pr "Dactyl cc exists"
	fi
	if [ ! -d "qmk_firmware/lib/chibios-contrib/os/hal/ports/MIMXRT1062/LLD/I2Cv1" ]
	then
		pr "Copying I2C to MIMXRT1062 directory"
		cp -r keyboards/dactyl_cc/teensy40/I2Cv1 qmk_firmware/lib/chibios-contrib/os/hal/ports/MIMXRT1062/LLD/
	else
		pr "I2C driver exists"
	fi

	grep -q "I2Cv1" "qmk_firmware/lib/chibios-contrib/os/hal/ports/MIMXRT1062/MIMXRT1062/platform.mk" 
	if 	[ ! $? -eq 0 ]
	then
		pr "Copying the platform makefile for I2C building"
		cp -r keyboards/dactyl_cc/teensy40/MIMXRT1062/platform.mk qmk_firmware/lib/chibios-contrib/os/hal/ports/MIMXRT1062/MIMXRT1062/platform.mk
	else
		pr "Platform makefile contains I2Cv1"
	fi
}

BUILD_DACTYL () {
	pr "Building dactyl with keymapping \"${KEYMAP}\"" 
	cd qmk_firmware
	make clean
	make -j 16 dactyl_cc:$KEYMAP
	cd ../
}

COPY_TO_ROOT () {
	cp qmk_firmware/.build/dactyl_cc_teensy40_$KEYMAP.hex ./
}
# Main script
if [ ! -d "qmk_firmware" ] 
then
	QMK 
else
	pr "QMK exists"
fi

QMK_DEV
COPY_KEYBOARD
BUILD_DACTYL
COPY_TO_ROOT

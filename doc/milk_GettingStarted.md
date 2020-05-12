# Getting Started {#page_milk_GettingStarted}

---

## Basics

### Command line interface (CLI)

milk help is accessible from the command line interface (CLI). Type

	$ milk
	milk > help
	
milk's help should get you started.

To exit the CLI, type

	milk > exitCLI


Examples below show how to use the milk command line interface (CLI) to perform simple operations. 

### Writing scripts

You can write scripts to enter milk, execute milk commands from the CLI and exit. Note that milk follows the standard bash comment syntax starting.

Example script file

~~~
#!/bin/bash

# Enter milk
# Executable name may vary
milk << EOF

# enter milk instructions here
creaim im1 512 512
listim
savefits im1 "!im1.fits"

# exit command line interface
exitCLI

# end of script
EOF

~~~

The examples below contain the CLI commands, which can be entered interactively, or included in a script

---

## Example 1: Creating images

	# create 2D image, 512 x 512 pixels
	# image will be initialized to zero
	creaim im1 512 512
	
	# not sure what creaim command does ? check it
	cmd? creaim
	
	# create a 3D image
	creaim im2 100 100 50
	
	# create a 2D image that is a disk
	# for details, type cmd? mkdisk
	mkdisk imdisk 512 512 256.0 256.0 100.0
	
	# check what images are in local memory
	listim
	
	# save image as FITS file
	savefits im1 "im1.fits"
	
	# save disk image to same file name
	# need to append "!" to overwrite existing file
	savefits imdisk "!im1.fits"
	
	# save compressed image
	savefits imdisk "im1.fits.gz"
	
	# check what has been saved (send command to shell)
	!ls -lh im1*
	
---

## Example 2: Loading image, some basic arithmetic

This example assumes example 1 has create the disk image im1.fits

	# load image as imdisk0
	loadfits im1.fits imdisk0
	
	# create another disk image, larger diameter
	mkdisk imdisk1 512 512 256 256 150
	
	# subtract imdisk0 to imdisk1
	imring=imdisk1-imdisk0

	# check what images are in local memory
	listim
	
	# save to disk as imring.fits (default name)
	savefits imring
	

---


[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/f9c4827075c7447db1880f95e2119353)](https://www.codacy.com/gh/milk-org/milk-package?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=milk-org/milk-package&amp;utm_campaign=Badge_Grade)

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

dev branch: [![Build Status dev](https://travis-ci.org/milk-org/milk-package.svg?branch=dev)](https://travis-ci.org/milk-org/milk-package)

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

master branch: [![Build Status](https://travis-ci.org/milk-org/milk-package.svg?branch=master)](https://travis-ci.org/milk-org/milk-package)


milk package, version 1.01


---

IMPORTANT NOTE: milk uses git submodules. Use `git clone --recursive` (see Downloading and Installing section)

---




## Download and Installing 


### Download

	git clone --recursive https://github.com/milk-org/milk-package milk


### Compile

	cd milk
	mkdir _build; cd _build
	cmake ..
	make


### Install

	
	sudo make install

Will install milk in /usr/local/milk-&lt;version&gt;. Multiple versions of milk can coexist in separate milk-&lt;version&gt; directories. To select the version to be used:

	sudo ln -s /usr/local/milk-<version> /usr/local/milk

	
Add environment variables. Add to .bashrc file or similar :

	export MILK_ROOT=${HOME}/src/milk  # point to source code directory. Edit as needed.
	export MILK_INSTALLDIR=/usr/local/milk
	export PATH=${PATH}:${MILK_INSTALLDIR}/bin


Ensure linker finds milk libraries :

	echo "/usr/local/milk/lib" > milklibs.conf
	sudo mv milklibs.conf /etc/ld.so.conf.d/
	sudo ldconfig -v

Note: Use the above method instead of setting up LD_LIBRARY_PATH environment variable. milk is installed with setuid bit. LD_LIBRARY_PATH is ignored at runtime for executables that have their setuid or setgid bit set. In addition, holding library locations in cache is faster than searching LD_LIBRARY_PATH directories.


### Creating tmpfs disk (optional)

Create tmpfs disk for high performance I/O:

    echo "tmpfs /milk/shm tmpfs rw,nosuid,nodev" | sudo tee -a /etc/fstab
    sudo mkdir -p /milk/shm
    sudo mount /milk/shm



## Documentation

[Online documentation]( http://milk-org.github.io/milk/index.html ) 



## LICENCE


[GNU General Public License v3.0]( https://github.com/milk-org/milk/blob/master/LICENCE.txt )

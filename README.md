---

milk: Modular Image processing Library tooLkit

---




[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)

Latest release: [![latesttag](https://img.shields.io/github/tag/milk-org/milk-package.svg)](https://github.com/milk-org/milk-package/tree/master) 

|     branch       |   version             |  status                     | latest        |
|------------------|-----------------------|-----------------------------|---------------|
**master** | 1.1.0 | [![Build Status](https://travis-ci.org/milk-org/milk-package.svg?branch=master)](https://travis-ci.org/milk-org/milk-package) | ![lastcommit](https://img.shields.io/github/last-commit/milk-org/milk-package/master.svg)
[**dev**](https://github.com/milk-org/milk-package/tree/dev) | 1.1 | [![Build Status dev](https://travis-ci.org/milk-org/milk-package.svg?branch=dev)](https://travis-ci.org/milk-org/milk-package) | ![lastcommit](https://img.shields.io/github/last-commit/milk-org/milk-package/dev.svg)




---

## Download and Installing 


### Download

&#x26A0;
milk uses git submodules. Use `git clone --recursive`

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
	export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:${MILK_INSTALLDIR}/lib/pkgconfig



### Creating tmpfs disk (optional)

Create tmpfs disk for high performance I/O:

    echo "tmpfs /milk/shm tmpfs rw,nosuid,nodev" | sudo tee -a /etc/fstab
    sudo mkdir -p /milk/shm
    sudo mount /milk/shm



## Documentation

[Documentation](https://milk-org.github.io/milk-package/)


## LICENCE


[GNU General Public License v3.0]( https://github.com/milk-org/milk/blob/master/LICENCE.txt )

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)

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

Will install milk in /usr/local/milk-<version>. Multiple versions of milk can coexist in separate milk-<version> directories. To select the version to be used:

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




## Documentation

[Online documentation]( http://milk-org.github.io/milk/index.html ) 



## LICENCE


[GNU General Public License v3.0]( https://github.com/milk-org/milk/blob/master/LICENCE.txt )

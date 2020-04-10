[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)

milk package, version 0


---

IMPORTANT NOTE: milk uses git submodules. Use `git clone --recursive` (see Downloading and Installing section)

---




## Downloading and installing 


The MILK package follows the standard git clone steps and GNU build process :

	git clone https://github.com/milk-org/milk
	cd milk
	git submodule init
	git submodule update
	autoreconf -i
	./configure
	make
	make install

Note: On OS X you need to use gcc-mp-5 for openMP:

	./configure "CC=/opt/local/bin/gcc-mp-5" CPPFLAGS="-I/usr/include/malloc/ -I/opt/local/include/readline" LDFLAGS="-L/opt/local/lib/"
(Replace "/opt/local/" is the location of your installed libraries. )



## Reporting bugs, issues

Report bugs and issues on [this page]( https://github.com/milk-org/milk/issues )


## Contributing to project


See [coding standards]( http://milk-org.github.io/milk/page_coding_standards.html ) 





## Documentation

[Online documentation]( http://milk-org.github.io/milk/index.html ) 



## LICENCE


[GNU General Public License v3.0]( https://github.com/milk-org/milk/blob/master/LICENCE.txt )

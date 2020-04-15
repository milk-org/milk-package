#include "Config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sched.h>
#include <omp.h>
#include <assert.h>
#include <pthread.h>
#include <CommandLineInterface/CLIcore.h>


#include <image_basic/image_basic.h>
#include <img_reduce/img_reduce.h>
#include <linopt_imtools/linopt_imtools.h>
#include <cudacomp/cudacomp.h>



#include <image_format/image_format.h>
#include <psf/psf.h>
#include <linARfilterPred/linARfilterPred.h>
#include <ZernikePolyn/ZernikePolyn.h>

#include <fft/fft.h>


#define STYLE_BOLD    "\033[1m"
#define STYLE_NO_BOLD "\033[22m"


DATA __attribute__((used)) data;





int main(
    int argc,
    char *argv[]
)
{
    char *AppName = "milk";

    printf(STYLE_BOLD);
    printf("\n        Modular Image processing toolKit (milk)\n");
#ifndef NDEBUG
    printf("        === DEBUG MODE : assert()         enabled ==========\n");
    printf("        === DEBUG MODE : DEBUG_TRACEPOINT enabled ==========\n");
#endif
    printf(STYLE_NO_BOLD);


    strcpy(data.package_name, PACKAGE_NAME);

    char versionstring[200];
    sprintf(versionstring, "%d.%d.%02d%s",
            VERSION_MAJOR, VERSION_MINOR,
            VERSION_PATCH, VERSION_OPTION);
    strcpy(data.package_version, versionstring);
    strcpy(data.sourcedir, SOURCEDIR);
    strcpy(data.configdir, CONFIGDIR);



    printf("\n");
    printf("        %s version %s\n", data.package_name, data.package_version);
#ifdef IMAGESTRUCT_VERSION
    printf("        Using ImageStreamIO version %s\n", IMAGESTRUCT_VERSION);
#endif
    printf("        GNU General Public License v3.0\n");
    printf("        Report bugs to : %s\n", PACKAGE_BUGREPORT);
    printf("        Type \"help\" for instructions\n");
    printf("        \n");

    // initialize milk modules for which no function calls is included by default
	libinit_fft();

   
    libinit_image_basic();
    libinit_img_reduce();
    libinit_linopt_imtools();
    libinit_cudacomp();



    libinit_image_format();
    libinit_psf();
    libinit_linARfilterPred();
    libinit_ZernikePolyn();



    runCLI(argc, argv, AppName);

    printf("NORMAL EXIT\n");


    // clean-up calling thread
    //pthread_exit(NULL);

    return RETURN_SUCCESS;
}

/**
 * @file    simplefunc_FPS.c
 * @brief   simple function example with FPS and processinfo support
 *
 * Demonstrates using FPS to hold function arguments and parameters.
 * See script milk-test-simplefuncFPS for example usage.
 */

#include "CommandLineInterface/CLIcore.h"


// variables local to this translation unit
static char *inimname;
static double *scoeff;


// CLI function arguments and parameters
static CLICMDARGDEF farg[] =
{
    {
        CLIARG_IMG, ".in_name", "input image", "im1",
        CLICMDARG_FLAG_DEFAULT, FPTYPE_AUTO, FPFLAG_DEFAULT_INPUT,
        (void **) &inimname
    },
    {
        CLIARG_FLOAT, ".scaling", "scaling coefficient", "1.0",
        CLICMDARG_FLAG_NOCLI, FPTYPE_AUTO, FPFLAG_DEFAULT_INPUT,
        (void **) &scoeff
    }
};


// binding between variables and function args/params
static errno_t variables_link()
{
    for(int i = 0; i < (int) (sizeof(farg) / sizeof(CLICMDARGDEF)); i++)
    {
        void *ptr = get_farg_ptr(farg[i].fpstag);
        *(farg[i].valptr) = ptr;
    }
    return RETURN_SUCCESS;
}

// CLI function data
static CLICMDDATA CLIcmddata =
{
    "simplefuncFPS",                              // keyword (CLI call name)
    "compute total of image using FPS",           // short description
    __FILE__,
    sizeof(farg) / sizeof(CLICMDARGDEF), farg,
    CLICMDFLAG_FPS | CLICMDFLAG_PROCINFO,         // supported capabilities
    NULL                                          // pointer to command settings (will be assigned when function is registered)
};






// Computation code
// Can be made non-static and exported to other translation units
static errno_t example_compute_2Dimage_total(
    IMGID img,
    double scalingcoeff
)
{
    resolveIMGID(&img, ERRMODE_ABORT);

    uint_fast32_t xsize = img.md->size[0];
    uint_fast32_t ysize = img.md->size[1];
    uint_fast64_t xysize = xsize * ysize;

    double total = 0.0;
    for(uint_fast64_t ii = 0; ii < xysize; ii++)
    {
        total += img.im->array.F[ii];
    }
    total *= scalingcoeff;

    printf("image total = %lf, scaling coeff %lf\n", total, scalingcoeff);

    return RETURN_SUCCESS;
}


// Wrapper function, used by all CLI calls
// Defines how local variables are fed to computation code
static errno_t compute_function()
{
    INSERT_STD_PROCINFO_COMPUTEFUNC_START

    example_compute_2Dimage_total(
        makeIMGID(inimname),
        *scoeff
    );

    INSERT_STD_PROCINFO_COMPUTEFUNC_END

    return RETURN_SUCCESS;
}




INSERT_STD_FPSCLIfunctions

// Register function in CLI
errno_t FPSCLIADDCMD_milk_module_example__simplefunc_FPS()
{
    int cmdi = RegisterCLIcmd(CLIcmddata, FPSCLIfunction);
    CLIcmddata.cmdsettings = &data.cmd[cmdi].cmdsettings;

    // Optional custom settings for this function
    // CLIcmddata.cmdsettings->procinfo_loopcntMax = 9;

    return RETURN_SUCCESS;
}

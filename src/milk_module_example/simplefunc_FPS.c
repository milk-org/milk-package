/**
 * @file    simplefunc_FPS.c
 * @brief   simple function example with FPS and processinfo support
 *
 * Demonstrates using FPS to hold function arguments and parameters.
 * See script milk-test-simplefuncFPS for example usage.
 */

#include "CommandLineInterface/CLIcore.h"

// CLI function arguments and parameters
static CLICMDARGDEF farg[] =
{
    {
        CLIARG_IMG, ".in_name", "input image", "im1",
        CLICMDARG_FLAG_DEFAULT, FPTYPE_AUTO, FPFLAG_DEFAULT_INPUT
    },
    {
        CLIARG_FLOAT, ".scaling", "scaling coefficient", "1.0",
        CLICMDARG_FLAG_NOCLI, FPTYPE_AUTO, FPFLAG_DEFAULT_INPUT
    }
};

static char *inimname;
static double *scoeff;

static errno_t variables_link()
{
    inimname = get_farg_ptr(".in_name");
    scoeff   = get_farg_ptr(".scaling");

    return RETURN_SUCCESS;
}

static CLICMDDATA CLIcmddata =
{
    "simplefuncFPS",
    "compute total of image using FPS",
    __FILE__, sizeof(farg) / sizeof(CLICMDARGDEF), farg,
    CLICMDFLAG_FPS|CLICMDFLAG_PROCINFO
};







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









INSERT_STD_FPSCONFfunction
INSERT_STD_FPSRUNfunction
INSERT_STD_FPSCLIfunction

errno_t FPSCLIADDCMD_milk_module_example__simplefunc_FPS()
{
    RegisterCLIcmd(CLIcmddata, FPSCLIfunction);

    return RETURN_SUCCESS;
}

/**
 * @file    simplefunc.c
 * @brief   simple function example
 *
 * Demonstrates how functions are registered and their arguments processed.
 * See script milk-test-simplefunc for example usage.
 *
 */

#include "CommandLineInterface/CLIcore.h"

// List of arguments to function
// { CLItype, tag, description, initial value, flag, fptype, fpflag}
//
// A function variable is named by a tag, which is a hierarchical
// series of words separated by dot "."
// For example: .input.xsize (note that first dot is optional)
//
static CLICMDARGDEF farg[] =
{
    {
        CLIARG_IMG, ".in_name", "input image", "im1",
        CLICMDARG_FLAG_DEFAULT, FPTYPE_AUTO, FPFLAG_DEFAULT_INPUT,
        NULL
    },
    {
        // argument is not part of CLI call, FPFLAG ignored
        CLIARG_FLOAT, ".scaling", "scaling coefficient", "1.0",
        CLICMDARG_FLAG_NOCLI, FPTYPE_AUTO, FPFLAG_DEFAULT_INPUT,
        NULL
    }
};

// Local variables pointers
// Within this translation unit, these point to the variables values
static char *inimname;
static double *scoeff;





static CLICMDDATA CLIcmddata =
{
    "simplefunc",             // keyword to call function in CLI
    "compute total of image", // description of what the function does
    __FILE__, sizeof(farg) / sizeof(CLICMDARGDEF), farg,
    0,
    NULL
};








/** @brief Compute function code
 *
 * Can be made non-static and called from outside this translation unit(TU)
 * Minimizes use of variables local to this TU.
 */
static errno_t example_compute_2Dimage_total(
    IMGID img,
    double scalingcoeff)
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


/** @brief Compute function call
 * Links local variables to function parameters
 */
static errno_t compute_function()
{
    example_compute_2Dimage_total(
        makeIMGID(inimname),
        *scoeff
    );
    return RETURN_SUCCESS;
}





INSERT_STD_CLIfunction


/** @brief Register CLI command
 *
 * Adds function to list of CLI commands.
 * Called by main module initialization function init_module_CLI().
 */
errno_t FPSCLIADDCMD_milk_module_example__simplefunc()
{
    RegisterCLIcmd(CLIcmddata, CLIfunction);

    return RETURN_SUCCESS;
}

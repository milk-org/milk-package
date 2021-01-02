/**
 * @file    simplefunc.c
 * @brief   simple function example
 * 
 * Demonstrates how functions are registered and their arguments processed.
 * 
 * Display function info :
 * > cmd? modex.simplefunc
 * 
 * run function :
 * > modex.simplefunc im1
 * 
 * Change parameters :
 * > modex.simplefunc .in_name im2
 * > modex.simplefunc .scaling 0.3
 * 
 * Run function with pre-set parameters :
 * > modex.simplefunc .
 * 
 */

#include "CommandLineInterface/CLIcore.h"




// List of arguments to function
// { type, tag, description, pre-set default value, flag }
//
static CLICMDARGDEF farg[] =
{
    { CLIARG_IMG,  ".in_name",   "input image", "im1" , CLICMDARG_FLAG_DEFAULT },
    { CLIARG_FLOAT, ".scaling", "scaling coefficient", "1.0", CLICMDARG_FLAG_NOCLI } // argument is not part of CLI call
};

static CLICMDDATA CLIcmddata = {
    "simplefunc",             // keyword to call function in CLI
    "compute total of image", // description of what the function does
    __FILE__, sizeof(farg)/sizeof(CLICMDARGDEF), farg
};




/** @brief Compute function
 */
static errno_t compute_2Dimage_total(
    IMGID img,
    double scalingcoeff
)
{
	if(resolveIMGID(&img) < 0)
	{
		printf("Missing input to function %s\n", __FUNCTION__);
		return RETURN_FAILURE;
	}
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



/** @brief Function call wrapper
 *
 * CLI argument(s) is(are) parsed and checked with CLI_checkarray(), then
 * passed to the compute function call.
 *
 * Custom code may be added for more complex processing of function arguments.
 */
static errno_t CLIfunction(void)
{
    if(CLI_checkarg_array(farg, CLIcmddata.nbarg) == RETURN_SUCCESS)
    { // if CLI call arguments check out, go ahead with computation
        
        // arguments not contained in CLI call line are extracted from the 
        // command argument list
        double scalingcoeff = data.cmd[data.cmdindex].argdata[1].val.f;
        
        compute_2Dimage_total(
            makeIMGID(data.cmdargtoken[1].val.string),
            scalingcoeff
        );

        return RETURN_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}




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

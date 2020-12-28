/**
 * @file    simplefunc.c
 * @brief   simple function example
 * 
 * Demonstrates how functions are registered and their arguments processed.
 */

#include "CommandLineInterface/CLIcore.h"

// required for image_ID
#include "COREMOD_memory/COREMOD_memory.h"


// List of arguments to function
// { type, tag, description, example/default value, flag }
//
static CLICMDARGDEF farg[] =
{
    { CLIARG_IMG,  ".in_name",   "input image", "im1" , CLICMDARG_FLAG_DEFAULT }
};

static CLICMDDATA CLIcmddata = {
    "simplefunc",             // keyword to call function in CLI
    "compute total of image", // description of what the function does
    __FILE__, sizeof(farg)/sizeof(CLICMDARGDEF), farg
};



/** @brief Compute function
 */
static errno_t compute_2Dimage_total(
    const char *restrict IDname
)
{
    imageID ID;
    uint_fast32_t xsize;
    uint_fast32_t ysize;

    ID = image_ID(IDname);
    xsize = data.image[ID].md[0].size[0];
    ysize = data.image[ID].md[0].size[1];

    double total = 0.0;
    
    uint_fast64_t xysize = xsize * ysize;
    for(uint_fast64_t ii = 0; ii < xysize; ii++)
    {
        total += data.image[ID].array.F[ii];
    }
    
    printf("image total = %lf\n", total);

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
    {
        compute_2Dimage_total(data.cmdargtoken[1].val.string);
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

/**
 * @file    simplefunc_FPS.c
 * @brief   simple function example with FPS support
 *
 * Demonstrates using FPS to hold function arguments and parameters.
 * See script milk-test-simplefuncFPS for example usage.
 */

#include "CommandLineInterface/CLIcore.h"

// CLI function arguments and parameters
static CLICMDARGDEF farg[] =
{
    {
        // arg 0
        CLIARG_IMG, ".in_name", "input image", "im1",
        CLICMDARG_FLAG_DEFAULT, FPTYPE_AUTO, FPFLAG_DEFAULT_INPUT
    },
    {
        // arg 1
        CLIARG_FLOAT, ".scaling", "scaling coefficient", "1.0",
        CLICMDARG_FLAG_NOCLI, FPTYPE_AUTO, FPFLAG_DEFAULT_INPUT
    }
};

static CLICMDDATA CLIcmddata =
{
    "simplefuncFPS",
    "compute total of image using FPS",
    __FILE__, sizeof(farg) / sizeof(CLICMDARGDEF), farg
};

// Local variables pointers
// Within this translation unit, these point to the variables values
static char *inimname;
static double *scoeff;

/** @brief Link local variables to FPS or CLI
 * 
 * TODO 
 * do something
 */
static errno_t variables_link()
{    
    inimname = get_farg_ptr(".in_name");
    scoeff   = get_farg_ptr(".scaling");

    return RETURN_SUCCESS;
}








/** @brief Compute function code
 *
 * Can be called from outside this translation unit(TU)
 * Minimizes use of variables local to this TU.
 */
errno_t example_compute_2Dimage_total(
    IMGID img,
    double scalingcoeff
)
{
    resolveIMGID(&img, ERRMODE_FAIL);

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










/** @brief FPS conf function
 * Sets up the FPS and its parameters.\n
 * Optional parameter checking can be included.\n
 */
static errno_t FPSCONFfunction()
{
    FPS_SETUP_INIT(data.FPS_name, data.FPS_CMDCODE);
    data.fps = &fps;

    /** ### ADD PARAMETERS
    *
    * The function function_parameter_add_entry() is called to add
    * each parameter.
    *
    * macros are provided for convenience, named "FPS_ADDPARAM_...".\n
    * The macros are defined in fps_add_entry.h, and provide a function
    * parameter identifyer variable (int) for each parameter added.
    *
    * parameters for FPS_ADDPARAM macros:
    * - key/variable name
    * - tag name
    * - description
    * - default initial value
    *
    * Equivalent code without using macro :
    *
    *     function_parameter_add_entry(&fps, ".delayus", "Delay [us]", FPTYPE_INT64, FPFLAG_DEFAULT_INPUT|FPFLAG_WRITERUN, NULL);
    */
    CMDargs_to_FPSparams_create(&fps);
    variables_link();
    
    /** ### START CONFLOOP
     *
     * start function parameter conf loop\n
     * macro defined in function_parameter.h
     *
     * Optional code to handle/check parameters is included after this
     * statement
     */
    FPS_CONFLOOP_START

    /** ### STOP CONFLOOP
     *
     * stop function parameter conf loop\n
     * macro defined in function_parameter.h
     */
    data.fps = NULL;
    FPS_CONFLOOP_END


    return RETURN_SUCCESS;
}



/** @brief FPS run function
 */
static errno_t FPSRUNfunction()
{
    /** ### CONNECT TO FPS
     *
     * The FPS name is taken from data.FPS_name, which has to
     * have been set up by either the stand-alone function, or the
     * CLI.
     *
     * Running FPS_CONNECT macro in FPSCONNECT_RUN mode.
     */
    FPS_CONNECT(data.FPS_name, FPSCONNECT_RUN);
    data.fps = &fps;

    /** ### GET FUNCTION PARAMETER VALUES
     *
     * Parameters are addressed by their tag name\n
     * These parameters are read once, before running the loop.\n
     *
     * FPS_GETPARAM... macros are wrapper to functionparameter_GetParamValue
     * and functionparameter_GetParamPtr functions, all defined in
     * fps_paramvalue.h.
     *
     * Each of the FPS_GETPARAM macro creates a variable with "_" prepended
     * to the first macro argument.
     *
     * Equivalent code without using macros:
     *
     *     char _IDin_name[200];
     *     strncpy(_IDin_name,  functionparameter_GetParamPtr_STRING(&fps, ".in_name"), FUNCTION_PARAMETER_STRMAXLEN);
     *     long _delayus = functionparameter_GetParamValue_INT64(&fps, ".delayus");
     */

    variables_link();
        
    compute_function();

    data.fps = NULL;
    function_parameter_RUNexit(&fps);

    return RETURN_SUCCESS;
}





static errno_t FPSCLIfunction(void)
{

    // GET ARGUMENTS AND PARAMETERS

    // Try FPS implementation

    // Set data.fpsname, providing default value as first arg, and set data.FPS_CMDCODE value.
    //    Default FPS name will be used if CLI process has NOT been named.
    //    See code in function_parameter.h for detailed rules.
    function_parameter_getFPSargs_from_CLIfunc(CLIcmddata.key);

    if(data.FPS_CMDCODE != 0)  // use FPS implementation
    {
        // set pointers to CONF and RUN functions
        data.FPS_CONFfunc = FPSCONFfunction;
        data.FPS_RUNfunc  = FPSRUNfunction;
        function_parameter_execFPScmd();
        return RETURN_SUCCESS;
    }
    else
    {
        // Use non-FPS implementation
        if(CLI_checkarg_array(farg, CLIcmddata.nbarg) == RETURN_SUCCESS)
        {
            variables_link();
            compute_function();
        }
        else
        {
            return CLICMD_INVALID_ARG;
        }
    }

    return RETURN_SUCCESS;

}


/** @brief Register CLI command
 */
errno_t FPSCLIADDCMD_milk_module_example__simplefunc_FPS()
{
    RegisterCLIcmd(CLIcmddata, FPSCLIfunction);

    return RETURN_SUCCESS;
}

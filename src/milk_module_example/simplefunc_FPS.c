/**
 * @file    simplefunc_FPS.c
 * @brief   simple function example with FPS support
 *
 * Demonstrates using FPS to hold function arguments and parameters.
 * See script milk-test-simplefuncFPS for usage.
 */

#include "CommandLineInterface/CLIcore.h"


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

static CLICMDDATA CLIcmddata =
{
    "simplefuncFPS",
    "compute total of image using FPS",
    __FILE__, sizeof(farg) / sizeof(CLICMDARGDEF), farg
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





/** @brief FPS conf function
 * Sets up the FPS and its parameters.\n
 * Optional parameter checking can be included.\n
 */
static errno_t FPSCONFfunction()
{
    FPS_SETUP_INIT(data.FPS_name, data.FPS_CMDCODE);
    CMDargs_to_FPSparams_create(&fps);

    FPS_CONFLOOP_START
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
    char *fps_IDin_name = functionparameter_GetParamPtr_STRING(&fps, ".in_name");
    double scalingcoeff = functionparameter_GetParamValue_FLOAT64(&fps,
                          ".scaling");

    compute_2Dimage_total(
        makeIMGID(fps_IDin_name),
        scalingcoeff);

    function_parameter_RUNexit(&fps);

    return RETURN_SUCCESS;
}




static errno_t FPSCLIfunction(void)
{
    // Try FPS implementation

    // Set data.fpsname, providing default value as first arg, and set data.FPS_CMDCODE value.
    //    Default FPS name will be used if CLI process has NOT been named.
    //    See code in function_parameter.h for detailed rules.
    function_parameter_getFPSargs_from_CLIfunc(CLIcmddata.key);

    if(data.FPS_CMDCODE != 0)  // use FPS implementation
    {
        // set pointers to CONF and RUN functions
        data.FPS_CONFfunc = FPSCONFfunction;
        data.FPS_RUNfunc = FPSRUNfunction;
        function_parameter_execFPScmd();
        return RETURN_SUCCESS;
    }

    // otherwise, use non-FPS implementation
    if(CLI_checkarg_array(farg, CLIcmddata.nbarg) == RETURN_SUCCESS)
    {
        // pull parameter value from argdata, as it is not in CLI call
        double scalingcoeff = data.cmd[data.cmdindex].argdata[1].val.f;

        compute_2Dimage_total(
            makeIMGID(data.cmdargtoken[1].val.string),
            scalingcoeff);

        return RETURN_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}


/** @brief Register CLI command
 */
errno_t FPSCLIADDCMD_milk_module_example__simplefunc_FPS()
{
    RegisterCLIcmd(CLIcmddata, FPSCLIfunction);

    return RETURN_SUCCESS;
}

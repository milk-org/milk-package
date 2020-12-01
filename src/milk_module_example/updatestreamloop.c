/**
 * @file    updatestreamloop.c
 * @brief   simple procinfo+fps example
 *
 * A pre-existing stream is updated at regular intervals, using the 
 * DELAY trigger mode.
 * 
 * This is the first example in a series of procinfo/fps examples. 
 * Subsequent example are more complex and demonstrate more powerful 
 * features.
 * 
 * The function parameter structure (FPS) is a structure holding function
 * parameters, and compatible with wide range of convenient tools to
 * for users to interact with functions.
 * 
 * To enable FPS support with command line interface (CLI) wrapper, four 
 * functions are required in a C program:
 * - FPSCONF function to configure function parameters and setup the FPS
 * - FPSRUN function that executes the code
 * - a self-contained execution function
 * - a CLI wrapper function
 * 
 * 
 */


#include "CommandLineInterface/CLIcore.h"

// required for create_2Dimage_ID()
#include "COREMOD_memory/COREMOD_memory.h"

// required for timespec_diff()
#include "COREMOD_tools/COREMOD_tools.h"

// required for timespec_diff
#include "CommandLineInterface/timeutils.h"



#define FPSFUNCNAME        milk_module_example__updatestreamloop
#define CLICMD_SHORTNAME   "streamupdate"
#define FUNCDESCRIPTION    "update stream"



typedef struct
{
	int type;
	char fpstag[100];
	char descr[100];
	char example[100];
} FPSCLIARG;

// Command line interface (CLI) arguments and corresponding function parameter structure (FPS) tag
static FPSCLIARG fpscliarg[2] =
{
//    type,         fpstag,        description,    example
    { CLIARG_IMG,  ".in_name",   "input stream", "ims1" },
    { CLIARG_LONG, ".delayus",     "delay [us]",  "2000" }
};










static errno_t CLIcheckargs()
{
	int nbarg = (int) sizeof(fpscliarg)/sizeof(FPSCLIARG);
	
	printf("Number of CLI args : %d\n", nbarg);
	
	int nberr = 0;
	for(int arg = 0; arg < nbarg; arg++)
	{
		if( CLI_checkarg(arg+1, fpscliarg[arg].type) != 0 )
		{
			nberr ++;
		}
	}
	
	printf("Number of arg error(s): %d\n", nberr);
	
	if(nberr==0)
	{
		return RETURN_SUCCESS;
	}
	else
	{
		return RETURN_FAILURE;
	}
}


static int makeCLIargstring(char *argstring)
{
    char tmpstr[1000];


    int nbarg = (int) sizeof(fpscliarg) / sizeof(FPSCLIARG);
    for(int arg = 0; arg < nbarg; arg++)
    {
        char typestring[100] = "?";

        switch(fpscliarg[arg].type)
        {

            case CLIARG_IMG:
                strcpy(typestring, "string");
                break;

            case CLIARG_LONG:
                strcpy(typestring, "long");
                break;
        }

        if(arg == 0)
        {
            sprintf(tmpstr, "<%s [%s] ->(%s)>", fpscliarg[arg].descr, typestring,
                    fpscliarg[arg].fpstag);
        }
        else
        {
            char tmpstr1[1000];
            sprintf(tmpstr1, " <%s [%s] ->(%s)>", fpscliarg[arg].descr, typestring,
                    fpscliarg[arg].fpstag);
            strcat(tmpstr, tmpstr1);
        }
    }
    strcpy(argstring, tmpstr);

    return strlen(argstring);
}



static int makeCLIcmdexamplestring(char *cmdexstring)
{
    char tmpstr[1000];

    sprintf(tmpstr, "%s", CLICMD_SHORTNAME);

    int nbarg = (int) sizeof(fpscliarg) / sizeof(FPSCLIARG);
    for(int arg = 0; arg < nbarg; arg++)
    {
        char tmpstr1[1000];
        sprintf(tmpstr1, " %s", fpscliarg[arg].example);
        strcat(tmpstr, tmpstr1);
    }
    strcpy(cmdexstring, tmpstr);

    return strlen(cmdexstring);
}



static int CLIargs_to_FPSparams_setval(FUNCTION_PARAMETER_STRUCT *fps)
{
    int NBarg_processed = 0;

    int nbarg = (int) sizeof(fpscliarg) / sizeof(FPSCLIARG);
    for(int arg = 0; arg < nbarg; arg++)
    {
        switch(fpscliarg[arg].type)
        {
            case CLIARG_IMG:
                functionparameter_SetParamValue_STRING(fps, fpscliarg[arg].fpstag,
                                                       data.cmdargtoken[arg + 1].val.string);
                NBarg_processed++;
                break;

            case CLIARG_LONG:
                functionparameter_SetParamValue_INT64(fps, fpscliarg[arg].fpstag,
                                                      data.cmdargtoken[arg + 1].val.numl);
                NBarg_processed++;
                break;

        }

    }

    return NBarg_processed;
}


static int CLIargs_to_FPSparams_create(FUNCTION_PARAMETER_STRUCT *fps)
{
    int NBarg_processed = 0;

    int nbarg = (int) sizeof(fpscliarg) / sizeof(FPSCLIARG);
    for(int arg = 0; arg < nbarg; arg++)
    {
        switch(fpscliarg[arg].type)
        {
            case CLIARG_IMG:
                function_parameter_add_entry(fps, fpscliarg[arg].fpstag, fpscliarg[arg].descr,
                                             FPTYPE_STREAMNAME, FPFLAG_DEFAULT_INPUT_STREAM, "nullim");
                NBarg_processed++;
                break;

            case CLIARG_LONG:
                function_parameter_add_entry(fps, fpscliarg[arg].fpstag, fpscliarg[arg].descr,
                                             FPTYPE_INT64, FPFLAG_DEFAULT_INPUT, NULL);
                NBarg_processed++;
                break;

        }

    }

    return NBarg_processed;
}




// =====================================================================
// Forward declaration(s)
// =====================================================================

static errno_t FPSCONF_FUNCTION_NAME(FPSFUNCNAME)(void);
static errno_t FPSRUN_FUNCTION_NAME(FPSFUNCNAME)(void);
static errno_t FPSFUNCNAME(void);



// =====================================================================
// Command line interface wrapper function(s)
// =====================================================================


static errno_t FPSCLI_FUNCTION_NAME(FPSFUNCNAME)(void)
{
    // Try FPS implementation

    // Set data.fpsname, providing default value as first arg, and set data.FPS_CMDCODE value.
    // Default FPS name will be used if CLI process has NOT been named.
    // See code in function_parameter.c for detailed rules.

    function_parameter_getFPSargs_from_CLIfunc(CLICMD_SHORTNAME);

    if(data.FPS_CMDCODE != 0)   // use FPS implementation
    {
        // set pointers to CONF and RUN functions
        data.FPS_CONFfunc = FPSCONF_FUNCTION_NAME(FPSFUNCNAME);
        data.FPS_RUNfunc  = FPSRUN_FUNCTION_NAME(FPSFUNCNAME);
        function_parameter_execFPScmd();
        return RETURN_SUCCESS;
    }

    // call self-contained execution function - all parameters specified at function launch
    if(CLIcheckargs() == RETURN_SUCCESS)
    {
        FPSFUNCNAME();
        return RETURN_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }
}







// =====================================================================
// Register CLI command(s)
// =====================================================================

//errno_t milk_module_example__updatestreamloop_addCLIcmd()

errno_t FPSCLIADDCMD_FUNCTION_NAME(FPSFUNCNAME)()
{
	char argstring[1000];
	makeCLIargstring(argstring);
	
	char cmdexamplestring[1000];
	makeCLIcmdexamplestring(cmdexamplestring);
    
    RegisterCLIcommand(
        CLICMD_SHORTNAME,
        __FILE__,
        FPSCLI_FUNCTION_NAME(FPSFUNCNAME),
        FUNCDESCRIPTION,
        argstring,
        cmdexamplestring,
        __FUNCTION__);
        
        //"errno_t milk_module_example__updatestreamloop(const char *restrict instreamname, long delayus)");

    return RETURN_SUCCESS;
}





/** @brief FPCONF function for updatestreamloop
 *
 * ## Purpose
 *
 * The FPCONF function sets up the FPS and its parameters
 *
 * ## Details
 *
 */
static errno_t FPSCONF_FUNCTION_NAME(FPSFUNCNAME)()
{
    /** ### INITIALIZE FPS
     */
    FPS_SETUP_INIT(data.FPS_name, data.FPS_CMDCODE);

    /** ### ADD PROCESSINFO STANDARD ENTRIES (optional)
     */
    fps_add_processinfo_entries(&fps);

    /** ### ADD PARAMETERS
     *
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
     *     long fp_delayus = function_parameter_add_entry(&fps, ".delayus", "Delay [us]", FPTYPE_INT64, FPFLAG_DEFAULT_INPUT|FPFLAG_WRITERUN, NULL);
     *     (void) fp_delayus;
     */
    CLIargs_to_FPSparams_create(&fps);
    // CUSTOM SECTION START (READ PARAMETERS)
    //
    long delayus = functionparameter_GetParamValue_INT64(&fps, ".delayus");
    // CUSTOM SECTION END

    /** ### START CONFLOOP
     *
     * start function parameter conf loop\n
     * macro defined in function_parameter.h
     *
     * Optional code to handle/check parameters is included after this
     * statement
     */
    FPS_CONFLOOP_START
    
    // CUSTOM SECTION START (CHECK PARAMETERS) - OPTIONAL
    //
    printf("delayus value before conf loop: %ld\n", delayus);

    //
    // CUSTOM SECTION END

    /** ### STOP CONFLOOP
     *
     * stop function parameter conf loop\n
     * macro defined in function_parameter.h
     */
    FPS_CONFLOOP_END

    return RETURN_SUCCESS;
}





/** @brief Loop process code example
 *
 * ## Purpose
 *
 * Update stream at regular time interval.\n
 * This example demonstrates combined use of processinfo and fps structures.\n
 *
 * ## Details
 *
 */
static errno_t FPSRUN_FUNCTION_NAME(FPSFUNCNAME)()
{
    /** ### CONNECT TO FPS
     *
     * The FPS name is taken from data.FPS_name, which is has to
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
	// CUSTOM SECTION START (GET PARAMETERS VALUES)
	//
    char *IDin_name = functionparameter_GetParamPtr_STRING(&fps, ".in_name");
    long delayus = functionparameter_GetParamValue_INT64(&fps, ".delayus");
	//
	// CUSTOM SECTION END

    /** ### SET UP PROCESSINFO
     * 
     * Equivalent code without using macros:
     *
     *     PROCESSINFO *processinfo;
     * 
     *     char pinfodescr[200];
     *     sprintf(pinfodescr, "streamupdate %.10s", _IDin_name);
     *     processinfo = processinfo_setup(
                      data.FPS_name, // re-use fpsname as processinfo name
                      pinfodescr,    // description
                      "startup",     // message on startup
                      __FUNCTION__, __FILE__, __LINE__
                  );

	*     // OPTIONAL SETTINGS
	*     // Measure timing
	*     // processinfo->MeasureTiming = 1;
	*     // RT_priority, 0-99. Larger number = higher priority. If <0, ignore
	*     // processinfo->RT_priority = 20;
	* 
	*     fps_to_processinfo(&fps, processinfo);
	* 
	* 
	*     int processloopOK = 1;
    */
    FPSPROCINFOLOOP_RUNINIT("streamupdate %.10s", IDin_name);

    /** ### OPTIONAL: TESTING CONDITION FOR LOOP ENTRY
     * 
     * Pre-loop testing, anything that would prevent loop from starting should issue message\n
     * Set processloopOK to 0 if tests fail.
     */
	// INSERT CUSTOM OUT-OF-LOOP SOURCE CODE HERE
	// local variables, setup, tests etc..
	//
	// CUSTOM SECTION START
	//
	imageID IDin = image_ID(IDin_name);
	//
	// CUSTOM SECTION END


    /** ### Specify input trigger
     * 
     * In this example, a simple delay is inserted between trigger\n
     * This is done by calling a macro.\n
     * 
     * Equivalent code without macro:
     * 
     *     processinfo_waitoninputstream_init(processinfo, -1, PROCESSINFO_TRIGGERMODE_DELAY, -1);
     *     processinfo->triggerdelay.tv_sec = 0;
     *     processinfo->triggerdelay.tv_nsec = (long)(_delayus * 1000);
     *     while(processinfo->triggerdelay.tv_nsec > 1000000000)
     *     {
     *         processinfo->triggerdelay.tv_nsec -= 1000000000;
     *         processinfo->triggerdelay.tv_sec += 1;
     *     }
     */
    // CUSTOM SECTION START (TRIGGER MODE)
    //
	PROCINFO_TRIGGER_DELAYUS(delayus);
	//
	// CUSTOM SECTION END

    /** ### START LOOP
     *
     * Notify processinfo that we are entering loop\n
     * Start loop and handle timing and control hooks\n
     * 
     * Equivalent code:
     * 
     *     processinfo_loopstart(processinfo);
     *     while(processloopOK == 1) {
     *         processloopOK = processinfo_loopstep(processinfo);
     *         processinfo_waitoninputstream(processinfo);
     *         processinfo_exec_start(processinfo);
     *         if(processinfo_compute_status(processinfo) == 1) {
     */
    PROCINFOLOOP_START
        
    // CUSTOM SECTION START (COMPUTATION CODE)
    //
    processinfo_update_output_stream(processinfo, IDin);    
    //
    // CUSTOM SECTION END  
    
    /** ### END LOOP
     * 
     * Equivalent non-macro code:
     * 
     *     }
     *     processinfo_exec_end(processinfo);
     *     }
     *     processinfo_cleanExit(processinfo);
     */
    PROCINFOLOOP_END
    
    function_parameter_RUNexit(&fps);

    return RETURN_SUCCESS;
}





/** @brief Self-contained execution function
 *
 * ## Purpose
 *
 * This function is called to perform all required steps :
 * - set up FPS
 * - call RUN function
 *
 * The CONF function loop will not be running.
 *
 *
 * ## Details
 *
 */
static errno_t FPSFUNCNAME()
{
	FUNCTION_PARAMETER_STRUCT fps;
	sprintf(data.FPS_name, "%s-%06ld", CLICMD_SHORTNAME, (long) getpid());
	data.FPS_CMDCODE = FPSCMDCODE_FPSINIT;
	FPSCONF_FUNCTION_NAME(FPSFUNCNAME)();
	function_parameter_struct_connect(data.FPS_name, &fps, FPSCONNECT_SIMPLE);
		
	/** ### WRITE FUNCTION PARAMETERS TO FPS 
	 */
	CLIargs_to_FPSparams_setval(&fps);
    function_parameter_struct_disconnect(&fps);

    /** EXECUTE RUN FUNCTION
     */
    FPSRUN_FUNCTION_NAME(FPSFUNCNAME)();


    return RETURN_SUCCESS;

}

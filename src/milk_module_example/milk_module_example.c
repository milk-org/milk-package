/**
 * @file    milk_module_example.c
 * @brief   example milk module
 *
 * milk module source code template\n
 * Demonstates how to add modules to milk and connect functions.\n
 * 
 * 
 * To compile with module :
 * > cd _build
 * > cmake .. -DEXTRAMODULES="milk_module_example" -DINSTALLMAKEDEFAULT=ON
 * 
 * 
 * To load, type "mload milkmilkmoduleexample" in CLI\n
 *
 * 
 *  File list :
 * - milk_module_example.c  : module main C file, includes binding code to milk
 * - milk_module_example.h  : function prototypes to be included by other modules
 * - create_example_image.c : source code, .c file
 * - create_example_image.h : source code, .h file
 * - CMakeLists.txt         : cmake input file for module
 */


#define _GNU_SOURCE

/* ================================================================== */
/* ================================================================== */
/*  MODULE INFO                                                       */
/* ================================================================== */
/* ================================================================== */

// module default short name
// all CLI calls to this module functions will be <shortname>.<funcname>
// if set to "", then calls use <funcname>
#define MODULE_SHORTNAME_DEFAULT "modex"

// Module short description
#define MODULE_DESCRIPTION       "Example module: template for creating new modules"






/* ================================================================== */
/* ================================================================== */
/*  HEADER FILES                                                      */
/* ================================================================== */
/* ================================================================== */


#include "CommandLineInterface/CLIcore.h"



//
// Forward declarations are required to connect CLI calls to functions
// If functions are in separate .c files, include here the corresponding .h files
//
#include "create_example_image.h"
#include "stream_process_loop_simple.h"






/* ================================================================== */
/* ================================================================== */
/*  INITIALIZE LIBRARY                                                */
/* ================================================================== */
/* ================================================================== */

// Module initialization macro in CLIcore.h
// macro argument defines module name for bindings
//
INIT_MODULE_LIB(milk_module_example)



/**
 * @brief Initialize module CLI
 *
 * CLI entries are registered: CLI call names are connected to CLI functions.\n
 * Any other initialization is performed\n
 *
 */
static errno_t init_module_CLI()
{

    //CLI_CMD_CONNECT("func1", "create_image_with_value");

	create_example_image_addCLIcmd();
	stream_process_loop_simple_addCLIcmd;

    // optional: add atexit functions here

    return RETURN_SUCCESS;
}









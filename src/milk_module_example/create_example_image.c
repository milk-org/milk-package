/**
 * @file    create_example_image.c
 * @brief   example : create image
 *
 *
 */


#include "CommandLineInterface/CLIcore.h"

// required for create_2Dimage_ID
#include "COREMOD_memory/COREMOD_memory.h"






// By convention, function name starts with <modulename>__
//
errno_t milk_module_example__create_image_with_value(
    char *imname,
    double value
)
{
    uint32_t xsize = 128; // by convention, pixel index variables are uint32_t type
    uint32_t ysize = 256; 
    uint64_t xysize = xsize * ysize; // overall image size is, by convention, uint64_t type

    // create 2D image
    // store image index in variable ID
    // by default, the image will be single precision floating point, so 
    // it is accessed as array.F
    imageID ID = create_2Dimage_ID(imname, xsize, ysize);

	// set each pixel to value
    for(uint64_t ii = 0; ii < xysize; ii++)
    {		
        data.image[ID].array.F[ii] = value;
    }

    return RETURN_SUCCESS;
}






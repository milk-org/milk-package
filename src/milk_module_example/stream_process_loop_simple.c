/**
 * @file    stream_process_loop_simple.c
 * @brief   template for simple stream processing loop 
 *
 *
 */


#include "CommandLineInterface/CLIcore.h"

// required for create_2Dimage_ID
#include "COREMOD_memory/COREMOD_memory.h"




/**
 * @brief simple stream processing
 *
 * Requires that streamA and streamB are already loaded in memory\n
 *
 *
 */
errno_t milk_module_example__stream_process_loop_simple(
    char *streamA_name,
    char *streamB_name,
    long loopNBiter
)
{
    int semtrig = 2;  // semaphore index for triggering processing

    // collect image identifiers
    // note: imageID type is long
    imageID streamA_ID = image_ID(streamA_name);
    imageID streamB_ID = image_ID(streamB_name);


    // check that streams are in memory
    //
    if(streamA_ID == -1)
    {
        PRINT_ERROR("Stream \"%s\" not found in memory: cannot proceed", streamA_name);
        exit(0);
    }

    if(streamB_ID == -1)
    {
        PRINT_ERROR("Stream \"%s\" not found in memory: cannot proceed", streamB_name);
        exit(0);
    }

	printf("Running stream loop, %ld iterations\n", loopNBiter);
	printf("ID : %ld -> %ld \n", streamA_ID, streamB_ID);
	
    for(long iter = 0; iter < loopNBiter; iter++)
    {
		
        // Wait for semaphore # semtrig of stream A
        // The call will block until semaphore is > 0, and
        // then decrement it and proceed
        //
        sem_wait(data.image[streamA_ID].semptr[semtrig]);


        // set write flag to one to inform other processes that the stream is being written
        data.image[streamB_ID].md[0].write = 1;

        // processing and computations can be inserted here to update stream B



        // increment image counter
        data.image[streamB_ID].md[0].cnt0++;
        // post all semaphores in output stream
        COREMOD_MEMORY_image_set_sempost_byID(streamB_ID, -1);
        // set write flag to zero
        data.image[streamB_ID].md[0].write = 0;
    }


    return RETURN_SUCCESS;
}






#include "pt.h"
#include "optiga/optiga_util.h"

static volatile optiga_lib_status_t optiga_lib_status;
static void optiga_lib_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
}

int8_t example_util_update_count(struct pt *pt)
{
	static optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
	static optiga_util_t * me_util = NULL;

	static const uint8_t initial_counter_object_data [] =
	{
	    //Initial counter value
	    0x00, 0x00, 0x00, 0x00,
	    //Threshold value
	    0x00, 0x00, 0x00, 0x0A,
	};

	static uint16_t bytes_to_read;
	static uint8_t new_counter_object_data[8];

	PT_BEGIN(pt);

    do
    {
        me_util = optiga_util_create(0, optiga_lib_callback, NULL); // create util
        if (NULL == me_util) break;

        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_open_application(me_util, 0); // open application
        if(return_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }
        PT_WAIT_WHILE(pt, (optiga_lib_status == OPTIGA_LIB_BUSY));
        if(optiga_lib_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }

        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_write_data(me_util,
												0xE120,
												OPTIGA_UTIL_ERASE_AND_WRITE,
												0,
												initial_counter_object_data,
												sizeof(initial_counter_object_data)); // write data
        if(return_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }
        PT_WAIT_WHILE(pt, (optiga_lib_status == OPTIGA_LIB_BUSY));
        if(optiga_lib_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }

        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_update_count(me_util, 0xE120, 0x02); // update count
        if(return_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }
        PT_WAIT_WHILE(pt, (optiga_lib_status == OPTIGA_LIB_BUSY));
        if(optiga_lib_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }

        bytes_to_read = sizeof(new_counter_object_data);
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_read_data(me_util, 0xE120, 0, new_counter_object_data, &bytes_to_read); // read data
        if(return_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }
        PT_WAIT_WHILE(pt, (optiga_lib_status == OPTIGA_LIB_BUSY));
        if(optiga_lib_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        	uint8_t *p = new_counter_object_data;
        	printf("Counter data:");
        	printf("%02X %02X %02X %02X ", p[0], p[1], p[2], p[3]);
        	printf("%02X %02X %02X %02X\n", p[4], p[5], p[6], p[7]);
        }
        else
        {
        	uint8_t *p = new_counter_object_data;
        	printf("Counter data:");
        	printf("%02X %02X %02X %02X ", p[0], p[1], p[2], p[3]);
        	printf("%02X %02X %02X %02X\n", p[4], p[5], p[6], p[7]);
        }

        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_close_application(me_util, 0); // close application
        if(return_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }
        PT_WAIT_WHILE(pt, (optiga_lib_status == OPTIGA_LIB_BUSY));
        if(optiga_lib_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }

        optiga_util_destroy(me_util); // destroy util
        me_util = NULL;

    } while(FALSE);

    PT_END(pt);
}

#include "pt.h"
#include "optiga/optiga_util.h"

static volatile optiga_lib_status_t optiga_lib_status;
static void optiga_lib_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
}

int8_t example_read_coprocessor_id(struct pt *pt)
{
	static optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
	static optiga_util_t * me_util = NULL;
	static uint16_t bytes_to_read;
	static uint8_t coprocessor_uid[32];

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

        bytes_to_read = sizeof(coprocessor_uid);
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_read_data(me_util, 0xE0C2, 0, coprocessor_uid, &bytes_to_read); // read data
        if(return_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }
        PT_WAIT_WHILE(pt, (optiga_lib_status == OPTIGA_LIB_BUSY));
        if(optiga_lib_status != OPTIGA_LIB_SUCCESS)
        {
        	printf("Error: file %s line %d\n", __FILE__, __LINE__);
        }
        else
        {
        	uint8_t *p = coprocessor_uid;
        	printf("Coprocessor UID components are mentioned below:\n");
        	printf("CIM Identifier                       : %02X\n", p[0]);
        	printf("Platform Identifier                  : %02X\n", p[1]);
        	printf("Model Identifier                     : %02X\n", p[2]);
        	printf("ROM mask ID                          : %02X %02X\n", p[3], p[4]);
        	printf("Chip type                            : %02X %02X %02X %02X %02X %02X\n", p[5], p[6], p[7], p[8], p[9], p[10]);
        	printf("Batch number                         : %02X %02X %02X %02X %02X %02X\n", p[11], p[12], p[13], p[14], p[15], p[16]);
        	printf("Chip position on wafer: X-coordinate : %02X %02X\n", p[17], p[18]);
        	printf("Chip position on wafer: Y-coordinate : %02X %02X\n", p[19], p[20]);
        	printf("Firmware Identifier                  : %02X %02X %02X %02X\n", p[21], p[22], p[23], p[24]);
        	printf("ESW build number, BCD coded          : %02X %02X\n", p[25], p[26]);
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

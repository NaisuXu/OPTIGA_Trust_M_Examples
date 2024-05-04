#include "pt.h"
#include "optiga/optiga_util.h"
#include "optiga/optiga_crypt.h"

static volatile optiga_lib_status_t optiga_lib_status;
static void optiga_lib_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
}

int8_t example_crypt_random(struct pt *pt)
{
	static optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
	static optiga_util_t * me_util = NULL;
	static optiga_crypt_t * me_crypt = NULL;
	static uint8_t random_data_buffer[32]; // 8 - 256bytes

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

        me_crypt = optiga_crypt_create(0, optiga_lib_callback, NULL);
        if (NULL == me_crypt) break;

        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_crypt_random(me_crypt, OPTIGA_RNG_TYPE_TRNG, random_data_buffer, sizeof(random_data_buffer)); // generate random
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
        	uint8_t *p = random_data_buffer;
        	printf("Random data:\n");
        	printf("%02X %02X %02X %02X %02X %02X %02X %02X\n", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
        	printf("%02X %02X %02X %02X %02X %02X %02X %02X\n", p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
        	printf("%02X %02X %02X %02X %02X %02X %02X %02X\n", p[16], p[17], p[18], p[19], p[20], p[21], p[22], p[23]);
        	printf("%02X %02X %02X %02X %02X %02X %02X %02X\n", p[24], p[25], p[26], p[27], p[28], p[29], p[30], p[31]);
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

        optiga_crypt_destroy(me_crypt);  // destroy crypt
        me_crypt = NULL;

    } while(FALSE);

    PT_END(pt);
}

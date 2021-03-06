/**
* \copyright
* MIT License
*
* Copyright (c) 2019 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*
* \endcopyright
*
* \author Infineon Technologies AG
*
* \file pal_os_datastore.c
*
* \brief   This file implements the platform abstraction layer APIs for data store.
*
* \ingroup  grPAL
*
* @{
*/

#include "optiga/pal/pal_os_datastore.h"

/// @endcond
/// Size of data store buffer
#define DATA_STORE_BUFFERSIZE   (0x42)

//Internal buffer to store manage context data use for data store
uint8_t data_store_buffer [DATA_STORE_BUFFERSIZE];

//Internal buffer to store application context data use for data store
uint8_t data_store_app_context_buffer [APP_CONTEXT_SIZE];

//Static shared secret value
const uint8_t optiga_platform_binding_shared_secret [] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
    0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40
};


pal_status_t pal_os_datastore_write(uint16_t datastore_id,
                                    const uint8_t * p_buffer,
                                    uint16_t length)
{
    pal_status_t return_status = PAL_STATUS_FAILURE;

    switch(datastore_id)
    {
        case OPTIGA_PLATFORM_BINDING_SHARED_SECRET_ID:
        {
            // Changing the Platform Binding Shared Secret is not implemented
            return_status = PAL_STATUS_FAILURE;
            break;
        }
        case OPTIGA_COMMS_MANAGE_CONTEXT_ID:
        {           
            // prevent out-of-bounds write
            if (length > DATA_STORE_BUFFERSIZE)
            {
                return_status = PAL_STATUS_FAILURE;
                break;
            }

            memcpy(data_store_buffer, p_buffer, length);
            return_status = PAL_STATUS_SUCCESS;
            break;
        }
        case OPTIGA_HIBERNATE_CONTEXT_ID:
        {
            // prevent out-of-bounds write
            if (length > APP_CONTEXT_SIZE)
            {
                return_status = PAL_STATUS_FAILURE;
                break;
            }

            memcpy(data_store_app_context_buffer,p_buffer,length);
            return_status = PAL_STATUS_SUCCESS;
            break;
        }
        default:
        {
            break;
        }
    }
    return return_status;
}


pal_status_t pal_os_datastore_read(uint16_t datastore_id, 
                                   uint8_t * p_buffer, 
                                   uint16_t * p_buffer_length)
{
    if (p_buffer_length == NULL)
    {
        // need a valid p_buffer_length
        return PAL_STATUS_FAILURE;
    }

    pal_status_t return_status = PAL_STATUS_FAILURE;

    switch(datastore_id)
    {
        case OPTIGA_PLATFORM_BINDING_SHARED_SECRET_ID:
        {
            // !!!OPTIGA_LIB_PORTING_REQUIRED
            // This has to be enhanced by user only,
            // if the platform binding shared secret is stored in non-volatile 
            // memory with a specific location and not as a const text segement 
            // else updating the share secret content is good enough.

            if (*p_buffer_length >= sizeof(optiga_platform_binding_shared_secret))
            {
                memcpy(p_buffer,optiga_platform_binding_shared_secret, 
                       sizeof(optiga_platform_binding_shared_secret));
                *p_buffer_length = sizeof(optiga_platform_binding_shared_secret);
                return_status = PAL_STATUS_SUCCESS;
            }
            break;
        }
        case OPTIGA_COMMS_MANAGE_CONTEXT_ID:
        {
            // !!!OPTIGA_LIB_PORTING_REQUIRED
            // This has to be enhanced by user only,
            // if manage context information is stored in NVM during the hibernate, 
            // else this is not required to be enhanced.

            // prevent out-of-bounds read
            if (*p_buffer_length > DATA_STORE_BUFFERSIZE)
            {
                return_status = PAL_STATUS_FAILURE;
                break;
            }

            memcpy(p_buffer,data_store_buffer,*p_buffer_length);
            return_status = PAL_STATUS_SUCCESS;
            break;
        }
        case OPTIGA_HIBERNATE_CONTEXT_ID:
        {
            // !!!OPTIGA_LIB_PORTING_REQUIRED
            // This has to be enhanced by user only,
            // if application context information is stored in NVM during the hibernate, 
            // else this is not required to be enhanced.

            // prevent out-of-bounds read
            if (*p_buffer_length > APP_CONTEXT_SIZE)
            {
                return_status = PAL_STATUS_FAILURE;
                break;
            }

            memcpy(p_buffer,data_store_app_context_buffer,*p_buffer_length);
            return_status = PAL_STATUS_SUCCESS;
            break;
        }
        default:
        {
            break;
        }
    }

    return return_status;
}

/**
* @}
*/

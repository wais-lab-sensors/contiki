/*
 * Copyright (c) 2014, Nimbus Centre for Embedded Systems Research, Cork Institute of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      SHT25 Sensor Resource
 *
 *      This is a simple GET resource that returns the temperature in Celsius
 *      and the humidity reading from the SHT25.
 * \author
 *      Arthur Fabre <af1g12@ecs.soton.ac.uk>
 */

#include "contiki.h"

#define DEBUG 0

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

// Ensure the platform supports SHT25 and battery readings
#ifndef PLATFORM_HAS_SHT25
    #error "Platform does not support SHT25 temperature and humidity readings" 
#endif
#ifndef PLATFORM_HAS_BATTERY
    #error "Platform does not support battery readings"
#endif

#include "res-wais.h"
#include <string.h>
#include "rest-engine.h"
#include "dev/sht25.h"
#include "dev/battery-sensor.h"

#define WAIS_MAX_SIZE 128

static void wais_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);


/* Get ressource. Returns temp, humidity, and battery voltage. */
RESOURCE(res_wais, "title=\"WAIS Sensor Readings\"", wais_get_handler, NULL, NULL, NULL);

void wais_activate_sensors(void) {
    SENSORS_ACTIVATE(battery_sensor);
    SENSORS_ACTIVATE(sht25);
}

/*
 * Handler that actually serves the request
 *
 * For data larger than REST_MAX_CHUNK_SIZE (e.g., stored in flash) resources must be aware of the buffer limitation
 * and split their responses by themselves. To transfer the complete resource through a TCP stream or CoAP's blockwise transfer,
 * the byte offset where to continue is provided to the handler as int32_t pointer.
 * These chunk-wise resources must set the offset value to its new position or -1 of the end is reached.
 * (The offset for CoAP's blockwise transfer can go up to 2'147'481'600 = ~2047 M for block size 2048 (reduced to 1024 in observe-03.)
 */
// TODO - this isn't threadsafe
static void wais_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {

    static char message[WAIS_MAX_SIZE] = {0};

    // Length of message in bytes
    static uint8_t message_len = 0;

    PRINTF("[RES-WAIS] Serving request! Offset %d, PrefSize %d\n", (int) *offset, preferred_size);

    // Get new data if this is a new request.
    if (*offset == 0) {

        PRINTF("[RES-WAIS] Offset 0, gathering data\n");

        int16_t temperature = sht25.value(SHT25_VAL_TEMP);
        uint16_t humidity = sht25.value(SHT25_VAL_HUM);
        int battery = battery_sensor.value(0);

        // Can't print floats, format them by hand
        message_len = snprintf(message, WAIS_MAX_SIZE, "{\"reading\":{\"temperature\":%d.%d,\"humidity\":%d.%d,\"battery\":%d}}", temperature / 100, temperature % 100, humidity / 100, humidity % 100, battery);

        if (message_len >= WAIS_MAX_SIZE || message_len < 0) {
            // Message has been truncated or there was an error.
            REST.set_response_status(response, REST.status.INTERNAL_SERVER_ERROR);

            const char *error_msg = "MessageTooLong";
            REST.set_response_payload(response, error_msg, strlen(error_msg));
            return;

        } else {
            PRINTF("[RES-WAIS] Got message %s. Length %d\n", message, message_len);
        }
    }

    // Dummy value that indicates the content type requested. We always serve JSON
    unsigned int accept;
    REST.get_header_accept(request, &accept);

    uint8_t buffer_len = 0;

    // If the remainder of the message fits in a single chunk
    if (message_len - *offset <= preferred_size) {

        PRINTF("[RES-WAIS] Last chunk!\n");

        memcpy(buffer, message + *offset, message_len - *offset);
        buffer_len = message_len - *offset;

        // Indicates this is the last chunk
        *offset = -1;

    } else {

        PRINTF("[RES-WAIS] Intermediate chunk!\n");

        // Copy data in chunks to the buffer
        memcpy(buffer, message + *offset, preferred_size);
        buffer_len = preferred_size;

        *offset += preferred_size;
    }

    REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
    REST.set_response_payload(response, buffer, buffer_len);

    PRINTF("[RES-WAIS] Next offset %d\n", *offset);
    PRINTF("[RES-WAIS] buffer length %d\n", buffer_len);
}

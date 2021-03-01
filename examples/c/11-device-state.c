/*
 * Copyright 2020 The Imaging Source Europe GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This example shows how to get/set the JSON property description for a certain camera 
 * For some cameras, e.g. DFK 72 models some properties, e.g. Exposure and 
 * Gain Automatic are performed in software. In this case some properties 
 * are available after a pipeline has been started. Therefore, this sample
 * creates a small pipeline and starts it by setting it in state PLAYING. 
 *
 * Please use the 01-list-properties sample in order to check which 
 * properties are available with and without the pipeline being started. 
 */


#include <gst/gst.h>

#include <stdio.h> /* printf and putchar */
#include "tcamprop.h" /* gobject introspection interface */


int main (int argc, char *argv[])
{
    gst_init(&argc, &argv); // init gstreamer

    GError *error = NULL;
    GstElement *pipeline;

    /* Create a pipeline with a sink*/
    pipeline = gst_parse_launch( "tcambin name=source ! fakesink" ,&error);
    /* Query the tcambin GStreamer element out of the pipeline*/
    //GstElement* source = gst_element_factory_make("tcambin", "source");
    GstElement* source = gst_bin_get_by_name(GST_BIN(pipeline), "source");

    const char* serial = NULL;

    if (serial != NULL)
    {
        GValue val = {};
        g_value_init(&val, G_TYPE_STRING);
        g_value_set_static_string(&val, serial);

        g_object_set_property(G_OBJECT(source), "serial", &val);
    }

    /* In  PLAYING state the all properties, even the software implemented ones are available */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    
    /* Wait for the pipeline has started */
    gst_element_get_state(pipeline,NULL,NULL,4000000000);

    /* Device is now in a state for interactions */

    GValue state = G_VALUE_INIT;

    g_value_init(&state, G_TYPE_STRING);
    /* Get the state JSON string and print it out for a before/after comparison */
    g_object_get_property(G_OBJECT(source), "state", &state);

    printf("State of device is:\n%s", g_value_get_string(&state));
    /* Change JSON description or save it to a file here. 
     * That shall not be part of this example. It is recommended to use a JSON library.
     */

    /* Pass a state JSON string to the tcambin */
    g_object_set_property(G_OBJECT(source), "state", &state);

    /* Second read and print for the before/after comparison */
    g_object_get_property(G_OBJECT(source), "state", &state);
    printf("State of device is:\n%s", g_value_get_string(&state));

    /* cleanup, reset state */
    gst_element_set_state(pipeline, GST_STATE_NULL);

    gst_object_unref(source);
    gst_object_unref(pipeline);

    g_value_unset(&state);

    return 0;
}

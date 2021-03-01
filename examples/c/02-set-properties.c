/*
 * Copyright 2019 The Imaging Source Europe GmbH
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

/* This example shows how to set properties for a certain camera 
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

void print_properties (GstElement* source)
{

    GValue exp_auto_value = G_VALUE_INIT;

    /* We are only interested in the value, this  */
    gboolean ret = tcam_prop_get_tcam_property(TCAM_PROP(source),
                                               "Exposure Auto",
                                               &exp_auto_value,
                                               NULL, NULL, NULL, NULL,
                                               NULL, NULL, NULL, NULL);

    if (ret)
    {
        printf("Exposure Auto has value: %s\n",
               g_value_get_boolean(&exp_auto_value) ? "true" : "false");
        g_value_unset( &exp_auto_value );
    }
    else
    {
        printf("Property Exposure Auto is not supported by current device\n");
    }

    GValue gain_auto_value = G_VALUE_INIT;

    ret = tcam_prop_get_tcam_property(TCAM_PROP(source),
                                      "Gain Auto",
                                      &gain_auto_value,
                                      NULL, NULL, NULL, NULL,
                                      NULL, NULL, NULL, NULL);
    if (ret)
    {
        printf("Gain Auto has value: %s\n",
               g_value_get_boolean(&gain_auto_value) ? "true" : "false");
        g_value_unset( &gain_auto_value );
    }
    else
    {
        printf("Property Gain Auto is not supported by current device\n");
    }

    GValue brightness_value = G_VALUE_INIT;

    ret = tcam_prop_get_tcam_property(TCAM_PROP(source),
                                      "Brightness",
                                      &brightness_value,
                                      NULL, NULL, NULL, NULL,
                                      NULL, NULL, NULL, NULL);

    if (ret)
    {
        printf("Brightness has value: %d\n",
               g_value_get_int(&brightness_value));
        g_value_unset(&brightness_value);
    }
    else
    {
        printf("Property Brightness is not supported by current device\n");
    }
}


int main (int argc, char *argv[])
{
    gst_init(&argc, &argv); // Init gstreamer
    GError *error = NULL;
    GstElement *pipeline;
    
    /* Create a pipeline with a sink*/
    pipeline = gst_parse_launch( "tcambin name=source ! fakesink" ,&error);

    /* Query the tcambin GStreamer element out of the pipeline*/
    GstElement* tcambin = gst_bin_get_by_name(GST_BIN(pipeline), "source");

    const char* serial = NULL;

    if (serial != NULL)
    {
        GValue val = {};
        g_value_init(&val, G_TYPE_STRING);
        g_value_set_static_string(&val, serial);

        g_object_set_property(G_OBJECT(tcambin), "serial", &val);
    }

    /* In  PLAYING state the all properties, even the software implemented ones are available */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    
    /* Wait for the pipeline has started */
    gst_element_get_state(pipeline,NULL,NULL,4000000000);

    /* Print the properties for a before/after comparison */
    print_properties(tcambin);

    /* Set the properties to other values */
    /* If GigE cameras are in use, use "Off" string instead of boolean FALSE. */
    GValue set_auto = G_VALUE_INIT;
    g_value_init(&set_auto, G_TYPE_BOOLEAN);

    g_value_set_boolean(&set_auto, FALSE);

    /* Properties are identified by their names */
    tcam_prop_set_tcam_property(TCAM_PROP(tcambin),
                                "Exposure Auto", &set_auto);

    /* Reuse set_autom because Auto Exposure and Auto Gain are or same type */
    tcam_prop_set_tcam_property(TCAM_PROP(tcambin),
                                "Gain Auto", &set_auto);

    g_value_unset( &set_auto );

    /* The following code shows how to set an integer property. Here 
     * Brightness is use as example. Not every device has a Brightness
     * property, so the code execution may fails.
     */
     
    /* Create the GValue variable for the property value */ 
    GValue set_brightness = G_VALUE_INIT;
    g_value_init(&set_brightness, G_TYPE_INT);

    g_value_set_int(&set_brightness, 200);

    /* Set a new brightness value */
    tcam_prop_set_tcam_property(TCAM_PROP(tcambin),
                                "Brightness", &set_brightness);

    g_value_unset(&set_brightness);

    /* Second print of properties and values for the before/after comparison */
    print_properties(tcambin);

    /* Cleanup and reset state */
    gst_element_set_state(pipeline, GST_STATE_NULL);

    gst_object_unref(tcambin);
    gst_object_unref(pipeline);

    return 0;
}

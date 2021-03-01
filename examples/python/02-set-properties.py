#!/usr/bin/env python3

# Copyright 2019 The Imaging Source Europe GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# This example will show you how to set properties
#
# For some cameras, e.g. DFK 72 models some properties, e.g. Exposure and 
# Gain Automatic are performed in software. In this case some properties 
# are available after a pipeline has been started. Therefore, this sample
# creates a small pipeline and starts it by setting it in state PLAYING. 
#
# Please use the 01-list-properties sample in order to check which 
# properties are available with and without the pipeline being started. 

import sys
import gi

gi.require_version("Tcam", "0.1")
gi.require_version("Gst", "1.0")

from gi.repository import Tcam, Gst


def print_properties(camera):
    """
    Print selected properties
    """
    (ret, value,
     min_value, max_value,
     default_value, step_size,
     value_type, flags,
     category, group) = camera.get_tcam_property("Exposure Auto")

    if ret:
        print("Exposure Auto has value: {}".format(value))
    else:
        print("Could not query Exposure Auto")

    (ret, value,
     min_value, max_value,
     default_value, step_size,
     value_type, flags,
     category, group) = camera.get_tcam_property("Gain Auto")

    if ret:
        print("Gain Auto has value: {}".format(value))
    else:
        print("Could not query Gain Auto")

    try:
        (ret, value,
         min_value, max_value,
         default_value, step_size,
         value_type, flags,
         category, group) = camera.get_tcam_property("Brightness")

        if ret:
            print("Brightness has value: {}".format(value))
        else:
            print("Could not query Brightness")
    except:
        print("Property Brightness is not availble on current device.")


def main():

    Gst.init(sys.argv)
    # Assign a serial number if a specific camera if you
    # do not want to use the default camera
    serial = None
        
    # Create a simple pipeline
    pipeline = Gst.parse_launch("tcambin name=source ! fakesink")

    # Query the tcambin from the pipeline
    tcambin = pipeline.get_by_name("source")

    # If serial is defined, then make the source open that device
    if serial:
        tcambin.set_property("serial", serial)

    # Start the pipeline, so all, even software properties are available
    pipeline.set_state(Gst.State.PLAYING)
    
    # Wait until the pipeline has been started
    pipeline.get_state(4000000000) 

    # Print properties for a before/after comparison
    print_properties(tcambin)

    # Set properties 
    # If GigE cameras are in use, use "Off" string instead of boolean false.
    tcambin.set_tcam_property("Exposure Auto", False)
    tcambin.set_tcam_property("Gain Auto", False)

    # Try to set an integer property. Please note, not all cameras have 
    # a "Brightness" property, so the code may fails.
    try:
        tcambin.set_tcam_property("Brightness", 200)
    except:
        print("Property Brigthness is not available")

    print_properties(tcambin)

    # cleanup, reset state
    tcambin.set_state(Gst.State.NULL)

if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3

# Copyright 2020 The Imaging Source Europe GmbH
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
# This example shows how to get and set the JSON property description for a certain camera 
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


def main():

    Gst.init(sys.argv)
    # Set this to a serial string for a specific camera
    serial = None

    # Create a simple pipeline
    pipeline = Gst.parse_launch("tcambin name=source ! fakesink")

    # Query the tcambin from the pipeline
    camera = pipeline.get_by_name("source")

    if serial:
        # This is gstreamer set_property
        camera.set_property("serial", serial)

    # Start the pipeline
    pipeline.set_state(Gst.State.PLAYING)
    
    # Wait until the pipeline has been started
    pipeline.get_state(4000000000) 

    # Print properties for a before/after comparison
    state = camera.get_property("state")

    print("State of device is:\n{}".format(state))

    # Change JSON description here
    # JSON handling is not not part of this example
    camera.set_property("state", state)

    # Clean up
    pipeline.set_state(Gst.State.NULL)

if __name__ == "__main__":
    sys.exit(main())

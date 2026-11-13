#!/bin/bash
# cd.sh <tag> : cdiff.py in the lane environment
export S=/opt/fable_lane; source $S/wine/env.sh >/dev/null 2>&1; export S=/opt/fable_lane; cd /home/user/FableDecomp
python3 /opt/fable_lane/w22/agent4007/tools/cdiff.py "$@" | cut -c1-200

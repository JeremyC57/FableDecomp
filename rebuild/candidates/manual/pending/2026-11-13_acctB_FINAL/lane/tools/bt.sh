#!/bin/bash
export S=/opt/fable_lane; source $S/wine/env.sh >/dev/null 2>&1; export S=/opt/fable_lane; cd /home/user/FableDecomp
python3 /opt/fable_lane/w22/agent4007/tools/bt.py "$@"

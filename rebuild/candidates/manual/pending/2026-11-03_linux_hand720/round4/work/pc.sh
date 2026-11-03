#!/bin/bash
# pc.sh <agentdir> <src> : cpp precheck (missing includes / #error) before cl
cd /home/user/FableDecomp
ST=tools/decomp_pipeline/linux_lane/lane_tools/coordinator/stubinc
cpp -P -nostdinc -undef -D_MSC_VER=1310 -D_M_IX86=600 -D_WIN32 -I $1/hdr -I rebuild/include -I $ST $2 > /dev/null 2> $1/work/cpp.err
grep -E "error|No such" $1/work/cpp.err | head -5

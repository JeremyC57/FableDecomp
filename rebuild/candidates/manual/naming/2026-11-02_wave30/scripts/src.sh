#!/bin/bash
# src.sh <addr> [t]: print the landed source (or test) path of a row
cd /home/user/FableDecomp; python3 -c "
import sys; sys.path.insert(0,'/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/naming/w30/scripts'); from rows import rows; R=rows(); a=sys.argv[1].lower().replace('0x','').zfill(8); d=R[a]
print('rebuild/tests/'+d['TestSource'] if len(sys.argv)>2 else 'rebuild/src/compiled/'+d['Source'])" "$@"

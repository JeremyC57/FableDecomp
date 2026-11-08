#!/bin/bash
# odis.sh <tag> [addr]: our listing of the graded row with relocation names (addresses as function offsets + 0x531ec0)
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent862; AD=${2:-00531ec0}
objdump -dr --no-show-raw-insn -M intel $A/work/g_${AD}_$1/$AD.obj | awk -v base=$((16#$AD)) '/^[0-9a-f]+ <.*>:/{infn=($2 ~ /'"${LEAFPAT:-Update@CComponent}"'/)} infn && /^ +[0-9a-f]+:/{split($1,a,":"); printf "%08x %s\n", strtonum("0x" a[1])+base, substr($0, index($0,$2))} infn && /DISP32/{print "         -> " $3}'

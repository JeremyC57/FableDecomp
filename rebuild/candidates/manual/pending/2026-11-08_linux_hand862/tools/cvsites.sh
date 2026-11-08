#!/bin/bash
# cvsites.sh <tag>: list ConvertVector call sites (ours) by address
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent862
python3 $A/bin/odis.py $1 | grep -B1 "ConvertVector\|_List_base\|CDeletion" | grep -v "^--" | paste - - | awk '{print $1, $NF}' | sed 's/?ConvertVector.*/CV/; s/??1?$_List_base.*/~LB/; s/??0CDeletion.*/CDcopy/' | tr '\n' ' '; echo

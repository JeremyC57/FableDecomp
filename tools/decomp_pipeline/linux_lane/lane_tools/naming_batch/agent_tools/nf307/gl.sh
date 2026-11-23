#!/bin/bash
# gl.sh <addr> <regex>: non-comment lines of the row's landed source matching regex, with line numbers (hint helper)
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF307
f=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$1'))")
grep -nE "$2" "$f" | grep -vE '^[0-9]+:\s*//' | cut -c1-230

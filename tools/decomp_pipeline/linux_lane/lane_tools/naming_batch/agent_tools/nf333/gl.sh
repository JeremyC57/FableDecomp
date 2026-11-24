#!/bin/bash
# gl.sh <addr> <regex>: non-comment lines of the row's landed source matching regex, with line numbers (hint helper)
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333
f=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$1'))")
grep -nE "$2" "$f" | grep -vE '^[0-9]+:\s*//' | cut -c1-230

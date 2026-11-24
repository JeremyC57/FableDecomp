#!/bin/bash
# cpp_err.sh <file>...: gcc preprocessor pre-pass with _MSC_VER=1310 (VC7.1 include tree): prints CPP_OK or the #error / missing-include lines
S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; A=$S/w22/agentNF297; R=/home/user/FableDecomp
for f in "$@"; do
  out=$(timeout 60 g++ -E -x c++ -D_MSC_VER=1310 -D_M_IX86=600 -D_WIN32 -I$A/hdr -I$R/rebuild/include -isystem $S/wine/vc71/include "$f" -o /dev/null 2>&1 | grep -E '#error|fatal error' | head -3)
  if [ -z "$out" ]; then echo "CPP_OK $f"; else echo "CPP_ERR $f"; echo "$out" | sed 's/^/   /'; fi
done

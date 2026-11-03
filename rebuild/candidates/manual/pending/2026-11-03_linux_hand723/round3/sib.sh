#!/bin/bash
# sib.sh: regrade round 2's 12 siblings under the final overlay, one at a time
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
cd /home/user/FableDecomp
D=$S/w22/agent723c; R=rebuild/candidates/manual/pending/2026-11-03_linux_hand723
for f in rebuild/src/compiled/00/b7/CEngineWaterRenderer_RenderSeaNonReflectiveForeground_00b77e20.cpp \
         rebuild/src/compiled/00/b7/CEngineWaterRenderer_RenderIceBackground_00b75ca0.cpp \
         rebuild/src/compiled/00/b7/CEngineWaterRenderer_RenderBackground_00b76bc0.cpp \
         $R/src/00b36ec0.cpp $R/src/00b67920.cpp $R/src/00b6d6d0.cpp $R/src/00b71880.cpp $R/src/00bc14e0.cpp $R/src/00c2a890.cpp \
         rebuild/src/compiled/00/62/global_GFPlayFullScreenXBoxMovie_006286f0.cpp \
         rebuild/src/compiled/00/b6/CEngineLandscapeRenderer_SetupForegroundStates_00b677d0.cpp \
         rebuild/src/compiled/00/bc/CEnginePrimitiveManagerLines_RenderPrimitive_00bc9800.cpp; do
  a=$(basename $f .cpp); a=${a: -8}
  L=$(sed -n 's,^// Leaf: ,,p' $f | head -1)
  if [ -n "$L" ]; then set -- "$L"; else set --; fi
  r=$(FABLE_HDR_OVERLAY=$D/hdr timeout 120 python3 $S/tools/q.py $a $f "$@" 2>&1 | grep -oE "(RELOCATION_MATCH|MATCH|DIFFER\([0-9]+v[0-9]+\)|SRC_FAIL|[A-Z_]+FAIL)" | head -1)
  echo -e "$a\t$r"
done

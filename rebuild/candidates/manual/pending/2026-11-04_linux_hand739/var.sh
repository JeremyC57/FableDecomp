#!/bin/bash
# var.sh <tag> <python-snippet operating on h (hdr) and b (body)>
export S=${S}; D=$S/w22/chunk739
T=$1; shift
python3 - "$T" "$@" <<'PY'
import sys
import os; D=os.environ['S']+'/w22/chunk739/w/'
t=sys.argv[1]; code=sys.argv[2]
h=open(D+'cad0_hdr.inc').read(); b=open(D+'cad0_body.inc').read(); c=open(D+'cls_nobrace.txt').read()
exec(code)
head="// NParticleEngine::CPSCUpdateNormal::UpdateAddParticle -- vtable slot 12\n// Leaf: ?UpdateAddParticle@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@VC3DVector@@2M@Z\n// Flags: none\n"
open(D+'cad0_%s.cpp'%t,'w').write(head+h+c+b)
open(D+'cad0_%s.hdr'%t,'w').write(h); open(D+'cad0_%s.body'%t,'w').write(b)
PY
bash $D/g.sh 00b0cad0 $D/w/cad0_$T.cpp $T

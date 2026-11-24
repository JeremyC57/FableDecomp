#!/bin/bash
# linkchain.sh <b...>: per batch, nf.py link: set 1 (negative control) = the batch's new objects alone; set 2 = the same plus the two
# landed owner objects compiled from HEAD (0x00CD3D2E, 0x009AC700). Watch: map operator[] symbols. timeout 900 each (agent NF353).
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF353
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
export FABLE_ROOT=/home/user/FableDecomp
cd $A/work
for b in "$@"; do
  objs=$(for a in $(cat $A/work/$b.txt); do echo $A/work/obj/N_$a.obj; done)
  timeout 900 python3 $A/tools/nf.py link $b '^\?\?A\?\$map@' $objs -- $objs $A/work/obj/H_00cd3d2e.obj $A/work/cobj/009ac700.obj >> $A/work/linkchain.err 2>&1
done
echo LINK_DONE "$@" >> $A/work/linkchain.log

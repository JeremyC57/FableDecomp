#!/bin/bash
# run3.sh: NF359 both pick-any checkers (--leaf, caller-given address file) on HEAD and probe objects
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF359; T=$A/tools; W=$A/work
timeout 900 bash $T/pk2.sh H_00455ef0 00455ef0 $W/hobj/00455ef0.obj '??0CThingNoiseDef@@QAE@XZ'
timeout 900 bash $T/pk2.sh c1 00455ef0 $W/exp/obj/c1_allocform_455ef0.obj '??0CThingNoiseDef@@QAE@XZ'
timeout 900 bash $T/pk2.sh c2 00455ef0 $W/exp/obj/c2_ool_455ef0.obj '??0CThingNoiseDef@@QAE@XZ'
timeout 900 bash $T/pk2.sh c3 00455ef0 $W/exp/obj/c3_nf86_455ef0.obj '??0CThingNoiseDef@@QAE@XZ'
timeout 900 bash $T/pk2.sh H_0044c1bb 0044c1bb $W/hobj/0044c1bb.obj '??0CTCNoiseDef@@QAE@XZ'
timeout 900 bash $T/pk2.sh d1 0044c1bb $W/exp/obj/d1_allocform_44c1bb.obj '??0CTCNoiseDef@@QAE@XZ'
timeout 900 bash $T/pk2.sh d2 0044c1bb $W/exp/obj/d2_ool_44c1bb.obj '??0CTCNoiseDef@@QAE@XZ'
timeout 900 bash $T/pk2.sh H_0045c5da 0045c5da $W/hobj/0045c5da.obj 'CThingNoiseDef_ScalarDeletingDestructor'
timeout 900 bash $T/pk2.sh e1 0045c5da $W/exp/obj/e1_allocform_45c5da.obj '??_GCThingNoiseDef@@UAEPAXI@Z'
timeout 900 bash $T/pk2.sh H_0045c5f6 0045c5f6 $W/hobj/0045c5f6.obj '??1CThingNoiseDef@@UAE@XZ'
echo RUN3_DONE

#!/bin/bash
# links367.sh: /FORCE links of the 0x00749810 / 0x00749A70 pair in both orders: HEAD (H_), P2e (HEAD 00749810 + p2e), device form (v6 + p2f),
# plus the owner objects (0x0044C1BB, 0x0045C5F6) for the CTCNoiseDef copies -> evidence/link_<tag>.txt (nf.py link: HEAD set -- new set). NF367.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF367; O=$A/work/obj; N="python3 $A/tools/nf.py link"
$N pair_810first . $O/H_00749810.obj $O/H_00749a70.obj -- $O/H_00749810.obj $O/00749a70_p2e.obj
$N pair_a70first . $O/H_00749a70.obj $O/H_00749810.obj -- $O/00749a70_p2e.obj $O/H_00749810.obj
$N dev_pair . $O/H_00749810.obj $O/H_00749a70.obj -- $O/00749810_v6.obj $O/00749a70_p2f.obj
$N dev_pair_rev . $O/H_00749a70.obj $O/H_00749810.obj -- $O/00749a70_p2f.obj $O/00749810_v6.obj
$N unit_owners . $O/H_00749810.obj $O/H_00749a70.obj $O/H_0044c1bb.obj $O/H_0045c5f6.obj -- $O/00749810_v6.obj $O/00749a70_p2f.obj $O/H_0044c1bb.obj $O/H_0045c5f6.obj
echo LINKS_DONE

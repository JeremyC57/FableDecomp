#!/bin/bash
# links367b.sh: /FORCE links of HEAD 0x00749810 with 0x00749A70's helper form (P2h), both orders, and with the owners (0x0044C1BB, 0x0045C5F6). NF367.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF367; O=$A/work/obj; N="python3 $A/tools/nf.py link"
$N p2h_810first . $O/H_00749810.obj $O/H_00749a70.obj -- $O/H_00749810.obj $O/00749a70_p2h.obj
$N p2h_a70first . $O/H_00749a70.obj $O/H_00749810.obj -- $O/00749a70_p2h.obj $O/H_00749810.obj
$N p2h_owners . $O/H_00749810.obj $O/H_00749a70.obj $O/H_0044c1bb.obj $O/H_0045c5f6.obj -- $O/H_00749810.obj $O/00749a70_p2h.obj $O/H_0044c1bb.obj $O/H_0045c5f6.obj
echo LINKS_DONE

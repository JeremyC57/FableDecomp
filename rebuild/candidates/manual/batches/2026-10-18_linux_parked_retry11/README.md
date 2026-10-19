# Parked-row retry batch 11

0093dc10 CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::IsStillRunnable, 192 bytes, recovered on third total source-change grade. Named squared-coordinate locals (`xx=X*X; yy=Y*Y; return yy+xx`) reproduce the retail x87 load order; swapping the textual addition operands alone did not.

The actual-source fixture checks base-state rejection and argument forwarding, strict positive/negative height limits, X/Y distance boundaries, radius expansion, NaN rejection, and receiver/call counts. Both host g++ and independent canonical VC7.1/Wine tests pass. Shared brain/state/physics headers provide matching fields; the component view supplies a typed intelligent-pointer interface. Final canonical results: landing_evidence.txt.

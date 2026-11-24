# nf361 b1: Reset<NUISystem::CComponent> copies made retail-shaped in four /O2 rows (005fb7e0 005fe090 006001e0 00600b60)

Agent NF361 (Account A naming queue; hand-back `pending/2026-11-24_nf361_handback/`), from the NF358 census (T3 nf358 0x00429C6F). Each row
defines the landed NF352 opt-ins FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY and _RELEASEDATA_SIZE_BODY (engine/CCountedPointer.h,
scaffolding). Reset 103 -> 59 B and ReleaseData 53 -> 44 B, both NEQ -> EQ; objsec: only those two COMDATs change, no symbol added or
dropped, leaf data- and relocation-equal; with RESET alone exactly one COMDAT changes (NF352 Q2). The ReleaseData copy equals the agent's
build of owner 0x004291DE; the Reset copy equals the owner object (compared against NF362's read-only HEAD build of 0x00429C6F) and the
HEAD copies of landed 0x00605570.
005fe090 / 00600b60 carry stale Account B packages (`pending/2026-11-11_acctB_c2029`, `_c2025`) equal to the landed text and no open claim:
landed under NF357 ruling Q5.

Coordinator verification from the repo (`$S/v361.sh`): vland `--recheck-landed` 4/4 RELOCATION_MATCH PASS; pk_all `--leaf` agent HEAD v
repo: only Reset / ReleaseData NEQ -> EQ, no new NEQ; reachflip 0 NEQ flips (4 pairs); pickany_vs_retail: no new NEQ.
Remaining NEQ copies (pre-existing): 005fb7e0's own 51 B GetDef<CUIMiscThingsDef>; 006001e0 / 00600b60's GetDef<CUIMiscThingsDef> 116 B
(row-local template; held/g1 ruling), GetAllLeafThings 0x00617A90 and map::operator[] 0x0042D5B1.
Skipped by NF361 (measured): 005ff190 00603000 00607330 0061b610 0061e6d0 (the noinline specialization moves the leaf), 006096e0 0060c900
0060eeb0 (leaf moves with both defines; RESET alone gives a ReleaseData NEQ reach flip n -> y), 00622a00 (its leaf-called 0x0061B610 copy
turns EQ -> NEQ).

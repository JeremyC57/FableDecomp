# Chunk 659 -- 1 of 4 landed (2026-11-01, the two parked pairs per the owner decision 2026-11-01)

Rows: 0x00626E40 ConstructItemCategoryList (never authored) + 0x006276F0 GetItemsListUI (parked chunk 652), 0x00935E40 PutOutFire (parked
chunk 654) + 0x009363D0 MinionWatchFires::Update (parked chunk 259). Landed via `coordinator/land_chunk.sh 659`: 0x006276F0
RELOCATION_MATCH + PASS (2 probes) -- the real 0x00626E40 body in the same file replaced chunk 652's invented declaration; the file ships
that sibling body as its same-TU callee (retail's own TU shape; coordinator decision) while 0x00626E40 itself stays parked at 1513v1471
(17 probes + 16 diagnostics: VC7.1 inlines ~list<CComponent*> 0x0042ABCA at the CDeletion sites where retail calls it -- the chunk-583
destructor-inlining gap; /Ob1 calls it but breaks other inlines). The Minion pair parks together in one source
(`pending/2026-10-27_linux_hand659/differ/`): 0x00935E40 476v468, 0x009363D0 442v438, two residues (an eax copy at the first inlined
LowerBound, PlayAnimation's zero register that depends on surrounding code). Old parked copies in hand652 / hand654 / hand259 removed.
5 NAME_FIXES queued (c659). Labels: ghidra_out/labels_chunk659_2026-10-27.tsv (11). Header debt: engine/CAIStateGroupBase.h and
CAIStateGroup_MinionWatchFires.h are flat `#pragma pack(1)` layouts (no virtual Update, no CIntelligentPointer member); engine/CManager.h
is the global-namespace CManager, not NUISystem::CManager. Lessons in GOTCHAS (1 line) and LESSONS.md.

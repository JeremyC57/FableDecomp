# nf363 b2 (unit nf363_noisedef): CThingNoiseDef / CTCNoiseDef constructors and ??_GCThingNoiseDef in the Alloc form (0x00455EF0, 0x0044C1BB, 0x0045C5DA)

NF359's option A (hand-back `pending/2026-11-24_nf359_handback/`), built with full gates by agent NF363 (`pending/2026-11-24_nf363_handback/`).
Ruling (advisor decision, owner delegated, 2026-11-24): option A with the 0x004304E0 precedent; no 0x0045C5C1 row (0x0045C5F6's object
already emits Alloc EQ). Each new object is section-for-section identical to landed 0x0045C5F6's object (16 sections SAME; DEF / UND equal),
graded on its own leaf: `??0CThingNoiseDef@@QAE@XZ`, `??0CTCNoiseDef@@QAE@XZ`, `??_GCThingNoiseDef@@UAEPAXI@Z` (0x0045C5DA's self, was the
stand-in `?CThingNoiseDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z`).

Agent gates: vland RELOCATION_MATCH PASS HEAD and new per row; new tests on engine/CThingNoiseDef.h / CTCNoiseDef.h pass, one mutant each
fails; retcmp 8/8 function COMDATs EQ; vftables 25/25 and 10/10 slots (slot 0 ??_E alias v retail ??_G only); checkers PASS with controls,
reach flips 0; drop test: only the stand-in self and the 0x0044C0C0 stand-in UNDs drop; unit link: 3 LNK2005 on ?Alloc@CThingNoiseDef, byte-
and relocation-identical (the nf184 pattern), unresolved set only shrinks.
Coordinator from the repo (`$S/v363.sh`): 3/3 RELOCATION_MATCH PASS; pk_all HEAD (agent) v repo: 0 new NEQ, 3 / 4 / 5 new EQ copies;
pickany_vs_retail 0 new NEQ; reachflip 0. Tests copied from the run dir. TABLE_CHANGES applied: `delete ??0CMemberBase_0044c0c0@@QAE@XZ
0044c0c0 00455ef0`. T3 nf355 0x00455EF0 / 0x0044C1BB closed (T3_CLOSED.tsv); nf359 0x0044C1BB (0x00749810's NEQ ??0CTCNoiseDef copy) parked
and restored: it stays as debt (the 0x00749810 first-wins race exists at HEAD; every copy this unit adds is EQ).
0x0045C5DA's STANDIN_LEAF warning is a standin_leaf.py gap (the catalog name is spelled `scalar_deleting_destructor'), not a stand-in.

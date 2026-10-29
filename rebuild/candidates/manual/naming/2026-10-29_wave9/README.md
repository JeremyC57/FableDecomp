# Naming pass wave 9 (2026-10-29): 456 rows / 457 files respelled

Inputs (brief `../briefs/BRIEF_W9.md`): NAME_FIXES_QUEUE rows for chunks 566-575 (plus one misfiled chunk-520 row), wave 8's
open decisions, the five wave 8 RENAMES proposals. Every spelling checked against both PDB name lists; `_W` lands as `G`.
DECISIONS_W9.tsv (387 addresses): 23 applied, 20 partly applied, 21 no change needed, 2 decided not applied, 321 open
(each with its reason: probable without a second source, identity check without a second source, mega family, virtual-ness
change, hand edit needed). 0x012C32F0 (the CIsThingAlive vftable) upgraded to proven.

0x0099EFB0 CCharString::operator= settled as the const& return `??4CCharString@@QAEABV0@ABV0@@Z` (Ego_r 0x00AFA810,
FableWin 0x02F61830); NAME_FIXES_QUEUE row 522 (non-const) is wrong. Sub-wave h9_opeq: 24 rows (16 row-local namespaced /
std CCharString classes moved to global scope, 8 stand-in functions replaced by the real operator=), own recheck all
RELOCATION_MATCH PASS. 18 wrong spellings bound to 0x0099EFB0 retired. Left: 00404720 declares the non-const form (fails
its behaviour test at baseline; DEFERRED).

Families (scripts/, composed by scripts/compose9.py: hand h9_opeq 24 / h9_self 2 / h9_ren 5, then acc9 120 (access/const),
cita9 145 (NScript::CIsThingAlive), xfl9 24, gror9 39, aia9 52, rtc9 46, gws9 9, nga9 9; some rows in two families).

Renames (RENAMES.tsv, all APPLIED with git mv + build_candidates.ps1): 00438570 CPlayerGui::PeekGuiDef, 00405f00
GFIntToWideString, 0051ed80 CThingManager::GetPThingWithUID, 0049d810 CWorld::GetWorldSeed, 008fcf80
CAIStateGroupBase::PeekPlayerManager.

Bindings: 95 retired, 17 added (exact names in both PDB lists), 19 kept, 19 no-bind; bindings_multi_check = 0.
Labels: 14 proven (`ghidra_out/labels_naming_wave9_2026-10-29.tsv`); six corrected catalog names (00405f00, 00e241b0,
007aed10, 0097b870, 00a5bef0, 00a64c10).

Deferred (DEFERRED.tsv, 17 with plans): mega families GetPItem wrappers (873 rows), ~CIntelligentPointer<CThing> (676; check
whether 0xA01C10 folds two dtors first), SetPItem(CThing*) (227), LowerBound 0x40F020 / 0x4365B0 (~250, probable),
CCharString dtor stand-ins (192); Transfer<bool> 004045c0 (58 rows model members as signed char); 00c28690 libpk Free;
004c9840 / 00a44a40 failed their own recheck; PDB-virtual members needing layout changes; nga9 reference-vs-pointer skips (33).

Recheck: recheck.log (coordinator full regrade of all 456 changed rows after composing against the current tree).

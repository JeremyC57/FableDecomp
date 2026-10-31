# Naming pass wave 12 (2026-10-31): 357 landed rows respelled (+33 funclet dependents rechecked)

Inputs: brief `rebuild/candidates/manual/naming/briefs/BRIEF_W12.md`, QUEUE_W12.tsv (1,072 rows). Every final spelling is in
Ego_r.pdb and/or FableWin.pdb unless noted (libpk descriptive names per the owner rule). Overlay composed from the CURRENT repo by
`scripts/compose12.py ov_final` (repo state: logs/repo_state_final.txt -- reloc_bindings.tsv 3126522 B 22:30:49Z,
build_candidates.ps1 16216548 B 22:28:09Z; baseline index state logs/repo_state_index.txt + idx/delta.jsonl for the 36 rows
landed/changed after it).

Sub-wave 1 (single-callee families), 245 rows: GetNodeAt / IsAnimCombinationAnim / GetHealthPercentage /
GetFirstCombinationAnimFromType (h12_s1a), PKCTRRedblackTree dtor 0x00C19250 in 33 libpk callers (h12_s1c; not the queue's
CPKListImpl: vtable 0x012B2F0C store + ".\PKCTRRedblackTree.cpp" 0x012B2E98), CAnimInit::Init / GFIntToWideString /
AccurateNormalise (h12_s1b), IsStopping IBE 45+4 (fam_isstop, fam_base isstop, h12_s1d), GetScriptThing QBE 30 (fam_decl gst, incl.
CThing_Retail->CThing), GetDefGlobalIndexFromName 10 (fam_base gdi2), IsActiveThreadTerminating 30 (fam_base isat / sbisat / cast
strategy + fam_free isat + h12_t1 test stubs), GetPVillage 57 (fam_free gpv, fam_gpv), ScriptedThingBase / N_006e7b40 13 (h12_s1e).
Sub-wave 2 (chunk rows), 118 rows: TeleportHero* const + g_pMainGameComponent, IsVisible private const member, 0x00A6CBB0 ->
CPhysicsMesh::GetSingleInstanceCollisionSet (rename) (h12_s2a); IsAbleToFire ABE, RemoveMode K / class owner (4 rows),
CModeBase slots OnActivate/OnDeactivate, 0x0088F620 bool callee (h12_s2b); CMemoryArea/CMemoryAreaChunk nested in
CMemoryAllocatorVariableSize with PDB access, 0x00A658E0 = CMemoryAreaChunk::FreeArea (rename) (h12_s2c); CIsHornTaskWithThisHorn
const functor with PHorn member (h12_s2d); CGameEvent(long) init-list ctor (h12_s2e); 97 queue-wide access/const-only PDB respells
(fam_auto: GetThingInPrimarySlot, IsPerformingAnyAction, GetString, HasMode, PeekWorld, ...; h12_t2 test stub).
(Rows touched by several stages counted once: 357 distinct.)
Sub-wave 3: only its access/const-only PDB rows (inside fam_auto); the rest deferred. Sub-wave 4: DECISIONS_W12.tsv (10 items).

Recheck: final 390/390 PASS (3 MATCH, 387 RELOCATION_MATCH) from ov_final incl. 33 unwind funclets of the libpk parents
(logs/recheck_final.log); per-stage logs logs/recheck_s1a/s1b/s1c/s1e/s2a/s2b/s2d/s2f/s2t.log.
Bindings (BINDINGS.tsv, runs/reloc_bindings.after.tsv): 129 RETIRE, 34 ADD, 20 KEEP; bindings_multi_check 0. Non-PDB ADDs are
class-rename side effects (CVillage->CTCVillage, CThing_Retail->CThing members), the CPhysicsMesh static, and the libpk
PKCTRRedblackTree dtor.
Renames: RENAMES.tsv (00a6cbb0, 00a658e0; src + test each). Labels: LABELS.tsv (19). Deferred: DEFERRED.tsv (30 items).
GOTCHAS amendment proposed: lines 274 / 366 "0xA14440 C3DVector::Normalise" -> AccurateNormalise (Normalise is the inline form).

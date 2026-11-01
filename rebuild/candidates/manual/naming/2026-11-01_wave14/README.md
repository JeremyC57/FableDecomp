# Naming pass wave 14 (2026-11-01): 160 landed rows respelled to the resolved wave 13 decisions

Inputs: brief `naming/briefs/BRIEF_W14.md`, `2026-11-01_wave13/DECISIONS_W13_RESOLVED.tsv` (owner rule "recreate retail
faithfully"), `naming/ESTABLISHED_SPELLINGS.tsv`, QUEUE_W14.tsv (the c646 / c651 / c653 queue rows plus one row per decided item).
One Opus 5.5 agent authored the overlay in `$S/naming/w14` (scripts/ here: compose14.py recomposes every stage on fresh copies of
the current repo; fam_*.py per family; hand14/ the per-row hand stages); the coordinator copied ov_final, applied the one RENAME
(git mv + build_candidates.ps1), BINDINGS and LABELS, and rechecked every changed row against HEAD: **160/160 (1 MATCH, 159
RELOCATION_MATCH) + PASS** (`logs/coordinator_recheck.log`; the agent's own run `logs/recheck_final.log`, also 160/160).
`bindings_multi_check.py` 0. Name-only: no byte changed anywhere. VIRTUAL_SCREEN_* and PMainGameComponent stayed gated.

Families, committed in this order (row lists `<fam>.rows.txt`):
1. gw (47 of 48): `?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ` 0x008FCFC0; 20 rows whose class sits on a flattened engine
   header carry a leading empty `class CAIStateGroupBase { protected: CWorld& GetWorld() const; }` base (DECISIONS_W14_RESOLVED:
   accepted, header debt until engine/CAIStateGroupBase.h carries the class). 00972590 deferred (free __fastcall Cleanup stand-in).
2. gcwf (16): `?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ` 0x004167C8 reached via g_pMainGameComponent, CTCBase::PeekComponent
   0x00686D60 or the CAIBrain component at +0x14; 0x0049D870 bound as `?GetFrame@CWorld@@SIJXZ` (0071d940, 00699560 respelled).
3. vcs (15): the vector<CCharString> destructor 0x00414EA0 from an STLport `std::vector<CCharString, allocator<CCharString> >`
   specialisation; 0x0049B8D0 now `??$TransferVector@VCCharString@@@CPersistContext@@...` (hand14/h14_vcs.py). 19 rows deferred
   (10 would change bytes, the rest need re-authoring: DEFERRED.tsv).
4. cvc (1): 0070c770 on `engine/CIVCountedPointeeBase.h`, slot 1 = Delete, stand-in Validate removed.
5. queue (53): EventScan const (2); GetType / GetDeviceType / GetKey return EInputEventType / EInputDeviceType / EInputKey and
   SetPlayer takes unsigned long (23); IsKeyboardEventInQueue const (CInputEvent&), IsPerformingAnyAction const (5);
   `?GetWorld@CThing@@IAEAAVCWorld@@XZ` 0x004C7AA0 (5); `?Initialise@CCombatActionBase@@UAEXXZ` virtual with a real vptr (20).
6. anon (28): the 0x0097ABF0 comparator family's `operator()(const CThing*, const CThing*) const` (25); 0097adf0 re-authored as the
   STLport `std::__push_heap<CThing**,int,CThing*,CThingCompare...>` instantiation (file renamed to std_pushheap_0097adf0);
   008e8a90 / 008e8b80 / 008e7e10 on the anonymous-namespace `CIsImposterHit` (`CIntelligentPointer<CThing>`).

Bindings (BINDINGS.tsv): 73 RETIRE, 4 ADD (`?SaveToScript@CThingManager@@` 0x00522600, `??0?$vector@VCCharString@@...@I@Z`
0x0057FCDD, the `_Rb_tree::_M_erase` self-call of 007ae0e0, `??RCIsImposterHit@?A0x4c72bf35@@QBE_NABV?$CIntelligentPointer@VCThing@@@@@Z`
0x008E8A10), 6 KEEP (5 old spellings still named by unedited rows; `?g_pWorld@@3PAVCWorld@@A` at 0x013B86A0 in 0095f9a0's
constructor is the gated PMainGameComponent family). Labels: `ghidra_out/labels_naming_wave14_2026-11-01.tsv` (7). Catalog: 6 names
+ `tools/sync_catalog_names.py` (7 renames, 15 manifest updates).

Decisions: DECISIONS_W14.tsv (agent) -> DECISIONS_W14_RESOLVED.tsv (coordinator, 2026-11-01, under the owner delegation "do what
retail did"): IS_THING_ALIVE 0x006961F0 is an ICF-folded body, each caller spells its own layout-proven instantiation (0x0097A630 =
CThingCreatureBase); CThing::GetWorld from CCreatureActionBase is modelled with `friend class CCreatureActionBase;` (PDB IAE, 0x00692C10
is Ego_r CCreatureActionBase::GetWorldMap); the empty-base CAIStateGroupBase model is accepted as header debt. Wave 15 = those two
respells + DEFERRED.tsv's re-authoring rows (the 10 vcs byte-changers, LowerBound's shared CVectorMap model: 244 + 103 rows).

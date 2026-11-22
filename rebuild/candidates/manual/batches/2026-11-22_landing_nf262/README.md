# Landing nf262 (Account A, 2026-11-22): GPLandscapeRenderer L129 units 3-4 (4 rows), with the AllocVirtual / FreeVirtual calls

Agent NF262 (L129 units 3 and 4; the rest planned; 0x009B0AC0 not attempted). Hand-back: `pending/2026-11-22_nf262_handback/` (NOTES = the
plan for the 14 remaining spellings on 25 rows).
- **Unit 3 (`NativeLandscapeRenderer`: 0x00BFBE40 / 0x00BFBF40 / 0x00BFC040):** `?g_LandscapeRenderer@@3PAUNativeLandscapeRenderer@@A` ->
  `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A`; the stand-in renderer type also carried stand-in names for its
  calls, so 0x00BD9AF0 -> `?AllocVirtual@CEngineLandscapeMemoryManager@@QAEPAXJPAVCMovableResource@@@Z` and 0x00BD9B90 ->
  `?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z` (both PDB name sets + the table already binding them there). On the landed
  GPLANDSCAPERENDERER opt-in and CEngineLandscapeMemoryManager.h's retail branch; each object differs from HEAD in exactly those three
  symbols (defined symbols identical, no pick-any copy, no vtable). Link: 0 duplicates (control 1).
- **Unit 4 (`gEngine`: 0x00BDFA10):** only the global's relocation changes.
- Table: four stand-in lines deleted; 0x01436EA8 16 -> 14 table lines, 0x00BD9AF0 6 -> 5, 0x00BD9B90 3 -> 2.
- **Recheck** (`logs/recheck.log`, landed tree): the 4 changed rows + the spelling witnesses 0x00BFC140 / 0x00BF6530 / 0x00BEB4D0 / 0x00BEAE50
  and the callees 0x00BD9AF0 / 0x00BD9B90 (10 rows): 10 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT. 0x00BD9B90 (unchanged) first graded DIFFER
  100v97: recheck.py forced `/Oy-` onto its add-only `CompilerFlags = '/d2inlT114'`; regraded with its own flag only it is RELOCATION_MATCH,
  and recheck.py now adds `/Oy-` only to an entry that spells a full optimisation set (15 add-only entries were affected).

## Coordinator rulings on NF262's questions
- **D1:** unit 3 with its two call spellings as one unit: accepted (fewer spellings at all three addresses).
- **D2:** the GB rows 0x00BF36A0 / 0x00BF36E0 / 0x00BF9920 (class operator deletes with placeholder leaves and wrong catalog names; FableWin
  `??3CEngineLandscapePatch@@SIXPAX@Z` / `??_V...SIXPAX@Z`, Ego_r only the placement forms, same bytes): queued as their own item, the global
  moving with their self-renames.
- **D3:** 0x00BDFA10's global move on top of its row-local `CEngineLandscapeMemoryManager : public CEngineLandscapeMemoryManagerLayout`
  (HEAD debt; the FreeVirtual spelling is the PDB one either way): accepted; the model's port queued.
Slip (a `sleep 0` in one background launch line): noted. 0x00BEB4D0 still binds `?DAT_01436ea8@@3PAEA` at 0x01436EA8 (queued with the rest).

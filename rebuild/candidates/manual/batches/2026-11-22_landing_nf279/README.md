# Landing nf279 (Account A, 2026-11-22): the CEngineWaterBackgroundSubPatch dtor unit (4 rows, 1 test, 1 header)

Agent NF279 (entry 1 done; entry 2: three rows rejected on measurement, two held; entry 3 not started). Hand-back:
`pending/2026-11-22_nf279_handback/`. No table or CompilerFlags change.
- **0x00BFD480** self leaf `??1CEngineWaterBackgroundSubPatch@@QAE@XZ` -> `UAE` (both PDBs; its only caller 0x00BFDC90 already binds UAE), on the
  shared retail CMovableResource, the water header and the retail CEngineLandscapeMemoryManager branch (the nf276 0x00BFDE20 shape). Leaf 144 = 144,
  bytes and relocations equal. The TU now emits `??_G` (38 bytes, pk EQ 0x00BFDC90) and a 4-slot `??_7` that vtchecks against retail 0x012A8334
  slot for slot (slot 3 Verify 0x009D5100 has no landed definer: queued).
- **0x00BFDC90** (`??_G`): a fold of 0x00BFD480's file by path (objsec ALL_EQ to 0x00BFD480's object); HEAD's 1-slot tables, `Construct_00bfdc90` and a
  placement-new copy gone; one identical strong `??1` (the accepted equal-copy LNK4006).
- **0x00BFD5D0 / 0x00BFD390** leaf-only self renames from stand-ins (`CEngineWaterBackgroundSubPatch_RelocateData` shell, `?Can@C@@QAE_NH@Z`) to the PDB
  slot spellings `?RelocateData@CEngineWaterBackgroundSubPatch@@EAEXJPAX0J@Z` / `?CanRelocateData@CEngineWaterBackgroundSubPatch@@EBE_NJ@Z` (both PDBs,
  RTTI slots 1 / 2, no call site); objects ALL_EQ with the leaf renamed; 0x00BFD5D0 also on the shared CIndexBuffer.h. Without them the new table
  would add a second spelling at each (the 0x00BFDDB0 precedent).
- **Header:** add-only opt-in `FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_VIRTUALS` in `engine/CEngineWaterBackgroundSubPatch.h` (virtual dtor, class
  delete, two private overrides); whole-tree equiv_check 57,125 rows, 0 differ.
- **Test** 0x00BFD480 on the shared headers (the NF277 harness): LNK2019 alone, PASS with the row or the fold object.
- **Link:** both orders 1 LNK4006, slots 1 / 2 resolve; at HEAD the UAE dtor did not resolve at all.
- **Recheck** (`logs/recheck.log`, landed tree): the 4 rows + every includer of the water header (0x00B6E010 / 0x00B6FA40 / 0x00B70230 / 0x00BE6090 / 0x00BE7BE0 / 0x00BFD290 / 0x00BFD3B0 / 0x00BFD670): 12 PASS (11 RELOCATION_MATCH, 1 MATCH), 0 RELOC_CONFLICT.

## Coordinator rulings on NF279's questions
- 0: form 2 for 0x00BE8E60 / 0x00BFBD80 (Verify declaration-only) is **held**: they are the only definers of `?Verify@CMovableResource@@UBEKXZ`, so
  form 2 would leave a reference the HEAD link resolves (to the wrong 3-byte body) with no definer at all (the nf268 q12744 rule: nothing may newly
  be left undefined). The real fix (folds onto the dtor rows 0x00BE6090 / 0x00BFB590 after their ports) and a 0x009D5100 row for authoring are queued.
- 0b: 0x00BFD390's row-local `class CEngineLandscapeMap` (the class header is quarantined) is accepted on the 0x00BFDDB0 precedent.
- 1 / 2: entry 1 lands as one set, with the two slot-row renames.
- 3: NAME_FIXES queued (0x00BFD420 is `??0CEngineWaterBackgroundSubPatch@@QAE@PBVCEngineLandscapeMap@@@Z`, catalogued as a CLandscapeLayerMesh ctor;
  0x00AC1F60 `?OnPostRelocation@CIndexBuffer@@QAEXXZ` v its self row's stand-in `?M@C00ac1f60@@QAEXXZ`).
- Rejected (accepted): 0x00AE04E0 / 0x00AE0540 / 0x00AE05A0 define no CMovableResource symbol (no vtable emitted; ruling 2's list was a source grep).
Slips (HEAD-side sources prechecked after their grades; several grades sharing one outer `timeout 900` line): noted.

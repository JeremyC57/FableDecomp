# 2026-11-16 landing nf40: name-fix round 4, NF40 (83 landed rows, 47 renamed)

Owner: "Continue the naming fix queue with two agents until complete". Round 4 continues NF38's half: tags land1115ga ..
land1116p23, 217 entries. Brief and records: `naming/2026-11-16_agentNF40/`.

## Queue (217 entries, counted from APPLIED.tsv)

| Status | Entries |
|---|---|
| done | 29 (71 binders respelled) |
| done-already | 1 |
| partial | 3 |
| blocked | 19 |
| rejected | 11 |
| carry | 154 |

- **partial:** 0x00467553 has no STLport `vector<bool>` model. 0x004E9642, 0x00466B37 and 0x004E7E36 are left for entries 67 / 68.
- **blocked:** port, authoring, header-wide and ICF reasons; one per entry, in its note.
- **rejected:**
  - 144: the body is GiveHeroGold.
  - 147: both PDBs put the class in an anonymous namespace.
  - 131-137 and 191: address-suffixed placeholder names.
  - 183: GameCODA SDK only.
- **carry:** mostly the large binder families: STL map/list entries 54-64, 71-86 and 154; data globals 40-53 and others; the
  Sound/Bank group 186-206.

## Rows (83; no test changed)
- **RENAMES section 1:** 47 self renames, including:
  - 0x00412DB0 / 0x00412E80, the named `TransferVector<T>` of CPersistContext.h;
  - 0x00C1BFF0 C3DVector::Copy (descriptive);
  - 0x009A14E0 CStreamingFontBank::GetStringExtents;
  - 0x0066C810 CRegionMinimap::GetWorldCoordinatesFromTGAPosition;
  - 0x00AEAAA0 NParticleEngine::GetGraphicBank;
  - 0x00654AF5 IS_THING_ALIVE<CThingObject*>;
  - 0x009A0EF0 CStreamingFontBank::SetUseFixedFunction;
  - eight STLport library instances, descriptive-by-library.
- **RENAMES section 2:** 142 caller respellings.
- **Recheck (`logs/recheck_part0..1.log`):** all 83 graded from the repo after landing: 3 MATCH and 80 RELOCATION_MATCH, every test PASS.
  No landed funclet includes a changed row, and funclet_includes_check reports 1,355 checked, 0 broken.
- **Headers (9, opt-in only):**
  - `FABLE_RETAIL_<CLASS>_TRANSFER` Transfer declarations for CFrontEndDef, CSpecialAbilitiesTurncoatSpellDef,
    CSpecialAbilitiesDrainLifeDef, CSpecialAbilitiesEnflameDef, CCreatureGenerationFamilyDef, CCombatTypeDef and CHeroAbilityDef.
  - CHeroAbilityDef's block also types DummyObject as the PDB's CDefIndex: 4 bytes, and the header's `sizeof == 0x9c` assert
    holds.
  - `FABLE_RETAIL_CREGIONMINIMAP_WORLDCOORDS_OUTOFLINE`.
  - `FABLE_RETAIL_CSTREAMINGFONTBANK_SETUSEFIXEDFUNCTION`.
- **Bindings:** rebind run, bindings_multi_check at baseline.

## Open (NOTES.md)
- **Callers still to follow a rename:** 0x0046457D and 0x00625630 (for 0x00464841); 0x00466B37 and 0x004E7E36 (for 0x004657F2).
- **0x013D2E08 GParticleGraphicBank** (descriptive): 0x00AEAA80 still calls it g_value.
- **0x013BDA40** has two names.
- **CDefinitionManager.h:** the GETDEF_EXPRESSIONDEF non-template GetDef exists in no PDB.
- **0x00F333B0:** its element type is disputed.

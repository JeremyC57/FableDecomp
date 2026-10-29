# Naming pass wave 8 (2026-10-29): 385 rows / 565 files respelled

Decisions (DECISIONS_W8.tsv, 512 addresses; every spelling checked against both PDB name lists, `_W` landed as `G`):
253 proven, 19 proven in one PDB, 117 PDB spelling with identity unverified, 121 probable, 2 rejected. Inputs: chunks
564-565 NAME_FIXES (44), ChatGPT pass 9 NAME_FIXES (465) and its three conflicts (25 rows). Outcome: 45 applied, 195
already final, 3 decided but not applied, 267 open (90 proven not reached, 117 identity checks, 60 probable).

Conflicts settled (the pass 9 side was right in all three):
- 0x004A96F0 is `CIntelligentPointer<CThing>` (vtable 0x01238C6C RTTI), 0x0056C860 is `<const CThing>`; 0072cba0 and
  0071c9f0 now build `CIntelligentPointer<CThing>` (Ego_r CTCHeroMorph::IdleParticleEmitters).
- CBankFileAsyncData: 0x009D5530 = GetData, 0x009D5580 = const PeekData (Ego_r and FableWin member order; 7 retail
  callers of 0x9D5530, one of 0x9D5580). The two rows' files were swapped by name.
- vector<bool>: 0x006860F0 is STLport `_M_fill_assign`; 006821e0 reaches it through an inline `assign`; `resize`
  rebound to 0x00467686 (structural evidence: the PDBs carry the Dinkumware bit-vector, retail runs STLport vector<T>).

Families (scripts/, composed by scripts/tools/compose8.py: hand scripts c8a / sn8a / sn8b, then families on their file
lists): opy8 (88, CCharString::operator+= const returns), att8 + att8j (51, AttachToThing(CThing&)), cbo8 (28, CBaseObject
protected ctor / operator=), atc8 (27, AddTC const CTCParamsBase*), i2w8 (20, GFIntToWideString), sm8 (28), aisg8 (53,
CAIStateGroupBase protected PeekPlayerManager / GetPlayerManager / GetPWorldSeed), rtd8 (21, GetRenderTargetDimensions),
sca8 (20, SetCurrentAction(const CCreatureActionBase&)), pgd8 (19, CPlayerGui::PeekGuiDef), sn8a (12), sn8b (19).
No header changes.

Bindings: 4 RETARGET, 14 added (incl. 2 anonymous-namespace hash pairs), 32 retired; NO-BIND stand-in not applied.
Catalog names corrected from LABELS.tsv (6 catalog rows). RENAMES.tsv: the 009d5530/009d5580 swap applied; 5 file-rename
proposals for defining rows not respelled yet. DEFERRED.tsv: 19 entries (CDefinitionManager const family, data-symbol
globals, nested-class items, 3 pre-existing RELOC_CONFLICTs: 005907ab, 007eb390, 0092b110).
recheck.log: the coordinator's full recheck of the 385 rows after applying.

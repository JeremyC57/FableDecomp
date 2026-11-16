# NF40 (round 4) notes

## Descriptive names (no PDB spelling; marked per the made-up-names rule)
- 0x00C1BFF0 `C3DVector::Copy() const` returning `_CVECTOR` (queue claim keeps the descriptive "Copy"; the `_CVECTOR` return is proven by the GameCODA source-position API).
- 0x013D2E08 `GParticleGraphicBank` (the pointer NParticleEngine::GetGraphicBank 0x00AEAAA0 dereferences); neither PDB names it. Follow-up: 0x00AEAA80 SetGraphicBank still spells it `g_value`; respell it to match (one address, one name).
- STLport library names, descriptive-by-library (the exact new symbol is in none of egor/fablewin/egod_pdb_names.tsv; grep of each symbol): 0x0049B1E0 / 0x0049B240 random_shuffle, 0x006E5360 _Destroy, 0x00A484B0 / 0x00535000 __copy, 0x00632D30 _Rb_tree::_M_insert, 0x00632C90 / 0x00A49F60 insert_unique.
- Exact PDB spellings (found in the name sets): 0x00A4A670 map<unsigned long,CCharString>::operator[], 0x0074CBC0 vector<CDefIndex>::at, 0x00568850 vector<float>::push_back.

## Header-wide issues found
- engine/CDefinitionManager.h FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSIONDEF declares a non-template GetDef that no PDB holds; 0x005F81BE now uses the template GetDef<CExpressionDef> (GETDEF_GLOBAL_BODY) instead.
- 0x00F333B0 (named TransferVector instance after CScriptDef::Transfer): its row says `long`, the CPersistContext.h comment says CDefString. Unresolved.
- 0x013BDA40 (glyph adjustment table) has two spellings: `g_pGlyphAdjustments` (0x009A14E0, kept) and anonymous-namespace `glyphTextureCacheGlyphAdjustment` (0x009A25D0 / 0x009A3640). The coordinator should pick one.

## Out-of-queue targets whose self rows now disagree with corrected caller bindings
- From recheck_final.log `WARN RELOC_NAME` targets minus the RENAMES section-1 addresses: 0x00468E10 (Transfer<NInventory::EInventoryIdentifier>), 0x00472A8D (TransferVectorOfSubComponents<CDialogueLayerDef>), 0x00479187 (Transfer<EDialogueLayer>), 0x004792CF (Transfer<std::list<COpinionTransientOffset> >), 0x009ADE10 (catalog GetPDefFromGlobalIndex; entry 127, still carry).
- From RENAMES section 2 targets whose catalog name is a stand-in: 0x004568BC / 0x004522F4 (catalog `_global::PersistTransfer_*`), 0x00465820 / 0x00465852 (GFSerialise rows).

## Callers that still do not follow a self rename (verified: callers.py + probe of the landed rows, work/probe_uncov.tsv)
- 0x00464841 TransferVector<unsigned long> (renamed outside the queue): 0x0046457D NUISystem::CUILocaleGraphicsDef::Transfer and 0x00625630 NUISystem::CUIStateDef::Transfer still bind non-template `?Transfer@CPersistContext@@...` stand-ins (they did before the rename too).
- 0x004657F2 TransferVector<CDefString> (entries 67/68, partial): 0x00466B37 CBrainDef::Transfer (`?Transfer@CPersistContext@@QAEXAAV?$vector@VCDefString@@...`) and 0x004E7E36 CSkeletalMorphDef::Transfer (`?Xfer_004657f2@...`) still need ports, besides 0x004E9642.
- 0x00466A47 TransferVector<long> (entry 8, partial): 0x00467553 (vector<bool>, below).
- Every other renamed row: all landed callers bind the new symbol (probe2.tsv / probe_uncov.tsv / new q objects); 0x00C1BFF0 and 0x009A0EF0 have no landed caller (callers.py sites 0x00C1C5B2 / 0x00403302 are in unlanded rows); the def Transfer rows have no direct call sites.

## Follow-ups this round opened
- The named-TransferVector family (stand-in `Transfer(const char*, vector<T>&)` rows such as 0x0049B8D0 CCharString, 0x00569FE0 CQuestScoreRecord) ports like 0x00412DB0 / 0x00412E80: explicit instantiation of CPersistContext.h TRANSFERVECTOR_NAMED_BODY, adding FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE where retail calls clear() out of line.
- 0x00410F30 (entries 140/141): CArray<char>(unsigned) on engine/CArray.h compiles to a forwarder (DIFFER 19v77); needs a header opt-in that expands the base vector(size_type). Lock 0x00410F30 is held with no change (attempt kept in work/abandoned/).
- 0x00467553 (entry 8): vector<bool> has no STLport model in fable_containers.h.
- 0x007C5080 (entry 97): register-order DIFFER(73v73) after 3 grades; row unchanged.
- Self-contained tests that still test the old local model (PASS, but stale): 0x0066C810, 0x009A14E0, 0x00654AF5, 0x00AEAAA0, 0x009A0EF0.
- 0x00A58720 (entry 147, rejected): re-author as `namespace { class CTransformWorldCoordsToPlaneCoords }` (both PDBs: anonymous namespace, 3-reference constructor).
- recheck_final.log: the 6 WARN STANDIN_LEAF lines are catalog names that RENAMES.tsv section 1 updates; the WARN RELOC_NAME lines also cover the out-of-queue targets listed above, which section 1 does not update.

## Funclets
No landed funclet row includes any of the 83 changed rows. Funclets include their parent by relative path (`#include "../src/compiled/00/c0/<parent file>.cpp"`, e.g. 0x01210A00); the same fixed-string grep finds that funclet from its parent's file name (positive control), and finds nothing for any changed row's file name, nor any `parent @ 0x...` comment naming 0x00550940 / the GetDef rows / the map and tree rows / 0x0066C810 / 0x009A14E0.

## Rule slips (work/slips.txt)
- One Bash call exited 1 (a BRIEF.md grep chained with `head` on a wrong rc1.log path).
- One write under the repo: mkrc.py wrote /home/user/FableDecomp/work/pay1.json (relative path after os.chdir). Moved to my work/ at once; the script now uses an absolute path.

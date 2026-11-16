# NOTES (agentNF33 name-fix pass, 2026-11-16, tags land1011fj .. land1113fz)

## Result
- Queue: 522 entries (505 addresses). APPLIED.tsv: 24 done, 33 done-already, 32 partial, 1 rejected, 2 deferred, 430 carry
  (never reached; marked `carry` with the screen in the note). Order worked: the land1011fk..fq BINDERS_WRONG / SELF_WRONG entries
  first, PDB-proven first, then one batch of land1113fy entries that shared the same rows (CProcessedInput / CInputProcessGameBase),
  then a scripted close of the ALREADY_OK / NO_BINDER screens.
- 80 changed rows (rows/), all in recheck_final.log in one run: 8 MATCH + 72 RELOCATION_MATCH, every test PASS.
  RENAMES.tsv: 26 section-1 self renames, 71 section-2 caller respellings (old_leaf cells marked `inferred from the landed source` where
  the landed row had no mangled Leaf line; old_catalog_name copied from the catalog). Most grades on one row: 0x0075A530 (6).
- Changed tests (rows/<addr>_test.cpp): 0x0071A730 (the old test linked the free CTCMapwho_OnDie stand-in) and the four CTCCoopSpirit
  rows 0x0066FB10 / 0x0066FB30 / 0x0066FB40 / 0x00670000 (rewritten on the row's model; they link the row). Each keeps its catalog
  PassPattern line. All other tests are unchanged.
- One overlay header: hdr/engine/NPlayerGui_CDrawNPCName.h, opt-in FABLE_RETAIL_NPLAYERGUI_CDRAWNPCNAME_SHOWSCREENSHOTMESSAGE
  (declaration only; hdr/README.md).
- Deferred to NF34: row 0x0047EEE4 (held by NF34) still binds the free ?FormatDateTime@@YIXABU_FILETIME@@PAG1J@Z; it should bind
  ?FormatDateTime@CXBConfig@@SIXABU_FILETIME@@PAG1H@Z (engine/CXBConfig.h already declares it).
- Purity: no `__asm`, `_emit`, `register` or the purity-scanner keyword for frame-less functions in any row or test I wrote. Three copied rows (0x0068C7F0, 0x0068F180,
  0x0068F320) carry pre-existing COMMENTS naming `__forceinline` (no use in code); untouched.

## Self renames (RENAMES section 1)
- New shared-header rows: 0x004BBFF0 GetFullScale, 0x0071A730 RemoveFromMapwho, 0x00747030 CTCStealth::SetActionPermittedToSneak,
  0x00ABFDC0 C3DMeshFileMaterialExtChunk::GetSelfIllumination, 0x00810F40 CEventHitBase::GetHitParams, 0x006B2BD0 CEnvironment::GetRainStrength,
  0x006B5D80 ~CBlendedEnvironmentTheme, 0x004379E0 CPlayerGui::ShowScreenshotMessage, 0x006A7B90 CThingFilter_IsHuman::operator(),
  0x00A0D980 CXBConfig::FormatDateTime, 0x006DE8B0 CTCTrophy::OnShowTrophyInitialise.
- Access / signature fixes on a local model: 0x006DDCD0 / 0x006DD7E0 / 0x006DEE80 (CTCTrophy, private), 0x006B0610 (protected const),
  0x00812340 (CThing&), 0x0074E500 (bool const).
- No shared header (local model, allowed): 0x009D47C0 ~CBankCreatorException (only reference: ThrowInfo 0x013693B4 pmfnUnwind, scan of
  the image), 0x0055D4C0 CTCPhotojournalCapture::TakeTheBloodyScreenshot, 0x0066FB10 / 0x0066FB30 / 0x0066FB40 / 0x00670000 CTCCoopSpirit
  (Ego_r donor order AddExperience, AddScore, GetScore, ResetScore, GetSpiritDef .. GetMeleeTargetRange; Ego_r layout).
- Descriptive-by-library (STLport 4.6; both PDBs are Dinkumware): 0x0049B760 vector<CCharString>::resize(size_type), 0x00464931
  vector<unsigned long>::resize(size_type, const unsigned long&), 0x009D3DF0 vector<CBankFileEntryUpdateData*>::resize(size_type, T const&),
  each an explicit instantiation over stlport/fable_containers.h; the element types are the ones the landed binders bind.
- The catalog module of 0x009D47C0 is `_global`; RENAMES gives CBankCreatorException.

## Gaps (partial)
- 0x00A358F0: 0x0075A530 keeps the stand-in ?GetMatrixFromRHSet@@YIABVC3DMatrix@@..; three respellings onto
  C3DMeshObject::GetPositionMatrixFromRHSet (header TransformVector, a named CMatrix3x4 local, a const-ref temporary) all DIFFER in the
  later BoundingRadius x87 scheduling.
- 0x007243C0: 0x00725C40 declares UpdateVelocityForImpassability on a flat local CTCPhysicsControlled; binding the protected
  CTCPhysicsStandard member needs the row moved onto engine/CTCPhysicsStandard.h.
- 0x00687CF0 / 0x00687D00: 16 / 2 binders declare the method on the derived class (CInputProcessMain, *Screen, *Minigame, ..);
  they need the base modelled through engine/CInputProcessGameBase.h. 0x00A0D390: 0x00689130 (NativeInput projection stand-in) left.
- 0x00464931 / 0x009D3DF0 / 0x0049B760: binder 0x009CEAE0 CBankFile::AllocateBuffers (one dense namespace X_009ceae0 CArray stand-in) and
  0x00B21A00 (local template CArray) left; moving them onto engine/CArray.h is a re-model.
- 0x006B5D80: 0x006B6340 (deleting-destructor stand-in struct T, catalog CLandscapeBackgroundPatch) still binds ?Dtor_006b5d80@T.
- 0x004DD2B1 and the land1011fm NF32 PORT/AUTHORING/CARRY entries: blockers unchanged, not attempted. land1011fn (GFSerialise* templates):
  authoring (each self row is a ~170-line stand-in).
- Related, outside the queue: 0x00657280 is landed as CAIStateGroup_BuyFromSeller::BoughtSomething; both PDBs and the header's donor
  order make it NPlayerGui::CDrawNPCName::ShowScreenshotMessage (0x004379E0 now binds that symbol).

## Rejected
- land1011fk 0x00464931 (by-value QAEXIK): retail STLport takes const T&; the const& form was applied under land1113fy. The fk
  0x009D3DF0 by-value spelling is rejected the same way (entry partial, REJECTED.tsv line).

## Made-up names
None. 0x00875FB0 resolved to the PDB NCameraModes::DrawSquare (was the made-up EditorDrawSphere in 0x007617A0).

## Checks after the first draft (advisor review)
- Leaf collisions: none of the 26 new section-1 leaves is bound to another target in reloc_bindings or used as another landed row's Leaf.
  The target scan found 0x00845350 CCreatureAction_ShowTrophy::Initialise binding 0x006DE8B0 as the descriptive
  ?SetShowingCreature@CTCTrophy@@QAEXPAVCThing@@@Z; it now binds ?OnShowTrophyInitialise@CTCTrophy@@QAEXAAVCThing@@@Z (row 80).
- 0x00464931: all 15 landed callers compiled; every one passes unsigned long, so the vector<unsigned long> self name is uncontested.
  8 bind it; 7 keep CArray<unsigned long> stand-ins (0x00A62120, 0x00AC5080, 0x00B21A00, 0x00B21C90, 0x00B3C4B0, 0x00F3A710, 0x009CEAE0).
- 0x00875FB0 (CLAIM): both PDBs name ?DrawSquare@NCameraModes@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@1M@Z; the address rests on
  the manifest label, the NCameraModes::CTools::GetMeshExtent neighbour at 0x00875F00 (bound so by the landed 0x00876CA0), the caller's
  argument shape (info, position, vector, float) and the body (two AccurateNormalise calls building two axes, two virtual draw calls,
  no loop: a planar square, not a sphere). label_trust.py has no trusted landed row for it.
- 0x009CC750 moved to partial/PORT (member respelled only in a local model of a class with engine/CBankStateBlock.h).
- ALREADY_OK spot-checks by compile: 5 (0x0066FB10, 0x006B2CA0, 0x0041A600, 0x005BEB57, 0x004083E0); 1 of 5 hid a wrong binder
  (0x00581B33 bound GetSaveDir to 0x0041A600; fixed). The other ~27 ALREADY_OK entries marked done-already rest on the binding table
  plus the self row Leaf only.

## Rule slips
- One compile (0x006DE8B0, first try) hung on a header `#error` until the 240 s timeout before the cpp precheck existed (g.sh now
  prechecks every file with cpp and refuses on #error).
- One Bash call was refused by the harness (a long sleep); no other non-zero exit.
- 0x00747330: its global stand-in `struct CTCGraphicAppearance` was turned into a namespaced local model
  `NGraphicAppearance::CTCGraphicAppearance` (a class with engine/NGraphicAppearance_CTCGraphicAppearance.h) to bind GetFullScale; the
  row was not moved onto the header. Treat it as a local-model respelling of a headered class (PORT still to do).

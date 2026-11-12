# NOTES (agentNF31 name-fix pass, 38 queue entries, tags land1011eu .. land1011fd)

- Result: 46 changed rows in rows/ (29 self-row renames in RENAMES section 1, 28 caller respellings in section 2, incl. the repair-pass 0x009FE550) and one rewritten test
  (rows/004c7580_test.cpp). FINAL_GRADES.txt: all 46 regraded once more in one loop with the final overlay -- every one MATCH or
  RELOCATION_MATCH -- and the rewritten test PASS. STATUS grades_used excludes that final loop (+1 each); the most on one row was 3+1
  (0x0094B400, 0x004C4160, 0x008FCFA0, 0x008137B0). No row has a catalog CompilerFlags entry.
- Entries: 25 done, 12 partial, 1 rejected (0x01436E9C). Three overlay opt-ins (hdr/README.md).
- Tests: only 0x004C7580's changed (the old one called the stand-in through inline asm; the new one links the row object through
  engine/CThing.h and still prints the catalog PassPattern OK_0x004c7580 as well as 004c7580_TEST PASS). Every other changed row's
  catalog test is a self-contained model (or a verbatim copy of the old source) that does not link the row; those still spell the old
  stand-in names in their own local code (00aab8b0, 00a69ad0, 005beb57, 004c7e50, 008d0250, 00883860, 00493bdd ...) and were left,
  as NF30 did, because respelling them would mean new local models of header classes.
- Correction to the queue: 0x013B8760 is NOT nameless. FableWin.pdb names `dynamic initializer for 'CStartupDefinitionManager::PClass'`
  (0x0100C6EC) and `dynamic atexit destructor for 'CStartupDefinitionManager::PClass'` (0x0101B611); the Anniversary publics carry
  ?PClass@CStartupDefinitionManager@@1V?$CCountedPointer@VCStartupDefinitionManager@@@@A. The queued g_StartupDefinitionManagerInstance
  (and the repo header comment "No PDB data name exists") is wrong; 0x004085F0 / 0x0043368D now bind PClass via the overlay opt-in.
  Left: 0x004336B0 (stand-in `g_pool_013b8760.Alloc`, semantically wrong body model) and 0x01228AF1 (atexit thunk on an
  AutoTinyThunkTarget stand-in; PClass is protected, so the dynamic-atexit body needs its own spelling) -- both need re-modelling.
- Descriptive-by-library (STLport; both PDBs are Dinkumware builds): 0x00883860, 0x00493BC2, 0x00493BDD, 0x005F8150, 0x007C2980. Only
  types the matched callers bind are used; 0x00493BDD's clear call 0x00493911 now binds ?clear@?$_List_base@PBVCMessageEvent@@..@std@@QAEXXZ
  while the 0x00493911 row is still the placeholder _global::StdList_Clear_00493911 (coordinator: one address, two spellings until that
  row is renamed).
- 0x00A69AD0's callee 0x00A69A80 now binds the landed row's own ?ReleaseTexture@CManagedTextureData@@QAEXXZ (was ?Close@CFrameData@@).
- 0x00982210's TU now binds 0x006924B0 as ?GetRandomAnimFromName@CCreatureActionBase@@KI.. (Ego_r 0x00518E00, the header's), as 0x009810B0
  does. Other landed binders of 0x006924B0 still carry stand-ins (GetAnimationFromName, GetTableAnimation, FindAnimation,
  GetRandomLoadedAnimation, GetAnimationEntry, GetRandomLoadedAnim: 0x0062DAD0, 0x0062D890, 0x0069F0F0, 0x00718650, 0x007E7FD0, 0x007ED450,
  0x00842A20) -- not in this queue.
- 0x008D0250's callee 0x008CFFE0 now binds the private ?InitFromThing@CGameScriptThing@@AAEXABVCThing@@@Z (Ego_r 0x00907010 / FableWin
  0x02AFE9F0); 0x008D0200 UpdateThingAttachment still declares InitFromThing(CThing_Retail*) (gap, not in queue).
- 0x007045F0's deleter store binds ?DeleteData@?$CCountedPointer@VCScriptThing@@@@SIXPAX@Z (0x007044B0); 0x00704A80 binds it as
  ?DeleteData@@YIXPAVCGameScriptThing@@@Z (gap).
- Partial entries and what blocks them:
  - 0x008FCFA0 PeekWorld: binders 0x009143C0 (h1_9143c0 free), 0x00940BD0 (Act_940bd0_A extern "C"), 0x00946630 (GetS_946630) are
    extern/free stand-ins whose bodies discard or misuse the result; 0x0092D600 declares GetPFrameWorld on a local CAIStateGroup_GoToBed
    with no CAIStateGroupBase base. Each needs re-modelling onto engine/CAIStateGroupBase.h.
  - 0x007046D0 SetImplementation: 0x00704A80 (CGameScriptThingManager ctor) models CCountedPointer / CScriptThing locally (non-template);
    the spelling needs CCountedPointer<CScriptThing>, i.e. a move onto engine/CCountedPointer.h + CScriptThing.h.
  - 0x004C7E50 GetScriptThing: 11 binders use stand-in classes (CThingNative, CThing_Retail, CThingRetail, CBaseObject_Retail,
    CGSIGetThingWithUID_Thing::WrapForScript, CScriptThing::MakeScriptThing, CBaseObject::BuildScriptThing, CThingAICreature):
    0x00891D50, 0x008976F0, 0x0089F910, 0x00904540, 0x008D0610, 0x00905130, 0x00893FF0 (source already says CThing; the reloc row is stale),
    0x0089BC50, 0x00891610, 0x008A9100, 0x008D0C50. Each needs the local CThing view replaced by engine/CThing.h.
  - 0x009FF220 UploadFrame(bool): 0x009FF6B0 (CFrameData_009ff6b0 + CompleteLoad / SetAsUnloadable / CBankFileAsyncData declarations not
    in the shared headers) and 0x009FF710 (local CFrameData / CResource / count-block models) need the move onto engine/CGraphicInfoFrame.h.
  - 0x006625C0 / 0x006625E0 GetNominalMeshRadius / Height: authoring (the self rows are CTCWeapon::GetAnimationSpeedValue stand-ins over a
    refcounted +0x70 pointer; the real body is a CDefPointer temporary read of the def's MeshRadius / MeshHeight). Caller 0x0084D910 is right.
  - 0x007F9C00 GetAllThingsInRoughArea: authoring (const member template of CThingSearchTools over STLport vector<CThing*> and
    CFilter_Bind3<..>; the row is a 161-line local model). Caller 0x007F9960 is right.
  - 0x00950C30 StartFlying: the full signature (CThingCreatureBase&, vector<C3DVector>&, C3DVector&, CCountedPointer<CCreatureActionBase>)
    needs the row moved onto engine/CAIStateGroup_JackDragon.h, and its callees (CModeDragonFlying ctor 0x008E1150, AddMode 0x0073ACC0,
    CCountedModePointer ctor 0x00718A00) carry stand-in spellings too.
  - 0x0099ED10 GetAsLowerCase: stand-in-class binders 0x009CCDF0 (CCharStringLowerView), 0x009EC5E0 (CRetailString), 0x009EB590
    (CCharString nested in X_009eb590) left.
  - 0x005EEDCF AddLearnedExpression: name proven (Ego_r 0x0057DAFE + FableWin 0x01EBECB4); the self row is a FableAddLearned_005eedcf
    local struct with a vector at +0x1BC; the retail CTCInventoryAbilities member at +0x1BC is not mapped in the shared header (Ego_r's
    Dinkumware offsets differ), so the header move is authoring.
  - 0x0073A680 CModeBase::GetWorld: 8 stand-in binders (0x008D7630 ModeIceTroll, 0x008E9850 GetGlobalComponents@CModeBase, 0x008D6580 /
    0x008EA500 CModeBase::GetWorld pointer forms, 0x008E7CF0 ModeBriar, 0x008DCB60 CModeJackOfBladesFlash, 0x008DC900
    NCreatureMode::CModeDragonStrafeBurninate::GetDisplayEngine, 0x008DDD70 NCreatureMode::CModeBase) declare it on local mode classes
    with no CTCCreatureModeManager::CModeBase base.
- 0x0099AF00: label check only. ??BCWideString@@QBEPBGXZ (landed) and the PDB's ??BCWideString@@QBEPB_WXZ are the same member (wchar_t is
  unsigned short in the retail build). Other binders with stand-in spellings (FableWideStringGetBuffer_0099AF00, AFileSupport,
  ExternalRetry_00999620, N_0099af00, ?GetBuffer@CWideString@@QBEPAGXZ from 0x0048F50C) are outside the entry.
- 0x00705170: the byte-identical next function 0x00705180 (Ego_r 0x00725070, the non-const GetPCarriedByThing) is still the stand-in
  CGuiControlListBox::GetCurrentSelectedEntryData (not in the queue).
- Local models: no new local model of a class with an engine header was added. Kept pre-existing local models where the shared header
  would change the bytes or needs a large move (0x008D0250 novtable CScriptThing base, 0x00410820 CDefPointer -- a converting operator=
  with the same inline body was added to that existing model -- 0x007324A0 CEnginePrimitive / CRGBColour, 0x005BEB57, 0x00934120,
  0x00967B40, 0x008D0730). Classes with no header got local models: CSoundEmitterLH (0x00AAC870 / 0x00AAB8B0), CTCTavernTankard
  (0x0094B400, the caller's model), CGameScriptThingManager (0x004C7E50), CEnginePrimitiveShadowedSpotLight (0x007324A0).
- Casts in stand-in callers: 0x00934120 passes `(const CThingAICreature*)PBrain->PCreature` (Ego_r CAIBrain::Creature is the
  CThingAICreature; the row's local CAIBrain types it CThing) and 0x008D0730 reads the carrier through `(const CThingRetail*)` (the row's
  flattened CThing view). Same bytes; both rows stay stand-in models.
- Made-up names: none new. The LHA::CEventPlayOptions SetFlag16 / SetFlag method names are the queue's descriptive ones (already in
  engine/LHA_CEventPlayOptions.h); CActiveSound member names are descriptive.
- `register` appears only in inherited prose comments (00934120, 009b05f0 mentions __forceinline in prose); no keyword added.
- Repair pass (verifier findings): 0x00A69B20's second landed binder 0x009FE550 (CGraphicInfoFrame::InitialiseFrame; reloc_bindings first
  caller of the wrong spelling ??4CManagedTexture@@QAEAAV0@ABV0@@Z) was missed at first. Its pre-existing local CManagedTexture model now
  declares `const CManagedTexture& operator=(const CManagedTexture&)` (the PDB const& return); regraded RELOCATION_MATCH, the object binds
  ??4CManagedTexture@@QAEABV0@ABV0@@Z and ?Release@CManagedTexture@@QAE_NXZ. Its catalog test is a self-contained model that does not link
  the row, so it is unchanged. No other landed source spells the non-const return. The row's local CManagedTexture model (header exists)
  stays as before; moving it onto engine/CManagedTexture.h is outside this entry.
- Repair pass: RENAMES section 1 for 0x00883860 / 0x00493BC2 / 0x00493BDD now carries the full std:: class in the module column and the
  method only in the name column (find<CThing*>, _List_base, ~_List_base); the template arguments are written out in full.

# Hand chunk 507 (CBaseAllocator .. CQ_PrisonEscapeRescueMotherScript::CMotherTorture): 29 of 30 byte-exact

All 29 matched rows grade RELOCATION_MATCH (0 plain MATCH, 0 retail_asm). Behaviour tests pass under g++ (tall.sh) and VC7.1/Wine (tall.py). vland dry run: 29 wins, no RELOC_IDENTITY. One RELOC_NAME note and four SHARED_HEADER_AVAILABLE false positives, covered below.

## Names

### Proven
- 0x00CABAE0 `CBaseAllocator::CBaseAllocator(TCHAR* = unsigned short*, IUnknown*, HRESULT*, BOOL, BOOL)`. Evidence: strmbase amfilter.cpp body; calls the WCHAR `CUnknown` ctor 0x00CACB80 as bound; landed CBaseAllocator rows (0x00CA9D00).
- 0x00CAC570 `CBaseAllocator::CBaseAllocator(CHAR*, IUnknown*, HRESULT*, BOOL, BOOL)`. Evidence: the same body calling the char `CUnknown` ctor 0x00CACC80 (`??0CUnknown@@QAE@PBDPAUIUnknown@@@Z`).
- 0x00CABB70 `CBaseAllocator::ReleaseBuffer`. Evidence: IMemAllocator slot 8 (reference 0x012BF830) of the CBaseAllocator-derived table 0x012BF810 (`this` is the +0xC subobject, which gives the null-tested CAutoLock conversion); strmbase body.
- 0x00CAC170 `CBasePin::EnumMediaTypes`. Evidence: IPin slot 12 of the pin table 0x012BF760 stored by 0x00CAB770; the inline CEnumMediaTypes ctor stores vtable 0x012BF6BC.
- 0x00CACE70 `CBaseDispatch::GetIDsOfNames`. Evidence: strmbase body with GetTypeInfo inlined (LoadOLEAut32 0x00CACB20, LIBID_QuartzTypeLib); static-local scope `?N@` agrees with the landed binding.
- 0x00CBA9B0 `std::__unguarded_linear_insert<CBindingPair*, CBindingPair, CBindingLess>`. Evidence: the STLport 4.6 template shape; the landed __linear_insert 0x00CBC3C0 of the same pair declares this address as its __unguarded_linear_insert callee, and landed 0x00CBB2D0 calls it too; callees FastCompare 0x009E6530 and CCharString::operator= 0x0099EFB0.
- 0x00CDEED0 `NScript::CPersonalScript_GlobalThingsScript::CClickableDoor::Main`. Evidence: slot 1 of vtable 0x012C32A4 stored by Alloc 0x00CDEE10; Ego_r.pdb and FableWin.pdb symbol.
- 0x00CF0560 `NScript::CGameflowAssistanceScript::OnPersist`. Evidence: level-script vtable slot 4; PDB symbol.
- 0x00CFAB10 `NScript::CQ_ArenaScript::OnPersist`. Evidence: level-script vtable slot 4; PDB symbol.
- 0x00CFADD0 `NScript::CQ_AwakeningTheOracleScript::Init`. Evidence: slot 3 of level-script vtable 0x012C78B0 stored by ctor 0x00CFAD30; PDB `UAE` (public).
- 0x00D01590 `NScript::CQ_BanditCampScript::CGate1GuardOuter::Init`. Evidence: slot 2 of 0x012C8604 stored by Alloc 0x00D014C0; PDB.
- 0x00D08F00 `NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Init`. Evidence: slot 2 of 0x012C9130 stored by Alloc 0x00D08E50; PDB.
- 0x00D243E0 `NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh::Init`. Evidence: slot 2 of the vtable stored by Alloc 0x00D24330; PDB.
- 0x00D29160 `NScript::CQ_EndGameScript::CM_UndeadSpawn::Main`. Evidence: slot 1 of the vtable stored by Alloc 0x00D290A0; PDB.
- 0x00D29320 `NScript::CQ_EndGameScript::CGatewayUndead::OnPredicateFail`. Evidence: slot 5 of the vtable stored by Alloc 0x00D29230; PDB.
- 0x00D2A8A0 `NScript::CQ_EndGameFocalSitesScript::Init`. Evidence: slot 3 of level-script vtable 0x012CBEB4 stored by ctor 0x00D2A820; PDB `UAE`.
- 0x00D374A0 `NScript::CQ_FireHeartScript::CFH_Villager::Init`. Evidence: slot 2 of the vtable stored by Alloc 0x00D373D0; PDB.
- 0x00D6CCC0 `NScript::CQ_HangingTreeEvilScript::MonitorBanditRegion`. Evidence: 0x00D6C4A0 builds a CSpawnedFunc named "MonitorBanditRegion" (0x012D2374) whose member pointer is this address; PDB symbol.
- 0x00D71A70 `NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::OnPredicateFail`. Evidence: slot 5 of the vtable stored by Alloc 0x00D71980; PDB.
- 0x00D8A560 `NScript::CQ_HerosOldHouseScript::CGhostFisherman::Init`. Evidence: Alloc 0x00D8A4B0, slot 2; PDB.
- 0x00D8A8A0 `NScript::CQ_HerosOldHouseScript::CExtraBooty::Main`. Evidence: Alloc slot 1; PDB.
- 0x00D8FD90 `NScript::CQ_HobbeCaveScript::CGranny::Init`. Evidence: Alloc slot 2; PDB.
- 0x00D8FEF0 `NScript::CQ_HobbeCaveScript::CNymph::OnPredicateFail`. Evidence: Alloc slot 5; PDB.
- 0x00D900E0 `NScript::CQ_HobbeCaveScript::CJames::Init`. Evidence: Alloc slot 2; PDB.
- 0x00DC4B70 `NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::OnPredicateFail`. Evidence: Alloc slot 5; PDB.
- 0x00DCC040 `NScript::CQ_OrchardFarmRaidScript::CQ_OrchardFarmRaidScript(CScriptDataBase*, CGameScriptInterfaceBase&)`. Evidence: stores the quest vtable 0x012DAF78, whose slot 2 is the landed CQ_OrchardFarmRaidScript::Main 0x00DCC770, after CScriptBase::CScriptBase 0x00CB8110, then builds the members in Ego_r.pdb order (CTimer RegisterTimer, CWideString x4, CCharString, CCrateTeamManager x2).
- 0x00DCF000 `NScript::CQ_OrchardFarmRaidScript::COrchardFarmWhisper::Init`. Evidence: Alloc slot 2; PDB.
- 0x00DD6950 `NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Init`. Evidence: Alloc slot 2; PDB.
- 0x00D6BAC0 (parked) `NScript::CQ_HangingTreeEvilScript::CHTE_AmbushPoint::Main`. Evidence: Alloc slot 1; PDB.

### Probable
- 0x00DCDED0 `NScript::CQ_OrchardFarmRaidScript::CGuardTeamMember::OnPredicateFail`. Slot 5 of vtable 0x012DB364, stored by the landed Alloc 0x00DCDE20 (the "GuardTeamMember" binding), gives the role. CGuardTeamMember is not a class in Ego_r.pdb or FableWin.pdb. The Ego_r twin is CCrateTeamMember, whose member names are used descriptively.

### Descriptive
- none

## What matched (29)
- **strmbase CBaseAllocator ctors (0x00CABAE0 WCHAR, 0x00CAC570 char).** These are the amfilter.cpp initialiser lists, in source order. `__declspec(novtable)` bases are used. The overload is set by the CUnknown name type: 0xCACB80 PBG vs 0xCACC80 PBD.
- **CBaseAllocator::ReleaseBuffer (0x00CABB70, /EHsc).** The landed GetBuffer model (0x00CA9F20) was pasted in, with an inline NotifySample. The CAutoLock(this) on the +0xC subobject gives the null-tested base conversion.
- **CBasePin::EnumMediaTypes (0x00CAC170, /EHsc).** `new CEnumMediaTypes(this, 0)` with the ctor inline. The `dec; and 8007000Eh` tail is the source's null check.
- **CBaseDispatch::GetIDsOfNames (0x00CACE70).** GetTypeInfo is defined in the same TU so it inlines. The static `?N@` scope needs the first CheckPointer `if` written without braces.
- **std::__unguarded_linear_insert (0x00CBA9B0).** This is the real STLport template over pair<CCharString, CCountedPointer> with an empty comparator passed by value, as in landed 0x00CBC3C0.
- **Entity-script Init / Main / OnPredicateFail rows** (0x00CDEED0, 0x00D01590, 0x00D08F00, 0x00D243E0, 0x00D29160, 0x00D29320, 0x00D374A0, 0x00D71A70, 0x00D8A560, 0x00D8A8A0, 0x00D8FD90, 0x00D8FEF0, 0x00D900E0, 0x00DC4B70, 0x00DCDED0, 0x00DCF000, 0x00DD6950).
  - These are straight-line interface calls on `Game`. Parent members sit 0x14 below Ego_r, and 4 lower again for each STLport container.
  - Loops are written `while (cond) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }`, with a terminating test after the loop.
  - `x * 2.0f` compiles to `fadd st,st` (0x00D29320).
  - The vector<bool>/vector operator[] must be `*(begin()+n)` (0x00D374A0).
  - The faction choice is an if/else of two whole calls (0x00DCF000).
- **Level-script rows** (0x00CF0560, 0x00CFAB10 OnPersist; 0x00CFADD0, 0x00D2A8A0 Init; 0x00D6CCC0 MonitorBanditRegion; 0x00DCC040 ctor).
  - Template `CPersistContext::Transfer<long/bool>` calls with literal defaults.
  - Init must be public (PDB `UAE`).
  - The ctor is the empty body of `CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)` with member ctors, in order.

## Parked for Fable 5.1 (1)
- 0x00d6bac0 NScript::CQ_HangingTreeEvilScript::CHTE_AmbushPoint::Main DIFFER(174v187): retail opens with `push ecx` and a dead `mov dword [esp+0Ch],0` to an unread 4-byte local, and has a 3-byte `lea ecx,[ecx]` pad before the aligned loop head; every other instruction matches | tried: by-value CCharString argument from GetDataString without a copy ctor (195 bytes), with the user copy ctor 0x0099EC30 declared (174 bytes) | next: find the source local behind the dead zero store (an address-taken or aggregate local VC7.1 keeps, e.g. a named CCharString/bool temp around the SpawnEnemies calls)

## Skipped
- none

## Label corrections
- 0x00CBA9B0: "SpeechFrame_InsertSortedWithComparison" -> `std::__unguarded_linear_insert<CBindingPair*, CBindingPair, CBindingLess>`. It is an STLport sort helper over (name, binding) pairs, not speech code.
- 0x00DCC040: "CHeroMorphDef_CopyOrInit" -> `NScript::CQ_OrchardFarmRaidScript::CQ_OrchardFarmRaidScript(CScriptDataBase*, CGameScriptInterfaceBase&)`.
- 0x00704580: "CCountedPointer_Set_00704580" -> `CCountedPointer<CScriptThing>::operator=` (`??4?$CCountedPointer@VCScriptThing@@@@QAEAAV0@ABV0@@Z`). The 0x00D374A0 call resolves there (vland RELOC_NAME).
- The 25 gap-scan placeholders `sub_<addr>` take the names above.

## Header debt
- No engine headers exist for any NScript quest or entity class used here. Every row carries a local model with `// Layout:` lines. Parent members are retail = Ego_r - 0x14, then -4 for each Dinkumware container; CQ_SunnyvaleMasterData offsets equal Ego_r.
- strmbase rows (0x00CABAE0, 0x00CAC570, 0x00CABB70, 0x00CAC170): vland warns SHARED_HEADER_AVAILABLE CBaseObject. `engine/CBaseObject.h` is Fable's 8-byte {vptr, IntelligentPointer} class. The strmbase CBaseObject (combase.h, empty in retail) is a different class with the same name, so the local model is correct.

## Callee names
- 0x00CACB80 `CUnknown::CUnknown(const WCHAR*, IUnknown*)`; 0x00CACC80 `CUnknown::CUnknown(const char*, IUnknown*)`; 0x00CACB20 LoadOLEAut32.
- 0x00BFEA1A operator new; 0x0099EBF0 CCharString(const char*, long); 0x0099EC30 CCharString(const CCharString&); 0x0099EAE0 ~CCharString; 0x0099E4B0 CCharString(); 0x0099EFB0 CCharString::operator=; 0x009E6530 CCharString::FastCompare.
- 0x0099AED0 CWideString(); 0x0099B510 ~CWideString; 0x0099A2D0 CBaseClass(); 0x0099A2E0 ~CBaseClass; 0x004AA840 ~CScriptThing.
- 0x0099E7F0 `GFCharStringToInt` (`?GFCharStringToInt@@YIJABVCCharString@@@Z`); 0x00CBE2FF IsDistanceBetweenThingsUnder; 0x00D6BB80 CQ_HangingTreeEvilScript::SpawnEnemies.
- 0x00410BE0 CPersistContext::Transfer<long>; 0x004045C0 CPersistContext::Transfer<bool>; 0x008AC9D0 vector<CScriptThing>::erase; 0x00704580 CCountedPointer<CScriptThing>::operator=; 0x00CB8110 CScriptBase::CScriptBase.
- Data: 0x0143E8F8 g_pGameScriptInterface; 0x0143E90C pGlobalScriptDef.

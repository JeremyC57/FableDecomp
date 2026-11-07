# NOTES -- chunk 840 (expert retry pass 3, ten 1-4-line scheduling parks)

Result: 4 matched (all RELOCATION_MATCH: 0x0052EA30, 0x009E5C90, 0x007BC8FC, 0x00773950), 6 parked with the same best residue
as before (0x0070C4E0, 0x00C52BB0, 0x0058301A, 0x0073ACC0, 0x0047ED76, 0x007534B0), 25-52 real attempts each (STATUS.tsv notes).
Grades all with FABLE_HDR_OVERLAY=$S/w22/agent840/hdr; names below marked PDB (two sources) or descriptive.

## 0x0052EA30 NUISystem::CComponent::ChangePositionDelta -- RELOCATION_MATCH (src/0052ea30.cpp, 27 attempts)
- Shape: engine headers (NUISystem_CComponent.h retail layout, NUISystem_CManager.h FABLE_RETAIL_CMANAGER, the overlay's
  NUISystem_CUIScreenUnitConverter.h opt-in). A local copy of the delta; when the component uses relative coordinates it is
  un-converted (UnConvertVector) and scaled per axis by CManager::GetInstance()->GetUIScale() (called twice, as retail); then
  `ChangePosition(d + Position, time, immediate)` (slot 31). Lever: pass the `operator+` temporary, not an accumulated local.
- Names (PDB): leaf ?ChangePositionDelta@CComponent@NUISystem@@UAEXABVC2DVector@@M_N@Z; IsUsingRelativePosition (slot 117),
  ChangePosition (slot 31), UnConvertVector 0x0052E530, GetInstance 0x0041E5F2, GetUIScale 0x0041CF47 (QBE, C2DVector). The
  catalog label `_global::ChangePositionDelta` is wrong (LABELS.tsv); the manifest's UnConvertVector float(float) signature and
  the parked copies' free ConvertX are wrong (NAME_FIXES.tsv). One-symbol-one-address: 0x0041CF47 and 0x0052E580 carry wrong
  spellings in landed rows (NAME_FIXES.tsv); UnConvertVector was bound nowhere else.
- Header: hdr/engine/NUISystem_CUIScreenUnitConverter.h opt-in block (hdr/README.md).

## 0x009E5C90 CSymbolMap::LoadFromFile -- RELOCATION_MATCH (src/009e5c90.cpp, 13 attempts)
- Shape: the hand757 model (vector-map clear, STLport set clear inline, CDiskFileWin32 local with its user ctor/dtor, malloc +
  inline memset, three GFRemoveComments passes, ParseBuffer, Sort, DeallocateUnusedMemory, free). Lever: the whole-file read as
  an inline helper taking the file object by reference (the buffer becomes an inlinee local, VC7.1 then makes the size reload
  the SIB base: retail `mov byte ptr [eax+esi],0`); Read called through the qualified name inside the helper.
- Names: leaf ?LoadFromFile@CSymbolMap@@QAE_NABVCWideString@@@Z (PDB, bound by the landed 0x0077F510); CDiskFileWin32::Open
  (FileAccessType enum from engine/CAFile.h) / Read / Close (engine/CAFile.h virtuals), CBaseClassNonCopyable ctor/dtor
  0x0099A2F0 / 0x0099A300 (catalog), GFRemoveComments ?GFRemoveComments@@YIXPADPBD1@Z (landed binding), CWideString operators,
  ParseBuffer (catalog). DESCRIPTIVE model names for the container helpers: CSymbolVectorMap::clear = 0x009E79E0 (catalog
  `CVectorMap::DeallocateMemory`, landed binding `?Clear@?$CVectorMap@VCCharString@@VCSymbolInfo@CSymbolMap@@...@@QAEXXZ` --
  the two disagree; the body only empties the map), CSymbolVectorMap::DeallocateUnusedMemory = 0x009E7C70 (catalog
  `CArray<std::pair<CCharString,CSymbolMap::CSymbolInfo>>::DeallocateUnusedMemory`), SortSymbols = 0x009E7E40 (catalog
  `std::sort<pair*, CCompareString<CSymbolInfo>>`), CNameSet::_M_erase = 0x004ABA70 (STLport _Rb_tree<CCharString,...>::_M_erase),
  g_CurrentSymbolFile = 0x013CAA34 (a CWideString; the landed `g_CCharString_013caa34` has the wrong type: NAME_FIXES.tsv).
  Header debt: a CSymbolMap engine header with the real CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo>> and
  std::set<CCharString> members would let the row bind the template spellings; engine/CSymbolMap.h exists but was not used
  (the parked model is the hand757 one; respelling the templates is the coordinator's call at landing).

## 0x007BC8FC NThingShot::CTCShotDefinitionTearInGround::InitMovement -- RELOCATION_MATCH (src/007bc8fc.cpp, 20 attempts)
- Shape: the hand757 model (size TU pragmas, IS_THING_ALIVE on the homed target pointer, direction = fire-at pos - physics
  position with Z cleared and normalised, ground position from GetGroundSizeZAt, SetPosition / SetVelocity / SetCollisionFlags,
  AddTC("CTCHitOnCollision"), UpdateMovement). Lever: `const float speed = PShotDef->Speed / g_framesPerSecond;` named before
  SetVelocity (IR-numbering tie flip at the AddTC call; identity change).
- Names (PDB): leaf ?InitMovement@CTCShotDefinitionTearInGround@NThingShot@@UAEXXZ; GetCreatureFireAtPos
  ?GetCreatureFireAtPos@NThingShot@@YI?AVC3DVector@@ABVCThing@@@Z (the landed 0x007BEDF0 leaf is a free `(CThing*)`:
  NAME_FIXES.tsv), IS_THING_ALIVE<const CThing*> 0x00436380, AddTC ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z
  (four wrong landed spellings: NAME_FIXES.tsv), GetGroundSizeZAt QBE, CTCBase::GetWorldMap (IAEAAVCWorldMap, engine/CTCBase.h),
  CCharString ctor/dtor, GetPItem, Normalise ?Normalise@C3DVector@@QAEAAV1@XZ (0x00A14440 also carries
  `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` in two landed rows: unresolved here, both names exist in the PDB set; this row keeps the
  hand757 spelling), SetVelocity ?SetVelocity@CTCPhysicsBase@@QAEXABVC3DVector@@@Z (no landed binding to check against).
  Vtable slots SetPosition (31) / SetCollisionFlags (43) on CTCPhysicsBase and UpdateMovement (35) are the hand757 slot names.

## 0x00773950 CTCShop::IsOpeningHours -- RELOCATION_MATCH (src/00773950.cpp, 24 attempts)
- Shape: the hand757 model (IS_THING_ALIVE inline on PShopKeeper, GetDef() by value twice with the GetSubDef same-TU stand-in
  body so the def pointer's address does not escape, the interface-map GetTC inline). Lever: `if (alwaysOpen) return true;`
  ahead of the villager block (IR-numbering tie flip of the key-address lea). The two CDefPointer temporaries' construction
  flags in ebx and the def held in ebp are as before.
- Names (PDB): leaf ?IsOpeningHours@CTCShop@@QBE_NXZ (const, engine/CTCShop.h); 0x007EE540 = CTCVillageMember::GetPVillage const
  (engine/CTCVillageMember.h, TCI_VILLAGE_MEMBER = 0x23; the parked copies' free GFGetPVillageInterface was descriptive:
  NAME_FIXES.tsv), IsCookTime QBE, GetPItem QBE, CShopDef::AlwaysOpen (Ego_r +0xAC; retail +0x9C with 12-byte STLport
  containers). DESCRIPTIVE: CInterfaceMap::LowerBound = 0x0040F020 (catalog `CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound`;
  the landed bindings are placeholders `?lower_bound@Vec@@...` etc.), the GetSubDef stand-in (`?GetSubDef@CDefClassBase@@QAE_NPAV?$CDefPointer@VCShopDef@@@@@Z`
  defined in the TU; the retail symbol is the const member template ??$GetSubDef@VCShopDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCShopDef@@@@@Z
  at 0x005130A0 -- a same-TU body, so no relocation is bound). Header debt: an engine CShopDef.h is missing (AlwaysOpen offset).

## 0x0070C4E0 CAnimateThing::GetFirstCombinationAnimFromType -- parked DIFFER(81v81), 2 lines, 47 attempts
- Residue: the not-found exit `pop edi ; xor eax,eax ; pop esi` (retail `xor eax,eax ; pop edi ; pop esi`). Retail's xor block is a
  join (empty-list and found-at-end jumps land on it) and is left unscheduled; ours interleaves. Tried: early returns, nested
  ifs, result variables, helper (forceinline) per search, ternaries, goto, while loops, sentinel forms, `(CAnimSet*)0` / named
  null, 16 documented flags, and a QFE-4035 diagnostic compile (same 2 lines: not a compiler-version gap).
- Names: leaf PDB; the catalog label `CAnimateThing::FindCombinationAnim` is wrong (LABELS.tsv, NAME_FIXES.tsv; the landed caller
  0x008E9710 binds the wrong one, 0x008619C0 the right one).

## 0x00C52BB0 CCpuInfo::GetAMDModelName -- parked DIFFER(140v140), 4 lines, 52 attempts
- Residue: the K5 test `cmp ecx,4 ; jae` where retail is `cmp eax,3 ; ja` (both the register and the constant). Diagnostics show the
  register choice follows the set of constants in the function (only the exact 4/5/6 family switch with the family-6 `Model == 4`
  test gives ecx; `<= 3`, `3 >= Model`, `!(Model > 3)`, signed ranges, `<= 3 || == 0` each move one thing and break another).
- Names: leaf ?GetAMDModelName@CCpuInfo@@QBEPBDXZ (Ego_r name set); catalog `_global::CPU_GetAMDBrandName` is descriptive
  (LABELS.tsv, NAME_FIXES.tsv).

## 0x0058301A CTCInventory::CreateEditBoxActions -- parked DIFFER(321v321), 2 lines, 34 attempts
- Residue: the address-taken `box` copy's home store `mov [ebp+10h],esi` right after `mov esi,eax` (retail after `mov ecx,esi`,
  between the vtable load and the SetPasswordBox call). The store sits at the copy's definition point: before the vtable load
  when defined before the call, after the call when defined after; one variable only goes memory-resident (331 bytes). Tried:
  every binding / cast / const / declaration-order / call-receiver form, named action pointers, flags /G3-/G6, /Oa, /Ow.
- Names: leaf and callee slots from hand612 (PDB signature, CEditBox slot names, engine NUISystem_CEditBox.h names).

## 0x0073ACC0 CTCCreatureModeManager::AddMode -- parked DIFFER(331v331), 2 lines, 40 attempts
- Residue: the early `return false` `xor al,al` after `pop ebp` (retail ahead of the pops; the true exit's `mov al,1` interleaves
  in both). Tried: result variables (single epilogue), `==end {..} return false` orders, else branches, helper find, found flag,
  for-condition / while loops, dtor early-return / Release() / engine CCountedPointer.h forms, `return 0`, do-while(0) break,
  goto, const type, engine bool GetTC form, folded idle delay, != 0 tests, branch inversion, 16 flags, QFE-4035 diagnostic.
- Names: leaf PDB ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z (engine header
  opt-in FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER); the local model emits `UCModePointer` (differ/ Leaf line). DESCRIPTIVE
  in the model: the list push_back 0x007421C0 (catalog says `std::list<CCountedPointer<CModeBase> >::push_front`; the row links
  before the sentinel, i.e. push_back -- the catalog label needs a look), CWorld::GetWorldFrame 0x0049D870 (catalog
  `CWorld::GetFrame`), CModeBase slots 1 / 5 / 6 / 11 (hand757 names), CTCLook::UpdateTurningAnims 0x00754050 (no catalog entry).

## 0x0047ED76 CTCInGameMenu::SetToSave -- parked DIFFER(366v366), 2 lines, 29 attempts
- Residue: `push ecx ; fsubp st(1)` at the ChangePosition float slot (retail `fsubp st(1) ; push ecx`). Tried: explicit / copy-init
  / const temporaries, named x / y / scale locals in blocks (they move the GetUIScale temporaries' slots), constant-side swaps,
  `/ 2.0f`, ctor body form, no copy ctor, dialog local, statement-order and IR-numbering rewrites, const GetUIScale, /Oa, /Ow.
- Names: leaf ?SetToSave@CTCInGameMenu@@QAEXJ@Z; DESCRIPTIVE in the hand757 model: `SetPosition` slot 31 (= NUISystem::CComponent::
  ChangePosition per engine/NUISystem_CComponent.h), `Show_slot43` (+0xAC; the engine header names slot 43 `Initialise` -- check
  before landing), GetScreenWidth / GetScreenHeight 0x0041CC14 / 0x0041CC2B (= NUISystem::CUIScreenUnitConverter::GetBackBufferWidth /
  GetBackBufferHeight, FableWin.pdb, engine header), GetUIScale should be const (QBE), FindChild takes CCharString by value (slot 3).
  Next idea in differ/: the engine-header rewrite with those names.

## 0x007534B0 CTCLook::IsPosInViewRange -- parked DIFFER(384v384), 4 lines, 27 attempts
- Residue: the `from` position copy as load/store pairs (retail hoists the second load above the first store). Tried: trivial /
  user / assignment copy ctors, user operator=, engine C3DVector.h, if/else assignments, const-ref sources, explicit copies,
  pointer select, the ternary straight into the call, const / direct-init, non-const GetPosition, IR-numbering rewrites, /Oa, /Ow.
- Names: leaf ?IsPosInViewRange@CTCLook@@QBE_NABVC3DVector@@M_N1@Z (the local struct model emits `ABUC3DVector`; differ/ Leaf
  line says so); callee names from hand757 (GetHeadPos 0x00751B30 landed, CThing::GetPosition 0x004C73D0 = engine CThing.h
  `GetPos() const` -- a naming difference to settle at landing, AngleBetweenPositions / AngleDifference forge-autonames).

## Label corrections
LABELS.tsv: 0x0052EA30, 0x0070C4E0, 0x00C52BB0 (the three the brief flagged), 0x00436380 (placeholder), 0x0052E530 (signature).

## Header debt (exact changes, not done here)
- engine/CShopDef.h: missing; retail CShopDef has AlwaysOpen at +0x9C (Ego_r +0xAC: four 12-byte STLport containers earlier).
- engine/CSymbolMap.h: not used by 0x009E5C90 (hand757 local model); a landing on it needs CVectorMap<CCharString, CSymbolInfo,
  CCompareString<CSymbolInfo>> with Clear / Sort / DeallocateUnusedMemory and std::set<CCharString> NamesUsed at retail sizes.
- engine/CTCInGameMenu.h: the retail opt-in has the per-slot vectors but not Components (+0x58, vector<CComponent*>), PSaveInProgress
  (+0xD0), SaveGameIndex (+0xC8), FramesToWaitToSave (+0xCC), SaveInProgressTime (+0xD8 double) as typed members.

## Verifier warnings (tmp/vland.log, read-only run: 4/4 RELOCATION_MATCH PASS)
13 `WARN SHARED_HEADER_AVAILABLE` lines on 0x00773950 (CDefPointeeBase, CDefPointer, CThing, CTCVillage ...), 0x007BC8FC (C3DVector ...)
and 0x009E5C90: the three rows keep the hand757 local models that matched (the engine-header rewrites are the parked rows' next
ideas, not tried on the matched ones once they closed). A retype at landing (retype_landed.py / the engine headers) is the
coordinator's call; the callee spellings in the local models are the PDB ones where NOTES marks them PDB.

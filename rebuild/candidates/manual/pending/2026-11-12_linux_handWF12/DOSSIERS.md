## 0x009FF220

# DOSSIER 0x009FF220 CGraphicInfoFrame::CFrameData::UploadFrame (540 B)

## 1. Retail shape (xdis 009ff220 0x22c)
- __thiscall, `ret 4` (one bool param `force`), NO frame pointer (`sub esp,8; push ebx,ebp,esi,edi`), no SEH, no /GS.
  -> plain speed TU, `// Flags: none` like the CFrameData siblings.
- Locals: result bool at [esp+0x13] (zeroed first, `mov byte [esp+17h],0` before 1st call), CGraphicFrame local at [esp+0x14] (else arm, `mov dword [esp+18h],0` = ctor).
- Volume arm stores level (movsx [esi+39h]) into the dead param slot [esp+1Ch] and the bank into [esp+18h] BEFORE the 2nd GetEntryDataType call;
  GraphicInfo re-read ([edi+0Ch]) AFTER that call for the header arg. Else arm: `mov ebx,[esp+1Ch]` = force read first, then TextureType, level, push order.
- Two DIFFERENT release shapes for CGraphicFrameData: Frame=frame assignment (0x9ff338) `dec [edi]; jne` (flags of dec);
  local-frame dtor (0x9ff3ae) `dec [edi]; cmp [edi],ebx; jne` (re-read). Neither stores FrameData=0 (dead). CTexture count block inside: Data(+8) zeroed before Info(+0xC) = default ~CCountedPointer order.
- FileData release (0x9ff2ab): Info(+0x34) zeroed BEFORE Data(+0x30) = default CCountedPointer::ReleaseData()/Reset(0) order (not the _RELEASEDATA macro).
- Tail = inlined CFrameData::Sizeof() (`lea edi,[eax+44h]`, test [esi+2Ch]) -> SetResourceSize(this) -> GetBank()->AddResource(*this) (`add ecx,164h`, CResourceBank base of CGraphicDataBank, no null test).

## 2. Identity (two sources) -- manifest label CFrameData::UploadFrame is right modulo the outer class
- Ego_r.pdb 0x00B5B6D0 `?UploadFrame@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z` (egor_pdb_names.tsv, egor_signatures.tsv);
  FableWin.pdb same mangling (_fablewin_api.tsv 0x01814C23/0x031285A0, FableWin.exe.signatures.tsv).
- Signature: `public: bool CGraphicInfoFrame::CFrameData::UploadFrame(bool)` (non-const, non-virtual). Leaf `?UploadFrame@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z`.
- Callers (xref): 0x009FF6DE InternalGetFrameForcedLoading (UploadFrame(1)), 0x009FF752 GetVolumeTexture (UploadFrame(true)), 0x009FF86C OnResourceLoaded (UploadFrame(0)).
  Landed 009ff6b0 and 009ff860 declare it `bool UploadFrame(long flags)` -- WRONG param type (NAME_FIXES/signature fix: bool).

## 3. Callees (retail addr -> landed row / landed name -> PDB name)
| addr | landed file | landed name | PDB / correct | draft spelling |
|---|---|---|---|---|
| 0x009CC210 | 00/9c/CBankFile_GetEntryDataType_009cc210.cpp | CBankFile::GetEntryDataType | ?GetEntryDataType@CBankFile@@QBEKK@Z (const; header CBankFile.h under FABLE_RETAIL_CBANKFILE_ENTRYQUERY) | draft non-const on CGraphicDataBank: fix |
| 0x009FD9B0 | 00/9f/CGraphicDataBank_LoadVolumeGraphic_009fd9b0.cpp | LoadVolumeGraphic | ?LoadVolumeGraphic@CGraphicDataBank@@AAE_NAAVCManagedTexture@@AAVCBankFileAsyncData@@ABVCGraphicHeader@1@_NK3@Z = private bool(CManagedTexture&,CBankFileAsyncData&,const CGraphicHeader&,bool,unsigned long,bool) | draft uses pointers + CGraphicInfo*: WRONG |
| 0x009FED30 | NOT landed | (manifest LoadGraphicFrame) | ?LoadGraphicFrame@CGraphicDataBank@@AAE_NAAVCGraphicFrame@@AAVCManagedTexture@@AAVCBankFileAsyncData@@PAVCGraphicInfo@@_NKW4ETextureType@1@4@Z = private bool(CGraphicFrame&,CManagedTexture&,CBankFileAsyncData&,CGraphicInfo*,bool,unsigned long,ETextureType,bool) | draft uses pointers for first 3: WRONG |
| 0x00A69B20 | 00/a6/CIVCountedHandle00a69b20_operator_00a69b20.cpp | CIVCountedHandle_00a69b20::operator= | ??4CManagedTexture@@QAEABV0@ABV0@@Z (Ego_r 0x00B7D230 + FableWin 0x017AC0C9) | landed NAME WRONG -> NAME_FIXES |
| 0x00A69AD0 | 00/a6/CFrameDataHandle_Clear_00a69ad0.cpp | CFrameDataHandle::Clear | ?Release@CManagedTexture@@QAE_NXZ (Ego_r 0x00B7D1E0, FableWin 0x017B23E3; landed StopLoading 009fdc20 already uses it) | draft `ClearHandle` WRONG; landed name WRONG -> NAME_FIXES |
| 0x009FCBA0 | 00/9f/CGraphicFrameData_Uninitialise_009fcba0.cpp | Uninitialise | ?Uninitialise@CGraphicFrameData@@QAEXXZ | ok |
| 0x009FCBF0 | 00/9f/CGraphicFrameData_Sizeof_009fcbf0.cpp | Sizeof | ?Sizeof@CGraphicFrameData@@QBEKXZ (const) | draft non-const |
| 0x00A69E60 | 00/a6/CManagedTexture_Sizeof_00a69e60.cpp | Sizeof | ?Sizeof@CManagedTexture@@QBEKXZ | draft non-const |
| 0x009FBF20 | 00/9f/CResource_SetResourceSize_009fbf20.cpp | SetResourceSize | ?SetResourceSize@CResource@@QAEXK@Z | ok |
| 0x009FC380 | 00/9f/CResourceBank_AddResource_009fc380.cpp | AddResource | ?AddResource@CResourceBank@@QAEXAAVCResource@@@Z (REFERENCE) | draft pointer: fix to `AddResource(*this)` |
| 0x00BFE9BC | 00/bf/global_operatordelete_00bfe9bc.cpp | operator delete | MSVCR71 | ok |
| [Info+4] | -- | CCPPointerInfo::DeleteFunc (fastcall, ecx=Info->Data) | CCountedPointer<T>::DeleteData | ok |

## 4. Members (Ego_r struct_layouts_egor.tsv; shared header rebuild/include/engine/CGraphicInfoFrame.h)
CFrameData (0x44, : CManagedTextureOwner : CResource): Frame CGraphicFrame +0x1C; ManagedTexture CManagedTexture +0x20; LoadingManagedTexture +0x28 (its Data CIVCountedPointer<CManagedTextureData> at +0x2C = IsValid());
FileData CCountedPointer<CBankFileAsyncData> +0x30 (Data)/+0x34 (Info); CurrentMipmapLevel schar +0x38; LoadingMipmapLevel schar +0x39; GraphicInfoFrame `const CGraphicInfoFrame*` +0x40.
CGraphicInfoFrame (0x10): GraphicInfo CGraphicInfo* +0xC. CGraphicInfo (44): Header CGraphicDataBank::CGraphicHeader +0 (IsFlatAnimatedGraphic:1 at +0x12 bit0 -> `test byte [ecx+12h],1`), TextureType +0x1C, Bank +0x20, Index long +0x24.
CGraphicFrameData (0x1B): ReferenceCount +0, PTexture CCountedPointer<CTexture> +8/+0xC, IsInitialised bit4 of +0x1A (`test [edi+1Ah],10h`). CManagedTexture: CBaseClass vptr +0, Data +4.
Opt-in macros (copy FreeTextureMemory 009fe460 set): FABLE_RETAIL_CGRAPHICINFOFRAME_FREE + FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS + FABLE_RETAIL_CCOUNTEDPOINTER + FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_CRESOURCELIST (+CCHARSTRING/CWIDESTRING/CARRAY), include engine/CGraphicInfo.h (pulls CGraphicDataBank.h, CGraphicInfoFrame.h) and engine/GFSizeof.h.
 These give: CFrameData::Sizeof() inline (sizeof(*this)+Frame.Sizeof()+LoadingManagedTexture.Sizeof() if IsValid()), CFrameData::GetBank(), CGraphicInfoFrame::GetBank/IsFlatSequence (declared; bodies defined in the row, see 009fe460), CGraphicFrame::Sizeof (row defines `{return GFPSizeof(FrameData);}`), CManagedTexture::IsValid/Sizeof, CGraphicDataBank::CGraphicHeader, friend CGraphicInfoFrame in CGraphicDataBank.
 Need also FABLE_RETAIL_CBANKFILE_ENTRYQUERY (GetEntryDataType decl) and FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT gives `CGraphicInfo::IsBump() {return Bank->GetEntryDataType(Index)==2;}` (used by 009fd700) -- but CONSTRUCT also flips CGraphicInfo access; check compatibility.
 HEADER GAPS (no macro yet): CManagedTexture::operator=(const CManagedTexture&) and bool Release(); CGraphicFrame ctor/dtor/operator= in the STATICBLOCKS branch (only FABLE_RETAIL_CGRAPHICFRAME_CTOR in the other branch); CGraphicFrameData dtor/Uninitialise/ReferenceCount access;
 CGraphicDataBank::LoadVolumeGraphic/LoadGraphicFrame decls; CGraphicInfo::IsVolumeTexture/PeekGraphicHeader/GetTextureType(FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE exists); CGraphicInfoFrame::IsVolumeTexture/UploadToTexture/UploadToVolumeTexture. Author must add header opt-ins (new macro) rather than local structs.
 Old draft models LOCALLY (all have shared headers): CCPPointerInfo/CCountedPointer (CCountedPointer.h), CGraphicFrameData+CGraphicFrame (CGraphicFrame.h), CManagedTexture, CGraphicDataBank, CResourceBank, CGraphicInfo, CGraphicInfoFrame, CResource, CFrameData.

## 5. Callee-body visibility / inline helpers (KEY FINDING)
FableWin.pdb names inline helpers whose bodies retail EXPANDED here (no call emitted):
- `?IsVolumeTexture@CGraphicInfoFrame@@ABE_NXZ` / `?IsVolumeTexture@CGraphicInfo@@QBE_NXZ` -> Bank->GetEntryDataType(Index)==4 (first test).
- `?UploadToVolumeTexture@CGraphicInfoFrame@@ABE_NAAVCManagedTexture@@AAVCBankFileAsyncData@@K@Z` (Ego_r out of line 0x00B580C0) and
  `?UploadToTexture@CGraphicInfoFrame@@ABE_NAAVCGraphicFrame@@AAVCManagedTexture@@AAVCBankFileAsyncData@@K_N@Z` (Ego_r 0x00B5B6A0, 0x30 B right before UploadFrame 0x00B5B6D0 => same TU, defined just before).
  Probable bodies: UploadToVolumeTexture(tex,data,level){return GetBank()->LoadVolumeGraphic(tex,data,GraphicInfo->PeekGraphicHeader(),true,level,GraphicInfo->IsBump());}
  UploadToTexture(frame,tex,data,level,force){return GetBank()->LoadGraphicFrame(frame,tex,data,GraphicInfo,true,level,GetTextureType(),force);}
  This explains the residue: level is an inline PARAMETER (evaluated at the call site, before the inlined body's IsBump call -> spilled to the param slot), bank loaded inside the body before IsBump, header re-read after.
- `?IsBump@CGraphicInfo@@QBE_NXZ`, `?PeekGraphicHeader@CGraphicInfo@@QBEABVCGraphicHeader@CGraphicDataBank@@XZ`, `?GetTextureType@CGraphicInfo/CGraphicInfoFrame`, `?GetBank@CGraphicInfo/CGraphicInfoFrame/CFrameData`, `?IsFlatSequence@CGraphicInfoFrame@@QBE_NXZ`, `?Sizeof@CFrameData@CGraphicInfoFrame@@QBEKXZ` (Ego_r 0x00B58110), `??0/??1/??4CGraphicFrame@@`, `??1CGraphicFrameData@@` -- all inline in retail.
- `?Get@?$CCountedPointer@VCBankFileAsyncData@@@@QBE...` / operator* for `*FileData`; `?DiscardAsyncData@CBankFileAsyncData@@SIXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z` exists (static fastcall) -- possibly the FileData drop spelling, unproven; plain `FileData.Reset(0)`/ReleaseData() matches the Info-then-Data store order.
Out-of-line calls in retail: CManagedTexture::operator= (0xA69B20) and Release (0xA69AD0) -- NOT visible inline (declare only). GetEntryDataType, Sizeof(s), SetResourceSize, AddResource, Uninitialise, LoadVolumeGraphic/LoadGraphicFrame all out of line. No STLport.

## 6. Sibling landed rows (same TU neighbourhood 0x009FD4D0..0x009FF8B0)
- 00/9f/CGraphicInfoFrameCFrameData_FreeTextureMemory_009fe460.cpp -- `// Flags: none`, macros FREE+STATICBLOCKS+CCOUNTEDPOINTER+DEF_CLASS_BASE+CRESOURCELIST+CCHARSTRING+CWIDESTRING+CARRAY; defines inline GetBank/IsFlatSequence/CGraphicFrame::Sizeof in the row. BEST TEMPLATE.
- 00/a0/CGraphicInfoFrame_ReleaseData_00a00680.cpp -- FREE+STATICBLOCKS+CCOUNTEDPOINTER+DEF_CLASS_BASE+CRESOURCELIST+CIVCOUNTEDPOINTER; Flags none.
- 00/9f/CResource_SetResourceSize_009fbf20.cpp -- DEF_CLASS_BASE+STATICBLOCKS+FREE; Flags none.
- 00/9f/CGraphicInfo_CGraphicInfo_009fd700.cpp -- STATICBLOCKS+CBANKFILE_ENTRYQUERY+CPIXELFORMAT+CTEXTURE+CRESOURCELIST+DEF_CLASS_BASE+CARRAY+CGRAPHICINFO_CONSTRUCT (IsBump()).
- 00/b2/CGraphicFrame_ScalarDeletingDestructor_00b2ed60.cpp -- the inline ~CGraphicFrame / ~CGraphicFrameData chain (local model, FABLE_FORCEINLINE).
- Older local-model siblings: CFrameData_StopLoading_009fdc20 (same FileData drop + Release + Sizeof tail), CFrameData_Sizeof_009fdbe0, OnResourceLoaded_009ff860.

## 7. Old draft model errors
1. Callee signatures: LoadVolumeGraphic/LoadGraphicFrame take references (and const CGraphicHeader& = GraphicInfo->Header), AddResource takes CResource&; GetEntryDataType/Sizeof are const; 0xA69AD0 is CManagedTexture::Release() returning bool, not ClearHandle.
2. Missing the inline helpers IsVolumeTexture / UploadToVolumeTexture / UploadToTexture / IsBump / IsFlatSequence / GetTextureType / GetBank -- the draft open-codes them with locals, which is the documented residue (level/bank register allocation in the volume arm). Spell: `if (GraphicInfoFrame->IsVolumeTexture()) { if (GraphicInfoFrame->UploadToVolumeTexture(LoadingManagedTexture,*FileData,LoadingMipmapLevel)) {...} } else { CGraphicFrame frame; if (GraphicInfoFrame->UploadToTexture(frame,LoadingManagedTexture,*FileData,LoadingMipmapLevel,force)) { if (!GraphicInfoFrame->IsFlatSequence()) {Frame=frame; ...} result=true; } }`.
3. CGraphicFrame release: draft uses one Release() for both operator= and dtor; retail has two shapes (`if(--rc==0)` in operator= vs `--rc; if(rc==0)` re-read in the dtor) -> spell operator= and ~CGraphicFrame bodies differently; no FrameData=0 store needed in either.
4. Draft models 12 local classes that have shared headers (verifier warns/strict rejects); GraphicInfoFrame should be `const CGraphicInfoFrame*`.
5. The tail should be `SetResourceSize(Sizeof()); GetBank()->AddResource(*this);` using the header's inline CFrameData::Sizeof (matches lea edi,[eax+44h]).
6. Callers 009ff6b0/009ff860 declare UploadFrame(long) -- signature fix to bool.

## 0x0082B7E0

# DOSSIER 0x0082B7E0 CMiniMapDisplay::DrawMarkers (541 bytes)

## 1. Retail shape (xdis 0082b7e0 0x22d)
- __thiscall, `ret 0x10` (4 stack args: info&, box&, alpha, bool). No frame pointer (`sub esp,8; push esi; push edi`), no SEH -> plain /O2 TU, no optimize pragmas needed.
- 8-byte frame: a CRGBColour temp at [esp+0xc..0xf] (Open's colour arg, 0,0,0,0xff built bytewise then copied as a dword) and the `heroDrawn` bool at [esp+0x13] (after the ebx/ebp pushes).
- ebx/ebp pushed LATE (0x82b8bb/0x82b8c2, after the fade block) and popped before the AddAutoMarker tail: shrink-wrapped.
- Loop 1 is aligned with `lea ebx,[ebx]` (6-byte nop) at 0x82b8ca; loops 2/3 not padded.
- Hero test in each loop: `call 438570 ; mov eax,[eax+958h] ; mov ecx,[ebp+8] ; cmp ecx,[eax]` = inline CMarkerInfo::IsHeroMarker() (out-of-line copy of the same TU at 0x0082A210: `call 438570; mov ecx,[eax+958h]; mov eax,[esi+8]; sub eax,[ecx]; neg; sbb; inc`).
- Tail: `call 438570 ; mov ecx,[eax+958h] ; mov edx,[ecx] ; add eax,958h (dead) ; push edx ; push esi ; call 0082A500` -- the dead `add eax,958h` is a residue of forming &MiniMapMarkerGraphics (vector ref / operator[]), i.e. arg = PeekGuiDef().MiniMapMarkerGraphics[0] through an accessor.

## 2. Identity
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv): `?DrawMarkers@CMiniMapDisplay@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC2DBoxF@@E_N@Z` -> protected, void DrawMarkers(const CEnginePrimitivePrepareInfo&, const C2DBoxF&, unsigned char, bool).
- Only caller: CMiniMapDisplay::DrawMap 0x0082BE60 (call at 0x0082C050, its last statement `DrawMarkers(info, box, alpha, front);`), landed rebuild/src/compiled/00/82/CMiniMapDisplay_DrawMap_0082be60.cpp (declares info non-const; PDB says const). Manifest label is CORRECT. Last param name per caller: `front`.
- Mangled leaf: ?DrawMarkers@CMiniMapDisplay@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC2DBoxF@@E_N@Z

## 3. Callees
| retail | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00440D70 | 00/44/stdvector_erase_00440d70.cpp | vector<CEnginePrimitiveHandle>::erase(first,last) | STLport erase | via clear() -- OK (STLport clear() = erase(begin(),end()) inline) |
| 0x00443F90 | 00/44/stdvector_Resize_00443f90.cpp | vector<CEnginePrimitiveHandle>::resize(n) (one-arg, ret 4) | resize | **draft says reserve -- WRONG, it is resize(n)**; fable_containers.h opt-in FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL declares `void resize(size_type n);` |
| 0x00434870 | 00/43/CFadeInFadeOutBase_Update_00434870.cpp | CFadeInFadeOutBase::Update(float) (modelled as CFadeInFadeOutBase_Methods) | ?Update@CFadeInFadeOutBase@@QAEXM@Z | OK |
| 0x00438570 | 00/43/CPlayerGui_PeekGuiDef_00438570.cpp | `static const CPlayerGuiDef& __fastcall CPlayerGui::PeekGuiDef()` | CPlayerGui::PeekGuiDef | **draft: free `CPlayerGuiDef* PeekGuiDef()` returning pointer -- wrong; landed siblings use the static const& form** |
| 0x0082A570 | not landed (labels: DrawMarker, bool, 7 args) | -- | ?DrawMarker@CMiniMapDisplay@@IAE_NABVCEnginePrimitivePrepareInfo@@ABVCMarkerInfo@1@JABVC2DBoxF@@E_N@Z (protected, bool, (const info&, const CMarkerInfo&, long, const C2DBoxF&, unsigned char, bool)) | OK |
| 0x00449970 | 00/44/CPlayerManager_GetMainPlayer_00449970.cpp | CPlayerManager::GetMainPlayer() (non-const, returns CPlayer&) | GetMainPlayer | **draft: GetPlayer() returning pointer -- wrong** |
| 0x00487DC0 | 00/48/CPlayer_GetPControlledCreature_00487dc0.cpp | CPlayer::GetPControlledCreature() | (labels: GetHeroCreature) | **draft: GetHeroThing -- wrong; use landed GetPControlledCreature** |
| 0x0082A500 | 00/82/CMiniMapDisplay_AddAutoMarker_0082a500.cpp | CMiniMapDisplay::AddAutoMarker(const CThing&, long), ret 8 | ?AddAutoMarker@CMiniMapDisplay@@IAEXABVCThing@@J@Z | **draft: AddMarker(const CThing*, long) -- WRONG NAME (AddMarker is 0x0082A890)**; call `AddAutoMarker(*hero, ...)` |
- Global 0x013B86A0 = CMainGameComponent::PMainGameComponent (protected static), read via `CMainGameComponent::Get()` (landed DrawMap, naming wave 18). Draft's `g_pGame`/CGame are made-up.

## 4. Members
- this (CMiniMapDisplay, rebuild/include/engine/CMiniMapDisplay.h; retail layout opt-in FABLE_RETAIL_CMINIMAPDISPLAY [+_ADDMARKER] gives typed `std::list<CMarkerInfo> Markers` +0x20 and nested CMarkerInfo; else opaque bytes): MarkerPrimitiveHandles +0x10 (vector<CEnginePrimitiveHandle>, opaque in header), Markers +0x20 (list), MarkerFlashFade +0x24 (CFadeInFadeOutBase, opaque under retail opt-in: `_pad_0x24[0x9c]`), PRegion +0xb8 (CRegion*). The retail opt-in branch hides +0x24..+0xbf, so the author must extend it or model locally (DrawMap models locally).
- CMarkerInfo (engine/CMarkerInfo.h + nested in CMiniMapDisplay.h): PThing +0 (CIntelligentPointer<const CThing>), GraphicIndex +8, **AutoMarker +0xc** (draft calls it "Flash" -- wrong). Loop 2 = !hero && !AutoMarker; loop 3 = !hero && AutoMarker.
- CFadeInFadeOutBase (engine/CFadeInFadeOutBase.h, PDB): Active +0, FadeInTime +4, FadeOutTime +8, Closing +0xc, Opening +0xd, OpenTimer +0x10, CloseTimer +0x14, ToColour +0x18 (CRGBColour). Opt-in: FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE (IsActive() const). Real method names (FableWin/_fablewin_api.tsv): Open(float,float,CRGBColour), Update(float), IsActive, IsOpen, IsOpening, IsClosing, IsClosed, Close(). Draft's IsFullyOpen -> **IsOpen()** (Active && !Opening && !Closing per bytes 0x82b89d), FadeOut -> **Close()** (Opening=false; Closing=true). Open is inline here (bytes 0x82b854-0x82b88c).
- CRGBColour (engine/CRGBColour.h exists): bytes B,G,R,A at +0..3 (retail writes +0xe,+0xd,+0xc = 0 then +0xf = 0xff -> CRGBColour(0,0,0,255)).
- CPlayerGuiDef (engine/CPlayerGuiDef.h): **MiniMapMarkerGraphics +0x958, vector<long>**; [0] = hero marker graphic (IsPulsingOnMap uses [1],[3],[0xc],[0xd]). Draft names it m_markerGraphics/CArray_long -- rename.
- CMainGameComponent (engine/CMainGameComponent.h): PPlayerManager +0x1c (GetPlayerManager()), PWorld +0x24 (PeekWorld/GetWorld), PDisplayEngine +0x28 (PeekDisplayEngine; opt-in FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE). Opt-ins: FABLE_RETAIL_CMAINGAMECOMPONENT (+ _PLAYERMANAGER/_WORLD/_DISPLAYENGINE/_PEEKDISPLAYENGINE).
- CWorld (engine/CWorld.h): RegionLoadStatus +0x104 (ERegionLoadStatus) -> inline `IsPerformingRegionLoad() const { return RegionLoadStatus != 0 && RegionLoadStatus != 9; }` (opt-in FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI, line 263); Paused +0xa4 -> `IsPaused() const` (opt-in FABLE_RETAIL_CWORLD_UPDATEINPUT). Draft's MiniMapPaused / RegionLoadStage made-up.
- CDisplayEngine (engine/CDisplayEngine.h): ScreenFadeOutInfo +0xbc (CFadeInFadeOutBase) -> `IsScreenFadeOut() const` (opt-in FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT). Draft's "CGameComponentState::Busy" is wrong.
- CThing (engine/CThing.h): Dead bitfield bit 0 of +0x91, `IsDead() const`. Draft's local m_dead.
- Locally modelled classes with shared headers in the draft: CRGBColour, CFadeInFadeOutBase, CThing, CPlayer, CPlayerManager, CWorld, CPlayerGuiDef, CMiniMapDisplay, CMarkerInfo (and CGame stand-in for CMainGameComponent).
- Ready-made include set: rebuild/src/compiled/00/7e/CTCFineDialogue_IsAllowed_007e2790.cpp defines FABLE_RETAIL_CGAMECOMPONENT, _CMAINGAMECOMPONENT, _CMAINGAMECOMPONENT_PEEKDISPLAYENGINE, _CDISPLAYENGINE_SCREENFADEOUT, _CPLAYERGUI, _CWORLD, _CWORLD_QUESTCOMPLETIONUI and uses `PeekDisplayEngine().IsScreenFadeOut()` / `GetWorld().IsPerformingRegionLoad()`.

## 5. Visibility
- CMarkerInfo::IsHeroMarker() const: same TU, out-of-line at 0x0082A210 (earlier address) and inlined here -> define it inline (`return GraphicIndex == CPlayerGui::PeekGuiDef().MiniMapMarkerGraphics[0];`). Retail order: PeekGuiDef call first, then GraphicIndex load -- matches that body. THIS is the most likely fix for the 556v541 residue (`add eax,958h; mov eax,[eax]` came from the draft's non-const front() on a local CArray struct with a pointer-returning PeekGuiDef).
- Same-TU neighbours (CMiniMapDisplay.cpp spans 0x0082A180..0x0082C0xx): IsRotatedOnMap 0x82a230, IsPulsingOnMap 0x82a270, RemoveAllDeadMarkers 0x82a2a0, AddAutoMarker 0x82a500 (called out of line), DrawMarker 0x82a570 (out of line), UpdateMarkers 0x82b7c0, DrawMap 0x82be60.
- STLport: vector clear -> erase(begin,end) out of line 0x440d70 (default fable_containers.h clear() inline calling erase; erase must be out of line: FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE); resize(n) out of line 0x443f90 (FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL). list::size() is inline (the count loop at 0x82b7f9). Open, IsOpen, Close, IsPerformingRegionLoad, IsScreenFadeOut, IsPaused, IsDead all inline.

## 6. Siblings
- 00/82/CMiniMapDisplay_DrawMap_0082be60.cpp (caller; local model of CMiniMapDisplay with MarkerFlashFade opaque, CMainGameComponent::Get(), GetMainPlayer().GetPControlledCreature(), `hero->Flags91 & 1`; no pragmas).
- 00/82/CMiniMapDisplay_AddAutoMarker_0082a500.cpp (local list/CIntelligentPointer model, ret 8).
- 00/82/CMarkerInfo_IsHeroMarker_0082a210.cpp, CMarkerInfo_IsPulsingOnMap_0082a270.cpp (PeekGuiDef as `static const CPlayerGuiDef& __fastcall CPlayerGui::PeekGuiDef()`, +0x958 modelled as a pointer member).
- 00/82/CMiniMapDisplay_InitialiseHouseMarkers_0082acd0.cpp (`#include "fable_compiler.h"`, MiniMapMarkerGraphics as CArray with const operator[]).
- None of these carry optimize pragmas -> default /O2 TU.

## 7. Draft model errors
1. Hero compare via non-const `front()` on pointer-returned PeekGuiDef -> extra `add eax,958h`; use inline const IsHeroMarker() with const& PeekGuiDef and operator[](0)/const front.
2. reserve(MarkerCount()) -> resize(Markers.size()) (0x443f90 is resize).
3. AddMarker(hero,...) -> AddAutoMarker(*hero, PeekGuiDef().MiniMapMarkerGraphics[0]).
4. Condition: `!world.IsPerformingRegionLoad() && !PeekDisplayEngine().IsScreenFadeOut()` then `if (!world.IsPaused())` -- world pointer read once (eax reused for +0x104 and +0xa4).
5. IsFullyOpen/FadeOut -> IsOpen()/Close(); Flash -> AutoMarker; MarkerCount -> list size().
6. hero ptr: GetPlayerManager().GetMainPlayer().GetPControlledCreature(); `if (hero && !hero->IsDead())`.
7. Control flow otherwise matches (three loops, heroDrawn bool, early-out ret, `if (!heroDrawn && PRegion)`).

## 0x0084D910

# DOSSIER 0x0084D910 -- NCameraModes::CBase::UpdateCameraStretchAroundHero (542 bytes)

## 1. Retail shape (xdis 0084d910 0x22e)
- __thiscall, `ret 8` (two pointer/ref args). NO frame pointer (sub esp,3Ch ; push ebx) -> default /Oy, no optimize("y",off).
- No SEH prologue. ebx=this; ebp/esi/edi pushed only AFTER the third guard (shrink-wrapped push at 0x84D962).
- esi = arg1 (position, [esp+50h]), edi = arg2 (lookAt, [esp+54h]).
- exp expanded INLINE (fldl2e/frndint/f2xm1/fscale) and fsqrt inline: `#include <math.h>` + `#pragma intrinsic(exp)` (sqrt too),
  as the landed sibling UpdateRaiseUp 0x0084D7E0 does (its exp is inline the same way).
- Constants: 0x0122DEDC = 0.0f (clamp and the Z*0 of the 3D scale), 0x0122DED8 = 1.0f, 0x01231564 = -0.01f.
- Clamp: `fcom [0.0]; fnstsw; test ah,5; jp` -> `if (d < 0.0f) d = 0.0f` (keep fstp/fld 0 form).
- Order of calls: vslot 8 (+0x20); GetPItem; [thing+0x10]==2; GetPItem->PeekPlayer->IsMainPlayer; GetPItem (hero -> ebp);
  offset = lookAt - position; GetPItem->GetPos (copied BY VALUE to frame [1c..24]); dx,dy,dz = position - heroPos;
  hero->0x6625C0 (nominal radius); hero->GetCurrentMeshRadius (stored into dead arg slot [esp+54h]); sqrt(dy*dy+dx*dx)-curR;
  clamp; blend=exp(d*d*-0.01f) (into dead arg slot [esp+50h]); hero->0x6625E0 (nominal height);
  vslot 15 (+0x3c) / nominalHeight ... ; then radius ratio.
- Tail: heightTerm = ((vslot15()/nomH - 1)*blend + 1)*dz stays on x87 st(1); scale = (curR/nomR - 1)*blend + 1;
  v = C3DVector(dx,dy,0)*scale (Z really computed as scale*0.0f); p = heroPos + v; stores X,Y, then Z = p.Z + heightTerm;
  then lookAt = offset + position (fld offset; fadd [esi] -> offset + position order).

## 2. Identity (manifest label is CORRECT up to namespace/const)
- Mangled: `?UpdateCameraStretchAroundHero@CBase@NCameraModes@@IBEXAAVC3DVector@@0@Z`
  sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x00845470), ghidra_out/_fablewin_api.tsv (0x017CE494, 0x02885EB0),
  naming_stragglers/ego_r.exe.signatures.tsv (582 bytes), FableWin signatures (523 bytes); shared header
  rebuild/include/engine/NCameraModes_CBase.h (CYLINDER branch) declares it `protected: void UpdateCameraStretchAroundHero(C3DVector&, C3DVector&) const;`
- => protected, CONST member, void, (C3DVector& position, C3DVector& lookAt). Old draft is NOT const -- wrong.
- Caller: single, 0x0084DD91 inside NCameraModes::CBase::FrameUpdate 0x0084DC10 (not landed; parked DIFFER in hand281).
- Neighbour order (Ego_r donor run, UpdateRaiseUp row header): ...0x0084D7D0 IsStretchAroundHero, 0x0084D7E0 UpdateRaiseUp,
  0x0084D8B0 GetObservedThingCurrentMeshHeight, 0x0084D910 THIS, 0x0084DB30 UpdateCameraOcclusion, 0x0084DC10 FrameUpdate.

## 3. Callees (draft spelling vs proven name)
| addr | landed file | proven name (PDB) | draft spelling |
|---|---|---|---|
| vslot 8 (+0x20) | 00/84/NCameraModesCBase_IsStretchAroundHero_0084d7d0.cpp | `virtual bool IsStretchAroundHero() const` (?IsStretchAroundHero@CBase@NCameraModes@@UBE_NXZ; header slot order) | UsesCameraStretchAroundHero -- WRONG |
| vslot 15 (+0x3c) | 00/84/CBase_GetObservedThingCurrentMeshHeight_0084d8b0.cpp | `virtual float GetObservedThingCurrentMeshHeight() const` (header CYLINDER slot 15) | GetStretchHeight -- WRONG |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (const) | GetPItem non-const -- fix const |
| 0x004C7A10 | 00/4c/CThing_PeekPlayer_004c7a10.cpp | ?PeekPlayer@CThing@@QBEABVCPlayer@@XZ -> `const CPlayer& PeekPlayer() const` | GetPlayer() returning CPlayer* -- WRONG (GetPlayer is 0x004C7A30) |
| 0x00487520 | 00/48/CPlayer_IsMainPlayer_00487520.cpp | ?IsMainPlayer@CPlayer@@QBE_NXZ (Ego_r sig hash identical 0432063c059da9ba) | ok (const) |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ `const C3DVector& GetPos() const` | GetPosition -- WRONG |
| 0x006625C0 | 00/66/CTCWeapon_GetAnimationSpeedValue_006625c0.cpp (propagated label, free __fastcall model) | ?GetNominalMeshRadius@CThingCreatureBase@@QBEMXZ: reads PBaseDef (+0x70) def +0x60 = CThingBaseDef::MeshRadius (Ego_r +0x64, retail 4 lower; rebuild/include/engine/CThingBaseDef.h:26) ; Ego_r 0x0053D450 | GetStretchRadius -- WRONG |
| 0x006625E0 | 00/66/CTCWeapon_GetAnimationSpeedValue_006625e0.cpp (same) | ?GetNominalMeshHeight@CThingCreatureBase@@QBEMXZ: def +0x5C = CThingBaseDef::MeshHeight (CThingBaseDef.h:25); Ego_r 0x0053D480 | GetStretchHeightScale -- WRONG |
| 0x00664BA0 | not landed | ?GetCurrentMeshRadius@CThingCreatureBase@@QBEMXZ (Ego_r 0x0053EBC0; Fable.exe signatures label) | ok |
Names for 0x6625C0/E0 = one source (Ego_r PDB names + layout offsets match: MeshHeight 0x60/MeshRadius 0x64 in Ego_r, -4 in retail; the
nominal/current radius ratio is what the formula needs). FableWin api also lists both names (0x0178FA37/0x01D99D50, 0x017B7BBD/0x01D9A010).
Hand back NAME_FIXES for the two CTCWeapon_GetAnimationSpeedValue rows (0x006625C0, 0x006625E0).

## 4. Members
- this+0x40: `CGameCameraManager& CameraManager` (private, NCameraModes_CBase.h CYLINDER branch; MEMBERS branch has `CGameCameraManager* CameraManager`).
  Draft's `m_pManager` is a made-up name; real name CameraManager (reference).
- manager+0x100: `CIntelligentPointer<const CThingGameObject> PObservedThing` (CGameCameraManager.h, FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND typed;
  default branch opaque `unsigned char PObservedThing[0x8]` line 247). Draft's `m_observedThing` -- wrong name.
- thing+0x10: CClass::Type (unsigned char), inline `unsigned long GetType() const` (rebuild/include/engine/CClass.h, FABLE_RETAIL_CCLASS). Draft `ThingType` -- wrong; spell `GetType() == 2`.
- thing+0x70: CThing::PBaseDef (CDefPointer<const CThingBaseDef>) -- read only inside the out-of-line 0x6625C0/E0.
- Locally modelled classes in the old draft that HAVE shared headers: C3DVector (engine/C3DVector.h, FABLE_RETAIL_C3DVECTOR), C2DVector (engine/C2DVector.h),
  CPlayer (engine/CPlayer.h, FABLE_RETAIL_PLAYER_ISMAINPLAYER_OUTOFLINE declares IsMainPlayer() const 0x00487520), CThing (engine/CThing.h,
  FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_PEEKPLAYER), CThingCreatureBase (engine/CThingCreatureBase.h, FABLE_RETAIL_CTHINGCREATUREBASE --
  but it does NOT declare GetNominalMeshRadius/Height/GetCurrentMeshRadius: author must add locally or derive), CGameCameraManager
  (engine/CGameCameraManager.h), NCameraModes::CBase (engine/NCameraModes_CBase.h, FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER [+ _INLINE_ACCESSORS]),
  CIntelligentPointer (engine/CIntelligentPointer.h).

## 5. Visibility / inline accessors
- The observed thing is fetched FIVE times, each `mov eax,[ebx+40h] ; lea ecx,[eax+100h] ; call GetPItem`: the inline-accessor-on-the-manager
  form (FableWin ?GetObservedThing@CGameCameraManager@@QBEPBVCThingGameObject@@XZ, inline) reached through inline
  ?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ. The out-of-line CBase::GetObservedThing COMDAT 0x0084F1B0 is
  `mov ecx,[ecx+40h] ; add ecx,100h ; jmp GetPItem` (add form) -- do NOT call it. Header declares CBase::GetObservedThing() const non-inline
  and CGameCameraManager has no accessor -> copy UpdateRaiseUp 0x0084D7E0's idiom (derived CGameCameraManager with inline GetObservedThing
  via reinterpret_cast of PObservedThing, CBase inline GetObservedThing).
- GetObservedThingCurrentMeshHeight (0x0084D8B0) is in the same TU and earlier, but it is called VIRTUALLY (call [edx+3Ch]) -- keep virtual.
- No STLport helpers called. Every non-virtual callee is out of line (calls), none inlined.
- GetType() inline (CClass.h); IsMainPlayer called out of line (use FABLE_RETAIL_PLAYER_ISMAINPLAYER_OUTOFLINE, not SWITCHCHARACTERS which makes it inline).

## 6. Siblings (same TU, retail neighbours)
- 00/84/NCameraModesCBase_UpdateRaiseUp_0084d7e0.cpp: const, `#include <math.h>` + `#pragma intrinsic(tanh, exp)`, engine/C3DVector.h,
  FABLE_RETAIL_CTHING + CThing.h, CGameCameraManagerLayout rename trick + inline GetObservedThing, CBase with `CGameCameraManager* CameraManager` +0x40;
  no pragmas/flags; uses dead arg slot idioms; float locals in `{}` blocks to control FPU pops.
- 00/84/CBase_GetObservedThingCurrentMeshHeight_0084d8b0.cpp (local model, inline manager accessor -> lea form; named float result).
- 00/84/NCameraModesCBase_UpdateCameraOcclusion_0084db30.cpp (local model, engine/C3DVector.h, FABLE_RETAIL_CTHING; named thing local).
- 00/84/NCameraModesCBase_IsStretchAroundHero_0084d7d0.cpp (macros: FABLE_RETAIL_DEF_CLASS_BASE, _NCAMERAMODES_CBASE_CYLINDER, _INLINE_ACCESSORS, _CCOUNTEDPOINTER, _C3DVECTOR).
- 00/87/NCameraModesCProjectileWeapon_GetPosFromParams_00878be0.cpp (CYLINDER + FABLE_RETAIL_CGAMECAMERAMANAGER + CTHINGGAMEOBJECT macro set).
- Only GetPWorldSeed 0x0084D2B0 in 00/84 carries a Flags/pragma line; the rest use default flags.

## 7. Old draft model errors
- Not const; wrong virtual names (slot 8 IsStretchAroundHero, slot 15 GetObservedThingCurrentMeshHeight); GetPlayer/GetPosition/ThingType/
  GetStretchRadius/GetStretchHeightScale/m_pManager/m_observedThing are all wrong spellings (see table). Local structs for classes with headers.
- Expression order: retail evaluates `GetObservedThingCurrentMeshHeight()/nomH` blend and `*dz` FIRST (kept on x87), then the radius ratio
  `curR/nomR` blend; the height blend is multiplied by dz BEFORE the scale is computed -> write `float height = ((GetObservedThingCurrentMeshHeight()
  / nomH - 1)*blend + 1) * dz;` immediately before the scale, and combine as `C3DVector p = heroPos + C3DVector(dx,dy,0.0f)*scale;` then
  `position = C3DVector(p.X, p.Y, p.Z + height)` or similar (retail stores X,Y from the sum temp then adds height to Z only).
- nomH is fetched (0x6625E0) AFTER exp, nomR (0x6625C0) BEFORE GetCurrentMeshRadius -- draft order for those matches.
- heroPos must be a value copy (`C3DVector heroPos = GetObservedThing()->GetPos();`), as retail copies three dwords to the frame.
- C2DVector flat in the draft is not in retail: retail keeps dx [28], dy [2c], dz [14] as separate floats.

## 0x006507D5

# DOSSIER 0x006507D5 -- NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer::DrawAt (547 bytes)

## 1. Retail shape (xdis 006507d5 0x233)
- thiscall, `ret 8` (two args), returns float on x87 (`fld [ebp+8]` before `leave`).
- ebp frame `push ebp; mov ebp,esp; sub esp,0xa0`, `leave`, unaligned entry, NO SEH => size-optimised TU `#pragma optimize("s",on)` (same as landed sibling 0x006509F8). Saves ebx/esi/edi; ebx=0xFF held whole body (255 constants).
- Body: width=0 [ebp-0xc], height=0 [ebp-4]; `if (Graphic(+0x20) > 0)` { PrimitiveHandles(+0x10).resize(1) out of line 0x00443F90; bank = g_pPlayerGui(0x013B8790)->+0x1c8->+0x14 kept in EDI; vslot 0x54/0x58 (21/22) (Graphic,0) unsigned->float (fild + 2^32 fixup 0x0122DCB4); C2DBoxF(pos, C2DExtentsF(w*Scale,h*Scale)) 0x0041A7E0 at [ebp-0x3c]; box copied to [ebp-0x4c] (inline GFToScreen copy); sprite ctor 0x0041BEB0 into [ebp-0xa0] with ScaleAlpha(255) (fld gui+0x2a0 * 255.0f 0x01230014, __ftol2); info.Engine(+0x34) vslot 0x5c (23) (PrimitiveHandles[0] i.e. push [esi+0x10], sprite, 0xc0) }.
- Then: EDI = font = g_pMainGameComponent(0x013B86A0)->+0x1614c (kept in edi to the end; final `fild [edi+4]`). pos re-read from [ebp+8] (NOT cached in a register; the sprite box also does `push [ebp+8]`).
- text_pos at [ebp-0x1c]: X = w*Scale + pos.X ; Y = pos.Y; Y += (h - (float)font.Height)*0.5f (0x0122F59C); GFACoordToVCoord fastcall(ecx=&text_pos, edx=&v [ebp-0x24]).
- seconds = MainGameComponent+0x24 (PWorld) ->+0x38 (PGameScriptInterface) vslot 0x168 (90) GetTimer(TimerIndex +0x1c); result spilled to [ebp+8] (dead pos slot).
- CWideString text at [ebp-8] default-ctor 0x0099AED0 AFTER the GetTimer call; text.Sprintf(L"%02d:%02d" 0x0125A694, seconds/60, seconds%60) cdecl 0x0099BA70 (`push 0x3c; pop ecx; cdq; idiv` = /Os idiom), add esp,0x10.
- Draw2DText 0x00438010 (ecx=g_pPlayerGui): args (v_pos, text, font(edi), colour [ebp+8], Scale, 0x4a, true). Colour object lives at [ebp+8] (reuses pos/seconds slot); its address pushed BEFORE the byte stores; stores: A(+b)=bl, R(+a)=bl hoisted; then `cmp seconds,0xb; jge` -> G,B=100 (0x64) else G,B=255. i.e. red-ish (255,100,100) when seconds < 11, white otherwise.
- Return: `if (height > 0.0f)` (fcomp 0x0122DEDC, test ah,41h) result=height*Scale+2.0f (0x0122DCDC) else (float)font.Height*Scale; stored to [ebp+8], ~CWideString 0x0099B510, fld [ebp+8]. Same tail as Counter 0x006509F8.
- NO call to Clear() at entry (Counter calls vslot 2 Clear first; Timer does not).

## 2. Name / signature (manifest label correct, but namespace-qualify)
- PDB mangled leaf: `?DrawAt@CQuestInfoElementTimer@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z`
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x006931B2), ghidra_out/_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv (0x0181AC13, 0x020EEF1A); rtti-vtables.tsv: vtable 0x0125A458 (NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer) slot 0 = 0x006507D5 (slot 3 = GetType 0x006567C7, landed). Ctor 0x0064A4E9 (landed) stores that vtable.
- => `virtual float NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer::DrawAt(const C2DVector& pos, const CEnginePrimitivePrepareInfo& info)` (public virtual, thiscall). DRAFT ERROR: draft takes `CEnginePrimitivePrepareInfo*`; PDB says const reference (ABV).
- xref: only the vtable reference 0x0125A458; called virtually from CDrawQuestInfo::Draw 0x00654438 (landed).

## 3. Callees (landed row / landed name / PDB) -- draft spelling mismatches flagged
- 0x00443F90 rebuild/src/compiled/00/44/stdvector_Resize_00443f90.cpp -- STLport vector<CEnginePrimitiveHandle>::resize(n) out of line; sibling spells `PrimitiveHandles.resize(1)` with FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE. DRAFT: local CRetailHandleVector::Resize -- WRONG model.
- bank vslots 21/22: CGraphicDataBank::GetGraphicFrameWidth/GetGraphicFrameHeight(unsigned long,unsigned long) const (rebuild/include/engine/CGraphicDataBank.h:19-20). DRAFT: GetGraphicWidth/Height(long,long) on local CGraphicBank -- WRONG names/types.
- 0x0041A7E0 C2DBoxF_C2DBoxF_0041a7e0.cpp: `??0C2DBoxF@@QAE@ABVC2DVector@@ABVC2DExtentsF@@@Z` -- second arg is C2DExtentsF, not C2DVector (draft wrong).
- 0x0041BEB0 CEnginePrimitive2DSprite_CEnginePrimitive2DSprite_0041beb0.cpp -- use header CEnginePrimitive2DSprite.h with FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS (sibling spells: `CEnginePrimitive2DSprite s(0,GFToScreen(box),ENGINE_2D_LAYER_6,Graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,CPlayerGui::Get().ScaleAlpha(255),255,255,255,2,C2DVector(0,0),C2DVector(0,0))`). Draft enum ENGINE_2D_LAYER_QUEST_INFO is made up; 0x4a = ENGINE_2D_LAYER_6 per sibling.
- info vslot 23 (+0x5c): CIEngine::SetPersistentPrimitive(handle&, primitive, 0xc0) via `info.Engine->` (CEnginePrimitivePrepareInfo.h:33 Engine +0x34). DRAFT: CEnginePrimitiveRenderer::AddPrimitive / `Renderer` -- WRONG.
- 0x00BFEA70 __ftol2 (toolchain_library) -- from ScaleAlpha's (unsigned char)(alpha*Alpha).
- 0x009E21D0 global_GFACoordToVCoord_009e21d0.cpp `?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z` = `void __fastcall GFACoordToVCoord(const C2DVector&, C2DVector&)`. DRAFT uses pointers -- type mismatch (same bytes, wrong spelling).
- vslot 90 (+0x168): CGameScriptInterface::GetTimer(long) const (rebuild/include/engine/CGameScriptInterface.h:131, retail 0x006E7710); path CMainGameComponent::Get()->PeekWorld()/GetWorld().GetGameScriptInterface().GetTimer(TimerIndex). DRAFT: CTimerSource::GetTimerSeconds / CGameWorld::Timers -- made-up.
- 0x0099AED0 CWideString_CWideString_0099aed0.cpp `??0CWideString@@QAE@XZ`.
- 0x0099BA70 CWideString_Sprintf_0099ba70.cpp `?Sprintf@CWideString@@QAAXPBGZZ` (fable_string.h:152). DRAFT: `Format` -- WRONG NAME.
- 0x0099B510 `??1CWideString@@QAE@XZ`.
- 0x00438010 CPlayerGui_Draw2DText_00438010.cpp `?Draw2DText@CPlayerGui@@QAEXABVC2DVector@@ABVCWideString@@ABVCFontBank@@ABVCRGBColour@@MW4EEngine2DLayer@@_N@Z` -- font is `const CFontBank&` (draft: CFontBank*).
- Globals: g_pPlayerGui 0x013B8790 = CPlayerGui::Get(); g_pMainGameComponent 0x013B86A0 = CMainGameComponent::Get() (PMainGameComponent; CMainGameComponent.h:175).

## 4. Members
- this: shared header rebuild/include/engine/NPlayerGui_CDrawQuestInfo.h; CQuestInfoElementTimer exists under `#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS` (TimerIndex +0x1c, Graphic +0x20 long; base: vptr, ID +4, ScriptID +8, float Scale +0xc, std::vector<CEnginePrimitiveHandle> PrimitiveHandles +0x10). Ego_r struct_layouts: TimerIndex @32, Graphic @36 (retail is 4 lower: 12-byte STLport vector). Base vtable: DrawAt 0, Update 1, Clear 2, GetType 3.
- CPlayerGui (CPlayerGui.h): PDrawQuestInfo +0x1c8 (CCountedPointer; GetQuestInfo() line 213), Alpha +0x2a0 (ScaleAlpha line 214); opt-ins FABLE_RETAIL_CPLAYERGUI, _QUEST_DRAW, _DRAWTEXT.
- CDrawBase::Bank +0x14 (CGraphicDataBank*, NPlayerGui_CDrawBase.h:67).
- CMainGameComponent +0x1614c PCutSceneMainFont via PeekMainFont() (FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT); +0x24 PWorld via FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD (PeekWorld const) / _GETWORLD.
- CWorld +0x38 PGameScriptInterface_ptr, accessor GetGameScriptInterface() const under FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE (CWorld.h:241-245).
- CFontBank +4 Height: GetFontHeight() (FABLE_RETAIL_CFONTBANK, _BASE, _HEIGHT).
- CEnginePrimitivePrepareInfo +0x34 Engine (CIEngine*); CIEngine opt-ins FABLE_RETAIL_CIENGINE, _RENDER (per sibling).
- CRGBColour (CRGBColour.h, layout B,G,R,A; FABLE_RETAIL_CRGBCOLOUR gives (r,g,b) ctor => Initialise(r,g,b,255)).
- Classes the draft models LOCALLY that have shared headers: C2DVector, C2DBoxF, CWideString, CFontBank, CRGBColour, CEnginePrimitiveHandle, CEnginePrimitive2DSprite, CEnginePrimitivePrepareInfo, CIEngine (draft "CEnginePrimitiveRenderer"), CGraphicDataBank (draft "CGraphicBank"), CPlayerGui, CMainGameComponent, CWorld (draft "CGameWorld"), CGameScriptInterface (draft "CTimerSource"), NPlayerGui::CDrawQuestInfo / CQuestInfoElement / CQuestInfoElementTimer, CDrawBase (draft "CDrawQuestInfoBase"), STLport vector (draft "CRetailHandleVector").

## 5. Callee visibility
- All inline accessors visible: CPlayerGui::Get/GetQuestInfo/ScaleAlpha, CMainGameComponent::Get/PeekMainFont/PeekWorld, CWorld::GetGameScriptInterface, CFontBank::GetFontHeight, CCountedPointer operator*, C2DExtentsF ctor, GFToScreen (sibling defines `inline C2DBoxF FABLE_FASTCALL GFToScreen(const C2DBoxF& box){return box;}` in NPlayerGui) -- all emitted inline.
- Out of line: vector::resize 0x00443F90 (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE, rebuild/include/stlport/fable_containers.h:540), C2DBoxF ctor, sprite ctor, CWideString ctor/dtor/Sprintf, Draw2DText, GFACoordToVCoord.
- Same TU neighbours: Counter::DrawAt 0x006509F8 follows immediately (landed); other DrawAts (Bar 0x0064fdb4?, Tick, CounterList, MiniGameBar 0x00651273) in the same file region. GetTimer is virtual (not inlined) -- no CTimer::GetTime involved (TimerIndex is a plain long here, read directly).

## 6. Siblings
- rebuild/src/compiled/00/65/NPlayerGuiCDrawQuestInfoCQuestInfoElementCounter_DrawAt_006509f8.cpp -- THE template (549 bytes, same frame 0xa0, same sprite/text/return blocks). Uses `#pragma optimize("s",on)` and defines: FABLE_RETAIL_DEF_CLASS_BASE, _CENGINEPERSISTENTPRIMITIVEBASE, _CENGINEPRIMITIVEHANDLE, _CIVCOUNTEDPOINTER, _CPLAYERGUI, _CPLAYERGUI_QUEST_DRAW, _CPLAYERGUI_DRAWTEXT, _CMAINGAMECOMPONENT_MAINFONT, _CFONTBANK, _CFONTBANK_BASE, _CFONTBANK_HEIGHT, _CWIDESTRING, _CCHARSTRING, _CRGBCOLOUR, _C2DBOXF, _C2DBOXF_COPY, _C2DBOXF_EXTENTS_DECL, _CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS, _CIENGINE, _CIENGINE_RENDER, _CIENGINE_CONVERTSCREENPOS, _CIENGINE_CONVERTWORLDPOS, _STATICBLOCKS_TEXTURE_HELPERS, _CRESOURCELIST, _STLP_VECTOR_RESIZE_OUTOFLINE, _CDRAWQUESTINFO_COUNTER, _NPLAYERGUI_CDRAWBASE_HANDLES; includes CEnginePrimitive2DSprite.h, CFontBank.h, NPlayerGui_CDrawQuestInfo.h, CPlayerGui.h, CMainGameComponent.h, CEnginePrimitivePrepareInfo.h, CIEngine.h, CGraphicDataBank.h. Its return uses `{double result=...; return (float)result;}` per branch.
  For Timer: swap _CDRAWQUESTINFO_COUNTER for _CDRAWQUESTINFO_ELEMENTS, add _CMAINGAMECOMPONENT_WORLD (or _GETWORLD) + _CWORLD_GAMESCRIPTINTERFACE + include CWorld.h / CGameScriptInterface.h.
- NPlayerGuiCDrawQuestInfoCQuestInfoElementTimer_CQuestInfoElementTimer_0064a4e9.cpp (ctor; layout notes, local model though).
- NPlayerGuiCDrawQuestInfoCQuestInfoElementTimer_GetType_006567c7.cpp; NPlayerGuiCDrawQuestInfo_Draw_00654438.cpp (virtual caller); NPlayerGuiCDrawQuestInfo_AddTimer_0064f93f.cpp.

## 7. Old draft model errors
- Signature: `CEnginePrimitivePrepareInfo*` must be `const CEnginePrimitivePrepareInfo&`; vtable modelled with ~CQuestInfoElement at slot 0 -- wrong (slot 0 is DrawAt, header has it).
- Wrong names: Format->Sprintf, AddPrimitive->SetPersistentPrimitive (CIEngine, `info.Engine`), GetGraphicWidth->GetGraphicFrameWidth, GetTimerSeconds->CGameScriptInterface::GetTimer, Renderer->Engine, DefaultFont->PeekMainFont(), World->Timers->GetWorld().GetGameScriptInterface(), Resize->resize, C2DVector size -> C2DExtentsF, ENGINE_2D_LAYER_QUEST_INFO -> ENGINE_2D_LAYER_6, CFontBank* -> const CFontBank&, GFACoordToVCoord pointers -> refs.
- Register residue (553 v 547): ours caches pos in edi and spills font; retail keeps the FONT in edi from its load to the end and never registers pos. Counter sibling (identical text/return code) gets the same tail with `const CFontBank& font_bank=CMainGameComponent::Get()->PeekMainFont();` -- copy Counter's statements verbatim (shared-header types change allocation vs. local POD models). Difference from Counter: no Clear() at entry; seconds spilled in [ebp+8]; colour conditional.
- Colour: retail stores A,R then branches on seconds<11 for G,B (100 v 255), object at [ebp+8], address pushed before stores. Try `CRGBColour colour(255,255,255); if (seconds <= 10) colour = CRGBColour(255,100,100);` is listed as tried; with real header ctors (FABLE_RETAIL_CRGBCOLOUR Initialise) retry ternary `seconds < 11 ? CRGBColour(255,100,100) : CRGBColour(255,255,255)` passed directly to Draw2DText.
- Local declaration order matters: retail order = width,height ... font, text_pos, v_pos, seconds (GetTimer) BEFORE `CWideString text;`, then Sprintf.

## 0x007AF490

# DOSSIER 0x007AF490 CTCBoss::KillHelpers (550 bytes)

## 1. Retail shape (xdis 007af490 0x236)
- __thiscall, plain `ret` (no args). this -> esi for the whole function.
- `push ebp; mov ebp,esp; and esp,-8; sub esp,0x168` = 8-ALIGNED FRAME (a local needs 8-byte alignment: the knockdown action
  temporary / its contents), NOT /Oy-. Locals are esp-relative. No SEH prologue (no fs:0) -> no EH unwinding; no /GS cookie.
- Pushes ebx, ebp, esi, edi. Index i in ebp, also homed at [esp+0x1c] (reloaded after the else-arm clobbers ebp).
- Loop: `size = (_M_finish - _M_start) >> 3` (sar 3), unsigned `jbe` entry / `jb` bottom test, size re-read every iteration.
  Element address `lea edi,[ebp*8]` + `[esi+0x30]` re-read each use (operator[] = *(begin()+i), as in SetHelperCreature 0x7AF310).
- Per element: GetPItem twice (null test, then Dead bit `test byte [eax+0x91],1`) -> skip to slot-clear.
- Def block: def local [esp+0x18]=0, `GetPItem()->[+0x70]` (CThing def ptr) -> `GetSubDef(&def)` 0x669580; if true:
  CCharString temp [esp+0x20] ("EXPLODE_ON_KILLING_BLOW" @0x0125AB88, -1), flag word [esp+0x14] |= 1, `GetEffectFromString(name, effect)`
  (effect long at [esp+0x24]), result materialised in byte [esp+0x13]; then conditional temp dtor (flag bit test, clear bit, ~CCharString).
  => classic VC conditional-temporary of `if (A && B(CCharString("..")))`. ebx = def.Object loaded from [esp+0x18] on BOTH paths
  (0x7af529 and 0x7af550) and held to the release at 0x7af64e.
- explode arm: `GetPItem()->Kill(true)` (push 1; call 0x4C9B80), jmp to release.
- else arm: ebp=GetPItem(); GetPItem()->Health(+0xB4) + 1.0f (0x0122DED8), fchs; vcall [vptr+0xEC](float, true).
  Then C3DVector(0,0,1) at [esp+0x30]; ebx = GetPItem() (ctor 1st arg, creature); [esp+0x24] = GetPItem() (SetCurrentAction object,
  spilled into the DEAD effect slot); push &dir; GetPItem()->GetPos() -> push; push 0 (hitter); push ebx; ctor 0x8679C0 into temp
  [esp+0x3c]; SetCurrentAction(temp) on [esp+0x24]; then inline ~Knockdown: ~CBaseIntelligentPointer(PThingThatHit @temp+0x118) 0xA01C10,
  ~CCharString(AnimName @temp+0x114) 0x99EAE0, ~CCombatActionBase 0x62E900 (no vptr store). Then reload ebp=[esp+0x1c], ebx=[esp+0x18].
- Join @0x7af64e: inline CDefPointer release `test ebx; je; dec [ebx+4]; jne; call [vptr+4]` (no Object=0 store: dead).
- Slot clear (for EVERY element, alive or not): temp CIntelligentPointer<CThingAICreature> at [esp+0x28]: base ctor 0xA01B10, vptr
  0x0123E7E0, SetPItem(0); then HelperCreatures[i].SetPItem(temp.GetPItem()) (inline operator=); direct ~CBaseIntelligentPointer 0xA01C10.
  => `HelperCreatures[i] = CIntelligentPointer<CThingAICreature>(0);`

## 2. Identity
- Name CTCBoss::KillHelpers, leaf `?KillHelpers@CTCBoss@@QAEXXZ` (public, non-virtual, void, no args, non-const).
  Sources: (a) ghidra_out/egor_pdb_names.tsv Ego_r 0x007B0E60 `?KillHelpers@CTCBoss@@QAEXXZ` (Ego_r address space, CTCBoss block);
  (b) ghidra_out/bsim_port_audit_final.tsv `7af490 ?KillHelpers@CTCBoss@@QAEXXZ 0.80 true`; (c) string EXPLODE_ON_KILLING_BLOW
  (fp_fable.tsv) + HelperCreatures +0x30 use; (d) TU order: retail SetHelperCreature 0x7AF310 -> KillHelpers 0x7AF490 -> AddEffect 0x7AF6C0
  (Ego_r: SetHelperCreature 0x7B0CE0, AddEffect 0x7B0E10, KillHelpers 0x7B0E60 -- same block). Manifest label is CORRECT.
  (_fablewin_api.tsv 0x017AF490 SetDynamicCast hit is a different binary address -- ignore.)
- Not in a vtable. Callers (xref.py): 0x0095AA3E, 0x0095AA4A, 0x0097561B (creature-mode code).
- engine/CTCBoss.h does NOT declare KillHelpers -- needs a declaration (no opt-in macro exists yet).
- Old draft leaf line is just `KillHelpers`; use the full mangled leaf.

## 3. Callees (landed file / landed name / PDB name; draft spelling mismatches flagged)
| addr | landed row | name (PDB) | draft spelling |
|---|---|---|---|
| 0xA01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ returns CBaseObject* | returns CThing* (WRONG type) |
| 0xA01B10 | 00/a0/..._CBaseIntelligentPointer_00a01b10.cpp | CBaseIntelligentPointer::CBaseIntelligentPointer() | ok |
| 0xA01B90 | 00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp | SetPItem(CBaseObject*) | takes CThing* (WRONG) |
| 0xA01C10 | 00/a0/..._CBaseIntelligentPointer_00a01c10.cpp | virtual ~CBaseIntelligentPointer() | draft non-virtual |
| 0x669580 | 00/66/CDefClassBase_GetSubDefCSpecialEffectsDef_00669580.cpp | ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z (const template, out CDefPointer<const CSpecialEffectsDef>*) | `CDefClassBase::GetSubDef(CDefPointer<CSubDef>*)` non-template, non-const (WRONG) |
| 0x669D30 | 00/66/CSpecialEffectsDef_GetEffectFromString_00669d30.cpp | ?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z (const, long&) | `CSubDef::CalcAndLookup(name, long*)` (WRONG name, class, pointer vs ref; renamed NF28 2026-11-12) |
| 0x99EBF0/0x99EAE0 | 00/99/CCharString_* | CCharString(const char*, long) / ~CCharString | ok |
| 0x4C9B80 | 00/4c/CThing_Kill_004c9b80.cpp | ?Kill@CThing@@QAEX_N@Z | ok |
| [vptr+0xEC] slot 59 | CThingPhysical vtable 0x01244E84 slot 59 = 0x006A68F0 (rtti-vtables.tsv) | ?ModifyHealth@CThingPhysical@@UAEXM_N@Z (Ego_r) | `ChangeHealth(float,bool)` (WRONG name) |
| 0x4C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | `GetPosition` (WRONG; file name stale too) |
| 0x8679C0 | 00/86/CCombatActionGenericStrikeResponseKnockdown_..._008679c0.cpp | CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase&, const CThing*, const C3DVector&, const C3DVector&) per engine header (landed row models CThingCreatureBase*) ; `ret 10h` | draft (CThing*, CThing*, ...) |
| 0x6644F0 | 00/66/CThingCreatureBase_SetCurrentAction_006644f0.cpp | ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z | draft on CThing taking CCombatActionBase (WRONG class/param) |
| 0x62E900 | 00/62/CCombatActionBase_CCombatActionBase_0062e900.cpp | ??1CCombatActionBase | ok |
Note sibling 0x008C8380 BanditKingStuck::Initialise still carries the stale "CCRC::CalcAndLookup" comment (same callees).

## 4. Members
- CTCBoss (engine/CTCBoss.h, Ego_r 0x2fa7d, sizeof 72): HelperCreatures `std::vector<CIntelligentPointer<CThingAICreature> >` +0x30
  (private! KillHelpers is a member so fine). Opt-ins: FABLE_RETAIL_CTCBOSS_EFFECTS / _LIVINGHELPERS / _SETHELPER / JACK_STAGE1_UPDATE
  (declarations only). No KILLHELPERS macro.
- CThing +0x70: def pointer (CThing::PDef / ThingDef; CDefClassBase-derived) -- check engine/CThing.h retail branch (FABLE_RETAIL_CTHING) for its name.
- CThing +0x91 bit0 `Dead` (PDB uchar:1 at 161 = +0xA1, retail -0x10); engine/CThing.h `bool IsDead() const { return Dead!=0; }` (line 280).
- CThingPhysical Health: PDB +204 (0xCC) -> retail +0xB4; engine/CThingPhysical.h `float Health` + `float GetHealth() const` under
  FABLE_RETAIL_CTHINGPHYSICAL; virtual ModifyHealth(float,bool) declared there (slot 59 with FABLE_RETAIL_CTHING_ORIGINAL_VTABLE?) -- verify slot index.
- Knockdown temp: AnimName +0x114 (CCharString), PThingThatHit +0x118 (CIntelligentPointer) -- engine/CCombatAction_GenericStrikeResponseKnockdown.h
  FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN (+ _FINISHACTION). Base engine/CCombatActionBase.h, CCreatureActionBase.h.
- CDefPointer<T> (engine/CDefPointer.h): default Release (in-place `--IVRefCount`) matches `dec [ebx+4]; jne; call [vptr+4]`. Opt-ins:
  _RELEASE_POINTEE / _RELEASE_COUNTREF / _RELEASE_SELFDELETING_POINTEE / _RESET / _POINTER_CTOR_ADOPT.
- CIntelligentPointer (engine/CIntelligentPointer.h, FABLE_RETAIL_CINTELLIGENTPOINTER): T* ctor inline, operator= default form
  `item=other.GetPItem(); SetPItem(item)`; also _ASSIGN, _CTOR_OUTOFLINE, _ASSIGN_OUTOFLINE, _GET_ORIGINAL_CHAIN, _COPY variants.
- CSpecialEffectsDef (engine/CSpecialEffectsDef.h): FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME (StatGetTypeName), _GETEFFECT_BODY
  (same-TU noinline body of GetEffectFromString), or FABLE_RETAIL_SKYBEAM_ORIGINAL branch.
- Old draft models LOCALLY (all have shared headers): C3DVector, CCharString (fable_string.h), CSubDef/CDefPointer, CDefClassBase, CThing,
  CBaseIntelligentPointer/CIntelligentPointer, CCombatActionBase, CCombatAction_GenericStrikeResponseKnockdown, CTCBoss, vector.

## 5. Callee visibility (what retail's compiler likely saw)
- GetSubDef<CSpecialEffectsDef> is a member TEMPLATE in the def headers -> body visible (emitted out of line as COMDAT 0x669580).
  Landed CTCUnholyPower ctor 0x007941D0 writes the template body in-row (line 86) and uses it; CTCMultiStrike::OnKill 0x007CCE00 likewise.
  This is the fix for the old residue "retail holds the sub-def pointer in ebx": the draft's out-only stand-in changed escape analysis.
- GetEffectFromString: FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY (same-TU noinline body; makes the effect local uncaptured).
- CIntelligentPointer<T>(T*), operator=, GetPItem<T> wrapper, CDefPointer dtor/Release, IsDead(), GetHealth(), C3DVector ctor: inline.
- Same-TU neighbours: SetHelperCreature 0x7AF310 (defined before), CountLivingHelpers 0x7AEEC0, AddEffect 0x7AF6C0 after.
- Knockdown dtor inline (members' dtors called directly, base dtor out of line).

## 6. Sibling landed rows
- 00/7a/CTCBoss_SetHelperCreature_007af310.cpp: no pragmas/flags; local models; IS_THING_ALIVE inline helper
  (`if (p.GetPItem()) if (!p.GetPItem()->IsDead()) return true;`), operator[] = *(begin()+i), slot assign from CIntelligentPointer temp.
- 00/7a/CTCBoss_CountLivingHelpers_007aeec0.cpp: `#include engine/CThing.h`, index-loop, `helper.Get() && !helper.Get()->IsDead()`.
- 00/7a/CTCBoss_FrameUpdate_007af050.cpp / ClearEffects_007aef60.cpp: `#pragma pack(push,1)` around engine/C3DVector.h, CVectorMap.h.
- 00/79/CTCUnholyPower_CTCUnholyPower_007941d0.cpp and 00/7c/CTCMultiStrike_OnKill_007cce00.cpp: macro set FABLE_RETAIL_CCHARSTRING,
  CCLASS, DEF_CLASS_BASE, CTCBASE, CTHING, CINTELLIGENTPOINTER(_ASSIGN), CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE,
  CDEFINITIONMANAGER, CDEFSTRINGTABLE, CSPECIALEFFECTSDEF_TYPENAME, CSPECIALEFFECTSDEF_GETEFFECT_BODY; + in-row GetSubDef template body.
- 00/8c/CCreatureActionBanditKingStuck_Initialise_008c8380.cpp: same GetSubDef && lookup("..") conditional-temp idiom (stand-ins).
- 00/66/CThingCreatureBase_OnCollision_00667310.cpp: SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdown(...)) temp idiom.

## 7. Old draft model errors
1. Stand-in out-only GetSubDef/CalcAndLookup bodies -> wrong escape/regalloc (residue: def in ebx, explode byte). Use the real template body
   + GETEFFECT_BODY noinline body.
2. Wrong callee names/types (GetEffectFromString, GetSubDef<CSpecialEffectsDef> const with CDefPointer<const CSpecialEffectsDef>,
   GetPos, ModifyHealth, SetCurrentAction on CThingCreatureBase taking const CCreatureActionBase&, GetPItem returns CBaseObject*).
3. Element type CIntelligentPointer<CThingAICreature>: GetPItem() through the template wrapper (static_cast) -> casts to
   CThingAICreature* for Kill / ModifyHealth / knockdown (CThingCreatureBase&) / SetCurrentAction.
4. Knockdown ctor 1st param is CThingCreatureBase& per header (`push ebx` of a pointer = reference); hitter is 0 (const CThing*).
5. Draft's 1.0f add then negate matches (fadd; fchs). ModifyHealth(-(Health+1.0f), true).
6. Evaluation order: creature (ebx) fetched before the SetCurrentAction object (spilled to [esp+0x24]) before GetPos -- draft's named
   `creature` then `target` locals reproduce the order; keep.
7. 8-aligned frame comes for free if the knockdown temp has an 8-aligned member (check the shared header vs local _hitInfo pad).

## 0x007F9960

# DOSSIER 0x007F9960 CTCTargetingSpirit::UpdateTargetList (551 bytes)

## 1. Retail shape (xdis 007f9960 0x237)
- thiscall, `ret` (no args), void. NO frame pointer (sub esp,5Ch; push ebx/ebp/esi/edi), NO SEH prologue
  (same in the big sibling 0x00804020 which also owns vectors -> project built without EH; do not add /EHsc tricks).
- this in ESI for the whole body (draft had it in ebp). ebx = 0 constant. edi = loop index, later targets._M_start.
- Frame 0x5C with slot reuse: [0x10] creature temp -> later IsTargetable(false,false) (2 bytes);
  [0x14] (long)MaxTargetingDistance -> later IsNotList {&players, 0}; [0x28..0x30] pos copy -> later CFilter_Bind3 {&IsNotList,&IsTargetable,&In3DRange};
  [0x40] first facing (dead); [0x4c..0x58] In3DRange {pos, (float)radius}; [0x5c..0x68] C2DBoxI; [0x1c..0x24] players vector; [0x34..0x3c] targets vector.
- Sequence: pos = parent.GetPos() copied (0x4C73D0); parent.PhysicsTC->vslot+0x118 (GetFacingVector, hidden ret ptr, result never read);
  ftol(MaxTargetingDistance)->radius; ftol(pos.Y); ftol(pos.X); ftol(Max+1.0f [0x0122DED8]) -> half;
  box = (x-half, y-half, x+half, y+half)  == C2DBoxI(pos.ToC2DCoordI(), (long)(MaxTargetingDistance+1.0f)) (ToC2DCoordI evaluates Y then X: C2DCoordI((long)X,(long)Y) args R->L);
  NOTE radius ftol happens BEFORE the coord ftols -> a named `long` local computed first (e.g. `long max_dist=(long)MaxTargetingDistance;`), used later as `(float)max_dist` (fild) for In3DRange.
- GetPlayerManager().GetMainPlayer().GetPControlledCreature() -- result discarded (dead local).
- vector targets then players zero-init (targets constructed first: freed last).
- for (long i=0;i<4;++i) { creature = GetPlayerManager().GetPlayer(i).GetPControlledCreature(); if (creature) players.push_back(creature); }
  test is `cmp eax,ebx; je` after storing to [0x10] (the push_back value temp, const CThing*). push_back inlined STLport:
  finish!=end -> placement-new null test, reload finish, +4; else _M_insert_overflow(finish, x, __true_type(), 1, true) 0x0049C370 (ret 14h).
- GetThingSearchTools().GetAllThingsInRoughArea<CThing>(box, Bind3, targets) -> call 0x007F9C00 (thiscall, 3 pushed refs).
- Comparator built in place on the stack (sub esp,34h; no copy-ctor call): Hero=parent(ebp), AnchorPos=parent.GetPos(), PreferredLine=GetFacingVector() (called first: R->L),
  AngleCompare 2.0f, SqrDistCompare 0.005f(0x3BA3D70A), OverrideShortDistanceFactor 0, OverrideShortDistanceFalloff 1.0f, CompareInXYPlane false, EnemyPreferenceFactor 1.0f.
  ==> CThingCompare_BestForTargeting2(parent, parent.GetPos(), parent.GetPhysicsTC()->GetFacingVector(), 1.0f, 2.0f, 0.005f, 0.0f, 1.0f)  (header ctor order: hero,pos,line,enemy,angle,dist,short_factor,short_falloff).
  [esi+4] loaded once into ebp before the facing call -> `CThing& parent=GetParentThing();` local (or CSE).
- std::sort(targets.begin(), targets.end(), cmp) -> 0x007FAE60 __fastcall (ecx=start, edx=finish; start/finish read into edi/ebp BEFORE the comparator build and reused after).
- SetPTargetedThing(targets.empty()? 0 : targets[0]) via vslot +0x88 (`xor eax,eax; cmp edi,ebp; je; mov eax,[edi]`) -- uses the cached start/finish (c2 knows sort does not capture the vector).
- dtors: players (free [0x1c] if nonnull) then targets (free edi if nonnull); free=0x00BFEA14 (STLport deallocate inline).

## 2. Identity
- Name: CTCTargetingSpirit::UpdateTargetList, mangled `?UpdateTargetList@CTCTargetingSpirit@@IAEXXZ` (PROTECTED, void, no params, non-const, non-virtual).
  Source 1: Ego_r.pdb (egor_pdb_names.tsv 0x007F49D0) between OnInitialActivate 0x007F4950 and FrameUpdate 0x007F4C60 -- same order as retail 0x007F98E0 / 0x007F9960 / 0x007F9B90.
  Source 2: FableWin.pdb (FableWin.exe.signatures.tsv 0x0272BBE0, also 0x0182E754).
  Source 3: sole caller CTCTargetingSpirit::FrameUpdate 0x007F9B90 (vtable 0x012419A4 slot 9) tail-jumps here (`jmp 0x7f9960` at 0x7F9BED); landed row calls `UpdateTargetList()`.
  Manifest label is CORRECT. Not virtual (not in rtti-vtables).

## 3. Callees (landed file / landed name / PDB name)
- 0x004C73D0 compiled/00/4c/CThing_GetPosition_004c73d0.cpp: `CThing::GetPos() const` -> const C3DVector&. Header engine/CThing.h declares `const C3DVector& GetPos() const;`. DRAFT spelled GetPosition -- WRONG (use GetPos). (Inline accessor, no PDB out-of-line name; catalog CThing::GetPos.)
- vslot +0x118 on CThing+0x60 = CTCPhysicsBase::GetFacingVector() const (C3DVector by value). engine/CTCPhysicsBase.h line ~124; CThing::GetPhysicsTC() inline (CThing.h ~402). Sibling 0x00804020 uses `parent_thing.GetPhysicsTC()->GetFacingVector()` (2 x `call [edx+118h]` there). DRAFT modelled "CThingGraphic Graphic / GetFacing" -- WRONG, it is PhysicsTC.
- 0x00BFEA70 ftol (compiled/00/bf/global_ftol2_00bfea70.cpp) -- just C casts to long.
- 0x00686D40 compiled/00/68/CTCBase_GetPlayerManager_00686d40.cpp: `CPlayerManager& CTCBase::GetPlayerManager()` (?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ) -- returns REFERENCE (draft: pointer).
- 0x00449970 CPlayerManager_GetMainPlayer_00449970.cpp: `CPlayer& GetMainPlayer()` non-const (?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ) -- reference.
- 0x004498C0 CPlayerManager_GetPlayer_004498c0.cpp: `CPlayer& GetPlayer(long)` non-const (?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z) -- reference.
- 0x00487DC0 CPlayer_GetPControlledCreature_00487dc0.cpp: `CThingPlayerCreature* CPlayer::GetPControlledCreature() const` (?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ). DRAFT spelled GetPCharacterThing returning CThing* -- WRONG.
- 0x0049C370 stdvector_Minsertoverflow_0049c370.cpp: `std::vector<const CThing*>::_M_insert_overflow(pos,x,const __true_type&,fill_len,atend)` (ret 14h). Element type const CThing* -> players is `std::vector<const CThing*>` (also required by CThingFilter_IsNotList ctor). DRAFT's CRetailVector<CThing*>::InsertGrow -- wrong name/type.
- 0x00686D80 CTCBase_GetThingSearchTools_00686d80.cpp: `CThingSearchTools& CTCBase::GetThingSearchTools()` (?GetThingSearchTools@CTCBase@@IAEAAVCThingSearchTools@@XZ) -- reference.
- 0x007F9C00 CThingSearchToolsCalls_GetAllThingsInRoughArea_007f9c00.cpp (landed local name `CThingSearchTools_Calls::GetAllThingsInRoughArea` -- landed spelling disagrees with PDB). PDB (Ego_r 0x007F4D60): `??$GetAllThingsInRoughArea@VCThing@@V?$vector@PAVCThing@@...@std@@V?$CFilter_Bind3@VCThingFilter_IsNotList@@VCThingFilter_IsTargetable@@VCThingFilter_In3DRange@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind3@...@@AAV?$vector@PAVCThing@@...@@@Z`
  = template<T=CThing, Container=std::vector<CThing*>, Filter=CFilter_Bind3<IsNotList,IsTargetable,In3DRange>> void GetAllThingsInRoughArea(const C2DBoxI&,const Filter&,Container&) const. -> targets is std::vector<CThing*>. DRAFT "FindThings" -- wrong name. Candidate NAME_FIXES row for 0x007F9C00.
- Filters (FableWin.pdb): `??$GFFilter_Bind3@VCThingFilter_IsNotList@@VCThingFilter_IsTargetable@@VCThingFilter_In3DRange@@@@YI...` exists (0x01813B89) => original source used GFFilter_Bind3(...) (header opt-in FABLE_RETAIL_FILTER_BIND3_FUNCTION). Ctors: ??0CThingFilter_IsNotList@@QAE@ABV?$vector@PBVCThing@@...@Z (const vector<const CThing*>&), ??0CThingFilter_IsTargetable@@QAE@_N0@Z (bool,bool), ??0CThingFilter_In3DRange@@QAE@ABVC3DVector@@M@Z (const C3DVector&, float). DRAFT names CThingFilter_NotInList / IsSelectable / C3DSphere / CThingFilter_Bind3 -- all WRONG.
- 0x007FAE60 std_sort_007fae60.cpp: `std::sort<CThing**,CThingCompare_BestForTargeting2>` __fastcall (??$sort@PAPAVCThing@@VCThingCompare_BestForTargeting2@@@std@@YIX...). Opt-in FABLE_RETAIL_STLP_SORT_FASTCALL (stlport/fable_containers.h:1157) declares sort __fastcall. DRAFT "SortTargets/CTargetSorter" -- wrong.
- vslot +0x88 = CTCTargetingBase::SetPTargetedThing(CThing*) virtual (engine/CTCTargetingBase.h line 35; landed FrameUpdate 0x7F9B90 uses slot 34 +0x88).
- 0x00BFEA14 free (STLport vector dtor/deallocate inline).

## 4. Members / headers
- this->ParentThing +0x04: CTCBase (engine/CTCBase.h), accessor `CThing& GetParentThing()` inline (line 143, under FABLE_RETAIL_CTCBASE). GetPlayerManager / GetThingSearchTools declared in CTCBase.h (FABLE_RETAIL_CTCBASE).
- this->MaxTargetingDistance +0x38 float (struct_layouts_egor: CTCTargetingSpirit 0x58: MaxTargetingDistance +0x38, Targets vector<CIntelligentPointer<CThing>> +0x3C, TargetedPos C3DVector +0x4C). NO shared header engine/CTCTargetingSpirit.h exists -> declare `class CTCTargetingSpirit : public CTCTargetingBase { float MaxTargetingDistance; ... void UpdateTargetList(); }` on top of engine/CTCTargetingBase.h (base 0x38).
- CThing +0x60 PhysicsTC (retail; PDB +0x70) via CThing::GetPhysicsTC() (CThing.h, FABLE_RETAIL_CTHING; sibling also defines FABLE_RETAIL_CTCPHYSICSBASE).
- CPlayerManager (engine/CPlayerManager.h), CPlayer (engine/CPlayer.h, FABLE_RETAIL_PLAYER), CThingPlayerCreature (engine/CThingPlayerCreature.h, needs CThingPlayerCreature* -> const CThing* conversion: derives CThingCreatureBase:...:CThing).
- C2DBoxI (engine/C2DBoxI.h): TLX,TLY,BRX,BRY; ctor (const C2DCoordI&, long radius) -- non-macro version does Initialise(x-r,y-r,x+r,y+r) with named x/y locals; FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS gives direct init. C3DVector::ToC2DCoordI() (engine/C3DVector.h:285).
- CFilter_Bind3 (engine/CFilter_Bind3.h): const-ref members F1/F2/F3; GFFilter_Bind3 factory under FABLE_RETAIL_FILTER_BIND3_FUNCTION (__fastcall inline).
- CThingFilter_IsNotList (engine/CThingFilter_IsNotList.h): PNotList +0, PNotList2 +4; ctors DECLARED ONLY (no inline body) -- retail inlines `PNotList(&list), PNotList2(0)`. Landed rows that define it inline locally: 0x007F6D70 and 0x00788617 (`PNotList(&list),PNotList2(0)`). Author needs an inline body (add header opt-in or local), retail stores 0 to +4.
- CThingFilter_IsTargetable: engine/TargetingPlayerSupport.h (under FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST): ZTargeting,UsingLightning; inline ctor (bool,bool).
- CThingFilter_In3DRange: no shared header (PDB: Pos C3DVector +0, Range float +0xC, ctor (const C3DVector&, float)) -> model locally.
- CThingCompare_BestForTargeting2 (engine/CThingCompare_BestForTargeting2.h) + CVectorCompare_BestForTargeting.h: the 8-arg ctor and const-ref Hero only under FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST.
- Draft locally models (all have shared headers): CThing, C3DVector, C2DBoxI, CPlayer, CPlayerManager, CTCBase-ish CTCTargetingSpirit base, CFilter_Bind3 (as CThingFilter_Bind3), CThingFilter_IsNotList (as NotInList), CThingFilter_IsTargetable (TargetingPlayerSupport.h), CThingCompare_BestForTargeting2 (as CTargetSorter), CThingSearchTools, std::vector (as CRetailVector), CTCPhysicsBase (as CThingGraphic).

## 5. Callee visibility (same TU)
- TU order (retail): CVectorCompare::GetCompareValue 0x7F9480, GetCompareValueForPoint 0x7F9660, CThingCompare_BestForTargeting2 copy ctor 0x7F9730, map<long,float>::find 0x7F9830, OnInitialActivate 0x7F98E0, UpdateTargetList 0x7F9960, FrameUpdate 0x7F9B90, GetAllThingsInRoughArea<...> 0x7F9C00 (right after -> instantiated by THIS TU: template body visible but not inlined), then comparator operator() 0x7F9F00, sort internals 0x7F9F60..0x7FAE60 (std::sort last).
- GetAllThingsInRoughArea body: CThingSearchTools.h offers FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY (`__declspec(noinline)` template body, line ~215; chunk 768 idiom: "caller then knows the result vector is not captured and keeps its start in a register") and FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA (inline body, line 159; used by 0x804020 with UPDATE_LIST). Retail keeps targets start/finish in edi/ebp across the sort -> visibility of a non-capturing body likely matters (ROUGHAREABODY needs NThingMap_CThingMap.h, FABLE_RETAIL_CTCMAPWHO(+_SEARCHID), FABLE_RETAIL_CTHING_STATTYPEFLAGS).
- std::sort: FABLE_RETAIL_STLP_SORT_FASTCALL (declaration only). Sibling 0x804020 also has stlport/targeting_algorithms.h.
- vector<const CThing*>::_M_insert_overflow(__true_type) body exists in fable_containers.h:620 (visible); retail calls it out of line 0x0049C370 -- fine. push_back inline (fable_containers.h ~479-482).
- GetPos 0x004C73D0, GetPControlledCreature, GetPlayer, GetMainPlayer, GetPlayerManager, GetThingSearchTools: other TUs, out of line (declared only).

## 6. Sibling rows
- compiled/00/80/CTCTargetingPlayer_UpdateTargetList_00804020.cpp (RELOCATION_MATCH, Flags: /d2inlT68): the closest idiom -- defines FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST, _CTCBASE, _CTHING, _CTCPHYSICSBASE, _C3DVECTOR(_PACKED), _CTHINGSEARCHTOOLS, _CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA, _CTCMAPWHO(_SEARCHID), _CTHING_STATTYPEFLAGS, _FILTER_BIND3_FUNCTION, _STLP_SORT_FASTCALL, _STLP_VECTOR_C, _PLAYER ...; includes engine/TargetingPlayerSupport.h + stlport/targeting_algorithms.h; writes `C2DBoxI area(pos.ToC2DCoordI(),(long)(MaxTargetingDistance+...))`, `GetThingSearchTools().GetAllThingsInRoughArea<CThing>(area, GFFilter_Bind5(...), target_list)`, `parent_thing.GetPhysicsTC()->GetFacingVector()`, `std::sort(v.begin(),v.end(),cmp)`.
- compiled/00/7f/CTCTargetingSpirit_FrameUpdate_007f9b90.cpp (same class/TU; local model, no flags): calls UpdateTargetList(); SetPTargetedThing slot 34 (+0x88), GetPTargetedThing slot 33.
- compiled/00/7f/CTCTargetingSpirit_OnInitialActivate_007f98e0.cpp (same TU; fable_compiler.h, local model, auto_inline(off) stand-in GetSubDef): MaxTargetingDistance +0x38 on CTCTargetingBase 0x38.
- compiled/00/7f/std_sort_007fae60.cpp: comparator model; "Family /Gr TU: every free template function is __fastcall" -> the TU may be /Gr (member functions stay thiscall).
- compiled/00/7f/CThingSearchToolsCalls_GetAllThingsInRoughArea_007f9c00.cpp (same TU): local Filters/IsNotList model shows the filter layouts.

## 7. Old draft model errors
- Wrong names everywhere: GetPosition->GetPos, GetPCharacterThing->GetPControlledCreature, FindThings->GetAllThingsInRoughArea<CThing>, SortTargets/CTargetSorter->std::sort/CThingCompare_BestForTargeting2, CThingGraphic::GetFacing->CTCPhysicsBase::GetFacingVector, NotInList/IsSelectable/C3DSphere/Bind3 -> IsNotList/IsTargetable/In3DRange/CFilter_Bind3 (via GFFilter_Bind3).
- Getters return references (GetPlayerManager/GetMainPlayer/GetPlayer/GetThingSearchTools), draft used pointers.
- players vector type must be std::vector<const CThing*>; targets std::vector<CThing*>; draft's CRetailVector with a fake InsertGrow body.
- CFilter_Bind3 holds const refs (12 bytes) - ok in layout but use header class.
- Draft's CTCTargetingSpirit puts vptr after members (ParentThing at +0 in declaration order with virtuals declared later -> layout ok only by accident); use CTCTargetingBase.
- Frame 0x88 vs 0x5C: retail reuses slots (pos copy -> Bind3; radius -> IsNotList; creature temp -> IsTargetable): build filters as temporaries inside the call (GFFilter_Bind3(CThingFilter_IsNotList(players), CThingFilter_IsTargetable(false,false), CThingFilter_In3DRange(pos,(float)max_dist))) and keep pos a block-scoped copy; the first facing call result is a dead local; GetMainPlayer().GetPControlledCreature() result dead.
- this in ebp vs esi: in retail ebp holds GetParentThing() (comparator Hero) and finish; esi = this.
- Comparator args order/values: (parent, parent.GetPos(), facing, 1.0f, 2.0f, 0.005f, 0.0f, 1.0f).

## 0x0096CB40

# DOSSIER 0x0096CB40 CAIStateGroup_EatAtTable::GetSeat (553 bytes)

## 1. Retail shape (xdis.py 0096cb40 0x239)
- thiscall, `this` -> esi; plain `ret` (no args); returns bool (`xor al,al` / `mov al,1`).
- NO frame pointer (`sub esp,10h ; push esi`), NO SEH frame, no /Oy- pragma needed. Default flags (no sibling uses `// Flags:` or pragmas).
- Callee-saved pushes are staged: esi first; edi pushed only after the building checks; ebp after the owner GetTC; ebx after village block.
  => early `return false` at 0x96CC33 is a SEPARATE pre-push exit (pop esi only); a second `return false` at 0x96CC29 (empty list,
  after pushes) and a third at 0x96CD0C (loop end). Two `return true` tails: second jumps into first at 0x96CD31 (partial tail merge).
- Loop: begin==end tested before the loop (rotated for), iterator kept spilled at [esp+10h]; end() RE-READ each iteration as
  `cmp eax,[ebp+0Ch]` (ebp = CTCThingOwner*, ObjectList node at +0x0C) => iterate via owner->GetObjectList() each time, not a cached end.

## 2. Identity (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x009917C0) and FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv 0x017CF966,
  0x02C52A30): `?GetSeat@CAIStateGroup_EatAtTable@@IAE_NXZ` = protected, non-virtual, non-const `bool GetSeat()`. Manifest label CORRECT.
- Order Ego_r: InitStateGroup, WalkTo, Eat, GetSeat, Update, OnReplace, IsStillValid, IsRunnable == retail 0x96C870, 0x96C880, 0x96C9A0,
  0x96CB40, 0x96CD70, 0x96CFC0, 0x96D190, 0x96D450 (same TU).
- Callers (xref.py): 0x0096CD7C (Update 0x96CD70, landed) and 0x0096D45E (IsRunnable 0x96D450, landed).

## 3. Callees
| addr | role | landed file | PDB / landed name | draft spelling problem |
|---|---|---|---|---|
| vtbl +0x38 slot 14 | building | EatAtHome 0x96CB20 / EatAtTavern 0x96D260 overrides (rtti-vtables slot 14) | `?GetBuilding@CAIStateGroup_EatAt{Home,Tavern}@@MBEPAVCThingBuilding@@XZ` -> `virtual CThingBuilding* GetBuilding() const` (pure in EatAtTable) | draft `GetPBuilding()` returning CThing*, non-const: WRONG name/const |
| 0x0040F020 | CVectorMap LowerBound (non-const) | 00/40/CVectorMap_LowerBound_0040f020.cpp | via CThing::GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED) | draft re-defines LowerBound bodies locally under auto_inline(off): drop, use shared CThing.h/CVectorMap.h |
| 0x004365B0 | LowerBound const | 00/43/CVectorMap_LowerBound_004365b0.cpp | via CThing::PeekTC (const) | same |
| 0x007EE540 | | 00/7e/CTCVillageMember_GetPVillage_007ee540.cpp | `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` | draft `CTCRegionLink::GetPRegion` WRONG |
| 0x008FCFD0 | | 00/8f/CAIStateGroupBase_GetPWorldSeed_008fcfd0.cpp | CAIStateGroupBase::GetPWorldSeed | ok |
| 0x004978A0 | | 00/49/global_GFFloatRandom_004978a0.cpp, engine/GFFloatRandom.h | `float __fastcall GFFloatRandom(float, unsigned long*)` | draft arg order (seed, range) WRONG |
| 0x00A01B50 | x4 per iteration | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` (returns CBaseObject*) | draft returns CThing* |
| 0x007B8B10 | | 00/7b/CTCTavernTable_GetEatingPoint_007b8b10.cpp | `?GetEatingPoint@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3@Z` = (CThing&, C3DVector&, CRightHandedSet&, long& action_point_index, long& seat_index) | draft uses pointers (same bytes) |
| 0x007B8840 | | NOT landed | `?GetSeat@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3PBVCCharString@@@Z` -> last param `const CCharString*` (retail pushes 0) | draft `bool flag` WRONG; not in engine/CTCTavernTable.h (only local decl in 00/94/CAIStateGroupGameMaster_GetSeat_0094cb40.cpp:105) |
| 0x004A96F0 | temp ctor | 00/4a/CIntelligentPointer_CThing_CIntelligentPointer_004a96f0.cpp | `??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z` | |
| 0x004A9760 | operator= | 00/4a/CIntelligentPointerCThingCreatureBase_operator_004a9760.cpp (file name stale; leaf is CIntelligentPointer<CThing>::operator=) | `??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z` | |
| 0x00A01C10 | temp dtor | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | `??1CBaseIntelligentPointer@@UAE@XZ` | |
| inline slot 1 (+4) | CDefPointer release | | CVillageDef (CDefPointer<const CVillageDef>) | draft `CEatDef` invented |
Arg order seen in pushes: GetEatingPoint(creature, SeatPos +0x24, SeatRHSet +0x30, SeatActionPointIndex +0x20, SeatIndex +0x1C);
GetSeat(same..., 0). Creature re-read (`mov ecx,[esi+4]; mov eax,[ecx+18h]`) for each call = GetCreature() each time.

## 4. Members
- this (no shared header for CAIStateGroup_EatAtTable): copy the class block from landed Update 00/96/CAIStateGroupEatAtTable_Update_0096cd70.cpp:
  Table +0x14 CIntelligentPointer<CThing>, SeatIndex +0x1C, SeatActionPointIndex +0x20, SeatPos +0x24 C3DVector, SeatRHSet +0x30, Duration +0x48 float (Ego_r).
  PBrain +0x04 (CAIStateGroupBase.h); GetCreature() = PBrain->+0x18 (FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE).
- CThing (engine/CThing.h): UsingComponents bitset +0x20 (bit 33 TCI_THING_OWNER `[+24h]&2`, bit 35 TCI_VILLAGE_MEMBER `[+24h]&8`,
  bit 184 TCI_TAVERN_TABLE `[+34h]&0x1000000`), Components +0x44; +0x91 bit0 Dead -> IsDead(); bit5 InLimbo -> IsInLimbo() (CThing.h:485, FableWin-only inline).
- CTCThingOwner (engine/CTCThingOwner.h; FABLE_RETAIL_CTCTHINGOWNER for StatGetInterfaceType): ObjectList list<CIntelligentPointer<CThing>> +0x0C; GetObjectList().
- CTCVillage (engine/CTCVillage.h, FABLE_RETAIL_CTCVILLAGE): PDef +0x94, inline by-value GetPDef() (FABLE_FORCEINLINE) -> CVillageDef::EatDuration +0x90 (engine/CVillageDef.h). AddRef null-checked (`test ecx,ecx; je`), Release not.
- CTCTavernTable (engine/CTCTavernTable.h): only the two calls; StatGetInterfaceType present unconditionally.
- Locally-modelled classes in draft that HAVE shared headers: CThing, CInterfaceMap/CVectorMap, CBaseIntelligentPointer/CIntelligentPointer, C3DVector, CRightHandedSet,
  CTCThingOwner (as CTCTableList), CTCVillageMember (CTCRegionLink), CTCVillage (CRegionThing), CVillageDef/CDefPointer (CEatDef*), CTCTavernTable, CAIBrain, CAIStateGroupBase, CThingBuilding.

## 5. Visibility / opt-ins
- GetTC/PeekTC/HasTC: inline expanded (FABLE_RETAIL_CTHING_GETTC_EXPANDED), LowerBound out of line.
- CIntelligentPointer<CThing>(T*) out of line: FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE; operator= out of line: FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
  (both used together e.g. 00/64/NPlayerGuiCDrawSpiritTargetingCursor_Draw_0064c869.cpp, 00/84/CCreatureActionPerformExpression_FinishAction_00846dd0.cpp).
- IS_THING_ALIVE: engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE (2 GetPItem calls), then IsInLimbo (3rd), then GetTC on a 4th GetPItem whose eax feeds both bit test and LowerBound.
- STLport list: FABLE_RETAIL_STLP_LIST (used by TavernServer GetServingPoint).
- Same TU: WalkTo 0x96C880, Eat 0x96C9A0 defined earlier (not called here); Update later.

## 6. Siblings
- 00/96/CAIStateGroupEatAtTable_Update_0096cd70.cpp (landed, SAME village/duration block -- copy verbatim):
  `const CTCVillageMember* member; if (GetCreature().PeekTC(&member) && member->GetPVillage()) { Duration = (float)member->GetPVillage()->GetPDef()->EatDuration; Duration += GFFloatRandom(45.0f, GetPWorldSeed()); }`
  macros: CCHARSTRING CCLASS CDEFSTRINGTABLE DEF_CLASS_BASE CTCBASE CTHING CTHING_GETTC_EXPANDED CTCWEAPONTRAIL_UPDATE (HasTC via const bool&) CTHINGPHYSICAL
  CTHINGGAMEOBJECT CTHINGCREATUREBASE(_ANIMATION) CTHINGAICREATURE CINTELLIGENTPOINTER ... CAISTATEGROUPBASE(_MEMBERS,_GETCREATURE) CAIBRAIN_GETCREATURE CTCVILLAGE CTCTAVERNTABLE_SEATS. No pragmas/flags.
- 00/96/CAIStateGroupTavernServer_GetServingPoint_00966da0.cpp: same ObjectList loop idiom `for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin(); it != owner->GetObjectList().end(); ++it) if (IS_THING_ALIVE(*it) && (*it)->HasTC(...))`;
  macros incl. CTCTHINGOWNER, IS_THING_ALIVE, STLP_LIST, CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN, CVECTORMAP_INLINE_LOWERBOUND (NOT wanted here: retail calls LowerBound).
- 00/94/CAIStateGroupGameMaster_GetSeat_0094cb40.cpp (calls CTCTavernTable::GetSeat 0x7B8840, local decl).
- 00/96/CAIStateGroupEatAtTable_IsRunnable_0096d450.cpp, _IsStillValid_0096d190.cpp (local models, no flags).

## 7. Draft model errors
- Old DIFFER(527v553) causes per NOTES: def pointer eax vs ecx (Update solved this with CTCVillage::GetPDef() by value through FABLE_RETAIL_CTCVILLAGE, not a hand CEatDefPointer);
  early exits merged (retail: separate pre-push exit -> building test must be its own `return false` before anything else is live);
  success tails merged fully (retail: only partial).
- Draft fetches owner GetTC(0x21) without the out-param GetTC form; retail = `CTCThingOwner* owner; if (!b || b->IsDead() || !b->GetTC(&owner)) return false;` (IsDead inline, not IS_THING_ALIVE on building).
- Draft loop uses hand-rolled CPointerListNode and caches nothing but checks `!HasTC(0xb8) continue`; retail shape = `IS_THING_ALIVE(*it) && !(*it)->IsInLimbo() && (*it)->GetTC(&table)` then
  `if (table->GetEatingPoint(...)) { Table = (*it).Get()/CIntelligentPointer<CThing>(...); return true; } if (table->GetSeat(..., 0)) { same; return true; }`.
- Wrong spellings: GetPBuilding->GetBuilding (const, CThingBuilding*), CTCRegionLink/GetPRegion->CTCVillageMember::GetPVillage, CEatDef::EatingTime->CVillageDef::EatDuration,
  GFFloatRandom arg order, CTCTavernTable::GetSeat last param const CCharString* (not bool), m_flags91&0x20 -> IsInLimbo().

## 0x007BDE31

# DOSSIER 0x007BDE31 -- NThingShot::CTCShotDefinitionFireballSpell::OnCollision (555 bytes)

## 1. Retail shape (xdis 007bde31 0x23b)
- thiscall, `leave ; ret 10h` (4 dword args). `push ebp ; mov ebp,esp ; sub esp,64h` => frame pointer: TU is
  `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (unaligned entry, dec-chain switch, `push 42h ; pop ebx`,
  `push 10h ; pop edi` = /Os idioms). NO SEH frame (no fs:0) although CHitParameters (0x60 local at [ebp-64h]) and
  CCharStrings have dtors -> same as siblings: no /EHsc unwind state emitted here.
- Frame: only one real local, CHitParameters at [ebp-64h]. `this` spilled to [ebp-4] (reloaded after Reset at 0x7BDE93).
  Dead arg slots reused: [ebp+10h] (normal) = key 0x89 for GetTC, later CCharString "CTCBurntEffect", later key 0x10 and a
  switch string; [ebp+14h] (thing) = key 0x42, then the CDefPointer<const CFireballSpellLevelDef> (zeroed at 0x7BDF4A,
  destroyed at 0x7BE044..0x7BE055); [ebp+0Ch] (position), [ebp+8] (type), [ebp-4] (this spill) = the other switch strings.
- Flow: base OnCollision(type,pos,normal,thing) first (args forwarded, thing kept in esi) ->
  if(thing && (thing->ThingTypeFlags & 8)) { if GetTC 0x89 (BurntEffect) -> Reset(); else AddTC("CTCBurntEffect", true, 0) }
  -> if(IS_THING_ALIVE(thing) [lea ecx,[ebp+14h] = &thing, fastcall] && GetTC 0x42 (EntityEvents)) { CHitParameters hp;
  hp.SetThingHitting(*PShooter.GetPItem()); events->AddEvent(new CEventHitBySpecialAbility(hp, (EHeroAbility)0x10)); }
  -> CDefPointer<const CFireballSpellLevelDef> def (=0); level = 0 (ebx); if(PShotDef->GetSubDef(&def)) level = def->SpellLevel;
  -> CTCSoundPlayer* s; if(ParentThing->GetTC 0x10) switch(level) {0..3: s->PlayAttachedCriteria("...", true,false,false,-1)}
  -> ~def (`cmp ecx,0 ; je ; dec [ecx+4] ; jne ; call [eax+4]`, no Object=0 store).
- Note the burnt-effect GetTC choose uses `jle` keep-it, the 0x42 / 0x10 ones `jg` -> all three are the same
  GETTC_EXPANDED conditional expression; the 0x89 one loads `mov ecx,[eax+4]` straight for Reset().
- Switch: `mov eax,ebx ; xor edi,edi ; sub eax,edi ; je c0 ; dec ; je c1 ; dec ; je c2 ; dec ; jne out` then bodies laid
  out case3 (str 0x0126E118), case2 (0x0126E0F0), case1 (0x0126E0C8), case0 (0x0126E09C); each arm builds its CCharString in
  a DIFFERENT slot (+10h/+0Ch/+8/-4) and jumps to ONE shared `call ~CCharString` at 0x7BE03B. edi=0 is reused as the
  `false,false` pushes and as the null compare of the def dtor. Strings (per draft): HERO_ABILITY_FIREBALL_EXPLOSION_NO_POWERUP /
  _LEVEL_1 / _LEVEL_2 / _LEVEL_3.

## 2. Identity (manifest label is RIGHT, namespace missing)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x007bd3ef, Ego_r address):
  `?OnCollision@CTCShotDefinitionFireballSpell@NThingShot@@MAEXW4ECollideType@@ABVC3DVector@@1PAVCThing@@@Z`
  => `protected: virtual void NThingShot::CTCShotDefinitionFireballSpell::OnCollision(ECollideType, const C3DVector&, const C3DVector&, CThing*)`.
- Vtable: xref -> only reference is .rdata 0x012410D4; RTTI vtable of NThingShot::CTCShotDefinitionFireballSpell is 0x01241064
  (rebuild/manifest/rtti-vtables.tsv) => slot (0x012410D4-0x01241064)/4 = 28 = CTCShotDefinitionBase::OnCollision slot 28
  (NThingShot_CTCShotDefinitionBase.h line 71-74). Two sources agree. No direct callers.
- FableWin.pdb has no OnCollision record for this class (only GetClassTypeName etc.); the Ego_r one is the leaf.

## 3. Callees (landed file / PDB name; flags = draft spelling wrong)
| addr | landed row | PDB name | draft |
|---|---|---|---|
| 007BF510 | not landed (DIFFER in same pending dir) | ?OnCollision@CTCShotDefinitionBase@NThingShot@@UAE... (header slot 28, FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE) | ok (but draft uses `long type`: must be ECollideType, engine/ECollideType.h) |
| 00410DE0 | 00/41/STLbitset_test_00410de0.cpp | ?test@?$bitset@$0BBC@@std@@QBE_NI@Z | draft local CPresenceBits -- use std::bitset<274> (FABLE_RETAIL_STLP_BITSET_TEST_DECL) via CThing::HasTC |
| 0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound | draft CInterfaceMap::LowerBound -- WRONG spelling (landed NF unified it) |
| 007CDD10 | 00/7c/CTCBurntEffect_Reset_007cdd10.cpp | ?Reset@CTCBurntEffect@@QAEXXZ | ok |
| 004C9D60 | 00/4c/CThing_AddTC_004c9d60.cpp | ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z | draft `void* params` WRONG -> `const CTCParamsBase*`; arg2 `bool replace` |
| 0099EBF0/0099EAE0 | CCharString ctor/dtor | CCharString(const char*, long=-1) / ~ | use fable_string.h (FABLE_RETAIL_CCHARSTRING), not local |
| 0040DCE0 | 00/40/global_ISTHINGALIVECThingBuilding_0040dce0.cpp | ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z (fastcall, CThing* const&) | ok |
| 007A3BC0 / 00789358 | CHitParameters ctor / dtor rows | ??0CHitParameters@@QAE@XZ / ??1 | ok (engine/CHitParameters.h, FABLE_RETAIL_CHITPARAMETERS_CTOR) |
| 008685F0 | 00/86/CHitParameters_SetThingHitting_008685f0.cpp | ?SetThingHitting@CHitParameters@@QAEXAAVCThing@@@Z | draft `SetHitter(CThing*)` WRONG (name + takes CThing&: `hp.SetThingHitting(*shooter)`) |
| 00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | draft local class returns CThing* WRONG; use engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER), CIntelligentPointer<CThing>::GetPItem inline cast |
| 00BFEA1A | global operator new | operator new(unsigned) | ok |
| 00811830 | 00/81/NEntityEventsCEventHitBySpecialAbility_..._00811830.cpp | ??0CEventHitBySpecialAbility@NEntityEvents@@QAE@ABVCHitParameters@@W4EHeroAbility@@@Z | draft `long ability` WRONG -> EHeroAbility (engine/EHeroAbility.h), value 0x10 |
| 008113C0 | 00/81/CTCEntityEvents_AddEvent_008113c0.cpp | ?AddEvent@CTCEntityEvents@@QAEXPAVCEventBase@NEntityEvents@@@Z | draft CEventBase global WRONG -> NEntityEvents::CEventBase |
| 007BE88C | 00/7b/CDefClassBase_GetSubDefCFireballSpellLevelDef_007be88c.cpp | ??$GetSubDef@VCFireballSpellLevelDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCFireballSpellLevelDef@@@@@Z | draft `CThingShotDef::GetSubDef(CDefPointer<CFireballSpellLevelDef>*)` WRONG: CDefClassBase const member template, out param CDefPointer<const CFireballSpellLevelDef>* |
| 0066BD00 | 00/66/CTCSoundPlayer_PlayAttachedCriteria_0066bd00.cpp | ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z (unsigned long ret; bool,bool,bool,long) | draft (bool,long,long,long) void WRONG; call is (str, true, false, false, -1) |
| vslot +4 | CDefPointer release -> pointee Delete (slot 1) | CDefPointer<const CFireballSpellLevelDef>::~CDefPointer (inline) | draft local CDefBase/CDefPointer -> use engine/CDefPointer.h |

## 4. Members
- this (CTCShotDefinitionBase, engine/NThingShot_CTCShotDefinitionBase.h): ParentThing +0x04 (CTCBase), PShooter +0x28
  `CIntelligentPointer<CThing>` (struct_layouts_egor names PShooter; Ego_r offsets shifted +4 after HitThingsList, header documents it),
  PShotDef +0x38 `CDefPointer<const CThingShotDef>` (needs FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT). Caveat: under
  FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION the header makes PShooter `private:` -- a derived-class access then fails;
  don't define COLLISION (or the author must handle it). FableWin has GetPShotDef() (QBE, returns const CDefPointer&) but no
  PShooter accessor; retail reads `[edi+38h]` / `lea ecx,[edi+28h]` directly.
- CThing (engine/CThing.h): UsingComponents std::bitset<274> +0x20, Components CVectorMap +0x44, ThingTypeFlags +0x6c
  (test byte bit 3). Opt-ins: FABLE_RETAIL_CTHING, FABLE_RETAIL_CTHING_GETTC_EXPANDED (exact retail GetTC(id,&p) body: key copied
  after the presence test, map through a pointer local, conditional expression -- matches all three lookups here),
  FABLE_RETAIL_CTHING_HASTC_FORCEINLINE. GetTC<T>(T**) needs T::StatGetInterfaceType: TCI_BURNT_EFFECT=137, TCI_ENTITY_EVENTS=66,
  TCI_SOUND_PLAYER=16 (engine/ETCInterfaceType.h). FableWin names: ??$GetTC@VCTCBurntEffect@@@CThing@@QAE_NPAPAVCTCBurntEffect@@@Z,
  ??$GetTC@VCTCEntityEvents@@@...@Z, ??$GetTC@VCTCSoundPlayer@@@...(two overloads, one with ETCInterfaceType).
- CFireballSpellLevelDef (engine/CFireballSpellLevelDef.h, size 0x2c): `long SpellLevel` +0x28 (struct_layouts_egor: SpellLevel @40).
  Draft name `PowerUpLevel` WRONG.
- CTCEntityEvents.h (FABLE_RETAIL_CTCENTITYEVENTS / _ADDEVENT), CTCSoundPlayer.h (FABLE_RETAIL_CTCSOUNDPLAYER; PlayAttachedCriteria
  declared unconditionally), CHitParameters.h (FABLE_RETAIL_CHITPARAMETERS_CTOR), CDefPointer.h (ENullDef ctor; default Release
  in-place form fits `dec [ecx+4]`), CIntelligentPointer.h, ECollideType.h, EHeroAbility.h, C3DVector.h all exist.
- Draft models LOCALLY (all have shared headers): C3DVector, CCharString, CThing (+bitset/map stand-ins), CHitParameters,
  CTCEntityEvents, CTCSoundPlayer, CDefPointer/CDefBase, CFireballSpellLevelDef, CBaseIntelligentPointer, CTCShotDefinitionBase.
  No shared header: CTCBurntEffect (needs a local class with StatGetInterfaceType()=TCI_BURNT_EFFECT and Reset()),
  NEntityEvents::CEventHitBySpecialAbility (local, as ForcePushBall does).

## 5. Callee visibility
- All callees are out-of-line calls in retail; no inline-member COMDAT emitted out of line here except the template
  GetSubDef<CFireballSpellLevelDef> 0x007BE88C (same TU, AFTER this row; COMDAT-like template instantiation -- just declare it).
- CVectorMap::LowerBound 0x0040F020 and bitset::test 0x00410DE0 are called (not inlined) -- GETTC_EXPANDED + bitset test decl give that.
- GetTC/HasTC, CIntelligentPointer<T>::GetPItem, CDefPointer ctor/dtor/operator->, IS_THING_ALIVE wrapper are inline (forceinline in headers).
- Same-TU neighbours (ShotDefinition .cpp, /Os frame-pointer): NymphFun::OnCollision 0x007BDB66, MultiArrow::GetShotDamage 0x007BDCFC,
  MultiArrow::InitAttachedEffects 0x007BDD77 precede; DrainLife::SetSoulEffect 0x007BE05C, ForcePushBall::OnCollision 0x007BE0F9 follow.

## 6. Sibling landed rows to copy
- rebuild/src/compiled/00/7a/CTCForcePushPower_PlayCastSound_007a5580.cpp -- THE switch idiom: `CTCSoundPlayer* sound;
  if (ParentThing->GetTC(&sound)) switch (SpellLevel) { case 0: sound->PlayAttachedCriteria("..._NO_POWERUP", true, false,
  false, -1); break; ...}` -- string as an implicit TEMPORARY per arm (gives distinct temp slots, matching retail's 4 slots).
  Defines FABLE_RETAIL_CTHING/CTCBASE/DEF_CLASS_BASE/CCHARSTRING/CINTELLIGENTPOINTER/CTCSOUNDPLAYER/CCLASS/CTHINGPHYSICAL/
  CTHINGGAMEOBJECT/CTHINGCREATUREBASE; includes fable_string.h, engine/CThing.h, CIntelligentPointer.h, CDefPointer.h, CTCSoundPlayer.h.
- 00/7b/NThingShotCTCShotDefinitionForcePushBall_OnCollision_007be0f9.cpp (RELOCATION_MATCH; same pragmas y-off/s-on; hit-event
  block `CHitParameters hit; hit.SetThingHitting(*shooter); events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 1));`,
  thing->HasTC(0x42)/GetTC<T>(0x42) local model, CDefPointer local model, AddTC(name,false,0) with block-scoped CCharString).
- 00/7b/NThingShotCTCShotDefinitionNymphFun_OnCollision_007bdb66.cpp (same pragmas, engine/C3DVector.h + CVectorMap.h with
  FABLE_RETAIL_STLP_BITSET_TEST_DECL, protected virtual OnCollision declared MAE).
- 00/7b/CDefClassBase_GetSubDefCFireballSpellLevelDef_007be88c.cpp (FABLE_RETAIL_DEF_CLASS_BASE, CSubDefClassBase.h, CDefPointer.h).
- 00/7c/CTCMultiStrike_FrameUpdate_007cce20.cpp (burnt-effect + CHitParameters + entity events, same layout).

## 7. Draft model errors vs disassembly
- Switch arms use an inlined `Play()` helper with a NAMED CCharString -> VC7.1 merges the slots; retail has 4 distinct slots ->
  write the literal as an implicit temporary argument per arm (PlayCastSound 0x007A5580 idiom) and the correct (bool,bool,bool,long)
  signature; the shared dtor tail then stays.
- `CThing* hit = thing;` copy + a second `CHitParameters hit` shadow: retail keeps the param itself in esi and takes `&thing`
  (lea ecx,[ebp+14h]) for IS_THING_ALIVE; use `thing` directly. The burnt GetTC is `if(thing->GetTC(&burnt)) burnt->Reset(); else
  thing->AddTC(CCharString("CTCBurntEffect"), true, 0);` -- AddTC arg order pushes (0, 1, &name): replace=true, params=0.
- Level: `long level = 0` must live in ebx and the def in the [ebp+14h] slot: write `CDefPointer<const CFireballSpellLevelDef> def;`
  (ENullDef default) then `long level = 0; if (PShotDef->GetSubDef(&def)) level = def->SpellLevel;` -- retail order is `xor ebx,ebx`
  then `mov [ebp+14h],ebx`, i.e. def zeroed after level=0 materialised; try declaring level before def if order differs.
- Sound player lookup via ParentThing (`mov esi,[edi+4]`), not a local `parent` copy needed; key 0x10 in edi then edi=0.
- Wrong types: ECollideType (not long), EHeroAbility, const CTCParamsBase*, NEntityEvents::CEventBase, CBaseObject* GetPItem,
  SpellLevel name, SetThingHitting(CThing&), PlayAttachedCriteria signature, GetSubDef on CDefClassBase (const) with CDefPointer<const T>*.
- Prior residue (STATUS.tsv, hand305): DIFFER 474v555 -- "dead-arg slot reuse, level in ebx, per-arm string slots (switch
  tail-merge)"; next: block-scoped CCharString per arm. The temporary-per-arm idiom above is the untried, landed-proven alternative.

## 0x007D2CC0

# DOSSIER 0x007D2CC0 CTCTavernGameCoinGolf::EnableCourse (558 B)

## 1. Retail shape (xdis 007d2cc0 0x23e)
- __thiscall, `ret 4` (one long arg), NO frame pointer (sub esp,0xc; push ebx/ebp/edi; esi pushed later only around the loop),
  NO SEH prologue despite CCharString + CDefPointer locals (TU built without EH -- same as sibling UpdateValues 0x007D4410,
  which also has a CDefPointer and no SEH). Siblings carry no `// Flags:` -> default /O2 build. No /Os idioms.
- Frame: 3 dwords. [L0]=CCharString name (lowest), [L1]=CDefPointer def, [L2]=loop index i (spilled). Register use:
  ebx=this, ebp=i*0x3c (strength-reduced Coin offset), edi=def (reloaded from L1 after the strcmp arms clobber edi), esi=name data.
  Old draft had "string/def frame slots swapped": retail puts name BELOW def (name at esp+0x10 after push esi, def at +0x14, i at +0x18).
- Flow: def=0; ParentThing->PeekBaseDef().GetSubDef(&def) (ecx=[[this+4]+0x70]); edi=def; if (course >= def->Courses) {release; return}
  (release with NO null test: `dec [edi+4]; jne; call [vt+4]`). count = Coins.size() (idiv-by-60 magic 0x88888889, signed),
  `je` skip if 0; loop rotated, `i < size()` unsigned (jb), size recomputed each iteration; Coins._M_start reloaded every use.
- Body per coin: name = copy-ctor(Coins[i].Thing->GetScriptName()) (GetPItem +0x74). Inline CCharString::operator==(const char*):
  null arm = inlined strcmp(text,"") as `repe cmpsb` (first "" vs "", then "NULL" vs ""), non-null arm calls
  CBasicString<char>::operator==(const char*) 0x004115A0 out of line. Then `course+1 < name.GetLength()` SIGNED (jge), with the
  null arm reaching it with eax=0 (GetLength's zero reused) and edx=0. Char = `test edx; mov eax,offset NullChar 0x0129AAF4; je; mov eax,[edx]`,
  `cmp byte [ecx+eax],'1'` (ecx=course+1 index, eax=text). '1': GetPItem->vslot28(+0x70)(true); GetPItem->0x004C7C40();
  GetPItem->0x004C7590(4). else GetPItem->vslot28(false). name dtor 0x0099EAE0.
- Tail: XAngle(+0x14c)=0.45f (0x3EE66666); angle = atan2(CoinStartPos.X +0x168, CoinStartPos.Y +0x16c) (fpatan) * 1/(2pi) [0x01237F08];
  if (angle<0.0f[0x0122DEDC] || angle>=1.0f[0x0122DED8]) { angle = fmod(angle, 1.0 [double 0x01237F00]) (0x00BFEB28); if (angle<0) angle+=1.0f; }
  ZAngle(+0x148)=angle; def released at end (again no null test). The float angle is spilled to the dead `course` arg slot [esp+0x1c].

## 2. Identity
- Name CONFIRMED: `?EnableCourse@CTCTavernGameCoinGolf@@QAEXJ@Z` -> `void CTCTavernGameCoinGolf::EnableCourse(long)`, public, non-virtual, non-const.
  Sources: Ego_r.pdb (egor_pdb_names.tsv @ Ego_r 0x007CFF40 -- NB egor_pdb_names keys are Ego_r addresses; its row at 007d2cc0
  is CTCTavernGameCardSorting::NewRound and is NOT this function); FableWin.pdb (fablewin_pdb_names / _fablewin_api 0x0273F7E0);
  BSim donor match ghidra_out/bsim_port_audit_05final.tsv (7d2cc0 <- ?EnableCourse..., sim 0.645, applied);
  naming_stragglers/Fable.exe.signatures.tsv 19833; manifest functions.tsv. Manifest label is correct.
- Callers (xref): 0x007D2F0C in SetupForNewRound 0x007D2EF0 (`EnableCourse(CurrentCourse)`), 0x007D34FB in EnterGame 0x007D34A0
  (`EnableCourse(0)`), 0x007D43E3 in OnInitialActivate 0x007D3F80 (not landed). Both landed callers declare `void EnableCourse(long course);`.
- Not in a vtable (QAE).

## 3. Callees
| addr | landed row | PDB name | draft spelling |
|---|---|---|---|
| 0x007D4BD0 | 00/7d/CDefClassBase_GetSubDefCTavernGameCoinGolfDef_007d4bd0.cpp | ??$GetSubDef@VCTavernGameCoinGolfDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameCoinGolfDef@@@@@Z (const template, arg CDefPointer<const CTavernGameCoinGolfDef>*) | WRONG: non-template non-const `bool GetSubDef(CDefPointer*)` with a fake same-TU body (GetSubDefIndex/GetCoinGolfDefFromGlobalIndex are invented) |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | returns CThing* in draft; real returns CBaseObject* (use CIntelligentPointer<CThing>::operator->/Get) |
| 0x0099EC30 | 00/99/CCharString_CCharStringcopy_0099ec30.cpp | ??0CCharString@@QAE@ABV0@@Z | ok |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ??1CCharString@@QAE@XZ | ok |
| 0x004115A0 | 00/41/CBasicStringchar_CBasicStringCharoperatorequals_004115a0.cpp | CBasicString<char>::operator==(const char*) const | draft local struct `CBasicStringChar` -- use shared fable_string.h |
| 0x004C7C40 | 00/4c/CThing_SetAsFrameUpdated_004c7c40.cpp | ?SetAsFrameUpdated@CThing@@QAEXXZ | ok |
| 0x004C7590 | 00/4c/CThing_SetAsRenderedThisFrame_004c7590.cpp | ?SetAsRenderedThisFrame@CThing@@QAEXK@Z (unsigned long) | WRONG: draft "DetachTarget(long)" |
| vslot 28 (+0x70) of CThing | retail 0x004C8E50 | ?SetAsDrawable@CThing@@UAEX_N@Z | WRONG: draft "SetVisible(bool)" |
| 0x00BFEB28 | 00/bf/global_CIfmod_00bfeb28.cpp | CRT _CIfmod (fmod intrinsic) | ok (fmod) |
| fpatan | intrinsic atan2 | | `#pragma intrinsic(atan2)` like SetupForNewRound |
| inline | CCharString::operator==(const char*), GetLength(), GetAsCharArray()/GetChar(long) (?GetChar@CCharString@@QBEDJ@Z FableWin 0x024B1FC0), CThing::GetScriptName() (?GetScriptName@CThing@@QBEABVCCharString@@XZ), CThing::PeekBaseDef(), vector::size()/operator[] | all inline in retail |

## 4. Members (retail = Ego_r PDB - 0x18 for CTCTavernGame-derived classes)
- this+0x04 CTCBase::ParentThing (CThing*). engine/CTCBase.h (FABLE_RETAIL_CTCBASE).
- ParentThing+0x70 CThing::PBaseDef (CDefPointer<const CThingBaseDef>, raw Object read) -> PeekBaseDef() (engine/CThing.h, static-asserted 0x70).
- def+0x50 CTavernGameCoinGolfDef::Courses (long) [struct_layouts_egor; generated engine/CTavernGameCoinGolfDef.h is flat/no base -> not
  usable with GetSubDef; the GetSubDef row declares its own `class CTavernGameCoinGolfDef : public CSubDefClassBase` -- copy that + add Courses at +0x50,
  or check whether CSubDefClassBase size makes +0x50 land]. Draft name "NumCourses" WRONG.
- def+0x04 refcount / vslot1 Destroy: via shared engine/CDefPointer.h dtor (Release variants FABLE_RETAIL_CDEFPOINTER_RELEASE_*).
- this+0xFC/+0x100 CTCTavernGameCoinBase::Coins (std::vector<Coin>, PDB +0x114) -- _M_start/_M_finish. Draft "CoinsStart/CoinsFinish" -> use vector.
- Coin (0x3C, packed): Owned bool +0x24, Thing CIntelligentPointer<CThing> +0x25, Radius +0x30, Owner +0x34, Sound +0x38 (PDB). No shared header; landed SetupForNewRound models it locally under pack(1).
- CThing+0x74 ScriptName (CCharString) -> FABLE_RETAIL_CTHING_GETSCRIPTNAME GetScriptName().
- this+0x148 ZAngle (float) [draft "AimAngle"], +0x14C XAngle (float) [draft "Power"], +0x168/+0x16C CoinStartPos.X/.Y (C2DVector) [draft "AimY/AimX"].
- No engine/CTCTavernGameCoinGolf.h / CTCTavernGameCoinBase.h exists; siblings model them locally.
- Locally-modelled in draft but shared header exists: CCharString/CBasicString (fable_string.h + FABLE_RETAIL_CCHARSTRING), CThing (engine/CThing.h),
  CBaseIntelligentPointer (engine/CIntelligentPointer.h + FABLE_RETAIL_CINTELLIGENTPOINTER), CDefPointer (engine/CDefPointer.h, needs FABLE_RETAIL_DEF_CLASS_BASE),
  CDefClassBase (engine/CDefClassBase.h, CDefClassBase_GetSubDef.h), CThingBaseDef (engine/CThingBaseDef.h).
- Opt-ins: FABLE_RETAIL_CTHING_ISPLACED + FABLE_RETAIL_CTHING_DRAWABLE (slot 28 SetAsDrawable), FABLE_RETAIL_CTHING (SetAsRenderedThisFrame),
  FABLE_RETAIL_CTHING_MOREACCESS (SetAsFrameUpdated), FABLE_RETAIL_CTHING_GETSCRIPTNAME, FABLE_RETAIL_CCHARSTRING_GETCHAR,
  FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_SELECT / _NULLRETURN (header says these give `[ecx+eax]` indexing as retail has; default ternary gives [eax+ecx]),
  FABLE_RETAIL_CINTELLIGENTPOINTER, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE.

## 5. Callee visibility
- GetSubDef<CTavernGameCoinGolfDef> 0x007D4BD0 sits in the CoinGolf address block (after LeaveGame 0x007D4A90) = template COMDAT instantiated
  in THIS TU -> body visible to retail's compiler. Use engine/CDefClassBase_GetSubDef.h with FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
  (exact precedent: row 0x0066BC10 -- "def read back from the out pointer then stays in edi"; retail here: `mov edi,[esp+0x10]` right after
  the call, release with no null test). This replaces the draft's fake stand-in.
- CCharString operator==(const char*), GetLength, GetAsCharArray: inline (expanded here). CBasicString<char>::operator==(const char*) out of line (0x004115A0).
- vector<Coin>::size/operator[] inline (stlport/fable_containers.h line 447/464).
- Not visible: GetPItem, CCharString copy/dtor, SetAsFrameUpdated, SetAsRenderedThisFrame, fmod.

## 6. Siblings (same TU, CoinGolf block 0x007D2900..0x007D4D90)
- 00/7d/CTCTavernGameCoinGolf_SetupForNewRound_007d2ef0.cpp: no flags, `#pragma intrinsic(atan2)`, pack(1) local Coin with
  CIntelligentPointer<CThing> Thing +0x25, local CTCTavernGameCoinBase/CoinGolf with real member names; ZAngle atan2(...)*0.15915494f idiom.
- 00/7d/CTCTavernGameCoinGolf_EnterGame_007d34a0.cpp: no flags; `EnableCourse(0)`.
- 00/7d/CTCTavernGameCoinGolf_UpdateValues_007d4410.cpp: no flags, local CDefPointer<const CTavernGameDef> specialization; no SEH.
- 00/7d/CTCTavernGameCoinGolf_GetCamera_007d3560.cpp: `#define FABLE_RETAIL_CINTELLIGENTPOINTER`.
- 00/7d/CDefClassBase_GetSubDefCTavernGameCoinGolfDef_007d4bd0.cpp: FABLE_RETAIL_DEF_CLASS_BASE + CSubDefClassBase.h + CDefPointer.h; template GetSubDef.
- Cross-TU idiom: 00/66/*_0066bc10.cpp (ParentThing->PeekBaseDef().GetSubDef(&p) with GETSUBDEF_NOINLINE + FABLE_RETAIL_CTHINGBASEDEF etc.).

## 7. Old-draft model errors
1. Fake non-template GetSubDef stand-in (invented helpers); real is const template GetSubDef<T>(CDefPointer<const T>*) via the shared header + NOINLINE.
2. ParentThing->m_pDefinition (local CThingDef) -> really ParentThing->PeekBaseDef() (CThing::PBaseDef +0x70).
3. Local CCharString: GetString() returned "" (0x0122D70E) but retail's null text is NullChar 0x0129AAF4; Length() unsigned, retail compares SIGNED
   (`jge`): shared GetLength() returns long -> `course + 1 < name.GetLength()`. Use `!(name == "")` (CCharString::operator!=(const char*) is declared
   out of line in the shared header -- would emit a call).
4. Coins as raw CoinsStart/CoinsFinish pointers -> std::vector<Coin> Coins; loop `for (unsigned long i = 0; i < Coins.size(); ++i)` with Coins[i].
5. Wrong names: SetVisible->SetAsDrawable (slot 28), DetachTarget(long)->SetAsRenderedThisFrame(unsigned long), NumCourses->Courses,
   Power->XAngle, AimAngle->ZAngle, AimY/AimX->CoinStartPos.X/.Y, ScriptName via GetScriptName().
6. Char tail: index register order `[ecx+eax]` -> try GetChar(course+1) or GETASCHARARRAY_SELECT/NULLRETURN spelling.
7. Frame order name below def: declare def before the loop (it is); name inside the loop -- residue says slots swapped; try making the name the
   first-declared local or varying scope.

## 0x004085F0 (not landed)

# DOSSIER 0x004085F0 CUserProfileManager::ResetAssignedInputs (557 bytes)

## 1. Retail shape (xdis 004085f0 0x23d)
- __thiscall (esi = ecx = this), no args, plain `ret` (two copies: 0x00408788 and 0x0040881C). No frame pointer (sub esp,8; push ebp/esi/edi as callee-saved regs, ebp is a GPR). No SEH prologue (no fs:0) even though CCharString temps and the CDefPointer local have dtors -- same as the landed sibling 0x004083E0 GetConfigOptionsDefaultsDef (no SEH either). Lane default flags; no Flags line needed by siblings.
- Frame: 8 bytes = [esp+0xc] def pointer (CDefPointer, init 0 at 0x004085FF, BEFORE the first branch), [esp+0x10] the CCharString temp.
- Branch order: g_[0x013B85F4] (left-click-is-king) -> else g_[0x013B85F3] (two-mode) -> else one-mode. Each scheme: if [0x013B86A0] (CMainGameComponent::PMainGameComponent) use CGameDefinitionManager::Get (0x0044C6B0) else if [0x013B8760] (startup-def-manager counted-pointer static, read DIRECTLY, not via Get) use CStartupDefinitionManager::Get (0x0043368D); else nothing -> ApplyValues.
- Call sequence per GetDef: push -1; push str; lea ecx,[temp]; call CCharString(const char*,long) 0x0099EBF0; push &def; push &temp; call Get (no args, __fastcall); mov ecx,eax; call GetDef 0x00410820 (ret 8); lea ecx,[temp]; call ~CCharString 0x0099EAE0 -> i.e. `Mgr::Get().GetDef("LITERAL", &def)` with an implicit CCharString temp (args evaluated before Get()).
- Strings: 0x0122E1A8 = ..._LEFT_CLICK_IS_KING, 0x0122E18C = FABLE_PC_CONTROL_SCHEME_GDD (two-mode passive), 0x0122E164 = ..._GDD_AGGRESSIVE, 0x0122E13C = ..._ONE_MODE (spellings from old draft; addresses from disassembly).
- Single-def schemes (LCIK, one-mode): tail at 0x004087D6 shared (cross-jumped: LCIK-game `jmp 0x4087D6`, LCIK-frontend `jmp 0x4087D1` into the one-mode frontend call). Tail: `mov edi,[esp+0xc]; lea ebp,[edi+0x3c]; ecx=this+0x54; push ebp; call op=; ecx=this+0x60; push ebp; call op=; test edi; dec [edi+4]; jne; call [vtbl+4]` -> def pointer and &def->Controls loaded ONCE, kept in edi/ebp across both vector operator= calls and into the release.
- Two-mode scheme: passive GetDef -> `mov edx,[esp+0xc]; add edx,0x3c` -> AssignedInputsPassive(this+0x54)=; then second GetDef (same def slot, NO release between: GetDef reassigns) -> tail 0x00408743 (game/frontend merged) `mov edi,[esp+0xc]; lea edx,[edi+0x3c]` -> AssignedInputsAggressive(this+0x60)=; release via edi; ApplyValues; own epilogue+ret (0x0040877B..0x00408788).
- Note `mov eax,[0x13B86A0]` scheduled before the je at 0x00408669 (shared by two-mode/one-mode heads).

## 2. Identity
- Name CONFIRMED: `?ResetAssignedInputs@CUserProfileManager@@QAEXXZ` (public, void, no params, non-const).
  - Source A: landed caller NUISystem::CRedefinerList::ResetKeysToDefaults(bool wasd) 0x005565C0 (rebuild/src/compiled/00/55/NUISystemCRedefinerList_ResetKeysToDefaults_005565c0.cpp) calls 0x00408820 = ResetAssignedInputsWASD for wasd and 0x004085F0 = ResetAssignedInputs otherwise; landed GetAssignedInputForAction 0x00408C90 also calls 0x004085F0 as ResetAssignedInputs.
  - Source B: Ego_r.pdb order (ghidra_out/egor_pdb_names.tsv): ResetAssignedInputs 0x00408530 directly followed by ResetAssignedInputsWASD 0x00408740 (size 0x210 = 528, retail 557) = retail adjacency 0x004085F0 -> 0x00408820. FableWin.pdb has the same leaf (_fablewin_api.tsv 0x01887180). ApplyNewProfileValues 0x0040CCA0 (landed) binds 0x00408820 = WASD (GDD_WASD strings), consistent.
  - Not ResetAssignedInputsToOneMode (Ego_r 0x00405470, far away). Manifest label is RIGHT.
- Xrefs (xref.py): 10 callers 0x0040882F(in WASD fn), 0x004088EE, 0x00408CA8, 0x00408D13, 0x00408DB1, 0x00408E83, 0x00408F6C, 0x00409002, 0x0040CB28, 0x005565DA.

## 3. Callees (landed file / landed name / PDB)
- 0x0099EBF0 CCharString(const char*, long) -- rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp (fable_string.h, FABLE_RETAIL_CCHARSTRING). Draft OK.
- 0x0099EAE0 ~CCharString -- rebuild/src/compiled/00/99/CCharString_CCharString_0099eae0.cpp. OK.
- 0x0044C6B0 `?Get@CGameDefinitionManager@@SIAAV1@XZ` static __fastcall returning CGameDefinitionManager& -- 00/44/CGameDefinitionManager_Get_0044c6b0.cpp; header engine/CGameDefinitionManager.h. DRAFT WRONG: free fn `GetGameDefinitionManager()` returning CDefinitionManager*.
- 0x0043368D `?Get@CStartupDefinitionManager@@SIAAV1@XZ` (unaligned = inline emitted out of line; body `mov eax,[0x013B8760]; ret`) -- 00/43/CStartupDefinitionManager_Get_0043368d.cpp; header engine/CStartupDefinitionManager.h (FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER gives public CDefinitionManager base). DRAFT WRONG: `GetFrontendDefinitionManager()`.
- 0x00410820 GetDef by name, PDB `??$GetDef@VCControlsDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCControlsDef@@@@@Z` (FableWin 0x017F2709/0x01897270; template, CONST, `ret 8`) -- landed 00/41/CDefinitionManager_GetDef00410820_00410820.cpp (folded body; its own instantiation left unresolved there, CControlsDef is the one this row needs). Shared header engine/CDefinitionManager.h line 53 already declares `template<class T> bool GetDef(const CCharString&,CDefPointer<const T>*) const;` (FABLE_RETAIL_CDEFINITIONMANAGER, needs FABLE_RETAIL_CRESOURCELIST + FABLE_RETAIL_DEF_CLASS_BASE). DRAFT WRONG: non-template, non-const, `CDefPointer*` untyped.
- 0x00411B90 vector<CActionInputControl>::operator=(const vector&) (`ret 4`; PDB ??4?$vector@VCActionInputControl@@V?$allocator@VCActionInputControl@@@std@@@std@@QAEAAV01@ABV01@@Z exists in Ego_r at 0x0040F120) -- landed as rebuild/src/compiled/00/41/stdvector_operator_00411b90.cpp under the folded SCameraTagEntry instance (28-byte element; caller 0x004086B2 is listed there). DRAFT: local struct `CActionInputControlVector` -- use std::vector<CActionInputControl> from stlport/fable_containers.h.
- 0x00407370 `?ApplyValues@CUserProfileManager@@IAEXXZ` (PROTECTED) -- 00/40/CUserProfileManager_ApplyValues_00407370.cpp; declared in engine/CUserProfileManager.h PASSIVEINPUTS/AUDIODEFAULTS blocks.
- vtable slot 1 (+4) of the def = CIVCountedPointeeBase::Delete (as landed 0x004083E0 / engine/CDefPointer.h), reached by the inline CDefPointer dtor release `dec [edi+4]; jne; call [eax+4]` (in-place form, default CDefPointer.h Release; no RELEASE_* nested opt-in needed).

## 4. Members / globals
- CUserProfileManager (engine/CUserProfileManager.h): AssignedInputsPassive +0x54 (Ego_r +84) and AssignedInputsAggressive +0x60 (Ego_r +100; Ego_r vectors 16 bytes, retail 12), type std::vector<CActionInputControl>. Header opt-in FABLE_RETAIL_CUSERPROFILEMANAGER_PASSIVEINPUTS gives ApplyValues + AssignedInputsPassive (+0x54) ONLY -- no opt-in names AssignedInputsAggressive (+0x60) or ResetAssignedInputs: the header needs a new nested opt-in (or extend PASSIVEINPUTS) adding `void ResetAssignedInputs();` and `std::vector<CActionInputControl> AssignedInputsAggressive; // +0x60`.
- CControlsDef: Controls +0x3c (Ego_r +64, retail = Ego_r-4), vector<CActionInputControl>. Only header is engine/_quarantine/CControlsDef.h (flattened, Controls as bytes) -> no live shared header; a restated retail-layout class on CParentDefClassBase/CDefClassBase is the precedent (GetMaxMeshDetail 0x004085A0 restates CConfigOptionsDefaultsDef on engine/CParentDefClassBase.h; GetToggleSneak 0x00445BA0 restates CControlsDef with Controls +0x3c).
- Globals: 0x013B86A0 = `?PMainGameComponent@CMainGameComponent@@1PAV1@A` (ESTABLISHED_SPELLINGS.tsv; read via inline `CMainGameComponent::Get()` in fable_game.h FABLE_RETAIL_CMAINGAMECOMPONENT) -- draft's g_pMainGameComponent is SUPERSEDED. 0x013B85F3: landed spelling `g_bAggressiveModeAvailable` (unsigned char; CPlayer_SetAggressiveMode_00487eb0, CTCCombat_IsUsingUnarmedCombatIdle_00733100) -- draft's g_bControlSchemeTwoModes VIOLATES one-address-one-name. 0x013B85F4: no landed/PDB name found (draft's g_bControlSchemeLeftClickIsKing is descriptive; mark so). 0x013B8760: landed `g_value_013b8760` in 0x0043368D (a CCountedPointer<CStartupDefinitionManager> static; no PDB data name).
- Old draft LOCALLY models (shared header exists): CCharString (fable_string.h), CDefPointer/CDefBase/CRefCounted/CDefVirtual (engine/CDefPointer.h, engine/CIVCountedPointeeBase.h, engine/CDefClassBase.h), CDefinitionManager (engine/CDefinitionManager.h), CUserProfileManager (engine/CUserProfileManager.h), the vector (stlport/fable_containers.h), CActionInputControl (engine/CActionInputControl.h).

## 5. Visibility
- KEY: retail keeps the def pointer (edi) and &Controls (ebp) across the out-of-line vector operator= calls although &def escaped into GetDef. VC7.1 does this only when operator='s body is visible in the TU and is seen not to capture: FABLE_RETAIL_STLP_VECTOR_C in stlport/fable_containers.h (chunk 741 note at line ~430: "declaration-only: reloaded", exactly this row's residue 622v557). FABLE_RETAIL_STLP_VECTOR_ASSIGN is declaration-only (would keep the reload). VECTOR_C needs the __uninitialized_copy/__copy_ptrs helpers (landed 0x0040FC50 / 0x0040E9D0) -- see 0x00411B90 row.
- GetDef<T>(const CCharString&,...) 0x00410820 is an aligned COMDAT emitted out of line; GetMaxMeshDetail's file (0x004085A0, same TU, just before) defines the sibling GetDef<T>(long,...) template body in-TU -- consider defining the by-name template body too (it calls GetPDefFromInstantiationName 0x009ADA40, const, returns CDefPointer<const CDefClassBase>).
- CStartupDefinitionManager::Get / CGameDefinitionManager::Get are called (not inlined) -> declaration only. CMainGameComponent::Get() inline (load of 0x013B86A0).
- Same-TU neighbours (user-profile TU): GetConfigOptionsDefaultsDef 0x004083E0, GetMax*Detail 0x004084B0..0x004085A0 (immediately before), ResetAssignedInputsWASD 0x00408820 (immediately after, same shape, not landed), GetAssignedInputsPassive 0x004088E0.

## 6. Sibling rows (idioms)
- 00/40/CUserProfileManager_GetMaxMeshDetail_004085a0.cpp: #define FABLE_RETAIL_CCHARSTRING, _DEF_CLASS_BASE, _CGAMECOMPONENT, _CMAINGAMECOMPONENT, _SCOPED_PTR, _CRESOURCELIST, _CDEFINITIONMANAGER, _CUSERPROFILEMANAGER, _CSTARTUPDEFINITIONMANAGER; includes fable_string.h, fable_game.h, fable_user_profile.h, engine/CParentDefClassBase.h, engine/CDefPointer.h, engine/CGameDefinitionManager.h, engine/CStartupDefinitionManager.h; `if (CMainGameComponent::Get()) ... CGameDefinitionManager::Get().CDefinitionManager::GetDef(index,&result); else if (CNewFrontendGameComponent::Get()) CStartupDefinitionManager::Get().GetDef(...)`; restated def class on CParentDefClassBase; template body in-TU.
- 00/40/CUserProfileManager_RedefineControlPassive_00407cd0.cpp: FABLE_RETAIL_CUSERPROFILEMANAGER_PASSIVEINPUTS (+ CACTIONINPUTCONTROL_GETDIRECTION_BODY), engine/CUserProfileManager.h; AssignedInputsPassive + ApplyValues.
- 00/40/CUserProfileManager_SetDefaultValuesAudio_004083b0.cpp: SETTERS + AUDIODEFAULTS opt-ins, tail ApplyValues().
- 00/41/stdvector_operator_00411b90.cpp: the operator= body (GOTCHAS 504/667 tags).
- None of the CUserProfileManager rows near here use pragmas/Flags (only /GS rows 0x00407040, 0x004071C0, 0x00408120, 0x004091C0 carry `// Flags: /GS`; this row has no cookie -> no Flags).

## 7. Old-draft model errors
- Control flow and source shape look right (scheme if/else-if/else, game then frontend, implicit CCharString temps, function-scope def with null init). Residue 622v557 = the reload of the address-taken def after each operator= -> fix by making vector operator= body visible (FABLE_RETAIL_STLP_VECTOR_C) rather than by restructuring (NOTES.md's `T*&` idea is not needed and would change the GetDef signature, which is pinned by the PDB).
- Wrong spellings: free GetGameDefinitionManager/GetFrontendDefinitionManager (-> CGameDefinitionManager::Get() / CStartupDefinitionManager::Get()), non-const non-template GetDef (-> const template GetDef<CControlsDef>), g_pMainGameComponent (-> CMainGameComponent::Get()), g_bControlSchemeTwoModes (-> g_bAggressiveModeAvailable), ApplyValues declared public (it is protected IAE; harmless to the leaf of this row but header has it).
- Frontend test: retail reads 0x013B8760 itself (the startup manager's counted-pointer static), not CNewFrontendGameComponent 0x013B871C as 0x004083E0 does -- keep a pointer-presence test of 0x013B8760 (landed spelling g_value_013b8760 or a CCountedPointer null test).
- Locally modelled CDefPointer/CDefBase, vector, CCharString, CUserProfileManager must move to the shared headers (strict verifier).


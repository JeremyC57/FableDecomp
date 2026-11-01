# Hand chunk 673 (CUserProfileManager .. CConversationAttitude): 10 of 10 byte-exact

All ten rows grade MATCH / RELOCATION_MATCH under q.py, gall.sh and the read-only vland run (10 WINS, behaviour PASS under
g++ and VC7.1). No row needed more than 2 probes, so the 12-probe parking rule never applied; nothing is parked, nothing is
int3-after-noreturn, nothing is toolchain_library.

## What matched (10)
- **CUserProfileManager** 0x00408990 `~CUserProfileManager` (RELOCATION_MATCH, 2 probes): user dtor
  `GFGetSystemManager()->GetDisplayManager()->RemoveDeviceResetCallback(this);` (the chunk 671 PDB accessor) then the members;
  the queue<ETutorialCategory>'s ~deque stays out of line (retail 0x00412210) with the landed deque text. Catalog label
  "LTextSymbol" was wrong.
- **CEnginePrimitiveManagerParticleMeshGroup** 0x00BC3C20 implicit dtor (RELOCATION_MATCH, 2 probes): CVertexShader / CPixelShader
  inline virtual dtors (chunk 668 model), CArray<CVertexShader> as an STLport vector (virtual element dtors), MeshBase implicit, so
  the tail jump lands in ~CEnginePrimitiveManagerOcclusionBase 0x00B38590.
- **CEngine** 0x00B261D0 `~CEngine` (RELOCATION_MATCH, 1 probe): user dtor `Uninitialise();` (devirtualised direct call), then
  members; ~CSimpleMemoryPool and ~CIEngine are inline user dtors (their ??_G bodies prove it), CDefPointer from engine/CDefPointer.h.
- **NScript quest scripts** (implicit dtors, all 1 probe, chunk 665/671 template: PDB ctor emits vtable + ??_G + implicit ??1,
  local CLevelScriptBase<CQ_SunnyvaleMasterData> without a dtor, Ego_r members minus 0x14 and minus 4 per Dinkumware container):
  0x00EDAD10 CV_TalentlessBardScript (vector<bool> is the 12-byte generic vector), 0x00EE6C50 CV_TourGuideScript (label
  CActionTalkedTo wrong), 0x00CFAC40 CQ_ArenaScript (vector<CArenaRoundDef> with the landed CDefClassBase def models; virtual element
  dtor), 0x00E8B750 CV_FisticuffsClubScript, 0x00EE40F0 CV_TravellingHeroesScript (label CTCTavernGame wrong; map<CCharString,
  CCharString> + CScriptGameResourceObjectScriptedThingBase from the landed ~CQ_HeroSoulsBriarScript model).
- **CPosPassThru::QueryInterface** 0x00CAFC40 (MATCH, 1 probe): DirectShow DECLARE_IUNKNOWN forwarder written out of class over the
  landed CPosPassThru model; m_pUnknown at +0xC from the IMediaSeeking sub-object.
- **CConversationAttitude `_$E1`** 0x0121A460 (RELOCATION_MATCH, 2 probes): `std::vector<CCharString>
  CConversationAttitude::GeneralAttitudeToAnimName(18, "");` with the STLport 4.6 vector(n, value) text; the literal must go
  through the implicit CCharString(const char*, long = -1) conversion (an explicit `CCharString("")` keeps the temp in ebx).

## Parked for Fable 5.1 (0)
None.

## Skipped (0)
None. The two gap-scan rows are single functions: 0x00CAFC40 is followed by an int3 and its own neighbours (AddRef 0x00CAFC50,
Release 0x00CAFC60), 0x0121A460 ends at `ret` before the next .CRT$XCU initializer 0x0121A4F0.

## Toolchain-library check (the two gap-scan rows)
- 0x00CAFC40: not a library object. DirectShow BaseClasses (strmbase) are compiled into Fable.exe from the DX9 SDK source (GOTCHAS
  chunks 498 / 594, Rich header: all compiled objects cl 3077); there is no vendor strmbase.lib for lib_verify.py. It is the
  compiler's COMDAT of the inline DECLARE_IUNKNOWN member, so it is ordinary C++ reconstruction.
- 0x0121A460: not a library object: the compiler-generated dynamic initializer (`.text$yc`, .CRT$XCU entry 0x013742E4) of a Fable
  static member.

## Proven
- 0x00408990 ??1CUserProfileManager@@UAE@XZ -- first store vtable 0x0122E23C (COL .?AVCUserProfileManager@@), sole caller ??_G 0x00412B00 = slot 0; Ego_r + FableWin ??1
- 0x00412B00 ??_GCUserProfileManager@@UAEPAXI@Z -- slot 0 of 0x0122E23C (rtti-vtables.tsv), scalar form; Ego_r.pdb
- 0x00BC3C20 ??1CEnginePrimitiveManagerParticleMeshGroup@@UAE@XZ -- sole caller ??_G 0x00BC3C00 = slot 0 of 0x012A63B8 (COL chain ... MeshBase : OcclusionBase ...); walks Ego_r members +0x20..+0x54
- 0x00BC3C00 ??_GCEnginePrimitiveManagerParticleMeshGroup@@UAEPAXI@Z -- slot 0 of 0x012A63B8; FableWin.pdb
- 0x00B261D0 ??1CEngine@@UAE@XZ -- first store vtable 0x012A0F3C (COL .?AVCEngine@@ : CIEngine : CInitBaseClass; CDeviceResetCallback +0xC), sole caller ??_G 0x00B2AB10
- 0x00B2AB10 ??_GCEngine@@UAEPAXI@Z -- slot 0 of 0x012A0F3C; FableWin.pdb
- 0x00EDAD10 ??1CV_TalentlessBardScript@NScript@@UAE@XZ -- ??_G 0x00EDACF0 slot 0 of 0x012F3428 <- ctor 0x00ED76E0 <- Alloc 0x00EDACC0 <- registration "V_TalentlessBard"/"S_VTB" (0x00CD9723)
- 0x00EE6C50 ??1CV_TourGuideScript@NScript@@UAE@XZ -- ??_G 0x00EE6C30 slot 0 of 0x012F46B8 <- ctor 0x00EE41F0 <- Alloc 0x00EE6C00 <- "V_TourGuide"/"S_VTG" (0x00CD97BC)
- 0x00CFAC40 ??1CQ_ArenaScript@NScript@@UAE@XZ -- ??_G 0x00CFAC20 slot 0 of 0x012C7798 <- ctor 0x00CFA660 <- Alloc 0x00CFABF0 <- "Q_Arena"/"S_QA" (0x00CD58D1)
- 0x00E8B750 ??1CV_FisticuffsClubScript@NScript@@UAE@XZ -- ??_G 0x00E8B730 slot 0 of 0x012EAF14 <- ctor 0x00E81F60 <- Alloc 0x00E8B700 <- "V_FisticuffsClub"/"S_VFCC" (0x00CD8964)
- 0x00EE40F0 ??1CV_TravellingHeroesScript@NScript@@UAE@XZ -- ??_G 0x00EE40D0 slot 0 of 0x012F407C <- ctor 0x00EDEBA0 <- Alloc 0x00EE40A0 <- "V_TravellingHeroes"/"S_VTH" (0x00CD98EE)
- 0x00EDACF0 / 0x00EE6C30 / 0x00CFAC20 / 0x00E8B730 / 0x00EE40D0 ??_G<Class>@NScript@@UAEPAXI@Z -- slot 0 of the vtables above (scalar form); each in at least one PDB
- 0x00ED76E0 / 0x00EE41F0 / 0x00CFA660 / 0x00E81F60 / 0x00EDEBA0 ??0<Class>@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z -- sole vtable stores; Alloc callers; Ego_r + FableWin ctor spelling
- 0x00EDACC0 / 0x00EE6C00 / 0x00CFABF0 / 0x00E8B700 / 0x00EE40A0 ?Alloc@<Class>@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z -- `push size ; new ; ctor`, registration stores; Ego_r + FableWin
- 0x00CAFC40 ?QueryInterface@CPosPassThru@@UAGJABU_GUID@@PAPAX@Z -- slot 0 of the CPosPassThru / CRendererPosPassThru IMediaSeeking vtables 0x012BF940 / 0x012BF9F0 (landed CPosPassThru slots 3..); DECLARE_IUNKNOWN; ctlutil.h
- 0x0121A460 _$E1 = `dynamic initializer for 'CConversationAttitude::GeneralAttitudeToAnimName'` -- .CRT$XCU order vs the PDB's three initializers; vector 0x013B8A2C read by Init 0x004CD670 and GetAnimName(EAttitude) 0x004CD9A0; atexit thunk 0x01228D00 (landed _$E2)
- 0x004CD9A0 ?GetAnimName@CConversationAttitude@@SIABVCCharString@@W4EAttitude@@@Z -- `mov eax,[0x013B8A2C] ; lea eax,[eax+ecx*4] ; ret` (static fastcall), PDB name set (not catalogued; not graded here)
- 0x0099A410 ??1CInitBaseClass@@UAE@XZ -- stores vtable 0x0129A7CC (COL .?AVCInitBaseClass@@) then CBaseClass's; tail of ~CEngine, call in ??_GCIEngine 0x00B23770
- 0x00A5C600 ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ -- private in both PDBs; called by ~CSimpleMemoryPool (??_G 0x00987F30)
- 0x009BF5A0 ?RemoveDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z -- called on GFGetSystemManager()->PDisplayManager; only CDisplayManager's in both PDBs
- Unlisted neighbours found (catalog candidates, not graded): 0x00CAFC50 CPosPassThru::AddRef, 0x00CAFC60 CPosPassThru::Release
  (`jmp [ecx+4]` / `[ecx+8]` forwarders), 0x00CAFC80 / 0x00CAFC90 the `sub [esp+4],4` adjustor thunks to QueryInterface / AddRef
  (IMediaPosition tables 0x012BF8F8 / 0x012BF9A8); 0x00CAFC70 (`mov eax,E_FAIL ; ret 8`) unidentified.

## Descriptive names
None in the graded code. Model-only enumerators were left empty (`enum EEngineVideoOptionQuality { };`, the unnamed
RelevantRegion enum of CV_FisticuffsClubScript): the PDB type dump carries no enumerator names.

## Label corrections
- 0x00408990 LTextSymbol::~LTextSymbol -> CUserProfileManager::~CUserProfileManager.
- 0x00EE6C50 CActionTalkedTo::~CActionTalkedTo -> NScript::CV_TourGuideScript::~CV_TourGuideScript.
- 0x00EE40F0 CTCTavernGame::~CTCTavernGame -> NScript::CV_TravellingHeroesScript::~CV_TravellingHeroesScript (the manifest's ctor
  label 0x00EDEBA0 "CQ_WizardBattleScript" is wrong as well; the catalog's ctor name is right).
- The brief's "CTCTavernGame / CActionTalkedTo have RTTI" was true of those class names but not of these rows: neither vtable has a COL.
- 0x0099A410 `_global::Dtor_SharedBaseOf5_0099a410` -> CInitBaseClass::~CInitBaseClass (vland WARN RELOC_NAME).
- Destructor labels in this chunk: 5 of 8 right (TalentlessBard, Arena, Fisticuffs, ParticleMeshGroup, CEngine).

## Header debt
- engine/CLevelScriptBase.h: out-of-line `virtual ~CLevelScriptBase();` (retail's is implicit) -- local copy without it (as chunks 664-671).
- engine/CEngine.h, CIEngine.h, CSimpleMemoryPool.h: flattened records (vptr as data, no bases) -- local polymorphic model with PDB names.
- engine/CEnginePrimitiveManagerParticleMeshGroup.h flattened; the landed ~CEnginePrimitiveManagerOcclusionBase 0x00B38590 uses
  address-suffixed stand-ins (`CCountedPointer_00b38590`, `CRefCountedShader_00bc9a00`, novtable) -- its real members are
  VertexShader (CVertexShader) +0xC, UnitBox (CCountedPointer<const C3DMesh2>) +0x14, SilhouetteTreshold +0x1C.
- No engine/CUserProfileManager.h, CCharString.h, CWideString.h, CDeviceResetCallback.h, CInitBaseClass.h: local models.
- vland WARN SHARED_HEADER_AVAILABLE CBaseObject on 0x00CAFC40 is the known DirectShow-vs-Lionhead name clash (Layout line added).

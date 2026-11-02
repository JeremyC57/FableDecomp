# chunk 715 notes

All ten rows are RELOCATION_MATCH and PASS under both g++ (tall.sh) and VC7.1 (tall.py). No row is parked.
Every scalar deleting destructor the TUs emit also grades RELOCATION_MATCH against its retail ??_G (0x00670C50,
0x007C3E70, 0x007442B0, 0x00743FC0, 0x008D4720, 0x009BE1B0, 0x00A2B610, 0x00A3BDC0, 0x00A630D0, 0x00E37660).
Extents: every row ends in its tail `jmp` followed by int3 padding and starts after padding (0x00A62FF0 directly
follows a `ret 14h` at an aligned address). The oracle extents are correct; no extent fixes.

## Identities and name evidence (two sources each)
| row | symbol | source 1 (retail) | source 2 (PDB) |
|---|---|---|---|
| 0x00E37680 | ??1CV_ArcheryCompetitionScript@NScript@@UAE@XZ (implicit) | ??_G 0x00E37660 slot 0 of RTTI-less vtable 0x012E2EE4 stored by ctor 0x00E32350; Alloc 0x00E37630 `push 0A4h` (V_ArcheryCompetition / S_VAC) | Ego_r 0x00E86990, FableWin 0x03942D10 |
| 0x00670090 | ??1CTCCoopSpirit@@UAE@XZ | vtable 0x0125B264 store, RTTI : CTCBase; ??_G 0x00670C50 | Ego_r 0x006777C0, FableWin 0x020D5030 |
| 0x007C31C0 | ??1CTCDrainLife@@UAE@XZ | vtable 0x0126E284 store, RTTI CTCDrainLife : CTCBase; ??_G 0x007C3E70 | Ego_r 0x007C21C0, FableWin 0x0270FBC0 |
| 0x0073AEA0 | ??1CTCCreatureModeManager@@UAE@XZ | vtable 0x01268854 store; ??_G 0x007442B0 | Ego_r 0x007519D0, FableWin 0x024C14D0 |
| 0x00A62FF0 | ??1CVertexShaderData@@EAE@XZ (private virtual) | vtable 0x0129DA50 store, RTTI : CBaseClassNonCopyable; only reached inline from ??_G 0x00A630D0 | Ego_r 0x00BCEE00 / FableWin 0x032F39A0 ??1CVertexShaderData@@EAE@XZ; BSim 1.0 vs Ego_r |
| 0x00743FE0 | ??1CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAE@XZ (implicit) | ??_G 0x00743FC0, vtable 0x01268774 RTTI : CModeStandBase : CModeBase | Ego_r 0x00756A20, FableWin 0x024BAA60 |
| 0x008D4740 | ??1CGameScriptThing@@UAE@XZ (implicit) | ??_G 0x008D4720, vtable 0x012852CC RTTI : CScriptThing : CBaseClass; stores CScriptThing vtable 0x01238C8C | Ego_r 0x0090B420, FableWin 0x02AFE830 |
| 0x009BDB40 | ??1CXMVPlayer@@UAE@XZ | vtable 0x0129B3BC store, RTTI CXMVPlayer : CThreadedFileCallback : CBaseClassNonCopyable; ??_G 0x009BE1B0 | Ego_r 0x00B22D80, FableWin 0x02FE6D50 |
| 0x00A2B630 | ??1CMeshData@CPhysicsMeshInfo@@UAE@XZ (implicit) | ??_G 0x00A2B610, vtable 0x0129CD78 RTTI CPhysicsMeshInfo::CMeshData : CResource | Ego_r 0x00B66E60, FableWin 0x03141980 |
| 0x00A3B970 | ??1CTextureRenderer@@UAE@XZ | vtable 0x0129D08C store, RTTI : CBaseVideoRenderer ...; ??_G 0x00A3BDC0 (slot 3) | Ego_r 0x00B96330 / FableWin 0x0320C610 name (spelled QAE, see decision) |

Callee spellings were each checked against rebuild/oracles/reloc_bindings.tsv (symbol, target, caller): every callee
binds to the one landed PDB spelling at its address (calls.py output in this session): GetPItem 0x00A01B50 QBEPAVCBaseObject,
??1CBaseIntelligentPointer 0x00A01C10, ?Kill@CThing@@QAEX_N@Z 0x004C9B80, ??1CTCBase 0x00686830, ??1CCharString 0x0099EAE0,
??1CBaseClass 0x0099A2E0, ??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300, ??1CModeStandBase@@UAE@XZ 0x0073D860 (global, RTTI
.?AVCModeStandBase@@), ??1CResource 0x009FC550, ??1CScriptBase 0x00CBD510, ??1CBaseVideoRenderer 0x00CA7310 (landed leaf),
?Uninitialise@CTexture@@QAEXXZ 0x009F9F70, ?AddMode@CTCCreatureModeManager@@QAE_NK@Z 0x0073AE10, _free, ??3@YAXPAX@Z,
?pDSTGame@@3PAVCGameScriptInterfaceBase@@A. No new spelling of mine collides with a landed row or gives an address a
second spelling, except where NAME_FIXES replaces a stand-in.

## Decision for the coordinator: 0x00A3B970 spelling
Both PDBs spell `??1CTextureRenderer@@QAE@XZ` (and `??_GCTextureRenderer@@QAEPAXI@Z`): in the builds behind them
CTextureRenderer was a 28-byte class with no base (Ego_r layout). Retail derives it from CBaseVideoRenderer (RTTI,
five vtable stores in the body, ??_G at slot 3, `new` of 0x180 bytes at 0x00A3BA79), so its dtor is virtual and VC7.1
mangles it UAE (test-compiled: an implicitly virtual dtor is `??1D@@UAE@XZ`). The row is authored with Leaf
`??1CTextureRenderer@@UAE@XZ`, which is what the TU emits; QAE cannot be produced by a retail-shaped class.
The landed ??_G 0x00A3BDC0 should likewise become `??_GCTextureRenderer@@UAEPAXI@Z`.

## Label corrections (catalog)
0x007C31C0 ~CTCCoopSpirit -> CTCDrainLife::~CTCDrainLife; 0x0073AEA0 ~CVertexShaderData -> CTCCreatureModeManager::~;
0x00743FE0 ~CNewFrontendGameComponent -> NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::~; 0x009BDB40
~CBankFileEntryUpdateData -> CXMVPlayer::~CXMVPlayer; 0x00A2B630 CMeshData -> CPhysicsMeshInfo::CMeshData;
0x00E37680 -> NScript::CV_ArcheryCompetitionScript; 0x00E37660 'CVtable_012e2ee4' -> ??_G of that class.
Stale labels seen in coverage.tsv / engine_api.tsv (the landed leaf is already right): 0x00743A90 'ResetRings'
(landed as ~vector<CIcicleData>; my TU's COMDAT of it also matches).

## Landed rows to respell at landing (stand-in callee names of my rows)
??_G rows 0x00670C50 (`?Dtor@CTCCoopSpirit@@QAEXXZ`), 0x007C3E70 (`CTCDrainLife_dtor`), 0x007442B0
(`CTCCreatureModeManager_dtor`), 0x00743FC0 (`CModeIceTrollGroundSmashIcicleAssault_dtor`), 0x008D4720
(`CGameScriptThing_dtor`), 0x009BE1B0 (`?Dtor@CXMVPlayer@@QAEXXZ`), 0x00A2B610 (`CMeshData_dtor`), 0x00A3BDC0
(`CTextureRenderer_dtor`): each calls a fastcall/plain stand-in for the ??1 this chunk authors. Landed ctor 0x00E32350
is spelled in the global namespace with (void*, CGameScriptInterface*); FableWin gives
??0CV_ArcheryCompetitionScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z. Full list in NAME_FIXES.tsv.

## Header debt (local models where no shared header exists)
- CBaseIntelligentPointer / CIntelligentPointer<T> (chunk 706 model, pack(1), vtable 0x0129C95C): rows 0x00670090,
  0x007C31C0, 0x00743FE0, 0x008D4740. A shared engine/CIntelligentPointer.h would retire six-plus copies.
- CCharString one-pointer model (0x00E37680, 0x008D4740); fable_string.h elsewhere.
- CTCCreatureModeManager + CModeBase (0x0073AEA0 full, 0x00743FE0 opaque) and CAnimFilterBase / CAnimFilterOfGroup.
  Retail CAnimFilterBase vtable 0x0122FFCC: slots 0/1 _purecall, slot 2 ??_G 0x0041CCE4; CAnimFilterOfGroup 0x01267568:
  operator() 0x0073E940, ??_G 0x0073E970, Clone 0x0073E9A0. Chunk 710's model put the dtor at slot 0 and labelled
  0x0073E9A0 operator() / 0x0073E970 Clone -- both wrong.
- CThreadedFileCallback, CXMVPlayer, CSubTitle, _RTL_CRITICAL_SECTION (0x009BDB40).
- CVertexShaderData (0x00A62FF0), CPhysicsMeshInfo::CMeshData + CIVCountedPointer<T> (0x00A2B630, chunk 712 model).
- strmbase classes (0x00A3B970): CBaseVideoRenderer is 0x160 bytes in retail; the landed 0x00CA7310 model truncates it at
  0x128 (its own row still matches, but a shared strmbase header should carry the full layout).
- STLport 4.6 vector / list templates written locally in four TUs.
- strmbase base-class name: retail RTTI (CTextureRenderer COL 0x0129D08C, base at +4) names DirectShow's CBaseObject
  `.?AVCMyBaseObject@@` (renamed so it cannot clash with the engine's polymorphic CBaseObject 0x0099A430). Row 0x00A3B970
  uses CMyBaseObject (this also clears vland's SHARED_HEADER_AVAILABLE CBaseObject warning); the landed strmbase models
  (0x00A3B510, 0x00CA7310) still call it CBaseObject and should be respelled when a shared strmbase header is made.

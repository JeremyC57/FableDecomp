# Chunk 708 (NA051) -- notes

All 10 rows are RELOCATION_MATCH, and every test passes under g++ (tall.sh) and VC7.1 (tall.py). The vland check run with
`FABLE_HDR_OVERLAY=hdr` gives 10 WINS, with no RELOC_IDENTITY and no warnings after the final sources. Every extent was checked against
the int3 padding and every one is right (0x00A66990 and 0x00A66F80 each start straight after a `ret 8`, at an aligned address). No row is
a fragment.

## Names and evidence (every one from two sources)
| address | symbol | evidence |
|---|---|---|
| 0x008C3E90 | ?Initialise@CCreatureAction_ScorpionTailPlunge@@UAEXXZ | Only reference: slot 12 of vtable 0x01286494 (RTTI CCreatureAction_ScorpionTailPlunge : CCombatActionBase). Spelled the same in Ego_r 0x008FB970 and FableWin 0x02A24250. Donor order Initialise / PerformAction / FinishAction = retail 0x008C3E90 / 0x008C4810 / 0x008C48A0. |
| 0x008D6690 | ?OnActivate@CModeCreatureIsProjectile@NCreatureMode@@UAEXXZ | Only reference: slot 5 of vtable 0x01285728 (RTTI : CModeStandBase : CTCCreatureModeManager::CModeBase). Spelled the same in Ego_r 0x008179F0 and FableWin 0x02844380. |
| 0x008E63B0 | ?FinishAction@CCreatureAction_CastStrengthMultiStrikeProper@@UAEXXZ | Only reference: slot 4 of vtable 0x012877A4. Spelled the same in Ego_r 0x0090E0C0 and FableWin 0x02AE4C30. |
| 0x009FECE0 | ?Close@CGraphicDataBank@@UAEXXZ | Only reference: slot 5 of vtable 0x0129C8CC. Spelled the same in Ego_r 0x00B5D3A0 and FableWin 0x03117B80. |
| 0x009FEB80 | ?CloseGraphics@CGraphicDataBank@@AAEXXZ | Private in both PDBs (Ego_r 0x00B5CE10, FableWin 0x03117BB0). Ego_r order ??0 < CloseGraphics < ??1 < Close matches retail 0x009FEA20 < 0x009FEB80 < 0x009FEBC0 < 0x009FECE0. Its body appears inline in both ??1 and Close. Byte twin of Close's head. |
| 0x00A47180 | ??1CNavigatorBase@@UAE@XZ | Stores vtable 0x0129D4A4 first. Callers: ??_G 0x00A47E80 and the derived dtors (CNavigatorAStar / CNavigatorFlyer). Spelled the same in Ego_r 0x00B9E620 and FableWin 0x03241EC0, and reloc_bindings already binds this spelling. |
| 0x00A66990 | ??1CKeyboard@@UAE@XZ | Stores vtable 0x0129DBA8 first. Callers: ??_G 0x00A66AE0 and ~CKeyboardDX. Spelled the same in Ego_r 0x00C08830 and FableWin, and reloc_bindings binds it. Byte twin of 0x00A66F80, named from its own vtable. |
| 0x00A66F80 | ??1CMouse@@UAE@XZ | Stores vtable 0x0129DBC4 first. Callers: ??_G 0x00A673B0 and ~CMouseDX. Spelled the same in Ego_r 0x00C08EB0 and FableWin, and reloc_bindings binds it. |
| 0x00BE2B20 | ??1CEngineLandscapeMeshBuilder@@UAE@XZ | Stores vtable 0x012A7FB8 first. Callers: ??_G 0x00BE32D0 and the owner dtor 0x00B67E16. The ctor 0x00BE32A0 zeroes 17x17 dwords. Spelled the same in Ego_r 0x009C0780 and FableWin 0x02CAD340. |
| 0x005567B0 | ?ClearList@CRedefinerList@NUISystem@@IAEXXZ | Protected in both PDBs (Ego_r 0x005CF980, FableWin 0x022AF2D0). Ego_r order matches retail. Callers: Refresh 0x00557000 and OnActivate 0x005570F0. |

Callee and global spellings are all from the PDBs:
- GetPItem returns CBaseObject*. Both PDBs agree, which contradicts the brief's house style (see below).
- ?ConstantFPS@CMainGameComponent@@1JA is read through the PDB inline `static long __fastcall GetConstantFPS()`.
- ?GetAnimDuration@CThingCreatureBase@@QBEMJ@Z.
- ?OnActivate@CModeStandBase@@UAEXXZ (a global class).
- ?FinishAction@CCombatActionBase@@MAEXXZ.
- ?Initialise@CCombatActionBase@@UAEXXZ.
- ?Kill@CThing@@QAEX_N@Z.
- ?SuspendAI@CThingAICreature@@QAEX_NPAX@Z.
- ?GetPos@CThing@@QBEABVC3DVector@@XZ.
- ?Close@CBankFile@@UAEXXZ and ?Close@CBankFileAsync@@UAEXXZ.
- The STLport vector erase spellings bound by the landed resize rows.
- ?Unregister@CClass@@QAEXAAVCClassFactory@@@Z.
- ??1CSteeringAgent@@QAE@XZ and ??1CBaseObject@@MAE@XZ.
- ??1CBaseClassNonCopyable@@MAE@XZ.
- ??1CInputEvent@@QAE@XZ.
- STLport `_List_base<CMaintainedEvent>::clear`.
- ?RemoveChild@CComponent@NUISystem@@UAEXK@Z, called through slot 64.
- ?ResetButtonCounts@CKeyRedefiner@NUISystem@@SIXXZ.

Descriptive names:
- No row or callee name is descriptive.
- The only placeholders are 63 unnamed **virtual** slots, NUISystem::CComponent v01..v63, in src/005567b0.cpp. A virtual's name emits no symbol and only fixes the slot index of RemoveChild (64). Naming them from the PDB virtual list is optional cleanup.

## Name collisions and second spellings (for the coordinator)
- **0x00A01B50 GetPItem.** The brief's house style and two reloc bindings use `?GetPItem@CBaseIntelligentPointer@@QBEPAVCThing@@XZ`. Both
  PDBs (Ego_r 0x00B4C7E0 and FableWin 0x030F5130) spell `...QBEPAVCBaseObject@@XZ`, and one binding already uses it. 0x008C3E90 and
  0x008E63B0 use the PDB spelling. The house style in the brief should be corrected, as listed in NAME_FIXES.
- The landed stand-ins for 0x00557840, 0x00A03B30, 0x009CE360, 0x00A37160, 0x00A66AE0 and 0x00A673B0 carry invented names. Each is
  respelled in NAME_FIXES.tsv, and the callers in this chunk use the correct symbols.
- Twin pairs: 0x009FECE0 / 0x009FEB80 are different functions, and 0x00A66990 / 0x00A66F80 are two classes. Each copy is named from
  its own vtable or callers, so no address has two names.

## Catalog label corrections
| address | catalog label | true identity |
|---|---|---|
| 0x009FEB80 | CGraphicDataBank::Close | CGraphicDataBank::CloseGraphics (private) |
| 0x00A66990 | _global::CBase_Destructor_Helper | CKeyboard::~CKeyboard |
| 0x00A66F80 | _global::CGuiControlTreePane_Destructor | CMouse::~CMouse |
| 0x00A37160 | CClassFactory::UnregisterAllocatedClass (manifest) | CClass::Unregister (the factory method is inlined into it) |
| 0x006627F0 | CTCAnimationComplex (manifest) | CThingCreatureBase::GetAnimDuration const |
| 0x009E4710 | SortTreeRecursively | STLport _List_base<CMaintainedEvent>::clear |

## Header corrections (hdr/, all opt-in; see hdr/README.md)
- CMovableResource.h
- CIndexBuffer.h
- CKeyboard.h
- CMouse.h
- CClass.h
- CNavigatorBase.h

The default branches are byte-identical to the repo, so no includer recheck was needed.

## Header debt (left as local models with a `// Layout:` comment)
- **engine/CCreatureActionBase.h**: the vptr is flattened. Retail is polymorphic, with about 70 slots (vtable 0x01259D24). Rows
  0x008C3E90 and 0x008E63B0 declare slots 2-12 locally, with DependentThings at +0x9C and PCreature at +0x08. Needed change: an opt-in
  polymorphic class over CBaseObject, with the retail slot list.
- **engine/CCreatureAction_CastStrengthMultiStrikeProper.h** has no base class. Retail is : CCombatActionBase (RTTI).
- **engine/CModeStandBase.h and CTCCreatureModeManager::CModeBase**: flattened. 0x008D6690 declares CModeBase slots 0-6 locally
  (dtor, GetType, AnimUpdate, FrameUpdate, GetAnimFilter, OnActivate, OnDeactivate) and CModeStandBase at 0x68 bytes.
- **CTCPhysicsBase**: slots 28-40 are named locally in 0x008D6690 (EnableNavigator is slot 40). That row uses engine/CTCBase.h with
  FABLE_RETAIL_CTCBASE.
- **engine/CThingCreatureBase.h**: flattened. Used here only through the layout-rename include, at its retail size 0x160.
- **engine/CBankFile.h**: on HOLD, so it is not edited. The bank hierarchy (CBankFile : CBaseClassNonCopyable slots 1-5,
  CBankFileAsync 0x164, CResourceBank 0x7C) is declared locally in 0x009FECE0 and 0x009FEB80. engine/CGraphicDataBank.h exists only
  under _quarantine.
- **engine/CClassFactory.h and engine/CNavigatorManager.h**: the vptr is flattened. 0x00A47180 declares CClassFactory :
  CBaseClassNonCopyable and CNavigatorManager : CClassFactory, plus CNavigatorExternalInterface (23 named pure slots, dtor at slot 23,
  Clone at 24) and CSteeringAgent (non-virtual out-of-line dtor).
- **NUISystem::CComponent family** (no headers for CComponent, CUIInterface or CActionParam):
  - Retail MI is CUIInterface @0, CObserver @4 (0x14) and CActionParam @0x18. CComponent's own members start at +0x20 and
    Children sits at +0xB0. Ego_r has Children at 0xB8 because of Dinkumware containers.
  - engine/CObserver.h is flattened. It is used through the layout-rename include, with a size assert.
- **CInputEvent / CMaintainedEvent**: no headers. They are declared locally in 0x00A66990 and 0x00A66F80: CInputEvent is 0x34 bytes
  with an out-of-line dtor, and CMaintainedEvent : CBaseClass.

## Coordinator decisions requested
1. Accept the `QBEPAVCBaseObject@@XZ` GetPItem spelling and correct the brief's house style.
2. Copy hdr/ in. Every edit is an opt-in branch, and agents 706 and 707 may touch CClass.h too.
3. Respell the landed stand-ins listed in NAME_FIXES.tsv. The ones that matter most are the free function `ResetButtonCounts` (0x00557840),
   `CClass_Retail::UnregisterFromFactory` (0x00A37160), the extern "C" `Forward` for CBankFile::Close (0x009CE360) and the __fastcall
   ??_G stand-ins (0x00A66AE0 and 0x00A673B0).

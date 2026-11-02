# Hand chunk 710 (CVtable_012b25a8 .. CModeDragonHover): 10 of 10 byte-exact

All ten rows grade MATCH (2) / RELOCATION_MATCH (8) under q.py (gall.sh, honouring `// Leaf:`) and in the read-only vland run (WINS 10,
behaviour PASS, no warnings; `tmp/vland.log`); tests PASS under g++ (tall.sh) and the lane VC7.1 (tall.py). Every Leaf confirmed with
`objdump -t`, every relocation of every leaf checked against the PDB spelling bound to retail's target (chk.sh). Every extent checked: each
row starts after int3 padding, ends at its last instruction and is followed by int3 padding up to the next 16-byte function start.
No asm, volatile, pragmas (the inherited pack(1) on CBaseIntelligentPointer was stripped: the bytes hold), flags, emission helpers or
forwarding overloads. All ten catalog labels were wrong.
Run with `FABLE_HDR_OVERLAY=$S/w22/agent710/hdr` (five rows include an overlay header; see hdr/README.md).

## What matched (10)
- **CVtable_012b25a8::Unlock 0x00C0E5C0 / Lock 0x00C0E5E0** (libpk, MATCH): `m_50.Unlock(); m_60.Unlock();` and `m_60.Lock(); m_50.Lock();`.
  VC7.1 does not devirtualise a virtual call on an embedded member object, so the plain member calls give retail's `lea ecx,[esi+N]` +
  `call [eax+N]` and the second call becomes `jmp [edx+N]`.
- **CTCCreatureModeManager::ResetModes 0x0073B0D0**: `RemoveAllModes(); SetupDefaultMode(); SetupInitialMode();` with RemoveAllModes a plain
  member defined above it in the file; VC7.1 inlines it and keeps the COMDAT, which itself grades RELOCATION_MATCH against retail
  0x0073AE70 (proving that row's name). Real STLport 4.6 list: `empty()` = `cmp [eax],eax`, `front()->OnDeactivate()` through the
  CCountedPointer at node+8, `clear()` -> out-of-line `_List_base::clear` 0x0073E6A0.
- **Seven implicit destructors**, each named from the ??_G that calls it (slot 0 of its class's RTTI vtable) + the FableWin.pdb ??1:
  ~CCombatSequenceGeneric 0x006F1770, ~CTCHeroSpecialMovement 0x007E0B20 (CDefPointer<const T> member released by engine/CDefPointer.h's
  inline Release(): `dec [ecx+4]; jne`, clear inside the non-null arm), ~CModeJackOfBladesFloat 0x00740420 / ~CModeScreamerAggressive
  0x00741390 / ~CModeScreamerSuperAttack 0x008AC4D0 (two CIntelligentPointer members, then the CAnimFilterOfGroup Filter: implicit
  ~CAnimFilterOfGroup + inline `virtual ~CAnimFilterBase() {}` expand), ~CTCNoise 0x00749A70 (embedded CTCNoiseDef: the shared retail def
  chain expands to the CDefPointeeBase vtable store + a `call ~CResource`), ~CModeDragonHover 0x008E31A0 (the base dtor that the
  CModeDragonFireBeam ??_G calls out of line). Retail's own constructor defined in each TU emits the vtable, the ??_G and the out-of-line
  ??1 (chunk 705 lesson) -- no emission helper. Each TU's emitted ??_G also grades RELOCATION_MATCH against its retail caller.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Names and their evidence
| address | symbol | evidence (two or more sources) |
|---|---|---|
| 0x00C0E5C0 | ?Unlock@CVtable_012b25a8@@QAEXXZ | **descriptive** (libpk: no PDB, no RTTI, no trace string for this class). Only caller CSystem::LoadBank 0x00C08D40 (.\CSystem.cpp), right after the bank load 0x00C17640 and next to the lock-pair 0x00C100F0 (slot 5); class = vtable 0x012B25A8 stored by ctor 0x00C0EDF0 (members +0x50 / +0x60 = 0x10-byte containers with vtables 0x012B2350 / 0x012B23A0, each pointing at an embedded .\PKCTRMorphable.cpp object), libpk vtable-address convention (catalog 0x00C0FFA0 "CVtable_012b25a8::`scalar deleting destructor'"); Lock/Unlock = chunk 692's descriptive names in the parked 0x00C08D40 source |
| 0x00C0E5E0 | ?Lock@CVtable_012b25a8@@QAEXXZ | **descriptive**; called right before the bank load next to lock-pair 0x00C10100 (slot 6); enters +0x60 then +0x50 (Unlock leaves them in reverse) |
| 0x0073B0D0 | ?ResetModes@CTCCreatureModeManager@@QAEXXZ | Ego_r donor order AddMode(K) / RemoveAllModes / ~ / ctor / SetupDefaultMode / SetupInitialMode / ResetModes / OnCreate / InitialActivate = retail 0x0073AE10 .. 0x0073B120 exactly; FableWin 0x024C2240; body = RemoveAllModes inlined; landed caller 0x0089DDF0 (EntityResetCreatureMode) already binds it |
| 0x006F1770 | ??1CCombatSequenceGeneric@@UAE@XZ | ??_G 0x006F1750 = vtable 0x01261D74 slot 0 (RTTI : CCombatSequenceBase); FableWin 0x0241BB30; Ego_r member Def |
| 0x007E0B20 | ??1CTCHeroSpecialMovement@@UAE@XZ | ??_G 0x007E0B00 = vtable 0x0126FE3C slot 0 (RTTI : CTCBase); FableWin 0x02772270; Ego_r PHeroSpecialMovementDef +0x18 |
| 0x00740420 | ??1CModeJackOfBladesFloat@NCreatureMode@@UAE@XZ | ??_G 0x00740400 = vtable 0x01267D10 slot 0 (RTTI : CModeStandBase); FableWin 0x024BAEF0; Ego_r Filter / PEmitter / PEmitter2 |
| 0x00741390 | ??1CModeScreamerAggressive@NCreatureMode@@UAE@XZ | ??_G 0x00741370 = vtable 0x012681EC slot 0; FableWin 0x024BD9C0; Ego_r Filter / PEffect / PTarget |
| 0x008AC4D0 | ??1CModeScreamerSuperAttack@NCreatureMode@@UAE@XZ | ??_G 0x008AC4B0 = vtable 0x01278338 slot 0; FableWin 0x02AAC090; Ego_r Filter / PTarget (const CThing) / PLightningBeamParticleEmitter |
| 0x00749A70 | ??1CTCNoise@@UAE@XZ | ??_G 0x00749A50 = vtable 0x01268A74 slot 0 (RTTI : CTCBase); FableWin 0x02598B40; Ego_r Definition (CTCNoiseDef) +0x0C -- the ctor 0x00749810 stores ??_7CTCNoiseDef 0x01231E9C there and ??_7CIntelligentPointer<const CThing> 0x01237F1C at +0x48 |
| 0x008E31A0 | ??1CModeDragonHover@NCreatureMode@@UAE@XZ | only caller CModeDragonFireBeam ??_G 0x008E3860 (vtable 0x01285900; RTTI FireBeam : CModeDragonHover), after it destroys FireBeam's own PBeamWeaponEffect; FableWin 0x024BF860; Ego_r Filter / PTarget / UseHeadTurning (0x80); CModeDragonHover's own ??_G 0x00741C10 inlines its dtor, so this is retail's only out-of-line copy |

Byte twins: 0x006F1770 / 0x007E0B20 are two symbols (two vtables, two classes, different member offsets and tail targets);
0x00740420 / 0x00741390 / 0x008AC4D0 are three symbols (three vtables / ??_G callers / classes). No pair is one symbol.

Callees (all PDB spellings, each checked against reloc_bindings by target address): ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10,
??1CCharString@@QAE@XZ 0x0099EAE0, ??_7CAnimFilterBase@@6B@ 0x0122FFCC, ??1CModeStandBase@@UAE@XZ 0x0073D860, ??_7CDefPointeeBase@@6B@
0x01230BA0, ??1CResource@@UAE@XZ 0x009FC550, ??1CTCBase@@UAE@XZ 0x00686830, ?AddMode@CTCCreatureModeManager@@QAE_NK@Z 0x0073AE10 (already
bound so). **New spellings that give an address a second spelling** (the landed rows / bindings below are wrong, see NAME_FIXES.tsv):
- 0x008B0A60 ??1CCombatSequenceBase@@UAE@XZ -- the landed own row spells it QAE (non-virtual model).
- 0x0073AF60 ?SetupDefaultMode@CTCCreatureModeManager@@AAEXXZ / 0x0073AFC0 ?SetupInitialMode@..@@AAEXXZ -- the landed own rows declare them
  public (QAE).
- 0x0073E6A0 the STLport `_List_base<CCountedPointer<CTCCreatureModeManager::CModeBase>, allocator<..>>::clear` -- bound as
  ?clear@CRetailList_CountedPointer@@QAEXXZ (landed own row + caller 0x0073AE70).
- 0x008E31A0 -- bound as ?CModeDragonFireBeam_Destruct@@YIXPAX@Z by the landed ??_G 0x008E3860.
- The seven landed ??_G rows (0x006F1750, 0x007E0B00, 0x00740400, 0x00741370, 0x008AC4B0, 0x00749A50, 0x008E3860) call this chunk's rows
  through free `__fastcall` *_dtor stand-ins (not in reloc_bindings) and are themselves spelled as free functions.
No new name collides with a landed row's spelling of a different address.

## Label corrections
- 0x00C0E5C0 / 0x00C0E5E0 CMapsAndRegionsDialog::OpenMapDialog -> CVtable_012b25a8::Unlock / Lock (libpk, descriptive).
- 0x0073B0D0 CMapsAndRegionsDialog::SetTooltipAsOpen -> CTCCreatureModeManager::ResetModes.
- 0x006F1770 / 0x007E0B20 C3DAnimationBlendState::~C3DAnimationBlendState -> ~CCombatSequenceGeneric / ~CTCHeroSpecialMovement.
- 0x00740420 / 0x00741390 / 0x008AC4D0 _Dest_val<std::allocator<CMacro>,CMacro> -> ~CModeJackOfBladesFloat / ~CModeScreamerAggressive /
  ~CModeScreamerSuperAttack (NCreatureMode).
- 0x00749A70 / 0x008E31A0 CThingNoiseDef::~CThingNoiseDef -> ~CTCNoise / NCreatureMode::~CModeDragonHover.
- Neighbours: 0x0073AE70 ClearModes -> RemoveAllModes; 0x0073E6A0 CRetailList_CountedPointer::clear -> STLport _List_base::clear;
  0x0073AEA0 (manifest "~CVertexShaderData", not in the catalog) -> ~CTCCreatureModeManager (uncatalogued, 93 bytes).

## Header debt
- engine/CModeStandBase.h and engine/CCombatSequenceGeneric.h: corrected here (opt-in branches, hdr/README.md).
- No shared header exists for CTCCreatureModeManager (with nested CModeBase), CCombatSequenceBase, CTCHeroSpecialMovement, CTCNoise,
  CTCNoiseDef, CModeDragonHover / CModeDragonFireBeam / CModeJackOfBladesFloat / CModeScreamerAggressive / CModeScreamerSuperAttack,
  CAnimFilterBase / CAnimFilterOfGroup, CBaseIntelligentPointer / CIntelligentPointer<T>: local models in each source, members at retail
  offsets (STL members not touched by a row are opaque storage `unsigned char Name_[size]; // Type Name +0xNN`). A shared
  engine/CTCCreatureModeManager.h (retail sizeof 0x24: CTCBase, PModes list +0x0C, TimeLeftForCurrentMode +0x10, TimeForNextIdleMode +0x14,
  AvailableIdleModes vector +0x18; CModeBase 0x1C: vptr, ParentThing, WeaponBasedFilter, FramesBeforeRemovingSelf, Active; 12 virtual slots
  as declared in the sources) would let CModeStandBase.h include it instead of relying on the includer.
- CModeBase slots 8 / 9 / 11 (three `xor al,al ; ret` bodies) are named IsIdleMode / IsRemoveOnDeactivate / IsAllowingPlayerSpecialMovementMoves
  from FableWin donor order; the exact assignment among those three is not proven (no row calls them).
- engine/CHeroSpecialMovementDef.h is the flattened def layout (not needed here: only the pointer is touched).
- The constructors defined to emit each vtable are retail's declared constructors with minimal bodies (base / member init only); their
  full bodies are not reconstructed (they are not graded rows).

## Decisions for the coordinator
- libpk class name for vtable 0x012B25A8: this chunk uses `CVtable_012b25a8` (the catalog's ??_G label convention). The landed dtor / ctor
  0x00C0ECE0 / 0x00C0EDF0 spell it `CCombatAbilityStrafeDef_00c0ece0` / `_00c0edf0` (two invented game-class names), the landed ??_G
  0x00C0FFA0 is a CLandscapeBackgroundPatch vector-deleting stand-in, and the parked chunk 692 LoadBank source calls it
  `CUnprovenPKObject_00c099f0`. One name should be chosen for all of them before 0x00C08D40 lands.
- Lock / Unlock are descriptive (bracket order around the bank load); the embedded PKCTRMorphable objects' slots 2 / 3 do not prove locking.

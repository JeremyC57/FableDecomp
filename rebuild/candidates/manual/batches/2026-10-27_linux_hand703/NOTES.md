# Hand chunk 703 (CTCSkeletalMorph .. CTCInventoryQuests): 10 of 10 byte-exact

All ten rows RELOCATION_MATCH under q.py (gall.sh, honouring `// Leaf:`), tests PASS under g++ (tall.sh) and VC7.1 (tall.py),
every relocation checked against retail with $S/w22/calls.py (import: scalar operator new 0x00BFEA1A only), read-only vland run
(see the end). No flags, pragmas, retail_asm or volatile. 12 probes in all (1 per row, 2 for 0x005596D0 and 0x005DF750); logs in probe/<addr>.log.
All ten catalog labels were placeholders or wrong except 0x005DF750 (name right) -- every row named from its own vtable slot /
vtable store + the PDB name sets. Extents: all ten checked against the padding / the next function and correct.

## What matched (10)
- CTCSkeletalMorph::Copy(const CTCBase&) 0x00786650 -- CTCBase slot 12 Copy (chunk 702 model): `Changed = true; Morphs = o.Morphs;`
  over `CArray<CSkeletalMorph> : std::vector` (STLport `_Vector_base` + `_STLP_alloc_proxy`), vector::operator= out of line; the
  implicit CArray operator= inlines to the tail `jmp`.
- CTCActivationReceptorDoor::Activate() 0x007F8120 -- `SetDoorState(true); CTCActivationReceptorBase::Activate();` with the
  landed sibling Deactivate 0x007F8140's model (same retail TU).
- CThingDeadCreature::WorldUpdateBulletTime() 0x00836240 -- `if (HasTC(0x83)) SetGraphicAppearanceChanged(2);
  CThingGameObject::WorldUpdateBulletTime();` (the landed WorldUpdate 0x00836260 next door has the same bitset test + call).
- CAIStateGroup_MinionReactToCrime::CAIStateGroup_MinionReactToCrime() 0x00900B41 / CAIStateGroup_VersusScorpionKing::
  CAIStateGroup_VersusScorpionKing() 0x0090143E -- user empty ctors over the PDB members; the tracked pointers at the odd offsets
  +0x1B / +0x1D need a 1-aligned CBaseIntelligentPointer (`#pragma pack(push, 1)` around CBaseIntelligentPointer /
  CIntelligentPointer<T> only; the state-group classes keep 4-byte alignment, matching the PDB sizes 36 / 40 and the factories'
  `new 24h` / `new 28h`). Both unaligned starts are real: the TU is size-packed (previous ??_G ends with `ret 4` right before).
- NGraphicAppearance::CTCGraphicAppearance::OnKill() 0x004BF8F0 -- STLport 4.6 list iterator loop over Components calling the
  component's slot 1 OnAppearanceClear(), then `Primitives.clear()` (inline `_Base::clear()` -> out-of-line _List_base clear).
- NUISystem::CScrollBar::OnLeftClicked() 0x005596D0 -- `if (!WasDragged) { PositionBeforeDrag = Position; DeltaBeforeAction.X = 0;
  .Y = 0; } CDraggable::OnLeftClicked();`. Probe 1 lost on two counts: derived members placed after a pad in the derived class
  start at the base's rounded sizeof (0x1A0, not 0x19D), and `= C2DVector(0.0f, 0.0f)` builds an 8-byte stack temporary.
- CAnimComponentCombatMisc::Alloc() const 0x005DF750 -- `return new CAnimComponentCombatMisc;` over the retail-form CIVCountedPointeeBase
  (user ctor `IVRefCount(1)`, `Delete() const`, mutable count; outside the pack) <- pack(1) CAnimComponentBase <- CAnimComponentCombatMisc
  (sizeof 10 = the `push 0Ah`). Probe 2 only moved the IV base to its retail form (same bytes).
- CTCInventoryMap::OnKill() 0x005FAEE0 / CTCInventoryQuests::OnKill() 0x0061A1F0 -- the landed inventory-screen OnKill model
  (0x005CCEAB) without its `#pragma optimize("s", on)`: these two TUs are speed-optimised (branchy null-checked this -> CBase).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Names and their evidence (all proven from two or more sources)
- 0x00786650 ?Copy@CTCSkeletalMorph@@UAEXABVCTCBase@@@Z -- only reference vtable 0x0126B6F4 slot 12 (CTCBase Copy, landed
  0x00778B10 / 0x00722570); Ego_r name set; members (Changed +0xC, Morphs +0x10) from Ego_r and the landed vector operator=.
- 0x007F8120 ?Activate@CTCActivationReceptorDoor@@UAEXXZ -- vtable 0x0124146C slot 28 (slot 29 the landed Deactivate); Ego_r order
  SetDoorState / Activate / Deactivate = retail 0x007F80C0 / 0x007F8120 / 0x007F8140; tail target bound
  ?Activate@CTCActivationReceptorBase@@UAEXXZ.
- 0x00836240 ?WorldUpdateBulletTime@CThingDeadCreature@@UAEXXZ -- vtable 0x01272B04 slot 20; CThing slot 20 = 0x004C8BA0
  WorldUpdateBulletTime (the tail target; its own body tests interface 0x46); Ego_r name, 0x20 before WorldUpdate in both builds.
- 0x00900B41 ??0CAIStateGroup_MinionReactToCrime@@QAE@XZ / 0x0090143E ??0CAIStateGroup_VersusScorpionKing@@QAE@XZ -- the vtables
  stored (RTTI), the member vtables (.?AV?$CIntelligentPointer@VCThingBuilding@@@@ / @VCThing@@@@) at the PDB offsets 27 / 29,
  the factory allocation sizes (= PDB sizes), the base ctors (CAIStateGroupBase 0x008FD255 / CAIStateGroup_VersusEnemyBase
  0x009013FA), Ego_r name sets.
- 0x004BF8F0 ?OnKill@CTCGraphicAppearance@NGraphicAppearance@@UAEXXZ -- vtable 0x0123986C slot 14; Ego_r name set and block order;
  the same loop + clear inline in the neighbours ClearAllPrimitives(DueToBeingOutOfRange) 0x004BF9A0 / 0x004BF960.
- 0x005596D0 ?OnLeftClicked@CScrollBar@NUISystem@@UAEXXZ -- vtable 0x0124B7E4 slot 146 (CDraggable slot 146 = the landed
  OnLeftClicked, the tail target); Ego_r name set; the members are CScrollBar / CDraggable's (retail = Ego_r - 0x30).
- 0x005DF750 ?Alloc@CAnimComponentCombatMisc@@UBEPAVCAnimComponentBase@@XZ -- vtable 0x01255738 slot 4 (siblings' Alloc in the same
  slot), Ego_r name set (const virtual, base-pointer return), allocation size = PDB sizeof 0xA.
- 0x005FAEE0 ?OnKill@CTCInventoryMap@@UAEXXZ / 0x0061A1F0 ?OnKill@CTCInventoryQuests@@UAEXXZ -- byte twins named from their own
  vtables (0x01257A9C / 0x012585B4 slot 14); both spellings in Ego_r and FableWin. Two classes, two symbols: no binding conflict.

## Callee names
Used: ??4?$vector@VCSkeletalMorph@CTCSkeletalMorph@@...@std@@QAEAAV01@ABV01@@Z 0x007871D0 (landed leaf),
?SetDoorState@CTCActivationReceptorDoor@@AAEX_N@Z 0x007F80C0, ?Activate@CTCActivationReceptorBase@@UAEXXZ 0x007F76E0,
?SetGraphicAppearanceChanged@CThing@@QAEXK@Z 0x004C7590 (the landed row's own DESCRIPTIVE spelling; see below),
?WorldUpdateBulletTime@CThing@@UAEXXZ 0x004C8BA0 (virtual; binding QAE wrong -> NAME_FIXES), ??0CAIStateGroupBase@@QAE@XZ
0x008FD255, ??0CAIStateGroup_VersusEnemyBase@@QAE@XZ 0x009013FA, ??0CBaseIntelligentPointer@@QAE@XZ 0x00A01B10,
?clear@?$_List_base@VCPrimitive@NGraphicAppearance@@V?$allocator@VCPrimitive@NGraphicAppearance@@@std@@@std@@QAEXXZ 0x004C5690
(STLport class-keyed allocator; the landed model's `struct allocator` mangles U -> NAME_FIXES), ?OnLeftClicked@CDraggable@NUISystem@@UAEXXZ
0x0055A5D0, ??2@YAPAXI@Z 0x00BFEA1A, ?GetPHeroInfoScreensManager@CPlayerGui@@QBEPAVCManager@NHeroInformationScreens@@XZ 0x00437CD0,
?DeregisterScreen@CManager@NHeroInformationScreens@@QAEXABVCBase@2@@Z 0x0066EB78, ?OnKill@CTCInventoryBase@NInventory@@UAEXXZ
0x005BC9C8, ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A 0x013B8790.
No new symbol collides with a landed row's spelling for a different address. No address got a second spelling from this chunk
except where NAME_FIXES replaces a wrong one.

Descriptive (unproven) callee: 0x004C7590 (`if (PTCGraphicAppearance +0x64) PTCGraphicAppearance->SetAsChanged(flags)`, CThing TU)
has no entry in either PDB name set; the landed row names it SetGraphicAppearanceChanged(unsigned long), while other landed callers
bind UpdateGraphicAppearance(long) (the neighbour 0x00836260), ForwardToGraphicAppearance(long), RefreshGraphic(bool),
SetActive(bool), Method_004c7590. I used the row's own landed spelling; the coordinator should pick one and respell the rest.

## Label corrections
All in NAME_FIXES.tsv: the nine row placeholders / propagated labels; neighbours 0x004C8BA0 (bound non-virtual), 0x00900B81's
callee spelling (N_00900b41 namespace), 0x008FD255 / 0x00A01B10 (CBase_/CMemberBase_ stand-ins in the generated ctor family,
~70 landed rows), 0x004BF9A0 (bound public, PDB protected), 0x004C5690 (U-keyed allocator / invented bindings), 0x005BC9C8
(catalog CTCInventoryBase_Retail), 0x00437CD0 / 0x013B8790 (invented spellings in reloc_bindings).
Also noticed (not fixed): 0x005DF770, the function right after 0x005DF750 (one int3 between), is an out-of-line ctor storing
vtable 0x01255760 with IVRefCount = 1 -- likely ??0CAnimComponentCombatFlourishTargetOffset@@QAE@XZ (unverified; not a chunk row).

## Header debt
- engine/CTCBase.h: flattened vptr (local polymorphic CTCBase models in 0x00786650 / 0x004BF8F0, as chunk 702).
- engine/CTCActivationReceptorBase.h: flattened vptr (the landed sibling's model reused).
- engine/CThing.h: Ego_r offsets past the containers; local CThing (UsingComponents +0x20) as the landed 0x00836260.
- engine/CAIStateGroupBehaviourBase.h is not these classes' header (the catalog label was wrong); CAIStateGroupBase /
  CBaseIntelligentPointer modelled locally (no engine header for the state groups; engine/CBaseClass.h included).
- engine/CAnimComponentCombatMisc.h: flattens CIVCountedPointeeBase into `void* __vftable` + `_base_0x04`, so it cannot be newed
  with its vtable; local pack(1) model. engine/CIVCountedPointeeBase.h's retail opt-in (FABLE_RETAIL_DEF_CLASS_BASE) lacks the
  default ctor `IVRefCount(1)` (FableWin ??0CIVCountedPointeeBase@@QAE@XZ), so vland's SHARED_HEADER_AVAILABLE warning stands.
- engine/CPlayerGui.h / engine/CManager.h (vland SHARED_HEADER_AVAILABLE on 0x005FAEE0 / 0x0061A1F0): CManager.h is NUISystem's
  manager, not NHeroInformationScreens::CManager; CPlayerGui is modelled with the one accessor, as the landed twins 0x005CCEAB etc.
- engine/CTCInventory.h is not used (the rows are CTCInventoryMap / CTCInventoryQuests over NInventory::CTCInventoryBase 0x130).
- No engine header for NUISystem::CScrollBar / CDraggable / CComponent at retail offsets (local models, as the landed rows).

## vland
Read-only vland run (tmp/vland.log): WINS 10, all RELOCATION_MATCH + behaviour PASS. Warnings only: SHARED_HEADER_AVAILABLE
CManager / CPlayerGui on 0x005FAEE0 / 0x0061A1F0 (headers not usable, see Header debt) and RELOC_NAME on 0x00836240's call to
0x004C7590 (catalog label CRenderTarget::DetachTarget; descriptive callee, coordinator decision above). lv/ deleted.

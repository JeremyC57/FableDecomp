# Hand chunk 706 (CLandscapeBackgroundPatch .. CCreatureAction_GoUnderground): 10 of 10 byte-exact

All ten rows RELOCATION_MATCH (q.py + the read-only vland run, overlay set) with behaviour tests PASS under g++ and VC7.1.
Grade with `export FABLE_HDR_OVERLAY=$S/w22/agent706/hdr` (rows 0x004DE253, 0x004DF32C, 0x00BE5C60 and 0x00802610 include corrected
headers from hdr/; see hdr/README.md). Probe counts are below the 12-probe minimum only because every row matched; none was parked.

## What matched (10)
- **CVtable_012bf0c8 (libpk DSP settings block)** -- 0x00C77470 `ReleaseBlock()` (descriptive): if the settings block exists, free it
  through `CPKObject::operator delete` 0x00C3C370 and clear the three block pointers. Shape: the landed class block of
  ~CVtable_012bf0c8 0x00C775A0 (plain out-of-line member + user dtor `{ ReleaseBlock(); }`); the leaf is ReleaseBlock. 1 probe.
- **CLandscapeBackgroundPatch** -- 0x00BE5C60 `GetEdgeStrip(LANDSCAPE_EDGE_STRIP_SIDE)`: `switch (side)` with cases 0..3 returning
  `&EdgeStrips[n]`, `return 0` after; VC7.1 emits the range check + the 4-entry jump table in case order (checked by hand: table
  0x00BE5C90 -> arms 0x00BE5C70/76/7C/82; vland trusted for the table). Now on the hdr/ opt-in chain with every Ego_r member. 2 probes.
- **CExplodingObjectDef** -- 0x004DE253 `Transfer(CPersistContext&)`: four `context.Transfer(Member);` under `#pragma optimize("s", on)`
  (without it DIFFER: this is the padding-free /Os def TU). 3 probes (the third moves the class into the header opt-in).
- **CTCVolumeContainmentTrackerDef** -- 0x004DF32C `Transfer(CPersistContext&)`: same shape over two bools and two floats. 2 probes.
- **CAnimComponentCombatFlourishTargetOffset** -- 0x005DF790 `Alloc() const`: `return new CAnimComponentCombatFlourishTargetOffset;`
  (0x11 bytes; the inlined ctor stores IVRefCount = 1 and the vtable). Base CAnimComponentBase slots 2..8 pure, all seven overrides
  declared. 2 probes.
- **CTCPhysicsRigidBody** -- 0x00759970 `UpdateInterpolation()`: `OldRHSet = RHSet; CTCPhysicsBase::UpdateInterpolation();` (24-byte
  struct copy, tail jump to the protected base). 2 probes.
- **CTCHeroReceiveItems** -- 0x007CA630 `OnKill()`: STLport 4.6 list walk over Items, item fetched once into a local, `if
  (IS_THING_ALIVE(item)) item->Kill(true);`, then `CTCBase::OnKill()`. 1 probe.
- **CTCTargetingPlayer** -- 0x00802610 `GetPTargetedThing() const`: first-person targeting returns the virtual
  GetPProjectileTargetedThing (slot 35), otherwise the base GetPTargetedThing (inline `PTargetedThing.GetPItem()`). 4 probes (the last
  moves CPlayer onto engine/CPlayer.h).
- **CCreatureAction_SpiritAttack** -- 0x008547D0 `FinishAction()` (private virtual `EAE` in both PDBs): kill the particle emitter if
  `IS_THING_ALIVE(PParticleEmitter)`, then `CCreatureActionBase::FinishAction()`. 2 probes.
- **CCreatureAction_GoUnderground** -- 0x008B6A20 `FinishAction()`: `if (!HasPerformedAction()) PerformAction();`, an AI creature
  (GetType() == 1) gets `SuspendAI(false, 0)`, then `CCombatActionBase::FinishAction()`. The cast result must be a named block local
  (`CThingAICreature* creature = static_cast<...>(PCreature.Get()); creature->SuspendAI(false, 0);`): the inline expression forms
  (probes 1-3) pushed the arguments before the second GetPItem call. 4 probes.

## Names and evidence
Every name below is in both Ego_r.pdb and FableWin.pdb name sets (exact spelling), and each identity is fixed by the row's own vtable
slot / RTTI plus donor order or callers:
- 0x00BE5C60 ?GetEdgeStrip@CLandscapeBackgroundPatch@@QAEPAVCPatchTesselationEdgeStrip@@W4LANDSCAPE_EDGE_STRIP_SIDE@@@Z -- donor order
  ClearTextures 0x00BE5C30 / GetEdgeStrip / InvalidateEdge 0x00BE5CA0; sole caller CLandscapeBackgroundTreeNode::GetEdgeStrip 0x00BF6930.
- 0x004DE253 ?Transfer@CExplodingObjectDef@@UAEXAAVCPersistContext@@@Z -- vtable 0x0123AD7C slot 18; member types/offsets match Ego_r.
- 0x004DF32C ?Transfer@CTCVolumeContainmentTrackerDef@@UAEXAAVCPersistContext@@@Z -- vtable 0x0123E0C4 slot 18.
- 0x005DF790 ?Alloc@CAnimComponentCombatFlourishTargetOffset@@UBEPAVCAnimComponentBase@@XZ -- vtable 0x01255760 slot 4; new size 0x11 = PDB sizeof.
- 0x00759970 ?UpdateInterpolation@CTCPhysicsRigidBody@@UAEXXZ -- vtable 0x0126996C slot 94; donor order 0x00759900/70/B0.
- 0x007CA630 ?OnKill@CTCHeroReceiveItems@@UAEXXZ -- vtable 0x0126EBFC slot 14; tail CTCBase::OnKill 0x006868F0.
- 0x00802610 ?GetPTargetedThing@CTCTargetingPlayer@@UBEPAVCThing@@XZ -- vtable 0x0124280C slot 33 (base 0x01271E74 slot 33).
- 0x008547D0 ?FinishAction@CCreatureAction_SpiritAttack@@EAEXXZ -- vtable 0x01275D74 slot 4; private in both PDBs.
- 0x008B6A20 ?FinishAction@CCreatureAction_GoUnderground@@UAEXXZ -- vtable 0x0127A894 slot 4. Its byte twin 0x008B6A90 is
  CCreatureAction_RiseUp::FinishAction (chunk 707's row, named from its own vtable); no name collision.
- 0x00C77470 ?ReleaseBlock@CVtable_012bf0c8@@QAEXXZ -- **descriptive** (libpk has no PDB/RTTI): the reverb-family class name kept from
  the landed 0x00C775A0 / chunk 699; callers CReverbProcessor 0x00C6A3D0 / 0x00C6A780.
Callee spellings (all PDB-proven, both sets): ??$Transfer@_N/M/J@CPersistContext (wave 19 member templates), ?PeekPlayer@CThing@@QBEABVCPlayer@@XZ,
?IsFirstPersonTargeting@CPlayer@@QBE_NXZ, ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ, ?Kill@CThing@@QAEX_N@Z,
?OnKill@CTCBase@@UAEXXZ, ?FinishAction@CCreatureActionBase@@UAEXXZ, ?HasPerformedAction@CCreatureActionBase@@QBE_NXZ,
?SuspendAI@CThingAICreature@@QAEX_NPAX@Z, ?FinishAction@CCombatActionBase@@MAEXXZ, ?UpdateInterpolation@CTCPhysicsBase@@MAEXXZ.
??3CPKObject@@SAXPAX@Z (0x00C3C370) is the landed descriptive libpk spelling. calls.py: every call target resolves to the expected
retail address.

## Extents
All ten extents agree with the oracle and the padding. 0x004DE253 / 0x004DF32C have odd / unaligned starts but are real function
starts in the padding-free /Os def TU (the previous function ends `ret 4` immediately before each). 0x00BE5C60 is 48 bytes of code
plus its own 16-byte jump table (the next function 0x00BE5CA0 follows the table). 0x008B6A20 has a single int3 after it. No row is a
fragment.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x00C77470 "CLandscapeBackgroundPatch::ClearTextures" -> CVtable_012bf0c8::ReleaseBlock (libpk; the real ClearTextures is 0x00BE5C30).
- 0x00759970 "CTCPhysicsRigidBody::vfunc_94" -> UpdateInterpolation.
- 0x006AF780 (labelled CTCPhysicsBase::vfunc_94; bound as ?InitialiseBase@CCreatureAction_TrollWhackGroundBase_Methods@@QAEXXZ) ->
  ?UpdateInterpolation@CTCPhysicsBase@@MAEXXZ.
- 0x00723730 (landed as CCreatureAction_TrollWhackGroundBase::Initialise) -> ?UpdateInterpolation@CTCPhysicsStandard@@MAEXXZ.
- 0x00C77650 (landed as CLandscapeBackgroundPatch vector deleting dtor) -> slot 0 of 0x012BF0C8, the libpk block's scalar deleting dtor.
- 0x00802610 / 0x007CA630 / 0x008547D0 / 0x008B6A20 catalog stand-in leaves -> the PDB spellings above.
Full list with every wrong spelling seen: NAME_FIXES.tsv.

## Header corrections (hdr/, all opt-in; details in hdr/README.md)
engine/CExplodingObjectDef.h, engine/CTCVolumeContainmentTrackerDef.h (retail def classes), engine/CMovableResource.h,
engine/CLandscapeBackgroundSubNodeBase.h, engine/CLandscapeBackgroundPatch.h (retail polymorphic chain, every Ego_r member),
engine/CPatchTesselationEdgeStrip.h + engine/CEdgeStipVertexArray.h (new), engine/CPlayer.h (guarded IsFirstPersonTargeting).

## Header debt
Local models still used because the shared header is flattened (vptr as a data member) or missing; each needs a retail opt-in:
- engine/CAnimComponentCombatFlourishTargetOffset.h (2 includers) flattens the polymorphic base; there is no engine/CAnimComponentBase.h.
  Needs `class CAnimComponentBase : public CIVCountedPointeeBase` (1-packed, slots 2..8 pure: GetType, Initialise, Alloc, Clone,
  TransferBinaryIn, TransferBinaryOut, SetDefaultValue) and the derived class with bool TargetOffset +8, C2DVector TargetOffsetVector +9.
- engine/CIVCountedPointeeBase.h retail branch lacks the inline default ctor `CIVCountedPointeeBase() : IVRefCount(1) { }` (FableWin
  ??0CIVCountedPointeeBase@@QAE@XZ) that 0x005DF790 needs. Adding it changes every opt-in includer that constructs, so all
  FABLE_RETAIL_DEF_CLASS_BASE includers need a recheck first.
- engine/CTCPhysicsBase.h (59 includers) / engine/CTCPhysicsRigidBody.h (4) flattened. Needs CTCPhysicsBase : CTCBase with protected
  virtual UpdateInterpolation (slot 94), and CTCPhysicsRigidBody with three STLport vectors +0x40/+0x4C/+0x58, PPhysicsMeshInfo +0x64,
  RHSet +0x68, OldRHSet +0x80, OffsetVector (C2DVector) +0x98, Body (CRigidBody) +0xA0; retail sizeof 0x258 (operator new size at
  0x004D399D; ctor 0x0075AA80). The local model in 0x00759970 now has every member, with Body as opaque `Body_[0x1B8]` (no CRigidBody
  model: Ego_r CMatrix3x4 + two CRigidBodyConfig).
- engine/CTCTargetingPlayer.h (2 includers) flattened; no engine/CTCTargetingBase.h. The local models in 0x00802610 now carry every
  member of both classes with real types (CTCTargetingBase +0x0C..+0x34 incl. PTargetingDef CDefPointer<const CTargetingDef>, sizeof
  0x38; CTCTargetingPlayer ZLockTargetSelectionTimer +0x38 .. LockedOntoEnemy +0xF4, sizeof 0xF8, from the retail ctor 0x004E2250;
  NPlayerTargeting::CTargetPoint 0x1C). Moving them into the headers is the remaining debt.
- No engine/CTCHeroReceiveItems.h. The local model in 0x007CA630 now has every member (Items +0x0C .. BackgroundPrimitiveHandle +0xA4,
  sizeof 0xAC from the retail ctor 0x007CA830 / `push 0ACh`); the two CEnginePrimitiveHandle members are opaque 8-byte storage.
- engine/CCreatureActionBase.h (101 includers), engine/CCreatureAction_SpiritAttack.h (2), engine/CCreatureAction_GoUnderground.h (0)
  flattened; no engine/CCombatActionBase.h. Retail: CCreatureActionBase : CBaseObject, 0xA8 bytes, PCreature +8, PerformedAction
  +0x62, slots dtor/EditUpdate(float)/PerformAction (pure)/FinishAction; CCombatActionBase 0x114 bytes with protected FinishAction;
  SpiritAttack PTarget +0xA8 / PParticleEmitter +0xB0 (Ego_r +0xB4/+0xBC).
- No engine/CThing.h; CThing / CThingCreatureBase / CThingAICreature / CThingObject chains are restated over CBaseObject + the CClass
  layout-rename in 0x007CA630 / 0x008547D0 / 0x008B6A20 (the established form from earlier chunks).
- engine/CLandscapeBackgroundSubNodeBase.h default path spells the class `struct` (RTTI says .?AV, class); left untouched.

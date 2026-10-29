# Hand chunk 568 (CAIStateGroup_BanditSitRoundCampfire .. CAIStateGroup_BanditSitRoundCampfire): 1 of 1 byte-exact

## What matched (1)
- **CAIStateGroup_BanditSitRoundCampfire**
  - 0093b1d0 `Update` RELOCATION_MATCH 1011v1011, test PASS under g++ and VC7.1.
  - What it does: it sets FrameToNextCheckIsRunnable to frame + 5*ConstantFPS. It takes a free action point from the campfire's CTCUserActionPointBase; if there is none it terminates. It then makes five tries to push the seat outward along normalised (seat - fire) by GFFloatRandom(1), rotated by GFFloatRandom(1). It keeps the first try the creature's CTCCreatureNavigation says is navigable, and sets GotSeatPos.
  - It then runs MoveToPos, then LookAtPos(seat forward + creature pos). If the creature carries a weapon it waits on CCreatureAction_SheatheWeapons.
  - Last it picks a random "ST_SIT_ON_FLOOR" anim through a CAnimFilterAnyGroup and creates `new CModeSitFloor(creature, anim.GroupName)` in a CCountedPointer<NCreatureMode::CModeSitBase>. It sets Sitting, performs CCreatureAction_SitDown, then calls SitMain and Terminate. Every failure is `Terminate(); return;`.
  - Shape levers, in order of effect:
    1. Same-TU `__declspec(noinline)` stand-in bodies for BOTH the CCountedPointer<CModeSitBase> ctor 0x008AB380 and dtor 0x008AB3D0 (I-3). Either one alone leaves `sitMode` in its own slot (frame 0x170 v 0x168). With both, it joins retail's shared 12-byte slot at esp+0x10, and every offset matched at once.
    2. The PDB vector operators: `C2DVector(pos - PCampfire->GetPhysPos()).Normalise()` with C3DVector operator- returning const C3DVector; `pos + offset` / `pos += offset` via the C3DVector(const C2DVector&) overloads; `const C2DVector operator*(float) const`.
    3. A named `C3DVector forward = orientation.Forward; C3DVector lookAt = forward + PeekPos();`.
    4. Early-return `!= 2` chains with `SitMain(); Terminate();` last.
  - The landed copy-ctor form `: Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }` matches once the frame is right. Before that, it showed a spurious Info reload.
  - The same-TU LowerBound stand-ins are inert here and were dropped.

## Names and evidence
- Row: `?Update@CAIStateGroup_BanditSitRoundCampfire@@UAEXXZ`.
  - Ego_r.pdb names the method, with donor 0095e770; OnReplace at 0095e530 precedes it in donor order, as retail OnReplace 0x0093AFC0 precedes 0x0093B1D0.
  - The manifest label agrees.
  - The leaf was checked with objdump -t.
- Callees use the Ego_r.pdb spellings (egor_pdb_names.tsv):
  - CWorld::GetFrame SIJXZ
  - CTCUserActionPointBase::GetFreeActionPoint `QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ@Z`
  - CThing::GetPhysPos QBE
  - C2DVector::Normalise / Rotate
  - CAIStateGroupBase::GetPWorldSeed IBEPAKXZ
  - GFFloatRandom YIMMPAK
  - CTCCreatureNavigation::IsPosNavigable QBE
  - CAIStateGroupBase::AIAction_MoveToPos / LookAtPos / PerformAction / PerformActionAndWait, all IAE with EAIActionReturn/EAIActionRepeat/EMovementType
  - CThingPhysical::PeekPos
  - CTCCarrying::IsThingCarryingWeapon SI
  - CThingCreatureBase::GetRandomAnimFromNameWithFilter QBE returning const&
  - CModeSitBase ctor `??0CModeSitBase@NCreatureMode@@QAE@AAVCThing@@@Z`
  - CCreatureAction_SitDown ctor
  - CCRC::Calc
  - CDefStringTable::GetString QBE
  - ?GDefStringTable@@3VCDefStringTable@@A
  - CAIStateGroupBase::Terminate
  - SitMain AAE_NXZ (landed 0x0093AD70)
- CCountedPointer<NCreatureMode::CModeSitBase> ctor 0x008AB380, dtor 0x008AB3D0 and DeleteData 0x008AA810 follow the Ego_r template spellings of other instances (see NAME_FIXES.tsv). These instances are not in egor_pdb_names.tsv.
- Vtables, by RTTI:
  - 0x0127261C CAnimFilterAnyGroup
  - 0x0122FFCC CAnimFilterBase
  - 0x01267BBC NCreatureMode::CModeSitFloor
  - 0x01267568 CAnimFilterOfGroup
  - SheatheWeapons ctor 0x008CC370 stores 0x01282CEC (CCreatureAction_SheatheWeapons)
- Descriptive or unproven, kept as bound:
  - 0x01375550 is reached through `CMainGameComponent::GetConstantFPS()` (inline `mov eax,[0x1375550]`; CGameScriptInterface::GetConstantFPS 0x0088E080 is the same load). The backing static is spelled `?ConstantFPS@CMainGameComponent@@1JA`, the one bound spelling that fits the accessor. Its real member name is unproven.
  - 0x013BB704 is `(anonymous namespace)::SIT_ON_FLOOR_ANIM_NAME` ("ST_SIT_ON_FLOOR"). The Ego_r dynamic-initializer order SIT_ON_FLOOR / GET_UP_FROM_FLOOR / MIN / RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS / TIME_TO_LEAVE_SHOUT_MESSAGE matches retail 0x013BB704..0x013BB714. A file-local cannot be named cross-TU, so the source keeps the bound extern `?g_CCharString_013bb704@@3VCCharString@@A`.
- Parameter names of AIAction_MoveToPos (a, b, c, d) and LookAtPos are descriptive. The PDB names set carries no parameter names.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x0062E900 is `??1CCombatActionBase@@UAE@XZ`, not ~CTCLightningOrb. It is the inlined dtor of the SheatheWeapons temporary.
- The landed model 0090f830 (MinionListenToStory SitDown) names vtables 0x0127261C / 0x01267BBC "CAnimFilterSit" / "CModeSitListen". RTTI says CAnimFilterAnyGroup / NCreatureMode::CModeSitFloor.
- Bound spellings for 0x004C7950, 0x00662EF0, 0x008AB9D0, 0x008ABAA0 and 0x008AA810 are wrong; corrections are in NAME_FIXES.tsv.

## Header debt
- No generated engine header models the polymorphic CAIStateGroup_BanditSitRoundCampfire. CAIStateGroupBase.h and CAIStateGroup_SitRoundCampfire.h are flat (`__vftable` as a data member), so the class and its base are local models with an Ego_r layout comment.
- These are also local models: C2DVector/C3DVector/CRightHandedSet, CCharString, CCountedPointer, CThing (bitset + CVectorMap Components), CAnimFilter*, CModeSit*, and the action classes (CCreatureActionBase 0xA8, CCombatActionBase 0x114).

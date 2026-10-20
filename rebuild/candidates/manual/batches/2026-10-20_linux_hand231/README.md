# Hand chunk 231 (CTCFadeOutAndIn .. CTCInGameMenu): 25 of 40 byte-exact

All 25 grade RELOCATION_MATCH under the RTM 13.10.3077 lane (`gall.sh` -> grades.txt) and every test prints PASS under both g++ (`tall.sh`) and VC7.1 run through Wine (`tall.py` -> tests_vc.txt).

## What matched (25)
- **CTCFadeOutAndIn** 006c3f40 FrameUpdate (catalog vfunc_9): the alpha step is `Alpha += direction * (amount / frames)`, and the x87 order follows that grouping.
- **CTCFireRangedWeaponOnLeavingStateGroup** 007e5b90 FrameUpdate (vfunc_9):
  - Two sequential `if (!QuitOut)` blocks keep the esi push at entry.
  - The creature lookup goes through a const thing, which calls the const LowerBound 0x004365B0.
  - `CWorld* world = GetWorld();` is taken before the last call.
- **CTCFishingSpot** 007b01c0 RemoveParticles: one `static __forceinline` per-pointer helper, expanded three times. The clear is `p = CIntelligentPointer<CThing>(0)` (a temporary assigned to the member).
- **CVtable_012B5870** 00c3d160 Cleanup (catalog CTCFishingSpot::CleanupAndDestructor): each array is freed with `if (p) { delete[] p; p = 0; }`. The flag is read as `(unsigned char)(flags >> 2) & 1`.
- **CTCGhostSword** 007c5560 ctor (leaf `??0CTCGhostSword`): the DrainLife constructor family over STLport `_Vector_base` + `_STLP_alloc_proxy` vectors (12 bytes each).
- **CTCHealLife** 007c4bd0 ctor (leaf `??0CTCHealLife`): an STLport list sentinel plus alloc-proxy vectors.
- **CTCGraphicAppearance**
  - 004bda30 GetAllHelperPointNames (the catalog label is wrong) and 004bdb40 GetAllDummyObjectNames: GetAllMeshes is inlined over a block of C3DMeshObject, and the two bodies differ only in the C3DMeshStats section flag.
  - 004bdd70 GetDummyObjectNamesStartingWith: the same family, with the prefix held in ebp.
  - 004be2c0 HasDummyObject: the mesh-info local is named and declared before the counted temporary.
  - 004bfb40 SetLightingChannel: in the GetInterface helper the end/hit choice is a single ternary inside the out store (`*pp = (T*)((miss) ? map->last : it)->component`), which puts the component straight into ecx.
  - 004bfe20 ClearMeshEffect: a same-TU stand-in for the map `find` keeps the owner argument in ebp.
- **CTCFish** 007dfb20 ctor (leaf `??0CTCFish`): an inline LCG `GFRandom(range, seed)` (`seed*0x24a1+0x24df`, ROR 13, `range ? seed % range : 0`). It is inlined at the first call and called out of line at the second, with its arguments evaluated right to left.
- **CTCInGameMenu**
  - 0047df8a ctor (leaf `??0CTCInGameMenu`): under `optimize("s")`, with STLport alloc-proxy vectors.
  - 0047da50 ConfirmedVideoOptionsNotAccepted: follows the sibling 0x0047D012 model, under `optimize("y", off)` + `("s", on)`.
- **CTCHeroStats**
  - 0057c9c4 Copy (vfunc_12, leaf `?Copy@CTCHeroStats`): under `optimize("s")`, using retail's offsets, which sit 8 and 4 below the PDB's.
  - 0057d9fd UpdateOutstandingCrimes: y-off/s-on, with `operator->` called on the returned CDefPointer temporary so the def loads before the next call.
  - 0057daf5 GetOutstandingFineAndFramesLeft: the same model, plus a Dinkumware `max` taken on a temporary.
- **CTCHitLocations** 008168b0 GetPCombatFirstHitLocationIntersectionWithFiniteLine: the twin of the landed WithTriangle 0x008169B0.
- **CTCHeroCentre** 0077d4b0 OpenTrainingScreen (`/GS`): the CGameEvent model of the landed CTCShop 0x00773B20 with an inline by-value AddULONG, plus the ternary-store GetInterface.
- **CTCHeroOnlineScoreboard**
  - 0055fb80 UpdateTransaction: a jump table over states 100..104. Case 100 is explicit and case 102 is `if (status == 2) ... else`, with fall-through between states. **The landing session must use `verify_land_jumptable.py`.**
  - 005625f0 OnPlayerSelectedClan: a same-TU LowerBound stand-in homes the lookup key in the dead argument slot.
- **CTCHeroCentreDoorMarker** 0077f650 OnSerialise: an `if (IsSaving) ... else ...` with the symbol map in a local. The save arm's default value is homed in the dead argument slot.
- **CTCHeroExperience** 006d7cb0 IsReadyToLevelUp() const:
  - A stat-track loop over 0..5 (strength-reduced; the cost is `level < Abilities.size() - 1 ? Abilities[level + 1].ExperienceCost : -1` with an unsigned compare).
  - Then a const GetInterface (const LowerBound) and an ability loop over 1..19.
  - The stat experience is read into a named local BEFORE the cost call.
- **CTCFireheartMinigame** 007e7ac0 DelayedQuit (`/GS`):
  - The sound name, the subtitle prompt and the game event sit in three sibling blocks, so the RTM compiler shares their frame slots (the GetText temporaries overlay the CCharString slot and the event overlays all of them).
  - The sound name is a named CCharString, re-addressed with `lea`.
  - The def, the font bank and the player GUI are locals taken before the argument pushes (edi/ebp/ebx).
  - The event uses an inline by-value AddBOOL.

## Parked for Fable 5.1 (14)
- 007e7760 CTCFireheartMinigame::SetHeroPos DIFFER(275v273): gsi = World+0x38 loaded into eax and spilled to [esp+34h] with the vtable in edi (retail: gsi in edi, vtable in ebp, y reloaded from its arg slot), the control out pointer stored in the dead x slot [esp+20h] instead of the key's slot [esp+28h] | tried: named gsi local, chained `GetWorld()->PGameScriptInterface->Start..`, inline out-pointer accessor (attempt 1 with a scoped_ptr `get()` had retail's edi/ebp/[esp+28h] allocation but kept `add eax,38h`) | next: scoped_ptr<CGameScriptInterface> with an inline `operator->` (the attempt-1 allocation without the address add)
- 004bfd10 CTCGraphicAppearance::CatchMessage DIFFER(264v264): register choice only -- `mov ecx,[edi+4] ; mov ecx,[ecx+70h] ; lea eax` vs retail `mov eax,[edi+4] ; mov ecx,[eax+70h] ; lea edx`, owner reload via edx vs eax, key leas eax/ecx swapped, release vtable via edx vs eax | tried: early-return form (dtor duplicated), pointee Release (long `dec/test/mov` release), DecRefToZero + single exit (re-read of the def after `dec`), + same-TU GetSubDef stand-in under auto_inline(off) (fixed the re-read; left the register names) | next: lane gap: register choice (try the landed LowerBound stand-in too)
- 007b2ab0 CTCGuard::OnDie DIFFER(306v306): `lea ecx,[esp+8] ; push ecx ; mov ecx,[ebx+4]` vs retail `mov ecx,[ebx+4] ; lea edx,[esp+8] ; push edx` for Create's script-name argument, then the money lookup key `lea edx` vs `lea eax` | tried: named "" CCharString in its own block (also moved the velocity copy: fixed by a physics local), implicit "" conversion temporary | next: lane gap: scheduling of the ParentThing load against the string address push (try an inline `GetParentThing()` accessor or `const CCharString&` from a static empty string)
- 007f4b60 CTCHasName::GetNPCNameAndFullTitle DIFFER(284v284): register rotation only (`mov eax,[esi+0Ch] ; mov edx,[eax+30h]` vs ours `mov edx,.. ; mov eax,..`; retail `mov esi,[esi+0Ch]` for PDef in the title block; lea/push register names; vtable via eax vs edx) | tried: `return name + L" - " + title;` (no temporary: 263v284), named `fullTitle` local (fixed the frame), named `def` local in the title block | next: lane gap: scratch-register rotation (try the landed CItem::GetItemDescription CIVCountedPointer release model or GetNPCShortName body as a same-TU stand-in)
- 007b0ba0 CTCFishing::ReelInButtonPressed DIFFER(276v276): the CTCFish lookup key `lea eax,[esp+8] ; mov [esp+0Ch],0DCh` vs retail `lea eax,[esp+0Ch] ; mov [esp+10h],0DCh` (key shares the slot of the cap in ours, of the pulling distance in retail) | tried: flag store before the branch (moved above the `je`), flag per branch (fixed), pulling distance declared before the lookup, cap at function scope | next: lane gap: frame-slot packing (try the out-parameter GetInterface form or the cap as an unnamed `rod->MaxStrain * 0.4f` spill)
- 0057de57 CTCHeroStats::PrepareToKickHeroToMostRecentSafeRegion DIFFER(308v302): `mov bl,al ; neg bl ; sbb bl,bl ; inc bl ; ... test bl,bl ; je` vs retail `mov bl,al ; ... test bl,bl ; jne` for the kick-point search, and framesLeft at [ebp-10h] instead of sharing the marker's [ebp-18h] | tried: named bool + marker block (bool spilled to [ebp-1], this in ebx), `if (!search(...))` (best, kept), inline `HasKickToPoint` helper (same as the named bool), block with early `return` (duplicated cleanup) | next: `if (search(...)) {} else {...}` with the marker in a block that closes before the loop (keep the raw bool in bl), or `bool found = ...; if (found == false)`
- 00801920 CTCIdleScheduler::RegisterIdleStateGroup DIFFER(320v297): `sub esp,8` vs retail `push ecx` (the sub-def CDefPointer homed in the dead `group` slot), weight-map/range registers swapped, long def releases vs `dec [ebp+4] ; jne`, `mov eax,[eax] ; cmp eax,[edi]` vs retail re-reading the iterator from the frame | tried: GetDef out slot copied into a register holder inside a block, DecRefToZero release + iterator `operator!=`/`end()` (worse: 313v297, the weights reference re-ordered) | next: a `__forceinline` helper declaring the sub-def out pointer (dead-slot homing, chunk 161 lesson), DecRefToZero on the holder only, and a same-TU stand-in for the map find 0x008014F0
- 00683b00 CTCCreatureOpinionOfHero::ClearPendingDeeds DIFFER(249v261): (catalog label "CTCHeroOpinionDeedLog::ApplyPendingDeedLog" is wrong) `push ebp` / `push ebx` sunk into the hero branch where retail pushes ebx/ebp at entry (all frame offsets 8 lower), plus eax/ecx/edx rotation in `back()` and the KnownDeeds index | tried: push_back (wrong: retail reads `[head]`, it is push_front -- fixed), same-TU LowerBound stand-in | next: lane gap: callee-saved push placement (try writing the no-hero clear first with `if (!hero) { clear; return; }`)
- 0062b7c0 CWorld::EAControlledCreatureSheatheWeapon DIFFER(280v280): (catalog label "CTCHeroRangedAttacks::OnWeaponSwitch" is wrong) esi/edi swap only (hero in edi and the interface map in esi; retail hero in esi = dead `this`, map in edi = dead `event`) | tried: chained HasMode (pushed 0x2f before GetPlayer -- fixed with a player local), out-of-line HasTC (fixed with `__forceinline`), hero declared before player | next: lane gap: register choice between two dead callee-saved registers (try a named `CInterfaceMap*` local or an inline `GetEventHero(event)` helper)
- 006d7b80 CTCHeroExperience::GetAbilityPossibleLevelUpAmount DIFFER(289v297): structure matches (nested single return-0 exit, max evaluated before level, levels-from-1 loop); retail keeps `this` and later `experience` in ebp and homes `nextLevel` in the dead ability arg slot, ours puts `this` in ebx and `experience` in memory | tried: early returns, nested exit, for-loop form, count-from-0 loop (which gets ebp right but reshapes the loop) | next: count-from-0 with a different bound spelling, or declaration-order changes
- 0071bcd0 CTCHeroMorph::CreateAlphaChannel DIFFER(298v298): instruction stream identical, only frame offsets differ; retail's 0x20 frame overlaps both CSurfaceLockedRect return temporaries with the GetDimensions temporaries, ours gives the Lock temps their own slots (0x38) | tried: direct-init locals (no copy, 250 bytes), assignment from Lock (right stream), scoped assignment blocks, by-value __forceinline helper | next: Lock result bound to a const reference, or CSurfaceLockedRect with a user copy ctor / default ctor
- 0071d940 CTCHeroMorph::AddTattoo DIFFER(299v300): (the CDefPointer<const CThingBaseDef>& overload) the only difference is an ebx/ebp swap; retail puts `this` in ebx and the iterator in ebp, ours does the reverse (the byte count differs because `mov esi,[ebp]` needs a disp8) | tried: no stand-in (325 bytes, tattoo re-read), same-TU GetSubDef<CTattooDef> stand-in (299), a while loop, the iterator declared at the top of the function | next: write the clash test as a __forceinline helper, or use `continue` after the erase
- 00c3da60 CCombatActionManager_00c3d610::CreateEnvironmentCombatAction DIFFER(261v282): (/EHsc; the label class "CTCHeroStats" is propagated) the instructions match the retail blocks, but the layout differs; retail puts the `if (!action) return 0` join straight after the pool block (the new/null arms jump back to it) and gives the error arm its own return-0 tails, while ours puts the join after the error arm and zero-initialises `action` | tried: `budget > 0 && (taken = Take())` (budget re-read), goto fail with a return in the error arm (251), goto fail with the error arm falling to the join (261), budget cached in a local (250, ebx) | next: start from the landed sibling 0x00C3DCD0 source verbatim and add the IsSecondaryAction mask and the empty-pool error edge one at a time
- 00706510 CTCHeroAttachableAppearanceModifiers::UpdateHiddenMaterials DIFFER(304v304): the instruction stream is identical (aligned frame, inline set clear, def/temp null tests shared, GetSubDef stand-in); only frame offsets differ, because retail's 0x18 frame puts the dropped `insert` pair result over the sub-def pointer's slot (dead once it is in edi) and ours gives it its own 8 bytes (0x20) | tried: Set() helper (321), operator=(const CDefPointerBase&) from a named result (321 with align(8)), plain pair (304, layout only), the same source under the QFE-4035 compiler (same) | next: the same temporary-packing gap as 0071bcd0; try the sub-def as a pointer filled by a helper returning the CDefPointer by value

## Skipped
- 006ca5a0 CTCHero::OnCreate: the catalog row holds OnCreate (0x006CA5A0..0x006CA64A, 171 bytes) plus four small bodies from 0x006CA650 on, so it contains more than one function.

## Label corrections
- 004bda30: is NGraphicAppearance::CTCGraphicAppearance::GetAllHelperPointNames, not GetAllDummyObjectNames. Evidence: the Ego_r.pdb source order and the C3DMeshStats helper-section flag; 004bdb40 is the real GetAllDummyObjectNames.
- 00c3d160: is CVtable_012B5870::Cleanup (no-RTTI vtable 0x012B5870, called only from destructor 0x00C3CB30), not CTCFishingSpot::CleanupAndDestructor.
- 00683b00: is CTCCreatureOpinionOfHero::ClearPendingDeeds, not CTCHeroOpinionDeedLog::ApplyPendingDeedLog.
- 0062b7c0: is CWorld::EAControlledCreatureSheatheWeapon, not CTCHeroRangedAttacks::OnWeaponSwitch.
- 00c3da60: the class "CTCHeroStats" is propagated across the 0x00C3Cxxx combat-action module. The owner is the combat-action manager of the landed RecycleAction 0x00C3D610 (siblings 0x00C3DCD0 / 0x00C3DDE0).
- Slot-only labels named here: 006c3f40 and 007e5b90 "vfunc_9" = FrameUpdate; 0057c9c4 "vfunc_12" = CTCHeroStats::Copy.
- Callee labels that are byte-twin or propagated:
  - Def lookups that name the wrong instantiation:
    - 0x007C6600 "GetDef<CThingPhysicalSwitchDef>" (here GetDef<CSpecialAbilitiesGhostSwordDef>).
    - 0x007B2600 "GetSubDef<CAICreatureWillPowerIndicatorDef>".
    - 0x00801670 and 0x0042AF3C (other GetSubDef<T> / GetDef<T> twins).
    - 0x005B1EFC "GetSubDef<CAbilityDef>" (here GetSubDef<CTattooDef>).
  - CTCHeroMorph's AddTattoo callees:
    - 0x0071FA60 "_Move<CMeleeCombatAbilityDef>" is SGI `__copy`.
    - 0x005B26E9 "_Destroy_range<CFlash>".
    - 0x00720810 "UnloadResource" is vector<CDefPointer>::erase (already landed under that name).
  - Other functions:
    - 0x006C5630 "CThingBuilding::GetFloorMaterial" is CTCHeroOpinionDeedLog::GetDeedEffectsDef.
    - 0x005DA4C0 "CXMVPlaybackInfo" is a CTCWeapon member.
    - 0x004292C0 "GetNoAllocatedAreas" is `_Rb_global<bool>::_M_increment`.
    - 0x006869D0 "_Cons_val<...>" is CTCBase::GetDefinitions.
    - 0x004ABA70 "BuildTreeArray" is set `_M_erase`.
    - 0x007E7580 / 0x007E7880 "CTCBetting::GetText / PlaySoundW" are CTCFireheartMinigame's own GetText / PlaySound.
    - 0x0099B6B0 "CCharString::AssignFromWide" is CWideString(const wchar_t*).
  - The 0x00C3Cxxx module:
    - 0x00C3C9A0 "DrawGetEnvironment" is CCombatActionPool::Take.
    - 0x00C3C360 "InformOfEvent" is the module's operator new.
    - 0x00C44580 "CIcicleData::Initialize" is the environment action's constructor.
    - 0x00C3CA30 "TransferFromGui" is CCombatActionPool::Touch.

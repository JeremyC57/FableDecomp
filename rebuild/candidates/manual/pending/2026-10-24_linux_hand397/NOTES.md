# Hand chunk 397 (CTCAtmosPlayer .. CTCAISummon): 36 of 40 byte-exact

All 40 catalog rows were `_global`. Each row's real class comes from the FableWin.pdb / Ego_r.pdb symbol, the callers, or the vtable slot. 35 rows are RELOCATION_MATCH and 1 is MATCH (007fb240). All 36 tests PASS under g++ and VC7.1.

## What matched (36)

### CTCWillResponse statics (6)
All six are `__fastcall` and take the thing.

- **007e4040 SetThingAsNotBloodyWillableAtAll:** adds the component through AddTC when it is missing. On clear, the inline Cleanup removes the component once every bit is back at its default.
- **007e4430 IsThingForcePushable:** checks the will-response bits, then the shield-spell TC, then creature mode 0x36.
- **007e44e0 IsThingLifeDrainable, 007e45d0 IsThingNotBloodyWillableAtAll, 007e4680 IsThingEpicPowerExplodable, 007e46d0 IsThingEpicSpellable:** each is a single bit read through `PeekTC`.
  - Shape: `PeekTC<const T>` with the map address in a named local.
  - The private inline per-bit getters follow PDB field order.

### CTCAtmosPlayer (1)
- **007e3610 SetAtmosDefIndex:** stores the index. When it is non-zero:
  - creates an emitter at the parent position (only when a sound system exists);
  - plays the def name on the emitter.

  Shape: `GetString(long)` on `.m_index`, with the def name passed as a named CCharString.

### CTCFireRangedWeaponOnLeavingStateGroup (1)
- **007e5b40 Cleanup:** static; deregisters the combatant and removes mode 0x19. Shape: `__forceinline` PeekTC with the non-const LowerBound copy.

### CTCOracleMinigame (1)
- **007e5f70 ProcessInput:** queues the input and pushes its symbol to the HUD.
  - Shape: STLport `size()` walks the list.
  - `push_back` is `insert(end(), x)`, with a placement null test on the node data.

### CTCAvoidRegionExit (1)
- **007e9250 UpdateBestDist:** takes the maximum over the static `DistMap`.
  - Shape: the iterator's `operator++` stores the result of a static by-value `_M_increment`. This gives retail's `mov eax,edx ... mov edx,eax`.

### CTCActionUseStartExtrapolationMorph (1)
- **007ebc20 OnActionUse:** calls the base class, then turns the user to face the parent: `WrapAngle(atan2 * 1/2pi) + 0.5`.
  - Shape: modelled non-virtual (QAE).
  - Uses the landed `__forceinline` WrapAngle.

### CTCVillageMember (1)
- **007ee540 GetPVillage:** returns the village TC of the pointed-to thing.
  - Shape: the `return 0` comes last, which gives retail's trailing `xor eax,eax`.

### CTCCarriedActionUseSpade (1)
- **007f0300 IsThingAbleToDig:** the creature cannot dig when it stands on a thing. Otherwise it can dig when the strongest engine theme under it has a material def with `IsDiggable` set.
  - The whole test sits inside `if (!GetThingStandingOn())`.
  - The def manager comes from an inline `GetDefinitionManager()` getter, which is evaluated before the arguments.
  - Two same-TU GetDef stand-ins (one per instantiation) let the two CDefPointer locals share one frame slot.

### NRumble / CTCDRumble (2)
- **007f1840 GetRumbleDistanceAttenuationAtPos:** `exp(-0.01 * |d|^2)`. Shape: a named `distSq` local over the C3DVector temporary's `GetLengthSquared`.
- **007f1ac0 CTCDRumble::Create:** THING_RUMBLE. Shape: a named CCharString in its own block, then the CTCDExperienceOrb::Create pattern.

### CCountedPointer<NBBBMaths::CLinearSpline> (1)
- **007f3fb0 DeleteData:** `delete (T*)data` on an explicit template instantiation.

### NTargeting (1)
- **007f59f0 IsThingVulnerableToMeleeStrikesFromThing:** kill-everything mode, then the best attack target, then enemies.
  - Shape: the one-ternary PeekTC pick, which puts the component in ecx.

### CTCActivationReceptor* / trigger functors (4)
- **007f7750 RemoveTrigger:** `erase(remove(...), end())`.
  - Shape: a noinline TEMPLATE `__find` with the real unrolled body.
  - Uses the SGI ternary `remove`.
- **007f7b70 `_For_each<CThing**,CInformTriggerOfReceptorDeath>`:**
  - Shape: the functor's `operator()` is `__forceinline`.
  - GetTC's lower_bound is written over one cursor.
- **007f7e70 `unary_negate<CIsTriggerActivated>::operator()`:** `!Activated`.
- **007f8160 CTCActivationReceptorRelay::SetTriggerState:** forwards to OnTriggerActivate (slot 28).

### Levitation helper (1)
- **007f8220 anonymous GetRelativePosition:** a spiral point.
  - Shape: the angle product is written inside both sin and cos.
  - A function pointer keeps the helper emitted.

### Targeting comparator and the SGI sort family (9)
- **007f9480 CVectorCompare_BestForTargeting::GetCompareValue:**
  - Shape: hand-declared `extern "C" double exp(double)` with an intrinsic.
- **The /Gr `__fastcall` SGI algorithms over `CThing**`** with the CThingCompare_BestForTargeting2 comparator inlined (the landed 0x7f9f00 body). All eight matched on the first grade as plain non-template functions in `namespace std`:
  - 007f9ff0 `__unguarded_partition`
  - 007fa0e0 `__unguarded_linear_insert`
  - 007fa710 `__make_heap`
  - 007fa7b0 `__pop_heap_aux`
  - 007fa830 `__final_insertion_sort`
  - 007fa890 `make_heap`
  - 007fac00 `partial_sort`
  - 007fae60 `sort`

### CTCKraken (1)
- **007fb240 HaveAllTentaclesBeenHit:** `if (!Phase) return false; return Tentacles.size() == 0;`. Exact MATCH.

### CTCDPhysicalObstruction (2)
- **007fd4b0 Create:** THING_PHYSICAL_OBSTRUCTION. Shape: the Rumble pattern plus an inline `SetParams`.
- **007fda80 `_For_each<CThingCreatureBase**,CKeepOutOffSmearedOutLine>`:**
  - The functor needs a user constructor (FableWin.pdb has one). A POD functor moves the hidden result pointer onto the stack.

### CTCAISummon (2)
- **007fdeb0 GetAllSummonedCreatures:** `AllSummonedCreatures.remove(CIntelligentPointer<CThing>(0))`.
  - Shape: the landed OnCreate temporary model.
- **007fdf60 GetNumberCanSummon:** STLport `min` by reference.
  - The work sits inside `if (IsTimeToSummon())`, with `return 0` last.
  - The def is read through CDefPointer's inline `operator->`, which keeps it in a shrink-wrapped ebp across the size loop.

## Parked for Fable 5.1 (4)
- 007e4530 CTCWillResponse::IsThingLightningable DIFFER(159v159): the merged `return true` block sits after the last test (retail places it after the 0x4E test, and the 0x18 test jumps back); the final flag is read as a dword `mov eax,[edi+6Ch] ; shr eax,3` where retail reads a byte `mov al,[edi+6Ch] ; shr al,3 ; and al,1` | tried: `(flags & 8) != 0`, `(flags & 8) ? true : false`, `(flags >> 3) & 1`, `(unsigned char)(flags >> 3) & 1`, `((unsigned char)flags & 8) != 0`, the 0x18 test as `&&` and as nested ifs | next: ThingTypeFlags as an `unsigned char` bit-field (bit 3) behind an inline getter, with the 0x4E / 0x18 tests as one `||` chain returning true
- 007ea1a0 map<CThing*,float>::operator[] (CTCAvoidRegionExit::DistMap) DIFFER(115v115): `mov [esp+18h],0` (`pair.second = 0.0f`) lands between the hint copy's `mov eax,esp` and `mov [eax],edx`; retail stores it after the hidden-result push (`mov [esp+1Ch],0`) | tried: a single map struct, a map over `_M_t` with `insert_unique(hint, v)`, a pair template with a const first, an STLport iterator with body assignments | next: `value_type(k, T())` as a named temporary built before the hint copy; else lane gap: store scheduling
- 007fa160 std::__push_heap<CThing**,int,CThing*,CThingCompare_BestForTargeting2> DIFFER(197v197): ours `fstp [esp+14h] ; mov ecx,[esp+10h] ; push ecx`, retail `mov ecx,[esp+10h] ; push ecx ; fstp [esp+18h]` (the spilled posB is reloaded and pushed before valueA is stored) | tried: named float locals (the landed comparator body), the compare as one expression with no float locals, pointer locals for the positions | next: lane gap: x87 store vs spill-reload scheduling (the only family member where posB spills)
- 007fb1c0 CTCKraken::KillTentacles DIFFER(106v106): ours `mov eax,[eax+4] ; lea ecx,[eax+8]`, retail `mov ecx,[eax+4] ; add ecx,8` (back()'s element address) | tried: `*(--end())`, STLport's `tmp = end(); return *(--tmp);`, the real `_List_node_base` / `_List_node<T>` split with a cast in `operator*`, a `CIntelligentPointer<T>::GetPItem` inline wrapper | next: back() through `rbegin()` (`reverse_iterator` deref), or a named `_List_node*` for `_M_node->_M_prev`; else lane gap: register choice

## Skipped
None. No row held two functions, and no row needed a bare `fistp`.

## Label corrections
- 0x007E3610 `Def_GetDetailsForDisplay` -> CTCAtmosPlayer::SetAtmosDefIndex(long).
- 0x007E4040..0x007E46D0 (`_global`) -> CTCWillResponse statics. Three catalog names were also wrong:
  - 0x007E45D0 "IsThingEpicPowerExplodable" -> IsThingNotBloodyWillableAtAll
  - 0x007E4680 "IsInterfaceVisibleWithBit" -> IsThingEpicPowerExplodable
  - 0x007E46D0 "ETCInterfaceType_GetVisibility" -> IsThingEpicSpellable
- 0x007E5B40 `Cleanup` (`_global`) -> CTCFireRangedWeaponOnLeavingStateGroup::Cleanup(CThingAICreature&).
- 0x007E5F70 `CInputProcessOracleMinigame_PushSymbolToQueue` -> CTCOracleMinigame::ProcessInput.
- 0x007E9250 `TreeWalk_FindMaxValue` -> CTCAvoidRegionExit::UpdateBestDist.
- 0x007EA1A0 `RBTree_LowerBound` -> map<CThing*,float>::operator[] (CTCAvoidRegionExit::DistMap).
- 0x007EBC20 `CTCAppearance_SetFacing` -> CTCActionUseStartExtrapolationMorph::OnActionUse.
- 0x007EE540 `IS_THING_ALIVE<CIntelligentPointer<CThingPhysical const>>` -> CTCVillageMember::GetPVillage.
- 0x007F0300 `IsThingAbleToDig` (`_global`) -> CTCCarriedActionUseSpade::IsThingAbleToDig.
- 0x007F1840 `Math_Distance3DExp` -> NRumble::GetRumbleDistanceAttenuationAtPos.
- 0x007F1AC0 `Create` -> CTCDRumble::Create.
- 0x007F18B0 "CTCDCameraPoint::Construct" -> CTCDRumble::Construct.
- 0x007F3FB0 `operator_delete_array` -> CCountedPointer<NBBBMaths::CLinearSpline>::DeleteData.
- 0x007F7750 `vector_Erase_RemoveElement` -> CTCActivationReceptorBase::RemoveTrigger.
- 0x007F7970 `vector_find_Value_16byte` -> STLport `__find<CThing**,CThing*>`.
- 0x007F7E70 `CTrigger_IsActivated` -> std::unary_negate<CIsTriggerActivated>::operator().
- 0x007F8160 `TCInterface_HandleEndCutFade` -> CTCActivationReceptorRelay::SetTriggerState.
- 0x007F81C0 / 0x007F81E0 "CGameScriptInterface::EndCutFade" -> CTCActivationReceptorRelay::Activate / Deactivate.
- 0x007F8220 `Ellipsoid_ComputePoint` -> anonymous-namespace GetRelativePosition(long, long).
- 0x007F9480 `CVectorCompare_BestForTargeting_ComputeValue` -> CVectorCompare_BestForTargeting::GetCompareValue(float, float) const.
- The sort family (all CThing** / CThingCompare_BestForTargeting2):

  | Address | Catalog label | Real name |
  |---|---|---|
  | 0x007F9FF0 | std_Partition_Quicksort_Things | `__unguarded_partition` |
  | 0x007FA0E0 | Vector_InsertionSort | `__unguarded_linear_insert` |
  | 0x007FA160 | _Push_heap | `__push_heap` |
  | 0x007FA710 | std_MakeHeap_CThingCompare | `__make_heap` |
  | 0x007FA7B0 | std_sort_Wrapper | `__pop_heap_aux` |
  | 0x007FA830 | std_IntroSort_Things | `__final_insertion_sort` |
  | 0x007FA890 | std_MakeHeap_Wrapper | `make_heap` |
  | 0x007FAC00 | Resize_PassToImpl | `partial_sort` |
  | 0x007FAE60 | UpdateTargetList_SortInit | `sort` |
- 0x007FB1C0 `LinkedList_KillAllThings` -> CTCKraken::KillTentacles.
- 0x007FB230 "_Dest_val<...CMusicManager...>" -> CTCKraken::OnDie (KillTentacles + EndHeadEffects).
- 0x007FB240 `CAIStateGroup_KrakenAttack_IsQueueEmpty` -> CTCKraken::HaveAllTentaclesBeenHit() const.
- 0x007FD4B0 `Create` -> CTCDPhysicalObstruction::Create.
- 0x007FD430 "CTCDCameraPoint::Construct" -> CTCDPhysicalObstruction::Construct.
- 0x007FDA80 `_For_each<CThingCreatureBase**,CApplyAdditionalImpulseIfInCombatAndTooClose>` -> `_For_each<CThingCreatureBase**,CKeepOutOffSmearedOutLine>`. The catalog functor name is propagated; that PDB class is 24 bytes, while retail's functor is 12.
- 0x007FD8D0 "CApplyImpulseIfCloseToCombatLine::operator()" (landed under that name) -> CKeepOutOffSmearedOutLine::operator()(CThing*) const.
- 0x007FDEB0 `GetAllSummonedCreatures` (`_global`) -> CTCAISummon::GetAllSummonedCreatures (static).
- 0x007FDF60 `CTCAISummon_GetMaxCreatureCount` -> CTCAISummon::GetNumberCanSummon() const.
- 0x0065DE8A "CEditControlCentre::SetScriptBrushThingsToMove" -> list<CIntelligentPointer<CThing>>::remove(const T&). Identified by use: called on CTCAISummon::AllSummonedCreatures with a null intelligent pointer.
- 0x0063F3A0 / 0x0066C340, both "GetDef<CThingPhysicalSwitchDef>" -> GetDef<CEngineThemeDef> / GetDef<CMaterialDef> (byte twins).

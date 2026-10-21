# Hand chunk 259 (CAIStateGroup_MinionWatchFires .. CAT_Phase1Archer): 21 of 40 byte-exact

All 21 are RELOCATION_MATCH under VC7.1 RTM cl 13.10.3077. No special flags are needed: no /GS, no /EHsc, and no jump tables.
Each row has src/ and test/, and both tests (g++ and VC7.1) print `<addr>_TEST PASS`.

## What matched (21)

- **CAIStateGroup_MinionWatchFires**
  - 0x00936210 IsRunnable: runnable when the idle scheduler prefers this group and a burning building is within sight range.
  - Shape: the search template is a same-TU noinline stand-in that does not let its argument addresses escape. The sight radius is a named float. IS_THING_ALIVE names the item before the dead-bit test; the plain `GetPItem() && !(flags&1)` form gives VC's `mov al,1; test al,cl` select instead.
- **CAIStateGroup_MoveGoodsBase**
  - 0x009162F0 Cleanup: finishes the pick-up and drops the carried goods (stops them, flags them, makes them non-solid) and leaves carry mode.
  - Shape: the carrying and mode-manager lookups use out-parameter GetInterface. The goods and solid lookups use `GetTC<T>()`, which returns 0 on a miss. The physics component goes through an inline const getter. The CTCSolidToThing name is a named CCharString in the else block.
- **CAIStateGroup_OpinionReactionRoot**
  - 0x00986510 MarriageCommentIsRunnable: true when a hero comment is due and a living wife from this village has marriage state 1..5.
  - Shape: one nested `if` chain with a single `return false`. The UID comes from an inline by-value getter. The state test is a named bool set in an `||` chain.
- **CAIStateGroup_RangedCombatUsePreferredFirePoint**
  - 0x0095D590 IsRunnable: takes the nearest preferred fire point and reserves it.
  - Shape: GetFirePointList is a noinline stand-in that does not let the vector escape, so begin/end stay in ebp/esi and the frame is 0xc. The IP assignment names the item. The comparator is a by-value temporary built in its argument slot.
- **CAIStateGroup_RecoverStealableItem**
  - 0x00965320 IsRunnable: finds a target point and a stealable item outside its range.
  - Shape: single-return `if` chain. IS_THING_ALIVE is `__forceinline` (VC out-lines the fourth expansion otherwise). The target point goes into a named pointer before the range read.
- **CAIStateGroup_ScorpionKing**
  - 0x0097B900 IsRunnable: latches the arena centre and the visible enemy, and registers as a combatant.
  - Shape: the GetPFirstThing out-pointer is initialised to 0. The name is a named CCharString in a block. GetPFirstThing is a noinline stand-in.
- **CAIStateGroup_ScreamerPursueTarget**
  - 0x0096DDB0 OnReplace: flags a live target and sets the brain output.
  - Shape: the derived action ctor is a `__forceinline` wrapper over the out-of-line base ctor, so the vptr store is visible and Clone devirtualises to [vtable+0x2c]. The action lives in its own block. `~CBrainOutput` is `__forceinline`.
- **CAIStateGroup_ServeFood**
  - 0x00923510 MakeFood: creates the tavern apple at a position for an owner.
  - Shape: the manager pointer is read into a local first, and the def-name string is a named local in a block. The player number is read before the implicit "" conversion.
- **CAIStateGroup_SleepOnFloor**
  - 0x00969AE0 OnReplace: releases the sleep point and plays the out-of animation.
  - Shape: the creature is a named local read before the occupant call. The animation name uses the implicit const char* -> CCharString conversion. The action is used through a CCreatureActionBase*.
  - 0x00969CB0 Update: walk, play INTO, push CModeSleepBed, idle until stopping.
  - Shape: the mode strings use the implicit conversion; with an explicit temporary, each push follows its own ctor.
- **CAIStateGroup_SummonerCharge**
  - 0x0096FA90 IsRunnable: charges a live enemy between the def's near and far ranges.
  - Shape: the two charge reasons are SEPARATE `if`s, each ending in `PTarget = enemy; return true;` (VC merges the tails). A single `||` or a bool flag rotates eax/edx.
- **CAIStateGroup_TavernKeeper**
  - 0x009689C0 LeanOnBar: lean-on-bar loop with into/loop/out animations.
  - Shape: the three names use implicit const char* -> const CCharString& conversions, so all temporaries are built before the first push. The creature comes through an inline getter. Both "needs attention" exits are written out in full (`FreeBarPoint(); SetPushableByHero(true); return true;`); a shared goto label swaps `push 1` / `mov ecx,esi` in the merged tail.
  - 0x00968E00 IsRunnable: registers the creature as the bar's keeper.
  - Shape: the workplace goes through an inline `GetCreature()` getter plus a cast, which gives `lea ecx,[eax+170h]`.
- **CAIStateGroup_TavernServer**
  - 0x00967B40 FillTankard: pour, wait up to 4 s, fill, use a jug serving.
  - Shape: every failure exit is written out in full instead of using a shared goto. VC7.1 cross-jumps the copies and keeps the first inline; a goto block gets sunk below the loop. The fps global is read into a local before GetWorldFrame.
- **CAIStateGroup_TellStory**
  - 0x00942D40 IsRunnable: tells a story at a story-building workplace at cook time when a STORY_PLACE_MARKER exists.
  - Shape: single-return `if` chain, a named CCharString in a block, and a noinline search stand-in.
- **CAIStateGroup_VersusScorpionKing**
  - 0x009633E0 GetChargeDodgePos: steps sideways of the scorpion's normalised facing.
  - Shape: C3DVector needs a DECLARED memberwise copy ctor, which fixes the fast-rsqrt estimate scheduling.
  - 0x00963640 IsRunnable: latches the arena centre and a circling direction.
  - Shape: a CDefPointer local filled by a GetSubDef stand-in. The negated search result is kept in bl. The target is the ternary `missing ? enemy : centre`.
- **CAIStateGroup_WifeGiveYouGift**
  - 0x009261F0 OnReplace: creates the gift, puts it in the hero's inventory and shows the subtitle.
  - Shape: the landed CTCContainer::AddItemToInventory (0x008ED090) model (HasTC/GetTC, range-checked bitset::test) matched on the first try. The "" and text symbol use implicit conversions. The description is an unnamed CWideString temporary.
- **CAIStateGroup_WorkHammerAnvil**
  - 0x00944E80 IsRunnable: picks the nearest free anvil owned by the workplace.
  - Shape: best/bestDistance are declared before the list copy. The creature is a named local inside the loop body. The property test is `(unsigned char)(props>>6)&1`.
- **CAT_BanditContact**
  - 0x00F09640 Init: registers the two watch callbacks.
  - Shape: two copies of the landed nested new-expression.
- **CAT_Phase1Archer**
  - 0x00F08A30 Init: makes the archer, the hero and the bandits mutual enemies.
  - Shape: the landed SetBanditAllegiences loop. The CScriptThing handle is a member struct with its own copy ctor.

## Parked for Fable 5.1 (19)

- 0x00963880 CAIStateGroup_VersusScorpionKing::IsScorpionChargingAtMe DIFFER(467v463): the inline fast-rsqrt Normalise schedules its integer estimate differently (retail hoists `mov edx,17ch` and the mantissa shr), and the x/y FPU order of the toMe lengthSq differs | tried: declared C3DVector copy ctor, n1-n3 Normalise forms, temporaries vs named, b..s | next: write the toMe vector as a named local with an inline LengthSq member
- 0x00917130 CAIStateGroup_ThrowLogsOnFire::Update DIFFER(437v451): retail inlines the SECOND STLport push_back (direct `_M_insert_overflow`) and calls the first out of line (0x916EF0); ours inlines neither, or both when forced | tried: plain push_back, `__forceinline` push_back, insert, reserve, the QFE compiler (b..h) | next: give the two call sites different vector types or element types so only one push_back is inline
- 0x0091C220 CAIStateGroup_NymphAttack::IsTargetInLineOfSight DIFFER(416v419): retail homes the creature pointer in the dead `target` arg slot and copies target to a frame local (edi from entry); ours does it the other way round | tried: 9 forms (b..i), including arg reassignment, a named copy, and IsNot filter temporaries | next: lane gap: arg-slot homing
- 0x0096AF60 CAIStateGroup_WhisperPotionCombat::DisappearJump DIFFER(419v415): the body matches, but the shared `Terminate(); return;` block is sunk to the function end where retail keeps it after the IsStopping test | tried: do-while, for, goto, if-else, flag, duplicated exits (b..h) | next: lane gap: block placement
- 0x009385F0 CAIStateGroup_UsesSeat::MoveToSeat DIFFER(461v457): retail packs `reached` and the look-at vector into the dead movement-arg slot and keeps the presence bit as a memory byte (frame 0x2c v 0x38); esi/edi are swapped | tried: base, b (bool-store GetInterface) | next: reuse the movement argument as the flag local
- 0x0093EA90 CAIStateGroup_VillageTaskTorch::Update DIFFER(422v452): retail pushes the creature before PTask.GetPItem(), keeps &PTask in edi without ebp, and uses `lea ecx,[eax+8]`; ours hoists GetPItem, uses ebp and mov/add | tried: base, b, c (QFE as well) | next: inline accessor `GetTask()->GetTorch()` (as in 0x00940310) plus a by-ref IP converting ctor
- 0x00940960 CAIStateGroup_VillageTaskHorn::Update DIFFER(416v431): same as 0x0093EA90 (argument order, ebp vs edi, mov/add vs lea, 0x110 action frame) | tried: base | next: the same accessor/IP-ctor shapes as 0x00940310
- 0x009363D0 CAIStateGroup_MinionWatchFires::Update DIFFER(442v438): the inlined STLport lower_bound loads first via eax->esi (ours esi directly, which shifts the align nop); the tail-loop this reload / Terminate entry is scheduled differently | tried: b..h, including duplicated Terminate blocks (worse) | next: lane gap: lower_bound eax-copy (also in 0x0093C030)
- 0x0091B3A0 CAIStateGroup_SeekPerceivedThing::RunToPerceivedThingPosition DIFFER(472v455): perception component in ebp across two nested move loops; register roles and the inner-loop exit differ | tried: base only (one attempt) | next: GetPerceivedThing takes no argument (the differ passes a wrong 3.5f); fix that, then rework the loops
- 0x0091B570 CAIStateGroup_SeekPerceivedThing::Update DIFFER(503v498): retail keeps the list sentinel in esi across GetAllTCsInRoughArea(&list), keeps x/y on the x87 stack for four __ftol2, and has frame 0xdc vs 0xe0; for_each 0x0091B890 takes its return slot in ecx | tried: base, b, c, d, /Oa (worse) | next: lane gap: LTCG-style escaped-local caching
- 0x0093C030 CAIStateGroup_SeekScriptTarget::Update DIFFER(505v505): this in esi (ours edi); lower_bound loads first via eax->edi; `mov ecx,this` hoisted above the cmp | tried: base, b, c, d (named combat/creature locals, by-ref IP converting ctor) | next: lane gap: lower_bound eax-copy
- 0x009692C0 CAIStateGroup_TavernKeeper::ReceivePayment DIFFER(445v426): the first `FreeBarPoint(); SetPushableByHero(true); return false;` is sunk to the end; frame slots are 8 bytes apart; RemoveMessage this/arg order differs | tried: base, b, c, d (duplicated exits, which fixed 0x00967B40 / 0x009689C0, is not enough here) | next: lane gap: block placement
- 0x00966AF0 CAIStateGroup_TavernServer::GetBar DIFFER(479v502): retail keeps the list sentinel in esi through the inlined push_back loop; the list dtor is inlined on the first return only; the IP ctor/operator= use folded copies 0x0065E133 / 0x007B82C0 | tried: base (one grade) | next: lane gap: LTCG-style escaped-local caching
- 0x0096C4F0 CAIStateGroup_Thunder::Update DIFFER(506v495): the CDefPointer target is loaded once into esi after GetDef(index,&def) and reused across later calls; ours reloads it; the string/def slots are swapped | tried: base, b, c | next: lane gap: LTCG-style escaped-local caching
- 0x0094D780 CAIStateGroup_TrollBase::Initialise DIFFER(487v427): with no frame, all locals (perception out-pointer, four CDefPointers) live in dead argument slots, with a fifth callee-saved register | tried: base (one grade) | next: lane gap: arg-slot homing with no frame
- 0x00938380 CAIStateGroup_UsesSeat::SitInSeat DIFFER(513v499): differences in local order/frame, `(flags>>16)&1`, the per-branch creature load, and the counted-pointer release held in esi/edi | tried: base (one rough grade) | next: CCountedPointer passed by value with a forceinline copy ctor (as in 0x00984D90); reorder the locals
- 0x00940310 CAIStateGroup_VillageTaskPickCrops::Update DIFFER(493v494): the crop pointer is at frame+0xc apart from the key slot (ours shares the slot); `mov eax,[eax+4]; mov ecx,eax` for the villager | tried: base, b, c, d (inline GetTask()->GetCrop(), by-ref IP ctor, retained setne) | next: a separate block scope for the crop local
- 0x00984D90 CAIStateGroup_OpinionReactionRoot::FleeIsRunnable DIFFER(455v455): retail `add esi,44h` vs ours `mov eax; lea esi,[eax+44h]`; everything else matches | tried: base, b, c, d (direct m_interfaces, forceinline CCountedPointer copy/dtor) | next: a creature-pointer local reassigned to `&creature->m_interfaces`
- 0x00986210 CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable DIFFER(483v461): bare dword `fistp` + `fild`/`fsub 1.0` + integer float-bits compare (inline-asm ceil helper) | tried: base | next: lane gap: inline-asm fistp (asm_bake family)

## Skipped

None.

## Label corrections

| Address | Current label | Actually |
|---|---|---|
| 0x008FCF90 | "CPixelShader::CountReferences" | GetPlayerManager |
| 0x00449970 | "Catch@0040d959" | CPlayerManager::GetMainPlayer |
| 0x00487DC0 | "_Cons_val<..EHeroMorphType..>" | CPlayer::GetHeroCreature |
| 0x0094B400 | "CVillageTaskCarryCrate::AssignCreatureToTask" | a folded tankard-user setter |
| 0x00842A80 / 0x00842AF0 | "CCreatureAction_AlertToPlayer" | tavern actions |
| 0x00744460 | "DrawGetSeaLevel" | perception GetSightRange |
| 0x009AD390 | "COptimisedPrimitive::ActualToUniqueIndex" | def global-index lookup |
| 0x007AED10 | "CWorld::DrawGetEnvironment" | thunder-combat state getter |
| 0x007AED50 | "CGameEventPackage::Clear" | thunder-combat state reset |
| 0x0096C6E0 | "CCombatAction_Guildmaster_HealFollowersInto" | the Thunder action ctor |
| 0x0062E900 | "~CTCLightningOrb" | the Thunder action dtor |
| 0x009400F0 | "_Dest_val<..CMusicManager..>" | the pick-crop action dtor |
| 0x009405F0 | "CCreatureAction_AddRealObjectToInventory" | the pick-crop action ctor (2 args) |
| 0x00940120 | "IsInSchoolHours" | a work-hours check |
| 0x00792240 | "SortTreeRecursively" | a list dtor |
| 0x0091B910 | (list dtor) | a list dtor |
| 0x008ABAA0 | "_Dest_val" | ~CCreatureAction_SitDown |
| 0x00740040 | (sit-mode ctor) | the sit-mode ctor |
| 0x00662000 | "CPixelShader::CountReferences" | a creature team/state getter |
| 0x0091B890 | (std::for_each) | std::for_each with the return slot in ecx (custom convention, `ret 0Ch`) |
| 0x004C73D0 | "DoSizeof" | CThing::GetPosition |
| 0x006A4D00 | | GetPhysicsPosition |
| 0x0049D870 | | static CWorld::GetWorldFrame |
| 0x0049D810 | | GetPRandomSeed |
| 0x008FCFC0 | | GetPWorld |
| 0x007EE540 | | GFGetPVillageInterface / CTCVillageMember::GetVillage |
| 0x008FCF80 | | a player-manager getter |
| 0x00449960 | | GetMainPlayer const |
| 0x00F35B30 | | IsDone |
| 0x006290E0 | | the UseObject action dtor |
| 0x004978A0 | | `float __fastcall GFFloatRandom(float range, unsigned long* seed)` |
| 0x00497890 | | `__fastcall GFROR13` |

Callers mislabelled by propagation (the exact labels were not recorded):

| Address | Note |
|---|---|
| 0x00936070, 0x00692620, 0x006860C0 / 0x00685BC0, 0x0087C390, 0x008645E0, 0x00963810 | callers mislabelled by propagation |
| 0x006584C5 / 0x006584F9 | folded bodies |
| 0x00659FED / 0x0065A00F | task-completed calls |
| 0x0073A8A0 | called on the mode manager |
| 0x00744430 | a perception method |
| 0x007449E0 | used as a functor |
| 0x008574D0 | on a combat action |
| 0x008CCBD0 | used for WhisperLand |

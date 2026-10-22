# Hand chunk 289 (CHistoryBookcase .. CList): 29 of 40 byte-exact

## What matched (29)
- **NUISystem::CList** (frame-pointer `/Os` TU: `#pragma optimize("y", off)` + `("s", on)`):
  - 0x00537302 ScrollDownProcess and 0x0053753C ScrollUpProcess (the second catalog row is the scroll-up twin).
  - 0x005363EA ScrollToFirstChild.
  - 0x00536DD9 ScrollUntilChild and 0x0053706F ScrollUntilChildContaining: same body with a different child test. The index search breaks out with `index = i`, and both sound arms take an `owner` GetInstance local.
- **NUISystem::CHoverable** 0x0055BF10 UpdateHovering: mouse-box hover tracking.
- **NInventory::CItem** 0x005BEDCB UpdateItemStats (`/Os` frame-pointer TU): every interface fetch goes through the inline out-parameter GetInterface. The key words then share one frame slot, the last one the dead argument slot.
- **CInputProcess\*::ProcessInput family:**
  - YesNoQuestion 0x0068C880 (/GS), FireheartMinigame 0x00691870 (/GS), InGameMenu 0x00690480 (/GS).
  - Combat::ProcessArmedMeleeInput 0x0068ECA0 (/GS): the combat pointer starts at 0, then the out-param GetInterface. The third argument is the unused weapon reference.
  - ControlCreatureRightStick 0x0068D890:
    - The mouse flag is written `bool x = false; if (...) x = true;`.
    - It needs a float `Abs((float)fabs(x))`.
    - `cos(PI / 180.0 * deg)` takes a float PI.
  - SpecialAbilities 0x00690030 (/GS), landed with the advisor's fix:
    - `processed.SetHandled()` is written in BOTH arms of the scroll-loop if/else; VC tail-merges them, and processed then wins esi over the countdown.
    - The hero-state test is a switch with case 0x10 first.
    - The GetName result is a named local.
- **NScript generated Main() bodies:**
  - Rows: CHobbeLarder 0x00D8F6D0, CHobbeLeader 0x00D8FA30, CJamesCommentMarker 0x00D907B0, CJamesCommentMarkerAdv 0x00D90B30, CIsAGuard 0x00DF9710, CInfectedBalverine 0x00E02470 and 0x00E02950 (two different script classes), CInitialScreamerPoint 0x00E1BC70, CKGG_SurpriseBalverine 0x00EA0B10, CHistoryBookcase 0x00EA7B40, CLeader 0x00EF3F00.
  - Common shapes:
    - `if (IsActiveThreadTerminating()) return;` after every statement.
    - Conditional CCharString temporaries with a flag word.
    - Inline-once destructors through destructor-less views with explicit Destroy/DestroyInline.
    - The JamesCommentMarker pair puts the counter, the distance and the loop in their own `{}` block, so the "James" temporary shares the counter's word.
- **CKeyboardDX** 0x00AB6240 UpdateKeyEvents: DirectInput buffered-data drain.
- **CLandscapeBackgroundPatch** 0x00BE61B0 UpdateTesselation: inline re-walking GetWorldX/GetWorldY accessors.
- **CLandscapeBackgroundTreeNode:**
  - 0x00BF84C0 ProcessLoadTask and 0x00BF8030 ProcessBackgroundLoading.
  - Retail inlines the load task's scalar deleting dtor once. The other deletes call a named stand-in for 0x00BF8C10.
- **CTCWillLightning::CLightningBeam** 0x0079FE90 Deactivate: the list size() counting loop and a one-ternary interface pick.

## Parked for Fable 5.1
- 005367f3 CList::Update DIFFER(573v573): only the SIB operand order of four RemoveState child loads ([edi+eax] v [eax+edi]). | tried: operator[] forms, long index, inline forwarders, member-object state holder, const& locals | next: a CArray<CComponent*> accessor from the landed CList siblings.
- 0068ef50 CInputProcessUseRangedWeapon::ProcessRangedAttackInput DIFFER(547v550): VC keeps the constant 1 in ebx where retail keeps the event, so the lookup keys miss the dead event slot. | tried: bool locals, AddBool forms, key locals | next: the 0x00690030 trick (a second use of the event in both arms) to raise the event's weight.
- 00691130 CInputProcessQuestCompletionUI::ProcessInput DIFFER(715v755): retail spills the 8-byte mouse position to a frame across GetInstance; ours keeps it in edi/esi. | tried: user copy ctor, operator=, by-value getter, float members | next: a same-TU noinline mouse-position getter that fills an out-param.
- 0068e510 CInputProcessQuickAccessItems::ProcessInput DIFFER(875v902): the constant -1 is CSE'd into ebp from entry; retail pushes ebx/ebp lazily and spills the special flag to the dead event slot. | tried: three flow variants (a/b/c) | next: split the -1 uses into separately typed constants (signed char vs long).
- 007a1380 CLightningBeam::Activate DIFFER(864v870): retail spills the emitter to a frame word shared with the LowerBound key; frame 0x54 v 0x50. | tried: emitter local scoping, key locals | next: a block-scoped emitter local after the lookup.
- 00a48f10 CLipSyncSampleData::GetFrameInterpolate DIFFER(845v811): known lane gap, an inline-asm fistp floor helper. | tried: C floor forms | next: none (asm_bake only).
- 00a48ba0 CLipSyncSampleData::WriteToFile DIFFER(873v875): inline-asm fistp weight byte (lane gap) plus the STLport map model. | tried: local map model | next: none for the fistp.
- 00be79d0 CLandscapeBackgroundPatch::RenderWithOcclusion DIFFER(528v528): matrix-build register allocation and the query<0 tail scheduling. | tried: operand orders, named locals | next: OriginX as a const& local.
- 00869020 CHitParameters::SetAsModifiedByArmour DIFFER(971v955): the make_pair temporaries. Retail keeps the converted pair's key in esi through the insert and both releases; ours re-reads the stack copy. There is also an x87 fstp/fld reload before the augmentation loop. | tried (10): const pair key, by-value make_pair, iterator-class find/end, cached end local, if-form find tail, begin/last locals, buff temp | next: insert taking the key and the float separately (an inline insert_equal(const key&, float)).
- 00542780 NUISystem::CIconText::AddIcon DIFFER(682v682): two instructions. Retail loads FrameWidth then FrameHeight back to back before the fild through the dead graphic-arg slot. | tried (14): casts, long/ushort/ulong locals in either order, SetSize helpers, C2DCoordI, C2DVector temp | advisor (14 more, probe/advisor/542780/NOTES.md): an inline `C2DVector(unsigned short, unsigned short)` ctor into a named temp gets the retail load order (684, but the fstp pair comes after both filds) | next: a C2DVector base-class or non-inline (float, float) ctor combined with the ushort path.

## Skipped
- 00691d70 CInputProcessCreatureMovement::ProcessInput: the row spans ProcessInput, its jump table, the next function (0x00691F80 GetInterface helper, `ret 4`) and a `ret 8` stub.

## Label corrections
- 0x0053753C is CList::ScrollUpProcess (the catalog says ScrollDownProcess).
- 0x00687B30 is CInputProcessGameBase::IsEventButtonReleased.
- 0x00B8D0B0 is GetQueryResult, not AddChildPrimitive; 0x00B8D090 and 0x00B8D0A0 are empty folded functions.
- The two "CInfectedBalverine::Main" rows (0x00E02470, 0x00E02950) are distinct script classes (dinner balverine v infected balverine).
- 0x0047D6F2 is a CTCInGameMenu input check.
- 0x009A4FB0 is labelled GetCanItemBeQuickAssigned but is HasInputFocus.
- 0x0066BD00 / 0x0066BDF0 are CTCSoundPlayer::PlayAttachedCriteria / PlayUnattachedCriteria (0x66BDF0 is labelled CParticleMorphs_LoadAndProcessMorphs).
- 0x00993B10 is ~CMemoryDataInputStream.
- 0x00BF8C10 is CLandscapeBackgroundLoadTask's scalar deleting destructor.
- 0x00766920 (labelled CParticleMorphs_InitFromDef) is the augmentation component's vector fill (GetAugmentations).
- 0x00733290 (labelled CTCCombat::Query_733290) is the BREAK_BLOCK validity test.
- 0x00687C80 is the IsEventQuickAccessItemButton forwarder; 0x00687C90 is IsOtherQuickAccessItemButtonHeld.
- 0x005EED4C is CTCInventoryAbilities::TriggerSpecialAbility.
- 0x00868E50 / 0x008686E0 / 0x008687A0 / 0x00868A80 / 0x00868B40 (forge-autoname CTCBase_* labels) are `__fastcall` armour helpers of CHitParameters::SetAsModifiedByArmour (thing, armour multimap, multiplier, damage&); 0x00869CA0 is that multimap's `_Rb_tree::_M_insert`, not CMemoryAllocatorVariableSize::InsertNode, and 0x00869BB0 its `_M_erase`.

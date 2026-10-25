# Hand chunk 477 (_global .. _global): 30 of 40 byte-exact

All 40 rows are `_global` catalog labels in 0x006E8EE0..0x00760020. Every matched row grades RELOCATION_MATCH (gall.sh) and its test passes under g++ (tall.sh) and VC7.1 (tall.py). STLport code is in namespace `std`.

## What matched (30)

**STLport vector `_M_insert_overflow` (9).** 0x006F4750 CCombatRing, 0x006FC6A0 pair<ulong, CSmallVector<CEvent,32> >, 0x00709840 CModifierGraphic, 0x00709D60 pair<CMeshInfo, CEngineTextureMorphSet>, 0x00720F40 pair<CSkeletalMorphs::CEntry,float>, 0x00739550 CIntelligentPointer<T>, 0x00749420 pair<CIntelligentPointer<const CThing>, CDangerStatus>, 0x00759190 pair<EBodyLocation, CWoundMorphs::CEntry>, 0x0075F860 CKeyCamera.
- What it does: the reallocation path of push_back and insert.
- Shape: STLport 4.x verbatim. The element size is read off the divide magic (0x2AAAAAAB/`sar`, 0x92492493/`add edx,ecx`, 0x2E8BA2E9). The helpers (`__uninitialized_copy`/`fill_n`) are out-of-line at their landed addresses. The element copy follows the element's real copy constructor: a user one for STLport pair or CKeyCamera, otherwise memberwise.

**Other STLport vector members (3).**
- 0x00709660 vector<vector<CModifierEntry> >::operator=, 0x0075B8D0 vector<C3DVector>::operator=: the three-way assign over the landed `_M_allocate_and_copy`/`__copy` helpers. The C3DVector user copy constructor has a trivial destructor, so `_M_clear` is only the free.
- 0x00759290 vector<pair<EBodyLocation,CEntry> >::_M_fill_insert: STLport verbatim, with `fill`, `__copy_backward_ptrs` and `uninitialized_fill_n` out of line.

**STLport algorithms (9).**
- `__introsort_loop` ×4: 0x006FD1E0, 0x00721C10, 0x00721E20, 0x00759660. Shape:
  - the depth-limit `partial_sort` fallback;
  - `--depth_limit` decremented in its own argument slot;
  - `__median` inlined;
  - the pivot copied into the by-value argument area.
- CAnimateThing stable_sort chain over list<CAnimSet>::const_iterator:
  - 0x007103C0 `__rotate`;
  - 0x00710DC0 `__rotate_adaptive`, which needs `__forceinline` copy/copy_backward;
  - 0x00711340 `__merge_adaptive`, where VC7.1 turns the second recursion into the loop.
- CTCStealth `__find_if` 0x00748E40 and `__find` 0x0074CEE0: STLport 4.6 random-access 4× unrolled bodies.

**STLport `_Rb_tree::_M_insert` (2).** 0x007203A0 (map<CCharString, pair<long,CDateAndTime> >) and 0x00756150 (map<CCharString, pair<long,long> >). `_M_create_node` is inlined (malloc plus the CCharString copy constructor), with the leftmost/rightmost header fix-ups.

**NAnimationEvents (2).**
- 0x006FAA90 InitialiseManagers: `Reset(new CManager(bank, GetDefDir() + L"...", flag))` twice. The string temporaries die under VC's conditional-temporary flag word. wchar_t is not built-in in the lane, so the literal is `const unsigned short*`.
- 0x006FABF0 LoadAnimationEvents: the twin of the landed SaveAnimationEvents, with progress display around each load.

**CTCHeroAttachableAppearanceModifiers (2).**
- 0x00706B50 GetAppearanceModifierListWithCoversBodyAreaFlags: a local STLport list filled by push_back and returned by copy. In `list(const list&)`, `insert(begin(), first, last)` homes its `__false_type` in the dead `type` slot.
- 0x00707B10 GetMeshesAndMorphSets:
  - per graphic, a local morph set gets one AddTextureMorph per replacement entry (blend type 3, diffuse 0,0,0,255 via `CRGBColour(r,g,b,a)` byte stores);
  - then `push_back(make_pair(meshInfo, morphSet))`;
  - natural inlining reproduces retail's split between the inline make_pair copy and the out-of-line copy inside push_back.

**CTCTextureDecal (1).** 0x0070A570 PeekEngineDecals:
- When dirty, it groups decals per texture in a local map<long, CEngineTextureDecal>.
- `_Rb_tree_base` allocates the header in its own constructor, before the derived `_M_node_count(0)`.
- The tree increment is a static `_Rb_global<bool>::_M_increment(node)`.
- It is built from same-TU noinline STLport bodies, plus declared-only blend-vector copy and push_back for the /Os COMDATs 0x005BB346/0x006CB010.

**NTCScriptedControl::CActionTalkBase (1).** 0x007167E0 InitialiseDefaultCameraTags: seven inline push_backs of one 24-byte tag temporary. The same-TU noinline `_M_insert_overflow` lets VC keep the finish pointer enregistered.

**CTCCombat (1).** 0x007345D0 GetAppropriateCombatAnimationFromList:
- three vector<long> index lists, each finish pointer in a register and re-stored after every push_back;
- the previous handedness is read once;
- the GFRandom pick is `range ? r % range : 0`;
- the operand order `previous == start` gives retail's `cmp [mem],reg`.

## Parked for Fable 5.1 (10)
- 006e8ee0 FindNearestIndexToPoint DIFFER(605v605): x-sum fadd operands commuted in all five unrolled/remainder copies (retail `fld [eax+edi] ; add eax,edi ; fadd [esi]`, ours `fld [esi] ; fadd [eax+edi]`) | tried: scalar dx/dy (567v605), C3DVector operator+/- temporaries (605v605), the PDB user copy constructor + const returns | next: `offset + points[i]` operand order or a member operator+ with the point as `this`; else lane gap: commutative operand order
- 006e94d0 CCombatManager::IsCreatureInAttackRingSector DIFFER(426v426): instruction-identical except stack-slot packing (retail frame 0x1c shares the find key / outerRadius / prong words, ours 0x24) | tried: all five callees same-TU (425v426), find declared only (428v426), GetRing inlined in GetRingRadius (426v426), early return on the ring index | next: block-scope the find key temporary so its words are recycled; else lane gap: stack-slot packing
- 006fda20 CGameCameraBase::IsThingCollision DIFFER(841v819): retail keeps the local list<CThingGameObject*> sentinel in ebx straight after malloc (no `_M_node = 0` pre-store) and across GetAllThingsInRoughArea 0x006FE7C0 (no reload, argument slot recycled); ours keeps the list in memory (frame 0xb8 v 0xa4, ebp pushed inside the loop) | tried: CThing* list (841v819), 4-byte polymorphic base + STLport 4.6 insert (841v819), passing `things.end()` so the list never escapes (819v819, which confirms the gap), the PDB-faithful CFilter_Bind2 / CMeshIntersectionData form (841v819) | next: lane gap: LTCG mod-ref across a same-TU template call
- 00702e00 CThingHeadingGauge::GetHeadingAlignment (descriptive) DIFFER(490v488): world-side virtual call `mov eax,[eax+18h] ; mov edx,[eax] ; mov ecx,eax` v retail `mov ecx,[eax+18h] ; mov edx,[ecx]` | tried: plain inline helpers (182v488), __forceinline helpers + if-body shape (490v488), 3D heading temp + float scale, the direction pointer in a local | next: find the real class (the image holds no references to it) and call the accessor through its real type; else lane gap: register choice
- 00733b70 CTCCombat::PlayCombatStrikeSounds DIFFER(383v380): conditional-temporary flag in ebx (`mov ebx,1 ; test bl,1`) and the condition in a frame byte, where retail has the flag word at [esp+18h] and the condition in bl; ours also reads the def through the returned pointer | tried: one if-condition (388v380), named bool + local def pointer (385v380), same-TU GetSubDef<CEntitySoundDef> (383v380) | next: nested ifs so the temporary is built in one arm only; else lane gap: conditional-temporary flag allocation
- 00733cf0 CTCCombat::UpdateGetHitCounterAttack DIFFER(287v288): retail copies the found iterator's node into ebp and reuses its frame word for the 0x42 key and predicate (frame 0x18, this in ebx); ours keeps the iterator in its own word (frame 0x1c, this in ebp) | tried: ability reference (287v288), reads through it (291v288), pair reference (288v288), iterator copy | next: end the iterator's scope before the 0x42 lookup (block-scoped find); else lane gap: stack-slot reuse
- 00746db0 NStealth::GetStabPosOnThing DIFFER(247v247): C3DVector temporaries copied x->ecx, y->edx, z->eax in retail (appearance loaded late), ours eax/ecx/edx | tried: no copy constructor (255v247), user operator= (237v247), two returns (223v247), user copy constructor (247v247) | next: lane gap: register preference in a struct copy
- 0074e140 CTCContainerRewardHero::GeneratePopoutVectorFromVars DIFFER(245v245): in the return copy retail loads direction.Y (`mov ecx`) before the X `fstp [eax]`, ours after | tried: named float count (251v245: index*(1/count)), inline (float)count conversions + user C3DVector copy constructor (245v245) | next: lane gap: scheduling only
- 00752b00 CTCLook::SetHeadLooking DIFFER(768v757): retail keeps PDef in edx across IsWithinYZHeadRange 0x00751FC0, ours reloads `mov ecx,[esi+14h]` x4; also retail hoists fabs(angleXY), tests the bool in al and uses fimul for range*sign | tried: the first full model (1 grade) | next: lane gap: LTCG caller-saved register live across a same-TU call (then an explicit abs local and an int sign multiply)
- 00760020 NShape::IsLineIntersectingWithOthersInList DIFFER(367v360): retail copies only X,Y of points[0] into previous (ours also a dead Z), iterator in esi and current in edi/ebp/ebx (ours ebx / esi,edi,ebp), dy squared before dx | tried: C2DVector temporaries + operator- (395v360), C3DVector user copy constructor (382v360), edge-member checks (395v360), float-constructor line + plain C2DVector | next: build previous as C2DVector(p.X, p.Y) from a const C3DVector& and square dy first in source

## Skipped
None. No row holds two functions.

## Label corrections
Each item below maps the address and its catalog label to the identified name, with the evidence.

**Rows**
- 0x006F4750 "Vector_InsertRepeatElements_Complex" → vector<CDefendingCombatantInfo::CCombatRing>::_M_insert_overflow.
- 0x006FC6A0 "Vector_Reallocate_WithElements" → vector<pair<ulong, CSmallVector<NAnimationEvents::CEvent,32> > >::_M_insert_overflow.
- 0x006FD1E0 "std_sort_Heapify_Recursive" → __introsort_loop over the same pair.
- 0x006FAA90 "InitialiseManagers" → NAnimationEvents::InitialiseManagers. 0x006FABF0 "LoadAnimationEvents" → NAnimationEvents::LoadAnimationEvents.
- 0x006FDA20 "CGameCameraBase_TracePhysics" → CGameCameraBase::IsThingCollision. Evidence: Ego_r.pdb `?IsThingCollision@CGameCameraBase@@AAE_NXZ`.
- 0x00702E00 "CViewBase_GetRotationDegrees" is wrong: 0x004C7AB0 is CThing::GetWorld, and the body is a 0..40 heading-alignment gauge. The real identity is unknown (the image holds no references); the source name is descriptive.
- 0x00706B50 "FilterAbilityClasses_ByFlags" → CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierListWithCoversBodyAreaFlags.
- 0x00707B10 "RandomAppearance_ProcessMorphList" → CTCHeroAttachableAppearanceModifiers::GetMeshesAndMorphSets.
- 0x00709660 "CFeatDef_VectorReassign" → vector<vector<CModifierEntry> >::operator=.
- 0x00709840 "Vector_Reallocate_CNavigationPosition" → vector<CModifierGraphic>::_M_insert_overflow.
- 0x00709D60 "Vector_Reallocate_CMusicSetEntry" → vector<pair<CMeshInfo, CEngineTextureMorphSet> >::_M_insert_overflow.
- 0x0070A570 "CEngineInternalPrimitive_RebuildDecals" → CTCTextureDecal::PeekEngineDecals.
- 0x007103C0 "RotateBuffer_GCD" → std::__rotate. 0x00710DC0 "StableSortMerge_Rotate" → std::__rotate_adaptive. 0x00711340 "StableSortMerge_AnimSet" → std::__merge_adaptive.
- 0x007167E0 "CActionTalkBase_InitMorphs_Hardcoded" → NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags.
- 0x007203A0 / 0x00756150 "RBTree_Insert" → _Rb_tree<CCharString, ...>::_M_insert. These are two different trees.
- 0x00720F40 "Vector_InsertAndReplaceMulti_12byte" → vector<pair<CSkeletalMorphs::CEntry,float> >::_M_insert_overflow.
- 0x00721C10 "TextureMorph_QuickSort" and 0x00721E20 "MorphData_TimSort" → __introsort_loop.
- 0x00733B70 "CTCCreature_PlayDeathFlourish" → CTCCombat::PlayCombatStrikeSounds.
- 0x00733CF0 "TCCombat_ApplyBlockResponse" → CTCCombat::UpdateGetHitCounterAttack.
- 0x007345D0 "GetAppropriateCombatAnimation_FilterAndSelect" → CTCCombat::GetAppropriateCombatAnimationFromList.
- 0x00739550 "Vector_Reallocate_IntelligentPointer" → vector<CIntelligentPointer<T> >::_M_insert_overflow.
- 0x00746DB0 "GetStabPosOnThing" → NStealth::GetStabPosOnThing.
- 0x00748E40 "LinearSearch_Range_WithValidityCheck" → std::__find_if.
- 0x00749420 "CIntelligentPointer_Vector_Resize" → vector<pair<CIntelligentPointer<const CThing>, CDangerStatus> >::_M_insert_overflow.
- 0x0074CEE0 "std_find<CBaseIntelligentPointer>" → std::__find.
- 0x0074E140 "Projectile_CalculateTrajectory" → CTCContainerRewardHero::GeneratePopoutVectorFromVars.
- 0x00752B00 "CTCLook_UpdateHeadDirection" → CTCLook::SetHeadLooking. Evidence: PDB `?SetHeadLooking@CTCLook@@AAEXM_N0MM@Z`. The landed caller 0x007530D0 also calls it UpdateHeadDirection.
- 0x00759190 "Vector_InsertRange" → _M_insert_overflow; 0x00759290 "Vector_Insert_Range_Shift" → _M_fill_insert; 0x00759660 "Sort_HeapsortPhase" → __introsort_loop. All three are over pair<EBodyLocation, CWoundMorphs::CEntry>.
- 0x0075B8D0 "Std_Vector_Assign" → vector<C3DVector>::operator=.
- 0x0075F860 "Vector_InsertRange_CopyFill" → vector<CKeyCamera>::_M_insert_overflow.
- 0x006E94D0 "Combat_CheckRingRangeToTarget" → CCombatManager::IsCreatureInAttackRingSector.
- 0x00760020 → NShape::IsLineIntersectingWithOthersInList.

**Callees**
- 0x006FE7C0 → CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, list<CThingGameObject*>, CFilter_Bind2<CThingFilter_MinAllowedRadius, CThingFilter_OverlappingSphere> >. Evidence: Ego_r.pdb names this instantiation.
- 0x006A7410 → CThingFilter_MinAllowedRadius::operator().
- 0x00A12450 → CPhysicsMeshObject point-to-point intersection; it writes a CMeshIntersectionData.
- 0x00A127D0 → ~CPhysicsMeshObject.
- 0x006B0850 → physics GetPhysicsMeshObject, by value.
- 0x005BBC93 → __uninitialized_copy<CEngineTextureMorph>. Catalog: _Uninit_copy<CArray<CBodyPartMesh>>.
- 0x005BBDF1 → vector<CEngineTextureMorph>(const vector&). Catalog: CExpressionSet::Add.
- 0x005BB346 → vector<CCompositeBlendDesc>(const vector&). Catalog: vector::operator=.
- 0x006CB010 → vector<CCompositeBlendDesc>::push_back.
- 0x0070ADB0 → _Rb_tree::_M_erase. Catalog: BuildTreeArray.
- 0x0070B150 → vector<CEngineTextureDecal>::erase.
- 0x006CA950 → CEngineTextureMorphSet::AddTextureMorph.
- 0x004C73D0 → CThing::GetPosition.
- 0x00663340 → GetHeroCombatDef. This conflicts with a chunk-261 label.
- 0x006D8BF0 → CTCHeroExperience::GetOnHitSound.
- 0x00669CB0 / 0x00738A00 → GetComponent (Handedness / Misc).
- 0x00440990 → vector<long>::_M_insert_overflow.

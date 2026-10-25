# agent902 near-miss retry pass 2 -- NOTES

Result: 42 of 135 queue rows matched (41 RELOCATION_MATCH + 1 MATCH), every one with src/ + test/ PASS under tall.sh (g++)
and tall.py (VC7.1); 93 DIFFER rows each have a STATUS.tsv line (grade count + what is left); 9 improved sources in differ/.
Plus one fix for an already-landed source (fixes/009e1da0.cpp: GFAXToVX without `volatile`).

## Matches (src/ + test/, PASS under tall.sh and tall.py)
- 0067f430 GFSerialiseVectorBinaryIn<CDegradableInfo> (RELOCATION_MATCH, 3rd grade): give CPersistTraits<T> its Ego_r.pdb
  `T DefaultVal` member (GOTCHAS 1236) on top of the noinline TransferBinaryIn body (GOTCHAS 1046). VC7.1 deletes the
  dead DefaultVal stores (the body never reads `this`) and packs the 28-byte traits over the dead resize(n, T())
  temporary -> traits `this` = retail [esp+1Ch]. The empty traits packed onto the dead count word ([esp+18h]).
  Block-scoped traits and a per-iteration CPersistTraits<T>() temporary were inert / worse.
  Label: catalog "<CBrainUpdateZone>" is propagated; element is CDegradableInfo (28 bytes).
- 00867fe0 CAnimationEntry::GetComponent<CAnimComponentFollowingOnAnim> (RELOCATION_MATCH, 1 grade): port of the landed
  F88 twin 0x005E0380. Byte twin 0x00867F60 (type 0x0B, CAnimComponentFollowingOnAnimFilter) is not queued -- same source
  with the type changed.
- DeallocateUnusedMemory family (coordinator recipe from agent422 / 0x00A74EA0), 4 rows, 1 grade each:
  009b6eb0 CArray<CInstantiatedDefInfo*>, 009b5db0 CArray<unsigned char>, 009b79b0 CArray<pair<unsigned long,
  CDefClassBase::CSubDefInfo>>, 00a725d0 CArray<SElement12 (12-byte record, descriptive name)>. One shared body
  (fam/dum_body.inc): STLport `_Vector_base` + `_STLP_alloc_proxy`, `CArray<T> temp; temp.reserve(size());
  temp.insert(temp.begin(), begin(), end()); DeallocateMemory(); swap(temp);` with same-TU `auto_inline(off)` + noinline
  bodies for reserve, the forward `_M_range_insert` (tag unnamed) and the out-of-line `DeallocateMemory`.
- 0087c070 NCameraModes::COrbitGame::OnInit (1 grade): alias-order principle -- `const CBase* mode =
  CameraManager->CurrentMode;` read BEFORE the `DefaultFOV = 0.2f` store. VC7.1 never moves a load above a store through
  a different base, so a retail load scheduled before a store means the source reads it first.
- 00594a08 map<CDateAndTime, CWideString>::operator[] (1 grade): STLport's `__default_constructed((_Tp*)0)` layer
  (`template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }`) re-addresses the default value; the
  parked `_Tp()` form built it at a different frame offset. Namespace std -> _STL.
- Chunk-422 recipe (same-TU noinline bodies for out-of-line callees that receive a local's address), 3 rows:
  - 00721a50 CVectorMap<long, CTCHeroMorph::CTextureLayer, CKeyPairCompareLess<..> >::Add (2 grades): stand-ins for the
    CCompositeBlendDesc copy ctor 0x005BA48A and _M_insert_overflow 0x00721060 -- VC then loads key and TextureID before
    the first placement store, as retail. Remodelled with the real template names (leaf `?Add@?$CVectorMap@JVCTextureLayer@CTCHeroMorph@@...`).
  - 0070a490 CTCTextureDecal::AddDecal (2 grades): stand-in for the CCompositeBlendDesc copy ctor only (the entry
    temporary's Desc address goes to it twice). Remodelled over `_STL::list<CTCTextureDecal::CEntry>`; leaf is exactly
    the Ego_r.pdb symbol `?AddDecal@CTCTextureDecal@@QAEJJAAVCCompositeBlendDesc@@@Z`.
  - 00a385a0 CCRCSymbolMap::AddSymbolDirty (1 grade, 203v195 -> match): stand-ins for set<ulong>::insert 0x00514C10 and
    _M_insert_overflow 0x009B61F0 fixed both residues (the "" preload into edx and the pair value kept in edx).
- 006384b0 NThingMap::CThingMapManager::GetAllActiveThingMapsInArea (3 grades): no more `/Oa`. A same-TU noinline
  READ-ONLY body for C2DBoxI::Overlapping 0x004FAB40 lets VC7.1 keep info->PThingMap cached in edi across the call (what
  /Oa had faked), plus a resize(n, x) stand-in (the null temporary's address no longer escapes) and the real
  _M_insert_overflow body. Class keys + `vector<T, allocator<T> >` give the PDB-shaped STLport leaf.
  Generalisation: a retail value cached across an out-of-line call that our build reloads = give that callee a same-TU
  read-only noinline body (VC7.1 does see no-write facts of same-TU bodies).
- 00a56aa0 CCardinalSpline::CCardinalSpline(start, end, tension, flag) (2 grades): C3DVector pack(1) with a memberwise
  user copy ctor fixed the first inline push_back's ecx/edx choice. The _M_insert_overflow stand-in alone was inert.
- Landed out-parameter GetInterface shape (0x007504E0), 2 rows: `__forceinline bool GetInterface(long type, T** out)`
  with the presence bit test INSIDE (`if (bits[type>>5] & (1 << (type&31))) { key; LowerBound; if (it == last ||
  type < it->type) it = last; *out = it->component; return true; } return false;`), called as `if (GetInterface(k, &p))`.
  The component then lands in eax (`mov eax,[eax+4] ; push ; mov ecx,eax` / `mov eax,[eax+4] ; mov ecx,[eax+0Ch]`).
  Without `__forceinline` it is called out of line. HasTC && GetTC, F88 tails, a named local, a void out-pointer
  GetInterface after HasTC: all leave the component in ecx.
  - 0076d1b0 CTCEnemy::AddEnemyThing (4 grades); leaf = the FableWin.pdb symbol `?AddEnemyThing@CTCEnemy@@QAEXPBVCThing@@@Z`.
  - 006ce730 (anonymous)::FrameUpdateThing::operator() (4 grades over the pass; the 4th was this shape).

- STL / map family (chunk-422 insert_unique stand-in), 7 rows: 009913d0 map<ulong, CFileObject*>::operator[] (the parked
  "pair zero-store scheduling" was NOT a lane gap), 009b7bc0 map<ulong, CDefPointer<CDefClassBase>>, 00802110
  map<EIdleStateGroup, CAIStateGroupBase*>, 007ea1a0 map<CThing*, float> (remodelled from "RBTree_LowerBound"), 0082fb90
  map<unsigned __int64, CThingPosAndOrientation>, 00a2d810 map<ulong, CCountedPointer<C3DAnimation2>> (stand-in that MAY write
  the counted pointer). Recipe: a same-TU `auto_inline(off)` noinline `insert_unique(iterator hint, const value_type& v)` body
  that READS v (one ignoring v lets VC drop the pair stores).
- 00938db0 CAddFreeSeatToList::operator() / 009438f0 CAddOccupiedSeatToList::operator() (byte twins): same-TU push_back stand-in.
- x87 recipes (see LESSONS 17-22): 00a57730 C2DLineF::GetDist (named float locals), 00a441e0 GFGetSignedAngleDifference and
  00a442e0 GFGetDistanceSquaredFromPointToLine (assign-in-condition + re-spelled expression), 0080c190 (anon)::
  GetExplosionEffectInterpolate (param clamped in place), 00670470 CTCCoopSpirit::UpdateHeightAboveGround (named `direction`),
  00875a30 NCameraModes::CTools::Get2DRotationAngle (`angle *= sign; return angle;`).
- 00997f80 CAFile::GetCurrentPath (one tail-duplicated `return CurrentPath;`).
- Real STLport 4.6 template family (pass-2 recipe, LESSONS 23-27): rows parked as register swaps / shrink-wraps over flat
  invented containers matched once modelled on `_STL` templates (`_STLP_alloc_proxy` header, `_Rb_tree_iterator` /
  `_List_iterator` over `_Const_traits` / `_Nonconst_traits`, pair with user ctors, erase via `_Destroy` + deallocate,
  postfix ++ through static `_Rb_global<bool>::_M_increment`): 004bcf50 CTCGraphicAppearance::GetPMeshEffect, 006e5990
  RemoveConversation (map<long, CScriptConversation>, inline erase), 00729960 list<CExpressionState>::push_front, 006d7ec0
  CTCHeroExperience::AddAttackOfTypeToAttackHistory (fam/stl_list46.inc), 00a557e0 _STL::copy(set<CCharString> iterators,
  back_insert_iterator) (set iterator with an IMPLICIT copy ctor).
- Chunk-422 extensions: 006fc3e0 _STL::__linear_insert<SKeyedEvents*> (same-TU CSmallVector::Copy keeps the by-value pair
  uncaptured), 0066bdf0 CTCSoundPlayer::PlayUnattachedCriteria (same-TU GetPotentialCriteriaStrings filler kills the CSE'd
  zero register).
- 004dfcea _STL::__unguarded_partition over pair<CCharString, CTCBase::CRegisteredTCInfo> (2 grades): the real template + the
  landed CBasicString<char>::operator< (0x00429950) as a same-TU noinline body: the pivot key stays in esi and the ebp frame the
  opaque call forced disappears (110v113 -> match).
- 006e9f60 (anon)::GetAttackerProngSelectionCurrentWeaponBaseCompareValue (3 grades): positive-first verdict (`if (IsUsing..)
  return want ? 5 : 0; return want ? 0 : 5;`) fixed the swapped result arms; the landed 0x007504E0 out-pointer GetInterface for
  the HasTC-guarded combat lookup put the component in eax. Anonymous namespace + function-pointer keep-alive.
- 005305a0 NUISystem::CComponent::ChangeColourDelta (4 grades, MATCH): one in-body colour local + `+=` with the landed CRGBColour
  copy ctor `Set(o.R, o.G, o.B, o.A)`; exact PDB symbol.
- x87 double-global family: 009e20f0 GFABoxToVBox(C2DBoxI) and 009e2240 GFACoordToVCoord(C2DCoordI) (1 grade each): the
  "operand order" lane gap is a source spelling -- `(float)g_VirtualWidth * x`; the Y converter reads the system manager into
  a local, forms the product, then the display manager into a named local. The same spelling matches the LANDED 0x009E1DA0
  GFAXToVX without `volatile` (fixes/009e1da0.cpp, both the one-expression and the ScaleToVirtual-helper form).

## Lane gaps / parked (STATUS.tsv has every row with its grade count)
Proven or strongly indicated lane gaps (several forms each, plus QFE and /G5-/G7 where noted):
- 0099bb80 CWideString::SprintfDynamic: `lea esi,[eax+1000h]` vs `mov esi,eax ; add esi,1000h` (8 grades over two passes).
- 009b99c0 CStringParser::ReadNextSymbol, 00a3f340 CSoundManagerLH::vfunc_41: register colouring (8 grades each).
- 008365f0 _STL::for_each: strlen-intrinsic zero register.
- 00978b90 / 0097af70 _STL::__adjust_heap: comparator copy pipelining.
- 00462e78: reassociation of x.start + size().
- 009e1960 / 009e1a80 GFStringToInt: jump threading of the zero-length guard.
- 004c3c60 / 004c3bf0 / 00adb4d0 __pop_heap_aux over 12-byte records: prologue push order (template, QFE, /G7 inert).
- 006379d0 NThingMap::InitLayerInfo: eax/ecx tie under optimize("s") (index, pointer walk, store order; RTM + QFE).
- Two-call float comparators 00807170 / 00807250 / 00808aa0 / 007fa160: the second argument is loaded between the first
  result and its fstp (007fa160: the real `_STL::__push_heap` template is inert too).
- Float loop promotion 00754d80 / 006b5890 (val.second never hoisted onto the x87 stack).
- Volatile candidates 009c37a0 / 009c3820 CFileInstaller::OnFile{Read,Write}Completion (dead ExitThread byte load).
Near misses with a source-level residue still open (best source in parked/ or differ/):
- 009e1dd0 GFAYToVY / 009e21d0 GFACoordToVCoord(C2DVector) / 009e2020 GFABoxToVBox(C2DBoxF): operand order fixed (differ/);
  only the Y product's `fstp` vs the `mov ecx,eax` this copy after the result-slot push remains (/G5, /G6, named local,
  operand order, one-expression all inert). The C2DCoordI caller (009e2240) matches with the same body.
- 005148d0 / 00516010 CReplaceSerialise<..>::ConvertPreviousUnknownVersionUp: the lane keeps the CSE zero in ebx and spills
  result.Object where retail spills the hidden pointer (body-assigned info ctor, `new; if`, previous-step stand-in, copy-temp
  return all inert); retail v7->v8 0x0051B940 shows the ours-shape, so the header content is the next lever.
- 0041cb10 CYesNo::ProcessEvent: VC7.1 threads the after-yes-call edge before RA (nested ifs, accessor, switch inert).
- 00939dd0 FindMostRecentMessage: m_pFirst hoist fixed (differ/), loop alignment residue left.
- 0072aaf0 / 00814c70: a call's `this` (ParentThing / appearance) loaded after the pushes instead of right after the CCharString
  temporary (named locals, inline accessor / wrapper inert) -- one shared residue, worth a family attack.
- 00924880 / 00954220: CIntelligentPointer register ties (the 4-argument twins of 00924880 match with the same source).
- Scheduling-only singles: 008f99a0 (push vs string-member stores), 00789ce0 (~CDefPointer refcount slot), 007881ad (__ftol2
  store slot), 0084cd10 / 0084d7e0 (camera Z read), 00c69a70 (memset count shl vs lea), 004c10e0 (zero-copy address register),
  0070bf50 (block layout), 008f58c0 (C2DBoxI corner store order), 009350c0 (two float frame slots), 00703210 / 00802d10
  (register ties).
- The 14 large UI / inventory builders (005a3e47, 005a6769, 005a79e6, 005f3f7b, 005f4c8f, 007fc210, 0058f016, 005f45b5,
  0059fee3, 005b6a66, 006276f0, 005cb2b0, 00628000, 00766dd0) were not regraded: frame-slot / register residues after
  agent901's batch stand-ins; 005a6769's last residue is the wanted-total operand order.
- Earlier rows of this pass (00a5a9d0, 00807a90, 00752510, 00732d40, 007e4530, 009d87f0, 0059afd7, 00749620, 00738a80,
  007212d0, 00864ff0, 00926690, 005e1210, 00756c60, 006f96c0, 0058a0c7, 008e8ff0, 0099e690, 006c1ed0, 006d8790, 0074a4e0):
  see their STATUS lines; the improved ones are in differ/.

## Label corrections
- 0067f430 catalog "<CBrainUpdateZone>" -> GFSerialiseVectorBinaryIn<CDegradableInfo>.
- 00867fe0 "CTCBase_GetInterfaceAbility" -> CAnimationEntry::GetComponent<CAnimComponentFollowingOnAnim>.
- 0087c070 -> NCameraModes::COrbitGame::OnInit.
- 009b6eb0 catalog CArray<pair<CCharString,...>> (propagated) -> CArray<CInstantiatedDefInfo*>::DeallocateUnusedMemory.
- 00594a08 "BinarySearchTree_FindAndConstruct_DateAndTime" -> _STL::map<CDateAndTime, CWideString>::operator[].
- 007e4530 -> CTCWillResponse::IsThingLightningable.
- 00721a50 "Vector_InsertRange_CCompositeBlendDesc" -> CVectorMap<long, CTCHeroMorph::CTextureLayer, ..>::Add.
- 0070a490 "CRenderTarget_EnqueueDecalUpdate" -> CTCTextureDecal::AddDecal.
- 00a385a0 "EnumValue_InsertIntoMap" -> CCRCSymbolMap::AddSymbolDirty.
- 006384b0 "CThingMapManager_GetOverlappingThings" -> NThingMap::CThingMapManager::GetAllActiveThingMapsInArea.
- 00a56aa0 "CPSCSplineBase_Initialize" -> CCardinalSpline::CCardinalSpline(const C3DVector&, const C3DVector&, float, bool).
- 0076d1b0 "CCreature_AddEnemyToList" -> CTCEnemy::AddEnemyThing.
- 006ce730 "CTCInterface_InsertPropertyToVector" -> (anonymous namespace)::FrameUpdateThing::operator().
- 0059afd7 "ProcessItemStats" -> _STL::__copy over deque<NUISystem::CComponent*> iterators.
- 00749620 "CTargetPoint_Vector_PushBack" -> vector<pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> >::push_back.

- 00997f80 -> CAFile::GetCurrentPath; 00a57730 -> C2DLineF::GetDist; 00a441e0 -> GFGetSignedAngleDifference; 00a442e0 ->
  GFGetDistanceSquaredFromPointToLine; 0080c190 -> (anonymous)::GetExplosionEffectInterpolate; 00670470 ->
  CTCCoopSpirit::UpdateHeightAboveGround; 00a2d810 -> map<ulong, CCountedPointer<C3DAnimation2>>::operator[]; 00875a30 ->
  NCameraModes::CTools::Get2DRotationAngle; 005e1210 -> _STL::__adjust_heap; 00754d80 -> _STL::for_each.
- 00938db0 / 009438f0 placeholder NAME -> CAddFreeSeatToList / CAddOccupiedSeatToList::operator(); 007ea1a0 "RBTree_LowerBound"
  -> _STL::map<CThing*, float>::operator[].
- 00a557e0 "BinaryTreeTraversal_Insert" -> _STL::copy(set<CCharString>::const_iterator x2, back_insert_iterator<vector>).
- 00729960 "ExpressionSet_InsertNode" -> _STL::list<CTCCreatureExpression::CExpressionState>::push_front.
- 004bcf50 "Appearance_OnAppearanceDrawHelper" -> NGraphicAppearance::CTCGraphicAppearance::GetPMeshEffect.
- 006fc3e0 "Vector_Erase_WithMove" -> _STL::__linear_insert<SKeyedEvents*, SKeyedEvents, CKeyPairCompareLess<..>>.
- 006d7ec0 "CViewBase_LogMemoryEntry" -> CTCHeroExperience::AddAttackOfTypeToAttackHistory (private).
- 006e5990 "CScriptConversation_RemoveAndCleanup" -> RemoveConversation over map<long, CScriptConversation>.
- 0066bdf0 "CParticleMorphs_LoadAndProcessMorphs" -> CTCSoundPlayer::PlayUnattachedCriteria.
- 004dfcea "Partition_Step" -> _STL::__unguarded_partition<pair<CCharString, CTCBase::CRegisteredTCInfo>*, ..>.
- 006e9f60: catalog name right; it is in the combat-manager TU's anonymous namespace (`?A0x..`).
- 005305a0 "CSprite::vfunc_39" -> NUISystem::CComponent::ChangeColourDelta (inherited slot 39).
- 009e2240 catalog "GFABoxToVBox" (propagated) -> GFACoordToVCoord(const C2DCoordI&, C2DCoordI&).
- 00978b90 / 0097af70 Dinkumware "_Adjust_heap" -> _STL::__adjust_heap; 00926690 "_Push_heap" -> _STL::__push_heap; 008365f0 ->
  _STL::for_each; 007fa160 "_Push_heap<..>" -> _STL::__push_heap.
- 0099bb80 -> CWideString::SprintfDynamic; 009b99c0 -> CStringParser::ReadNextSymbol; 00a3f340 -> CSoundManagerLH::vfunc_41
  (descriptive); 00864ff0 "CThingPhysical_RenderWeaponTrails" -> CTCCreatureHitNotification::DrawHitNotification.
- 0041cb10 "vfunc_1" -> NUISystem::CYesNo::ProcessEvent; 00c69a70 "NScript::CQ_GuildTrainingSkillScript::AllocateMemory"
  (propagated) -> a reverb processor's AllocateMemory (audio DSP 0x00C6xxxx, descriptive class); 005148d0 "CEngineWorldMap_
  MoveConstructor" -> CReplaceSerialise<COldFileFormatMapHeader2, COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp;
  00789ce0 -> (anon)::GetDamageMultiplierFromThingAtBuildUpTime; 007881ad "CTCCombatBase_UpdateEngageable" ->
  CTCTrapBase::UpdateTriggerThings; 00924880 -> CCreatureAction_KrakenLowPoseInto::FinishAction; 00954220 "AIState_
  CheckCanReach" -> CAIStateGroup_MazeRangedCombat::IsStillRunnable; 0072aaf0 "UseSpecialAbility_CTCMultiStrike" ->
  CTCSpecialAbilities::SetupMultiStrike; 00814c70 "GetDummyObjectOrigin" -> NHitLocations::CPhysicalPrimitiveSphere::
  GetSphereBase; 008f99a0 "LSocket_ReceiveWithTimeout" -> LHNetworkLib::LSocket::Receive(vector<char>&); 004c10e0 "CTCGraphic
  Appearance_UpdateHelperPointOffset" -> CTCGraphicAppearance::GetStaticFocalPos.

## Already-landed sources with issues
- rebuild/src/compiled/00/9e/Global_GFAXToVX_009e1da0.cpp uses `volatile` on g_FableVirtualWidth_013961E8 (FIDELITY NOTE).
  FIXED here: agent902/fixes/009e1da0.cpp grades RELOCATION_MATCH with no `volatile` -- the product spelled
  `(float)g_VirtualWidth * x / display->GetRenderTargetDimensions().X` (the ScaleToVirtual-helper form matches too).
- Several landed CVectorMap::Add / list siblings (0x006FCF40 CVectorMap_AnimEvents, 0x00759570) use invented class names
  (`CVectorMap_AnimEvents`, `CRetailVector`) instead of the template spelling; their symbols do not match the PDB names.
- rebuild/src/compiled/00/59/*Deque* (0x0059AAF9, 0x0059A8F3) model STLport deque iterators in namespace `std`, one
  template parameter; STLport 4.6 has `_STL::_Deque_iterator<T, _Traits>` over `_Deque_iterator_base<T>` (_M_increment /
  _M_subtract are base members, which is why one body serves const and non-const iterators).
- Landed CRetailTreeIterator_Increment_004c2560 and CRetailTreelongkey_find_006e6990 use invented classes; they are STLport
  `_Rb_tree_iterator<..>::operator++` and `_Rb_tree<long, pair<const long, CScriptConversation>, ..>::find` (0x004BCF50 /
  0x006E5990 declare them with the real names).
- Landed 0x0066BD00 CTCSoundPlayer::PlayAttachedCriteria uses a flat CCharStringVector plus a dead-store hack; the 0x0066BDF0
  recipe (same-TU GetPotentialCriteriaStrings filler over the real `_STL::vector<CCharString>`) is the likely clean form.
- Landed 0x007293A0 calls push_front on a model list type; the real symbol is `_STL::list<CTCCreatureExpression::
  CExpressionState>::push_front` (0x00729960 here).
- Landed 0x0051B940 (v7 -> v8 ConvertPreviousUnknownVersionUp) and 0x00835C80 use different CCPPointerInfo spellings (mem-init
  ctor vs a body-assigned Reset); both match their rows, so the counted-pointer header is not settled.
- parked 00756c60 models the tree with invented class names (see its STATUS line).

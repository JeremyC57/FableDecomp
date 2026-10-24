# Hand chunk 387 (_global .. _global): 30 of 40 byte-exact

All 40 rows are catalog `_global` rows; their real owners are listed under Label corrections. The 30 sources in src/
grade MATCH (2) or RELOCATION_MATCH (28) under gall.sh, and every test in test/ prints PASS under tall.sh (g++) and
tall.py (VC7.1). Two families make up most of the chunk: CTCHeroExperience combo-multiplier code (0x6D4xxx-0x6DBxxx)
and STLport container internals (the opinion-deed map, the texture-morph vectors and the steering-agent vector).

## What matched (30)
- **CTCHeroOpinionDeedLog / deed map (0x6C59E0, 0x6C8350, 0x6C8D40)**
  - RemoveDeed is `find; if (it != end()) { delete it->second; erase(it); }`. It only matches with the same-TU
    `find` body under `auto_inline(off)`, which keeps the node in edi.
  - The `_Rb_tree` equal_range needs the STLport iterator: a base holding `_M_node` plus a user copy ctor.
    Otherwise pair.second is stored first.
  - CThingFilter_IsDeedTypeEnabled::operator() uses `__forceinline` GetInterface (out parameter). The opinion
    pointer is left uninitialised, as in retail.
- **Pointer vector of CThingCreatureBase (0x6CE230 reserve, 0x6CE2B0 `_M_allocate_and_copy`, 0x6CE530 push_back)**
  - These are STLport 4.x verbatim. `_M_allocate_and_copy` is a member template (`??$`).
  - The push_back `__true_type` tag has no user ctor, which gives the retail zero byte store.
- **vector<pair<CCharString,long>> (0x6D4680 reserve, 0x6D4B40 GFSerialiseVectorBinaryIn)**
  - The `__false_type` tag needs a user ctor.
  - The copy loop needs its body in the TU (noinline) so the tag is homed over the live `n` slot.
  - The serialiser's pair traits build DefaultVal from the element traits' defaults.
- **Texture-morph vectors (0x6CB010, 0x6CB170, 0x6CB390)**
  - 0x6CB010 is vector<CCompositeBlendDesc>::push_back, with `_M_insert_overflow` inlined: `(max)(old_size, 1)` by
    reference over two frame words, then the null-tested free and `_M_set`.
  - 0x6CB390 is vector<CEngineTextureMorph>::_M_insert_overflow.
  - 0x6CB170 is `__uninitialized_fill_n<CEngineTextureMorph*>`, with the STLport vector copy ctor inlined.
  - All three are STLport verbatim. Every fresh `__false_type()` tag (with a user ctor) homes over a live or dead
    argument slot once the out-of-line copy and fill loops have same-TU noinline bodies. With bare declarations
    the TU gets an 8-byte frame (first try on 0x6CB390).
- **CTCFollowed (0x6D4E10 ctor, 0x6D5050 RemoveEmoteIcon)**
  - The list ctor follows the landed std::list model.
  - In RemoveEmoteIcon, the current icon must be a named local read before the GetDefinitions call.
- **Brain / creature helpers (0x6D5AF0 IsFacingCorrectDirectionForMovementToPos, 0x6D6210 CCountedPointer<CBrainOutput>::DeleteData)**
  - `m_kind != 7 ? true : false` gives `setne al`.
  - The anonymous-namespace helper is kept alive by a file-scope pointer.
  - DeleteData is `delete static_cast<CBrainOutput*>(data)` with the implicit dtor inlined.
- **CTCHeroExperience (0x6D7430, 0x6D75C0, 0x6D7640, 0x6D80E0, 0x6D86D0, 0x6D8D00)**
  - GetNextExperienceFallbackMultiplier is `find_if + bind2nd(greater<long>())` followed by
    `if (it != end()) return *it; return back();`. The `__find_if` body sits above it under `auto_inline(off)`.
  - The (anon) GetSoundCriteriaStringFromMap needs a forwarding inline `CDefString::GetString()` for the temporary's
    re-lea.
  - In the (anon) PlayCombatMultiplierSound, the GetInterface key and the CCharString are sibling scopes, so they
    share one frame word.
  - SetAsHit gets the clamp folded into `> 1` through a named multiplier local.
  - GetOnHitSoundIntensity uses STLport `rbegin()->first`. The same-TU `operator--` body keeps PDef in edi across
    the call.
  - AddSuccessfulStrikeWithHitParams needs `#pragma pack(1)` on CHitParameters, with DamageMap at +0x30.
- **CPersistTraits TransferBinaryOut (0x6D9EE0 CAttackHistoryCombo, 0x6DB060 CComboMultiplierData, 0x6DB0F0 CHeroStatIncreaseData)**
  - The count comes first, then an iterator walk with end() re-read every pass and the STLport
    `_M_node = _M_increment(_M_node)` step.
- **STLport algorithms and containers (0x6DADD0, 0x6DB340, 0x6DB9D0, 0x6DBC00, 0x6DBC50, 0x6DBDC0)**
  - 0x6DADD0 is `__find_if` (4-way unrolled, switch fall-through), `__fastcall` and explicitly instantiated.
  - 0x6DB340 and 0x6DB9D0 are the list `_M_fill_insert` and `resize`. In resize, end() is passed by value with a user
    copy ctor.
  - 0x6DBC00 and 0x6DBC50 are {long, `_Rb_tree`} default ctors. `head(0)` comes before the malloc.
  - 0x6DBDC0 is `__copy_backward` with the implicit operator=.

## Parked for Fable 5.1 (10)
- 006c9b70 std::__copy<_Rb_tree_iterator<...>, back_insert_iterator<vector<pair<long,COpinionDeed*> > > > DIFFER(190v187): retail `cmp eax,[edi+8]` with both pair halves in ecx/edx; ours loads end_of_storage into edx first and spills/re-reads the pair's second half (`mov ecx,[esp+14h]`) | tried: plain pair, STLport pair copy ctor, `_Rb_global` increment step, `_STLP_alloc_proxy` end_of_storage | next: a back_insert_iterator::operator= / pair-conversion spelling that builds the temporary before the capacity test
- 006c9e40 GFSerialiseVectorBinaryIn<pair<long,COpinionDeed*> > DIFFER(156v156): retail `sub esp,8` with the element traits `this` sharing the spilled vector-reference word (`lea ecx,[esp+1Ch]` twice); ours `sub esp,0Ch` with the traits in their own word | tried: value-initialised traits with DefaultVal (172), empty user ctor, ctor-less non-aggregate traits, named uninitialised traits locals | next: lane gap: VC7.1 slot sharing for empty never-written objects
- 006ca8c0 CTextureMorphs::AddMorph DIFFER(128v138): in the in-place arm retail stores MorphID from edi (zero) and omits the temporary's blends free, with its own `++finish; return finish-1` tail; ours reloads `[esp+0Ch]` and merges both arms into one dtor + back() tail | tried: unnamed temporary, const named local, visible-but-not-inlined vector copy ctor, the PDB (long, long) ctor | next: find what lets VC7.1 treat the temporary as unmodified by the out-of-line vector copy (a const-qualified member/base, or a different temporary type)
- 006ce730 FrameUpdateThing::operator() DIFFER(196v193): retail has no frame; the converted `const CSteeringAgent*`, the lookup key and the empty tag all share the dead `thing` slot. Ours opens `push ecx` for the agent value and loads the component chain into ecx/edx | tried: direct member argument (199), named local per arm plus user-ctor tag, same-TU LowerBound body, explicit static_cast temporaries (all 196) | next: lane gap: temporary slot sharing (try an inline helper taking the agent by value)
- 006d63f0 CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete DIFFER(64v68): retail `fld [def+64h] ; fdivr 1.0f` with the clamp merged into one `fxch ; fmul st(1)` tail; ours `fld 1.0f ; fdiv [def+64h]` and `fmul 0.0f` on two tails | tried: reciprocal local, named perUnit with `?:`, two-division form, STLport max (76) | next: a by-value float accessor feeding the division
- 006d6fa0 CTCHeroExperience::GetTrainableStatPossibleLevelUpAmount DIFFER(243v220): retail re-reads `TrainableStatLevels[stat]` each pass with `lea eax,[eax+eax*8]`; ours hoists the level into ebp and strength-reduces the index (ebx = (level+1)*72) | tried: `for(;;)`, `while`, by-reference fraction helper, `optimize("g", off)` (249) | next: find the aliasing store (address-taken local or by-ref helper inside the loop) that blocks the hoist
- 006d73d0 CTCHeroExperience::GetExperienceFallbackMultiplierForMultiplier DIFFER(84v82): retail homes the float, the __find_if result and the tag all in the dead `multiplier` slot; ours adds a `push ecx` frame word for the result | tried: user-ctor tag, iterator_traits tag hierarchy, named result, named predicate | next: lane gap: temporary slot sharing
- 006d7ec0 CTCHeroExperience::AddAttackOfTypeToAttackHistory DIFFER(91v89): ours loads `type` into edi and keeps the frame in ebx (extra push/pop ebx); retail keeps the frame in edi and reads `type` from its slot (`mov edx,[esp+0Ch]`) | tried: pair temporary, make_pair, named frame local, by-value record ctor | next: an inline record helper taking `type` by value after the frame call
- 006d8790 CTCHeroExperience::AddSuccessfulStrikeOfType DIFFER(124v124): retail keeps the found node in ebx and the thing in eax/cl after the timer store; ours reuses edi for the node and passes the find arguments in edx/eax | tried: early return, assigned iterator (122), full STLport iterator/tree/map layering (125) | next: the landed sibling AddSuccessfulStrikeWithHitParams' iterator model with a same-TU find body
- 006d8bf0 CTCHeroExperience::GetOnHitSound DIFFER(150v172): retail keeps PDef in esi and re-reads the map header/leftmost after the loop, sharing the GetString copy/destroy tail; ours CSEs begin() into edi and duplicates the tail | tried: two named locals (201), inline GetString direct (120), GetString with named local (150), STLport base iterator (150) | next: `CCharString sound = it->second.GetString(); return sound;` with the forwarding inline (the 0x6D75C0 spelling) plus a default that blocks the begin() CSE

## Skipped
- None. Every row is one function. Lengths were checked against the code end. The catalog lengths include the int3
  padding, and no row is a noreturn mis-cut.

## Label corrections
| Address | Catalog label | Real identity |
|---|---|---|
| 0x6C59E0 | AIState_DestroyWithPointerCleanup | CTCHeroOpinionDeedLog::RemoveDeed |
| 0x6C8350 | BinaryTree_FindInsertionPoint | `_Rb_tree<long, pair<const long, COpinionDeed*> >::equal_range` |
| 0x6C8D40 | CTCInterface_GetProperty | CThingFilter_IsDeedTypeEnabled::operator() |
| 0x6C9B70 | BinaryTree_InsertPairs | STLport `__copy(_Rb_tree_iterator, back_insert_iterator<vector<pair<long,COpinionDeed*>>>)` |
| 0x6C9E40 | GFSerialiseVectorBinaryIn<CDefString> | GFSerialiseVectorBinaryIn<pair<long, COpinionDeed*> > |
| 0x6CA8C0 | Map_InsertEntry | AddMorph on vector<CEngineTextureMorph> (descriptive name from the landed caller 0x6CA950): `push_back(CEngineTextureMorph(i, 0)); return &back();` |
| 0x6CB010 | CCompositeBlendDesc_VectorGrow | vector<CCompositeBlendDesc>::push_back |
| 0x6CB170 | Std_UninitCopyArray | STLport `__uninitialized_fill_n<CEngineTextureMorph*, unsigned, CEngineTextureMorph>` |
| 0x6CB390 | Array_Insert_range | vector<CEngineTextureMorph>::_M_insert_overflow |
| 0x6CE230 | CActiveFile_Reserve | vector<const CThingCreatureBase*>::reserve |
| 0x6CE2B0 | CActiveFile_AllocStringBuffer | vector<const CThingCreatureBase*>::_M_allocate_and_copy |
| 0x6CE530 | CThingFilter_AppendMatch | vector<const CThingCreatureBase*>::push_back |
| 0x6CE730 | CTCInterface_InsertPropertyToVector | (anonymous namespace)::FrameUpdateThing::operator()(const CThing&) const (Ego_r.pdb); TC 7 = CTCCreatureNavigation, TC 0xFE = CTCObstacle |
| 0x6D4680 | CCombatWheel_ReserveRings | vector<pair<CCharString,long> >::reserve |
| 0x6D4B40 | GFSerialiseVectorBinaryIn<CCharString> | GFSerialiseVectorBinaryIn<pair<CCharString,long> > |
| 0x6D4E10 | CEngineSubPrimitive2DClockSprite_Constructor | CTCFollowed::CTCFollowed |
| 0x6D5050 | GUI_SetEmoteIcon | CTCFollowed::RemoveEmoteIcon |
| 0x6D6210 | DeleteData | CCountedPointer<CBrainOutput>::DeleteData |
| 0x6D63F0 | Math_FracPart | CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete |
| 0x6D6FA0 | Experience_GetLevelAndFraction | CTCHeroExperience::GetTrainableStatPossibleLevelUpAmount |
| 0x6D73D0 | CEngine_GetExperienceMultiplier | CTCHeroExperience::GetExperienceFallbackMultiplierForMultiplier |
| 0x6D7430 | Combat_GetNextFlourishLevelAsFloat | CTCHeroExperience::GetNextExperienceFallbackMultiplier |
| 0x6D7EC0 | CViewBase_LogMemoryEntry | CTCHeroExperience::AddAttackOfTypeToAttackHistory |
| 0x6D80E0 | CTCHeroExperience_OnComboHit | CTCHeroExperience::SetAsHit (caller CThingGameObject::OnHit) |
| 0x6D86D0 | Flourish_UpdateMultiplier | CTCHeroExperience::GetOnHitSoundIntensity |
| 0x6D8790 | CTCHeroExperience_ApplyDamageMultiplier | CTCHeroExperience::AddSuccessfulStrikeOfType |
| 0x6D8BF0 | CTCHeroExperience_GetFlourishName | CTCHeroExperience::GetOnHitSound |
| 0x6D8D00 | Experience_ProcessHitCombo | CTCHeroExperience::AddSuccessfulStrikeWithHitParams |
| 0x6D9EE0 | GFSerialiseVectorBinaryOut_WritePair | CPersistTraits<CAttackHistoryCombo>::TransferBinaryOut |
| 0x6DADD0 | std_lower_bound_int | STLport `__find_if<const long*, binder2nd<greater<long> > >` |
| 0x6DB060 | TreeNode_SerializeRecursive | CPersistTraits<CComboMultiplierData>::TransferBinaryOut |
| 0x6DB0F0 | TreeNode_SerializeRecursive_v2 | CPersistTraits<CHeroStatIncreaseData>::TransferBinaryOut |
| 0x6DB340 | LinkedList_InsertElements | list<pair<EDamageAttribute,long> >::_M_fill_insert |
| 0x6DB9D0 | LinkedList_Trim | list<pair<EDamageAttribute,long> >::resize |
| 0x6DBC00, 0x6DBC50 | StdList_CtorHeadAtField1_* | {long, _Rb_tree} pair default ctors |
| 0x6DBDC0 | `_Copy_backward<CLipSyncPhoneme*>` | STLport `__copy_backward<CAttackHistoryCombo*>` |

Neighbours (not chunk rows):

| Address | Catalog label / landed name | Real identity |
|---|---|---|
| 0x6CB0D0 | "std::vector::_Uninit_copy_arrays" | `__uninitialized_copy<CEngineTextureMorph*>` |
| 0x6CB470 | "vector<pair<int,CDefPtr>>::emplace_back" | the out-of-line vector<CEngineTextureMorph>::push_back |
| 0x5BB346 | "std::vector::operator=" | vector<CCompositeBlendDesc>'s copy ctor |
| 0x5BAF28 | "_Uninit_copy<CArray<CRandomAppearanceMorph::CBodyPartMesh>*...>" | `__uninitialized_copy<const CCompositeBlendDesc*, CCompositeBlendDesc*>` |
| 0x6CD680 | "Vector_InsertElements" | vector<const CSteeringAgent*>::_M_insert_overflow |
| 0x6CB340 | landed as CRetailVector_006cb340::Destroy | vector<CEngineTextureMorph>::_M_clear |
| 0x6DABE0 (landed) | — | STLport `__find_if` with CPredIsFallbackMultiplierForMultiplier |
| 0x6C7370 | — | CTCHeroOpinionDeedLog::OnSerialise |
| 0x66BD00 | — | CTCSoundPlayer::PlayAttachedCriteria |

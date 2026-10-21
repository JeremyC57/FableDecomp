# Hand chunk 249 (CQ_TraderConflictGoodScript .. CStopDrawing): 29 of 40 byte-exact

All 29 grade RELOCATION_MATCH with RTM cl 13.10.3077 (no QFE row; no /GS, /EHsc or jump tables; size-optimised TUs for 0046a568 and 006463ff). Every test prints `<addr>_TEST PASS` under g++ (tall.sh) and VC (tall.py).

## What matched
- 0046a568 CSpecialAbilitiesLightningSpellDef::Transfer: size TU; four Transfer<T> overloads in sounds/angles/vectors order.
- 00a88de0 CQuaternion::SetFromRotationMatrix: Shoemake; the else-arm `s` is a double.
- 006bbfd0 CRegion::SaveBinary: retail 0x58-byte CRegion local model; inline STLport _M_increment walk.
- 00a87580 CSBGoal::Process: engine/CSBGoal.h; the position local lives in an inline helper so it shares a frame slot.
- 00851ed0 CScriptModuleTransitionBetweenPoints::SetUpTransitionCamera: inline operator/(float) via reciprocal.
- 0054ba20 CSlider::Update: the product is split into a helper statement, which stops reassociation.
- 0053ebf0 CScrollingViewport::ChangeState: plain switch; CObserver conversion null test.
- 00555220 CScrollableList::Update: reference-returning min.
- 009ddff0 / 009de140 / 009de2a0 CRenderManager2D: Draw2DTriangle (vertex array), Draw2DBoxOutline, DrawArc.
- 00e63730 CSpectator::Init, 00eed8b0 CStopDrawing::Main, 00da5f40 CRockTroll::Main: script models.
- 0054b360 CSlider::~CSlider: epsilon `!(a == b)` with `<=`.
- 0041a980 CSprite::DrawWithRenderManager: fieldwise box; system manager fetched into a local.
- 00bdebf0 CQuadTreeElement::Update.
- 004b3260 / 004b35e0 CQuestManager::Uninitialize{Day,Night}Section: SameText nested inside CCharString operator==.
- 004b6380 CQuestManager::LoadGameState: named binary-mode bool.
- 004b1ba0 CQuestManager::AddNewObjective:
  - the text bank comes from an inline getter;
  - `GetTextInfo && GetPGroupEntry().Data` keeps a conditional counted-pointer temporary;
  - list push_back is STLport insert(end()).
- 004ca230 CThing::CreateTCsFromString (catalog says CScriptBase::LoadGameState) and 00cb8690 CScriptBase::LoadGameState: parser helpers with named keyword locals.
- 009041a0 / 00904cd0 / 00904540 CScriptGameResourceObjectScriptedThing:
  - StopFollowingThing, MoveToAndPickUpGenericBox, WaitForEntityToFinishPerformingTasks.
  - In 00904540, VC inlines the first CScriptThing temporary's dtor and calls the second one out of line by itself.
- 006463ff CQuestInfoElementBar::Update: size TU; raw IS_THING_ALIVE stand-in.
- 00f3a710 CRangeCompressor::TestRedundantBitStrip: explicit in/out bit masks; the scan bools' declaration order sets the dl/bl split.
- 0098a9a0 CShaderRenderManager::CreatePixelShader:
  - The coordinator's idiom: a same-TU noinline body for the growth insert 0x00440990 lets VC home push_back's empty tag over the live CCharString.
  - CPixelShader is released through the ReleaseTemporary form.

## Parked for Fable 5.1
- 00dfc920 CQ_TraderConflictGoodScript::WatchForRegionTransitions DIFFER(354v354): the by-value CScriptThing argument copy reads the element as `[edi+ecx+4]` with the destination in eax; ours forms the element pointer (`add eax,edi`) with the destination in ecx | tried: `_M_start[i]`, PImp member copy, copy-ctor forms | next: a same-TU stand-in for the copy, or an iterator-offset loop.
- 00aa39a0 CQuaternion::ToMatrix DIFFER(332v332): our scheduler hoists the int zero and 1.0f stores of the row ends and translation | tried: CQuat4 norm temporary (fixed the load order), store orders, row helpers | next: a row-at-a-time inline setter, or a noinline helper call between the rows.
- 004afe20 CQuestManager::RemoveBoast DIFFER(356v345): register allocation (retail keeps this in edi and the group in the dead id slot); our inline strcmp gives sete | tried: helper, id reuse, operator stand-ins | next: noinline stand-ins for the list/erase callees.
- 004b4260 CQuestManager::ActivateMultipleQuests DIFFER(336v359): register allocation.
  - Retail: i is frame-only, activated is in bl, this is in edi (spilled), esi is re-zeroed per pass.
  - Ours: i is in edi, activated is in the frame.
  - | tried: declaration orders, continue form, per-use indexing, _Destroy helper, push_back noinline body | next: a different loop shape (pointer walk with a separate counter), or a by-value CActivateQuestInfo ctor.
- 0086b9d0 CRandomAppearanceMorph::GetRandomSkeletalMorph DIFFER(389v389): register allocation (this in ebp, the map in ebx, `end` spilled) | tried: reference local, class iterators, bounds stand-ins | next: noinline bodies for 0x0086A930/0x0086BC10.
- 00becde0 CRandomAppearanceMorph::UpdateMorphArray DIFFER(340v346): retail loads the entry key once (a in ebx, b in eax plus a frame copy; frame 0x28) and walks the entries by byte offset; ours reloads each key field per store (frame 0x24) | tried:
  - forceinline Add,
  - explicit copy ctors,
  - range ctor for the temporary,
  - noinline _M_insert_overflow / uninitialized_copy bodies (the tag now homes in the arg slot)
  - | next: a pair<const K, V> shape with the key taken from a key local, or a different key type.
- 0066c900 CRegionMinimap::IlluminateRegionMinimapTheme DIFFER(348v354): pop-esi placement and the x87 liveness of light.Z | tried: copy order, dot forms, += chains | next: an inline Dot taking by value.
- 009dc6e0 CRenderManager2D::Draw2DTexture DIFFER(384v388): zero-constant register choice and copy-chain scheduling | tried: inline Set method, vertex array | next: per-vertex ctor with explicit zero args.
- 00a22e10 CRightHandedSet::MakePerpendicular DIFFER(358v358): scheduling only (`mov esi,0x17C` vs `fmul st(4)`) | tried: Normalise forms, dot chains | next: a different Normalise inline nesting.
- 005590c0 CScrollBarOutside::OnLeftClicked DIFFER(360v365): rough attempt; register allocation and spills are far off | tried: one structural pass | next: model the scrollbar/list vtable views from the landed CScrollBar code.
- 00b20ba0 CSplinePosParamData::Update DIFFER(397v391): frame 0x64 vs 0x58; retail copies C3DVector with integer moves and keeps the transformed X on the x87 stack | tried: user copy ctor, float ctor, SetAt | next: POD C3DVector with a Transform that returns by value into a named result.

## Skipped
- None (every row attempted at least once).

## Label corrections
- 0x004CA230 is CThing::CreateTCsFromString (called from 0x004CAE62 after CThing::AddTC), not CScriptBase::LoadGameState.
- 0x00BECDE0 "CRandomAppearanceMorph::UpdateMorphArray" (forge autoname) is a CVectorMap-style method: `this` is a {vector, comparator, Dirty} map at CEngineInternalPrimitiveMorphedAnimatedMesh +0x128.
- Callee labels:
  - 0x009B9BA0 "SerialiseGameState" = CStringParser::NextItemIs
  - 0x0049D870 = GFGetCurrentFrame
  - 0x00593666 = GetDef<T>
  - 0x0052EA30 = ChangePositionDelta (CSlider slot 32)
  - 0x0086B900 = CreateRandomMorphEntry
  - 0x0099EAE0 "_Cons_val<...>" = ~CCharString
  - 0x00CB5AD0 "QuestPersist_FindInfoByName" = CScriptInfoManager::GetScriptInfo (this+0x78)
  - 0x004C7E50 "GFPredicateAnd" = CThing::GetScriptThing
  - 0x004AA840 "~C3DClothPrimitive" = ~CScriptThing (folded)
  - 0x005BB346 "std::vector::operator=" = a vector copy ctor

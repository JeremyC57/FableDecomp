# Hand chunk 228 (CRandomAppearanceMorph .. CSplinePosParamData): 24 of 40 byte-exact

## What matched (24)
- CAudioContext (catalog: CScreenEffectPriorityStack) -- 00c55b70 `Reset(bool)` (MATCH): clears the context's entry-point members one by one. It matched once the separate entry-point members were modelled; the catalog label is propagated.
- CSpecialAbilitiesBerserkDef / CSpecialAbilitiesLightningSpellDef -- 004572ce and 00457b8b, the def ctors. Both need `#pragma optimize("s", on)` and the STLport vector model (`_Vector_base(const allocator&)` plus an `_STLP_alloc_proxy` end pointer, 12-byte retail vectors). LightningSpell uses `lea` only where it is shorter.
- CSpecialAbilitiesEnflameDef -- 00465258 `Transfer(CPersistContext&)`: explicit per-element Transfer calls under optimize("s"). Leaf is the mangled `?Transfer@CSpecialAbilitiesEnflameDef`.
- CReactionMatch_MRPyramid -- 006ff810 `TransferBinaryIn`: the sphere is read with three inline Read calls into one float local.
- CReactionFrequencyTraits_PeriodicFrequency -- 006ffb10 `CalculateWeight`: twin of ControlledCount. The cube is written in statement order (`f = t*t; f *= t`).
- CSplinePosParamData -- 00b21c90 `ReadBinary`: twin of the landed ReadFromFile 0xAC5080, plus an inline ReadBool.
- CShaderRenderManager -- 00989540 `UpdateObjectSpaceFogTransform` (MATCH): three-term dot products written `x + y + z`. VC loads the right inner operand first.
- NUISystem::CSlider -- 0054a5d0 `Initialise`: inline counted-pointer dtor that re-reads the block from its frame word, and plain nested virtual calls.
- NUISystem::CRedefinerList:
  - 00556810 `UpdateUndefinedWarning(float)`: the child scan re-reads each element for its null test; the C2DVector is passed by value in the argument area.
  - 005566a0 `Initialise`: a null-tested CDefPointer released through the def's ReleaseTemporary on its ref base (+4), with a counted-pointer local. The def's Destroy is virtual slot 1.
- CScriptGameResourceObjectScriptedThing:
  - 00903ac0 dtor (Leaf `??1`): action ctors used inside `new` must be `__forceinline`; the intermediate dtor is implicit and the PImp release is inline.
  - 00903e20 `MoveToThing`: the HasTC bool is bound to a `const unsigned char&` inside GetInterface, which gives retail's frame-byte spill; the class is a local polymorphic model.
  - 00904430 `Wait(float)`: two complete `AddAction(new CActionWait(frames))` arms with `__forceinline` action ctors.
- NQuestManager::CRuntimeQuestPersistence -- 004b4e30 `SetQuestScriptPersistScript` (label fix from SetQuestMasterDataPersistScript): inline Find/Add/SortIfDirty and a CPersistInfo local.
- CScriptConversation -- 006e5690 `OnCleanup(bool)`: an IsValid loop; the out-param GetInterface result is in eax and the HasTC/ternary GetTC in ecx.
- CScriptConversationManager -- 006e61a0 `MakeNewConversation`: `insert(make_pair(index, conversation))` with an inline ctor. The same-TU implicit copy ctor is inlined at its first use and out of line at its second.
- CScreenMessage -- 004380a0 `GetText`: an inline BeginsWith on the converted temporary (one null test, `? false : true`) with a block-scoped prefix.
- CString (catalog: CShaderResource::Assign) -- 00c0dab0, the library `CString::CString(const CString&)`. Leaf `??0CString@@QAE@ABV0@@Z`; uses the LIB_ASSERT GetData model of 0xC0BEE0.
- CShaderResource -- 00a5d620 `GetPixelShader`: returns a CPixelShader by value through inline copy ctor/dtor; the data is released on its ref base (+8) with `delete this`.
- NUISystem::CSpellContainerList -- 00544c10 dtor: an inline `CCountedPointer<CAction>(new CAction)` chain, all `__forceinline`, plus a vector free and the base dtor.
- NCameraModes::CScriptModuleFullTransition -- 008525f0 ctor (Leaf `??0CScriptModuleFullTransition`): `operator/(float)` goes through a reciprocal that is CSE'd across both increments; the copies are implicit `operator=`.
- NCameraModes::CScriptedControl -- 0087bbf0 `OnDie`: a distance float local preset to 1.0f, C3DVector value operators plus implicit `operator=` (the `lea` copies), and an inline CutCamera.
- CSoundEmitterLH -- 00aac1d0 `vfunc_5(long)`: the success path is nested so the single `return -1` shares the failure tail; an unnamed counted-pointer temporary goes to push_back; CActiveSound is modelled at its real size.

## Parked for Fable 5.1 (15)
- 009fc250 CResourceList::ProcessLoadingQueue DIFFER(260v260): register rotation in the CanAbortResourceLoad argument (retail loads LastUsedFrame into ecx before CurrentFrame into eax) and in the inlined ListRemove relink | tried: operand order of the subtraction, named frame locals, inline ListRemove | next: lane gap: register allocation
- 00903f30 CScriptGameResourceObjectScriptedThing::FollowPreCalculatedRoute DIFFER(254v254): routeThing/movement type swapped between ebx and ebp | tried: const bool& HasTC spill (reproduced), declaration orders | next: lane gap: register allocation
- 009049f0 CScriptGameResourceObjectScriptedThing::PlayAnimation DIFFER(250v250): `this`/&PThing swapped between ebx and edi | tried: thing pointer local, inline Thing() accessor, a `self` local declared first, PThing as a real member | next: lane gap: register allocation
- 00904bd0 CScriptGameResourceObjectScriptedThing::PlayLoopingAnimation DIFFER(253v253): `this`/&PThing swapped between ebx and edi, same shape as 009049f0 | tried: the 009049f0 forms | next: lane gap: register allocation
- 004b4f30 NQuestManager::CRuntimeQuestPersistence::SetQuestMasterDataPersistScript DIFFER(255v255): the `Dirty = true` store sinks below the comparator load (retail stores it right after the pair dtors) | tried: the +0 twin's source (004b4e30 matches from it), statement order | next: lane gap: scheduling
- 004b01b0 CQuestManager::IsRegionValidInRegionLockingMode DIFFER(281v277): loop 1 loads the iterator start into ebx and copies it to edi (retail: edi, then ebx); loop 2 builds the malloc size with `lea edx,[eax*4]` (retail `shl eax,2`) and rotates eax/ecx/edx | tried: a same-TU `__find` stand-in under auto_inline(off) (fixed the tag slot), named locals | next: hold n in a narrower scope for the copy size
- 00547050 NUISystem::CScrollingComponent::UpdatePosition DIFFER(261v261): the X parent-offset add is `fld PP.X ; fadd RP.X` (retail `fld RP.X ; fadd PP.X`; Y is the other way round) | tried: PP.X+RP.X, RP.X+PP.X, member += with named locals | next: an inline C2DVector operator+ on the whole vector
- 0087bf40 NCameraModes::CRotateAround::OnInit DIFFER(296v295): retail computes both XAxis differences on the FPU before storing (`fxch`, two fstp) and reads XAxis.Y through edi; ours stores X first and re-addresses [esi+0B0h] | tried: a reused C3DVector local (fixed the frame), a temporary per axis | next: a C2DVector XY difference temporary
- 0087ba90 NCameraModes::CScriptedControl::OnInit DIFFER(294v294): the module vtable is loaded into edx before the module stores (retail eax, after the PCameraManager store), which rotates eax/ecx/edx through the rest of the body | tried: direct member stores, a module local, an inline SetOwner | next: lane gap: register allocation
- 007621c0 NShape::CShape::UpdateSelectedPoint DIFFER(243v243): this/iterator are in ebx/ebp (retail ebp/ebx); the dx spill is squared from the FPU (retail `fld [x] ; fmul [x]`); the push_back tag sits in a frame byte (retail: in the dead `position` argument slot) | tried: inline AddUnique + `__find` stand-in (fixed the frame), const C3DVector& param per the PDB | next: a dx local bound by reference
- 007622c0 NShape::CShape::UpdateSplit DIFFER(242v274): the midpoint adds straight from the points (24-byte frame); retail copies b.Y, b.Z and a.Z into frame words (36-byte frame) | tried: member const& operator+, free by-value operator+, named point copies | next: an operator+ taking both C3DVectors by value with a user copy ctor
- 009db700 CRenderManager2D::AddText DIFFER(266v266): the colour is copied through edx->eax with the flags load hoisted (retail eax->ecx, flags loaded last) | tried: member order, colour/scale/font/flags statement order | next: lane gap: scheduling
- 0086be40 CRandomAppearanceMorph::GetRandomBodyParts DIFFER(292v294): the empty test forms the element address with `add eax,ebx` first (retail folds [eax+ebx]/[eax+ebx+4] loads before the add) | tried: !empty(), begin() != end(), `part != 3` (fixed jl to jne) | next: a `const CArray<CBodyPartMesh>*` row pointer stepped by ++
- 00f3a5d0 CRangeCompressor::TestRangeBias DIFFER(318v318): the prologue spill of maxU lands one slot early, and the epilogue pops are hoisted between the Flags test and the RangeBias store | tried: unsigned-first declarations (322v318), comparison orientation, ternary min/max, signed-first copies (318v318) | next: lane gap: scheduling (or store the RangeBias after the flag update)
- 0056eb48 CSliderAnimator::Update DIFFER(305v305): the fabs(step) spill `fstp [ebp-4]` comes before `push ecx; mov ecx,esp; push -1` (retail after) | tried: a named step local (spilled before the test), fabs before the test, the test on the unnamed expression (305v305), `if (!Play)` first | next: lane gap: scheduling

## Skipped
- 00ca53f0 (1 row): it holds two functions, a 13-byte adjustor/vcall thunk (`jmp [ecx+4]`) and a separate body at 0x00CA5400.

## Label corrections
- 0x00C55B70: CAudioContext::Reset(bool), not CScreenEffectPriorityStack::Reset.
- 0x004B4E30: CRuntimeQuestPersistence::SetQuestScriptPersistScript; the catalog row was SetQuestMasterDataPersistScript, which is the +0x100 twin 0x004B4F30.
- 0x004B01B0: CQuestManager::IsRegionValidInRegionLockingMode, not CScriptBase::ProcessQuestRegions. Its caller 0x4A3817 runs it on g_pQuestManager 0x13B89FC.
- 0x00C0DAB0: the library CString::CString(const CString&), not CShaderResource::Assign.
- 0x00557940 (callee): CKeyRedefiner::IsUndefined, not a CTCKraken label.
- 0x00555EB0 (callee): CScrollableList::Initialise, not CChangingStateComponent::Initialize.
- 0x00A61FB0 (callee): a shader-data lookup, not CTCInventoryAbilities::GetAbilityLevel.
- 0x00A0AAA0 (callee): GetViewportBoxFloat, not DoSizeof.
- 0x0084D070 (callee): GetCamera.
- 0x00A3ED30 (callee): the sound bank's FindSample.
- 0x0056E545 (already landed as CRecord_0056e545 / "CDateAndTime"): this is the CSliderAnimator ctor. Its layout {pSlider, StartValue, EndValue, MaxValue, a = 0, Overall, Play = 0, Finished = 0} is exactly the PDB CSliderAnimator.
- 0x00405F00 (callee): IntToWideString(long), __fastcall; the manifest says CGuiControlNumber::SetSLongValue.

# Hand chunk 651 (CGamePlayerInterface .. CTCCreatureOpinionOfHero): 3 of 10 byte-exact

## What matched (3)
- **CTCCreatureOpinionOfHero::TransformOpinionFromHeroStats 0x006817C0 (MATCH, 3 probes)** -- BaseOffset + value * HeroStatScaling
  from the personality def's trait for the opinion; value unchanged with no def. Shape: the real engine/CDefPointer.h
  (`IsNull()` then a fresh `operator->` read: retail tests in eax and re-reads into ecx) with the NON-NULL path as the
  fall-through (`if (!PPersonalityDef().IsNull()) return ...; return value;`). Name proven (NAME_FIXES row): the catalog's
  TransformForcedOpinion is the twin 0x00681790.
- **`anonymous namespace'::IsAnimationStateBaseValid 0x0070BF50 (RELOCATION_MATCH, 16 probes)** -- a state is valid when its
  animation is loaded (plain / alpha) or both ends of a transition are valid (recursive). Shape: three GetType() arms, each
  with a named `const unsigned long index = static_cast<...>(state).AnimationIndex;` local -- the two leaf arms become
  register-identical and VC7.1 cross-jumps them as retail does. An exported `IsAnimationStateBaseValid_Use` wrapper keeps the
  file-local function emitted (leaf `?IsAnimationStateBaseValid@?A`; our TU hash differs from retail's A0x22a65229).
- **CGamePlayerInterface::GetInputProcessEvent 0x00446330 (RELOCATION_MATCH, /GS, 14 probes)** -- scan the queued input
  events, offer each unprocessed one to every input process (CLinkedListScanInfo-registered scan), keep the best-priority
  accepted CProcessedInput, flag an event a process consumes (type 2) as processed, copy the best out and flag its event.
  Shape: the landed 0x00A0D6B0 CLinkedListScanInfo model with the scan helpers re-reading the stored Current
  (`s->Next = s->Current->Next`, kills a `mov ecx,eax` node copy), the list-side RemoveScanInfo (FirstScanInfo stored through
  `this`, so the empty-list path threads into it) ending with the scan resets, a plain for loop (VC7.1 rotates it), and the
  CInputEventScanInfo local declared after bestPriority/found (retail's store order). Headers: CGamePlayerInterface.h
  (InputProcessList accessor over the flattened base at +0x04), CProcessedInput.h, CGameEvent.h, CPlayerManager.h, CWorld.h.

## Parked for Fable 5.1 (7)
- 0x00D259A0 NScript::CQ_DragonBossFightScript::CDragon::OnPredicateFail DIFFER(64v64): `mov eax,[esi+14h]; mov [eax+48h],4` vs retail `mov esi,[esi+14h]; mov [esi+48h],4` | tried: 17 shapes (retail NScript headers, inline/non-inline SetDragonState, reference member, guarded read-before-store reproduces esi but adds bytes) + /Og- + QFE | next: find the source form of a plain store VC7.1 allocates like a read-modify-write
- 0x00E1CAE0 NScript::CQ_WizardBattleScript::MonitorMazePhases DIFFER(564v568): ~CScriptGameResourceObjectScriptedThingBase inlined at 3 exits (retail 7; retail calls 0x007E74D0 only at the two MazeDefeated exits) | tried: 7 shapes + 7 diagnostics + /Ob1 /Ox /Ot | next: the real loop nesting of the script (inlining tracks loop depth), else lane gap: per-site dtor inlining
- 0x007C7C40 CTCTurncoat::FrameUpdate DIFFER(744v744): def pointer in esi where retail uses ebp (`mov ebp,[edi+14h]`, 3 uses) | tried: 17 shapes (int stage count fixes the fxch; `_M_start` pointer local fixes the grow loop; engine CDefPointer) + QFE | next: what gives the def pointer the higher register priority (a second use in the grow loop?)
- 0x00F39730 CSurfaceCompositor::StartComposition DIFFER(794v796): retail `mov ebx,0Fh; cmp ecx,ebx; ... mov [eax+4],ebx` (push ebx, frame +4), ours folds 0xF | tried: 13 shapes (ternary, if-chain, const table, two-level inline, landed 0x00B677D0 member SetRenderState, non-inline mask helper) | next: a retail sibling calling SetColourWriteEnable(true) to see whether the constant always stays in a register
- 0x00918B90 CAIStateGroup_Gaze::Update DIFFER(802v802): retail's normal-path def release has no null test (`dec [ecx+4]`) while the early exit keeps one, so the early Terminate is not cross-jumped; ours merges it (edi/ebx swap follows) | tried: 20 shapes (block-scope bools, in-place jitter scale, end-frame locals, literal 0.0027777f, engine CDefPointer + GetSubDef<CCreatureNavigationDef>, __assume, test-less dtor) | next: a landed two-exit sibling around a GetSubDef def pointer
- 0x0078A690 CTCHeroRangedAttacks::FrameUpdate DIFFER(1097v1097): frame 0x18 v 0x14 (retail homes every key, GetTC out and the spline pressure in one dword) and retail's ebp holds the roots `_M_start` to the final free (ours keeps 0 in ebp) | tried: 12 shapes (pressure via inline helper param, by-value/const-ref roots, start pointer local, STLport-style accessors, sibling-scope out) | next: the PDB GetTC<T>(T**) / PeekTC<const T>(const T**) const accessors
- 0x00490494 CSubtitleRenderer::DrawPreparePrimitivesForEngine DIFFER(1073v1094): retail keeps `drawn` in [ebp-1] (stored after each DrawButtonText, tested before the embedded release) so nothing is jump-threaded and the chain is laid out C, B, D; ours eliminates it | tried: 12 shapes (16-byte layout box, __fastcall GFVXToAX/GFVYToAY, goto-shaped setup-before-test, C2DVector(x,y) pos, const float& time select, chain order, flag placement) + /Og- | next: what keeps `drawn` memory-resident (a by-address use or a release guard)

## Skipped
None.

## Label corrections
- 0x006817C0: catalog/manifest CTCCreatureOpinionOfHero::TransformForcedOpinion -> TransformOpinionFromHeroStats (twin 0x00681790 is TransformForcedOpinion).
- 0x00D259A0: catalog `_gapscan::sub_00d259a0` -> NScript::CQ_DragonBossFightScript::CDragon::OnPredicateFail (vtable 0x012CB770 slot 5; Ego_r/FableWin ?OnPredicateFail@CDragon@CQ_DragonBossFightScript@NScript@@EAEXXZ).
- 0x0070BF50: IsAnimationStateBaseValid is file-local (`anonymous namespace'), PDB hash A0x22a65229.
- 0x00446330 callees (see NAME_FIXES.tsv): 0x00A03BE0 is CInputEvent::SetAsNull (bound as a non-PDB Clear); 0x009F55C0 is
  CInputManager::FlagEventAsProcessed (the prior's RequeueEvent was invented; labels_familypass7 agrees; body appends to the
  processed-event vector); 0x00449990 is CPlayerManager::GetPlayerNumberFromJoystickDeviceNumber (bound as CInputThing::GetDeviceNumber);
  catalog labels `_global::GetMember_dword0x0_00a03b70` (= CInputEvent::GetKey), `_global::SetMember_dword0x0_00a0d3a0`
  (= CProcessedInput::SetPlayer), `_global::GetMember_dword0xac_00a0d3b0` (= CProcessedInput::GetPriority) and
  `CInputEvent::Clear` 0x00A03BE0 (= SetAsNull) are wrong (vland RELOC_NAME warnings).
- 0x007E74D0: manifest says ~CScriptGameResourceObjectMovieBase, reloc_bindings binds ??1CScriptGameResourceObjectScriptedThingBase (used by 0x00E1CAE0; not re-proven here).
- 0x00918B90: the prior's CCreatureNavigationDef::AvoidOtherCreatures (+0x34) is Ego_r AvoidDynamicObstacles; 0x006CD2F0 is
  ??$GetSubDef@VCCreatureNavigationDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCCreatureNavigationDef@@@@@Z called on the creature's base def, not a CTCCreatureNavigation method.
- 0x00490494 callees: 0x009E1E40 / 0x009E1E10 are __fastcall GFVYToAY / GFVXToAX (the prior declared them cdecl ScaleY/ScaleX).

## Names: proven vs descriptive
- Proven (PDB + a second source): every symbol in the three landed sources except those listed next.
- Descriptive (no PDB symbol): 0x00446330's CInputProcessList / StartScanAtHead / ScanForward / RemoveScanInfo (the landed
  0x00A0D6B0 inline model of CLinkedList<CAInputProcess>), the result-type values 1 / 2; 0x0070BF50's
  `IsAnimationStateBaseValid_Use` export wrapper. The differ/ sources keep many descriptive callee names (each says which).

## Header debt
- 0x00446330: CAInputProcess is the landed 0x00A0D6B0 local model (engine/CAInputProcess.h flattens the base and has no
  virtuals); CInputEvent, CInputManager, CInputEventScanInfo, CMainGameComponent are local (no engine headers);
  CGamePlayerInterface.h flattens the CInputProcessManager base, so InputProcessList is a reinterpret accessor at +0x04.
- 0x0070BF50: CMeshDataBank / C3DAnimationInfo / CIVCountedPointeeBase now come from engine headers (Layout-derived for the
  two callees); the C3DAnimationStateBase family is local (no engine headers). The landed sibling 0x0070C770 still carries
  the older local CIVCountedPointeeBase with an invented `Validate` at slot 1 (slot 1 is CIVCountedPointeeBase::Delete).

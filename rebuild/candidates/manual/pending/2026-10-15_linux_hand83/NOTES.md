# Hand chunk 83 (CTCCameraPointScripted .. CTCCarrying): 32 of 40 byte-exact

All 32 matched sources re-graded through gall.sh (grades.txt: 2 MATCH, 30 RELOCATION_MATCH); every test prints
`<addr>_TEST PASS` under g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). Most catalog labels in this chunk are
propagated over unrelated byte-identical bodies; line 1 of each source names the real function and `// Leaf:` picks it.

## What matched (32)
- STL helpers mislabelled CTCCarryable::OnKill:
  - 0040f560 / 009138b0 `PopFrontAux`, 0059ad35 / 00a0e620 `PopBackAux` -- STLport deque `_M_pop_front_aux` /
    `_M_pop_back_aux` (null-tested inline free, `_M_set_node(node +/- 1)`, cursor reset). Shape: the set_node inline
    stores node, first, last = first + 0x80 bytes (the element size only fixes the `add eax,N` constant), then
    `_M_cur = _M_first` re-reads the member / `_M_cur = _M_last - 1` reuses the register. 0059ad35 is a size TU
    (`pop ecx` after free: `#pragma optimize("s", on)`), the others are speed TUs (`add esp,4`).
- _global (labels CTCCameraPointScripted::*):
  - 0076d6f0 / 0076d850 `FindThingInList` -- SGI `find` front end compiled `__fastcall` over a list of
    CIntelligentPointer<const CThing>. Shape: hidden result in ecx, value ref in edx, iterators by value; the 0076d6f0
    iterator has a user copy constructor (each copy built in its slot), the 0076d850 one only a user default ctor; the
    empty tag (user ctor) homed in the dead `last` slot.
  - 00c1bfb0 `SwapYZ` -- `__fastcall` returning `C3DVector(v.X, v.Z, v.Y)` through the three-float constructor
    (arguments right to left: two x87 loads, X through an integer register).
- CTCCameraPointShapeActivationBase / CTCCameraPointTrack:
  - 0075c1e0 GetTrackPosition (slot 35) -- returns ParentThing->GetPosition() by value; the three alternating
    edx/eax dword moves need a MEMBERWISE user copy constructor over dword members.
  - 0075d8f0 IsLeafCriteriaMet (slot 29) -- global enable byte + `__forceinline` out-parameter GetInterface(0x44) +
    component flag; nested `if` gives the single `xor al,al` tail.
  - 0075dc00 vfunc_17 -- out-param GetInterface(0x44), `*errorText = "There is a CAMERA_POINT_TRACK ..."`, return the
    track lookup's bool; the out-pointer local is homed in the dead errorText slot.
- CTCCameraPointScriptedSpline:
  - 0075d050 InitialiseSpline -- float zeros assigned in the body share `xor eax,eax`; end node stored, then
    overwritten with `size() - 1` for -1 (signed /56 via imul).
  - 0075d8b0 IsCombatSplineValidForAnimAndThing -- the find iterator is a named local returned into the `push ecx`
    slot and re-read from it; compared with the tree head.
  - 00771510 (label EditCycleNextKeyCamera; really a definition-stack PopState) -- `if (!empty()) { pop_back();
    if (!empty()) Apply(back()); return true; } return false;` keeps the `xor al,al` tail last.
- CTCCarriedAction*:
  - 007edb80 IsWallMountTargeted -- interface 8 component used as `this` of virtual slot 33: retail loads it straight
    into ecx, which needs the explicit three-way entry select (`if (it == end) sel = end; else if (it->type <= type)
    sel = it; else sel = end;`), not the out-param GetInterface.
  - 007edfb0 / 007edfd0 trophy slots 28/32 -- `Base::OnUseBase(user); OnActionUseCommon(user);` (byte-identical twins).
  - 007ed060 drop-generic-box slot 28 -- `user->SetCurrentAction(&CCreatureAction_AlertToPlayer(user))` temporary
    in the frame, destroyed after the call; no EH frame.
- Constructors / destructors (catalog labels wrong):
  - 004d4f94 is CTCStealableItemLocation::CTCStealableItemLocation, 004d3d0e CTCCarryable::CTCCarryable -- size TUs
    (pass-through `push dword ptr [esp+8]`), float init-list members via fld1/fldz.
  - 0084b7c0 is NCreatureMode::CModeSleepBed::CModeSleepBed -- argument order fixed by the loads (group feeds the
    filter, the name the string), counted pointer's inline ctor zeroes both words.
  - 00aab910 is CSoundEmitterLH::CActiveSound::CActiveSound -- init list in declaration order, the name's
    out-of-line ctor runs before the last member store.
  - 009d5ee0 is ~CLoadingMemoryFailureHandler -- DeleteCriticalSection in the body, the member CArray's inline
    destructor frees its block, the empty inline base destructor leaves only its vtable store.
  - 008b0a60 is ~CCombatSequenceBase -- empty body; the member vector's inline destructor passes its empty tag from
    the high byte of the `push ecx` slot.
- CTCCarryable / CTCCarrying / others:
  - 00705150 CTCCarryable::OnKill (slot 14) -- `if (PCarryingTC) PCarryingTC->RemoveThing(ParentThing);`.
  - 00705b00 GetCarriedPositionInfo -- overload call with false + time, then two implicit POD assignments.
  - 006a1060 IsCarryingThing / 006a19b0 RemoveThing -- `__forceinline` FindThing loop over the 12-byte carry-slot
    entries returning end(), `!= end()` re-reads `last`.
  - 006a1440 GetThingInPrimarySlot -- list walk with result in edi and two exits.
  - 00833ba0 is CThingAICreature::OnLevelUnload -- base call, inline AddSuspender(0) with the setne/shl/xor bitfield
    store, null-tested brain call; the retail object sits 0x30 below the PDB offsets.
  - 00caa080 ReleasePending (semaphore), 00c6ac60 Scale (MMX sample scaler front end), 00aa5bd0 ClearBoth (two
    STLport `clear()`: `__copy_trivial` for pointers, the counted `__copy` loop for an 8-byte record with a user
    assignment).

## Parked for Fable 5.1 (8)
- 0075cd90 CTCCameraPointScripted::GetEndPos DIFFER(81v81): X sum `fld [eax] ; fadd [esi+4Ch]` vs retail `fld [esi+4Ch] ; fadd [eax]`, one fstp/push pair scheduled differently | tried: free inline operator+ (a.X + b.X), operands swapped, member operator+ over a named position reference | next: lane gap: commutative fadd operand order chosen by the scheduler
- 006a1330 CTCCarrying::IsSlotPrimarySlot DIFFER(62v60): def pointer loaded into eax and copied to ecx for the release (`mov eax,[esp+4] ... mov ecx,eax`) vs retail `mov ecx,[esp+4] ; mov eax,[ecx+4]` | tried: `if (--m_p->m_refCount == 0) m_p->Destroy();` (69v60), local pointer copy + Release() on the pointee (66v60), explicit count local (62v60) | next: the count-local release written as a member of the pointee (so `this` lands in ecx), else lane gap: register choice
- 009f0570 CNameRegistry_009f0570::Contains (catalog CTCCameraPointScriptedSpline::AddAnimIndexToAvailableAnimList) DIFFER(39v39): lane `cmp eax,esi ; setne bl` before the by-value CCharString destructor and `mov al,bl` vs retail `mov edi,eax`, destructor, `cmp edi,esi ; setne al` | tried: `return Find(name) != end;`, named node local, iterator with inline operator!=, `? true : false` | next: lane gap: register allocation of the compare across the parameter destructor
- 005202b0 CPointerListOwner_005202b0::Add (catalog EditCyclePreviousKeyCamera) DIFFER(57v55): lane builds the converted value in a `push ecx` slot; retail re-stores it into the dead argument slot (`mov [esp+4],edx`) and passes that one address as both value and tag | tried: rvalue cast / inline accessor argument, landed SScalarTag push_back | next: lane gap: 24l empty tag overlaid on a live argument slot
- 006bc4a0 CRetailVector_006bc4a0::push_back (catalog EditCyclePreviousKeyCamera) DIFFER(51v48): lane gives the tag its own frame byte (`push ecx`, `lea edx,[esp+0Bh]`); retail passes the by-value parameter's slot as both value ref and tag, no frame | tried: landed SScalarTag push_back, inline `push_back(const T&)` front end over the by-value parameter (agent82's 0x00747450 trick; still 51v48) | next: lane gap: 24l empty tag overlaid on a live argument slot
- 00778b30 CPointerListOwner_00778b30::Add (catalog EditCyclePreviousKeyCamera) DIFFER(57v55): as 005202b0 with the vector at +0xc | tried: rvalue cast / inline accessor argument, landed SScalarTag push_back | next: lane gap: 24l empty tag overlaid on a live argument slot
- 007f7810 CPointerListOwner_007f7810::AddRecordItem (catalog EditCyclePreviousKeyCamera) DIFFER(60v58): fetched value (`[arg+4]`) built in a `push ecx` slot vs retail re-store into the dead argument slot shared with the tag | tried: rvalue inline accessor argument, landed SScalarTag push_back | next: lane gap: 24l empty tag overlaid on a live argument slot
- 007fef40 CPointerListOwner_007fef40::Add (catalog EditCyclePreviousKeyCamera) DIFFER(56v53): lane gives the tag its own frame byte; retail passes the parameter slot as both value ref and tag, no frame | tried: lvalue parameter bound to push_back(const T&), landed SScalarTag push_back | next: lane gap: 24l empty tag overlaid on a live argument slot

## Skipped
None. No row in this chunk holds a second function body after int3/nop padding.

## Label corrections
- CTCCarryable::OnKill is vtable slot 14 @ 0x00705150 (manifest "CTCCarryable::vfunc_14"); the "CTCCarryable::OnKill"
  label on 0040f560, 0059ad35, 009138b0, 00a0e620 (STLport deque pop aux), 00833ba0 (CThingAICreature::OnLevelUnload,
  slot 43), 008b0a60 (~CCombatSequenceBase), 009d5ee0 (~CLoadingMemoryFailureHandler), 00c6ac60 (sample scaler) and
  00caa080 (semaphore release) is propagated and wrong.
- 004d4f94 "CTCCameraPointTrack::CTCCameraPointTrack" = CTCStealableItemLocation::CTCStealableItemLocation (vtable 0x0123C65C).
- 0084b7c0 "CTCCarriedActionUseTrophy::CTCCarriedActionUseTrophy" = NCreatureMode::CModeSleepBed::CModeSleepBed (vtable 0x01275760).
- 00aab910 "CTCCarrying::CTCCarrying" = CSoundEmitterLH::CActiveSound::CActiveSound (vtable 0x0129E67C).
- 0076d6f0 / 0076d850 "CTCCameraPointScripted::GetLookDirection" = SGI `find` front ends over a CIntelligentPointer<const CThing> list.
- 00aa5bd0 / 00c1bfb0 "CTCCameraPointScripted::EditSetThingForEditorSetThing" = a two-vector clear and a free SwapYZ.
- 005202b0 / 006bc4a0 / 00778b30 / 007f7810 / 007fef40 "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" =
  retail 4-byte-element vector push_backs; 00771510 "EditCycleNextKeyCamera" = a definition-stack PopState;
  009f0570 "AddAnimIndexToAvailableAnimList" = a string-keyed tree Contains.
- Placeholders resolved: 0075c1e0 vfunc_35 = GetTrackPosition, 0075d8f0 vfunc_29 = IsLeafCriteriaMet, 00705150 vfunc_14 = OnKill.

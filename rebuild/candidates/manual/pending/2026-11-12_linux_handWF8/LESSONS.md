## 0x0091B3A0

A named action local (ctor first, then push false + its address) vs a temporary argument (push false first, ctor result pushed as eax; frame 4 bytes larger): check the push order around a CCreatureAction ctor before calling it a register residue (0x0091B3A0).

## 0x00B81F30

- A counted-pointer local whose release is inline at some returns and a COMDAT call at another needs NO manual Release()/Destroy() model: the real shared `CCountedPointer<T>` local with plain early returns reproduces it under VC7.1 (0x00B81F30; sibling 0x00B82100 hand-modelled it).
- Raw start/finish pointer stand-ins for a CArray give different register roles than the shared `CArray` (`empty()` / `size()` / `operator[]`): port the container first (0x00B81F30: 485v455 -> 455v455 with 5 hunks in one step).
- An empty non-virtual base of a polymorphic class sits at +4 under VC7.1 (`add ecx,4` on the base conversion); a base passed unadjusted is itself polymorphic (CParticleContext, vtable 0x012A38E0).
- vt.py must run with the repo as cwd (`No module named verify_and_land` otherwise).

## 0x008C61C0

- A def pointer cached in a callee-saved register across opaque calls (and the this pointer moved to another register) = the GetSubDef<T> template body is visible in the TU (CDefClassBase_GetSubDef.h, NOINLINE opt-in); a three-slot by-value normalise with a pointer in eax on both arms = a ternary written in the row, not a helper (0x008C61C0).

## 0x006D0630

- (wf8, 2026-11-12) A join block (post + epilogue) laid out straight after the FIRST arm, with the other arms behind the epilogue jumping back, means the last arm does not fall into it: look for ONE guarded statement after the last arm's if-else that the compiler duplicated into its success paths (two identical `call ; mov [member],eax ; jmp epilogue` tails). Writing that store in each path keeps the join at the end (0x006D0630: `if (result) FrameLastReceivedNiceGift = CWorld::GetFrame();`).
- (wf8, 2026-11-12) c2 re-sorts goto-ordered source into its own block order; gotos, do/while(0)+break, a sentinel or a flag all give the same layout, so a layout residue is a missing or extra statement, not a spelling of the jumps (0x006D0630).
- (wf8, 2026-11-12) `call GetFrame` before the right-hand operands of `GetFrame() - a > b * c` are read is a `long frame = CWorld::GetFrame();` local; it also reassigns the callee-saved registers of the surrounding locals (0x006D0630).
- (wf8, 2026-11-12) A CDefPointer release as `mov r,[eax+4] ; dec r ; mov [eax],r` on a handle passed to GetDef<T> is the callee body not being visible: FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY gives `dec dword ptr [ecx+4]` and moves the epilogue pops in front of the releases (0x006D0630).

## 0x0072EB40

HASTC residue shr-vs-test in a parked draft = local bool HasTC model; engine/CThing.h FABLE_RETAIL_CTHING_HASTC_FORCEINLINE fixes it (0x0072EB40 matched on the first ported grade).

## 0x00BA4A40

- (wf8, 0x00BA4A40) A defaulted local declared BEFORE the accessor read that guards it (`type=DEFAULT; idx=mat->GetIdx(); if(idx) type=...`) keeps the result in eax and the object pointer in ecx/edx; the reverse order costs a register rotation and an extra `mov`.
- (wf8, 0x00BA4A40) CGraphicDataBank::PeekGraphicInfoFrame used twice in one function is not expanded by plain `inline` under VC7.1; FABLE_FORCEINLINE in the row (Inline-budget line) gives retail's two expansions.

## 0x007BBE34

- (wf8, 0x007BBE34) A CRightHandedSet temporary that retail keeps in its own frame slot (not overlaid on a dead C3DVector call temporary) needs the constructor BODY visible though still called out of line: FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE, not _CTOR. Same blocker in the shot twins 0x007BC004 / 0x007BD845.
- (wf8, 0x007BBE34) FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN is not TU-wide in the shot TU: a returned temporary scaled by a constant (`call() * K`) holds K on the x87 stack (`fld K ; fmul st(1..3) ; ... fst Z ; fstp st(0) ; fstp st(0)`) only with the default operator*; move the region cut with operator^ groups (CROSS_XY_GROUP) instead.
- (wf8, 0x007BBE34) `static const float` at file scope is read from its own .rdata symbol (no __real@ pool entry): a float constant with a single reader outside the pool region is a named TU constant.
- (wf8) q.py called directly without FABLE_HDR_OVERLAY hangs Wine cl on a missing overlay header; grade through fg.py.


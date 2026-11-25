# NF452 d1 notes

## Decisions
1. Headers in NF447's PanDelay form: names, access, member order from the SDK CODAApiDbg.lib Debug/SalRenderDisable*.obj CodeView
   (CSalRenderDisable : public CSalRender; ParamsDisable { FilterPan pan; }, FilterPan { unsigned short gain; short gainDelta; }); offsets
   retail (+0x28 / +0x2C / +0x30 / +0x34; SDK +0x2C..+0x38 because the retail CSalRender is 0x28). FilterPan is guarded by
   FABLE_GAMECODA_FILTERPAN_DEFINED: SalRenderPan.obj declares the same struct and the Pan header will reuse it.
2. SIMD ctors 0x00C550B0 / 0x00C55110: inline `{}` in the SIMD headers; VC7.1 calls them out of line from 0x00C55270 (retail `call 0x00C550B0`
   / `call 0x00C55110`, graded RELOCATION_MATCH), exactly as the SDK Release SalSw3DVoice.obj does (its CreateRenderer relocates calls to the
   ??0CSalRenderDisableSSE2 / MMX COMDATs, while it expands the PanDelay SIMD ctors). So they are option S thin rows of 0x00C55270, like
   NF447's 0x00C55070 / 0x00C550D0.
3. Fold of 0x00C6EBA0 Params3DInit (owner) + 0x00C6EBE0 UpdateFilters (thin): both are private SDK members declared in the header; two TUs
   defining Params3DInit collide (HEAD already had that duplicate: forcelink DUP- ?ComputeGainRamp@CVtable_012bedb8). One TU reproduces
   retail's inline expansion of Params3DInit in both arms of UpdateFilters with no helper of our own.
4. SIMD Process rows keep their HEAD kernels (static ramp kernel repeated verbatim, cdecl constant kernel) and the HEAD tap-mixer helper
   functions (descriptive, no rows); only the class model moves onto the headers (SGainRamp -> FilterPan, m_crossfade -> m_bUpdate,
   m_pCurrent / m_pPrevious -> m_pParamsCurrent / m_pParamsNew, the lock -> CSalMutex::Enter / Leave). The CAudioMaster view of
   g_pAudioMaster (+0x1BC) stays HEAD text (SDK: g_pSal / CSalPlatform::GetSWOutputChannels = mov eax,[ecx+1BCh] in every SalRender*.obj).
5. Tests: every test links the row object (the unit object for funclets / thin rows) and models no class that has a header; stubs for the
   members the row does not define; private helpers are reached through member stubs of slots the unit does not define (SetGain ->
   UpdateFilters, SetPosition -> Params3DInit). The SIMD Process tests run the row's real hand ramp kernels (exact per-sample sums) and
   record the constant-gain kernel through a stub.

## Per-row ties (evidence/tie_table.tsv, tie_kernels.tsv; SDK = GameCODA 2.10 CODAApi.lib Release, relocation lines excluded)
| row | name | retail insns / ret | SDK insns / ret |
|---|---|---|---|
| 00c6e760 | ??0CSalRenderDisable | 11 / ret | 11 / ret (+ SDK SIMD COMDATs call it = retail 550b0 / 55110 call 6e760) |
| 00c6e780 | ~CSalRenderDisable | 2 | 2 |
| 00c6eba0 | Params3DInit | 17 / 4 | 17 / 4 |
| 00c6ebe0 | UpdateFilters | 35 / 4 | 35 / 4 (Release SetGain / SetPosition relocate their one call to it) |
| 00c6ec50 | SetGain | 12 / 0Ch | 12 / 0Ch |
| 00c6ec80 | SetPosition | 15 / 1Ch | 15 / 1Ch |
| 00c6eef0 / 00c6fe10 | Disable SSE2 / MMX Process | 191 / 8 | 174 / 8; same Enter / fade x2(+2) / swap / fade x2(+2) / Mix x2(+2) / Leave relocation order |
| 00c550b0 / 00c55110 | ??0CSalRenderDisableSSE2 / MMX | 18 B | SDK COMDATs of SalSw3DVoice.obj, called by CreateRenderer in the same order |
Retail object order 0x00C6E760..0x00C6ECB0 (ctor, dtor, 0x00C6E790, 0x00C6E820, 0x00C6E8B0, Process 0x00C6E8F0, Params3DInit, UpdateFilters,
SetGain, SetPosition, ??_G) = the SDK Release SalRenderDisable.obj function order (ctor, dtor, MixFadeIn, MixFadeOut, Mix, Process,
Params3DInit, UpdateFilters, SetGain, SetPosition, ??_G); MixFadeIn / MixFadeOut tie exactly (53 / 54) -> NAME_FIXES.tsv.

## Follow-ups (not done here)
- Slot-0 stand-ins: 0x00C6ECB0 (??_GCSalRenderDisable), 0x00C55520 / 0x00C55540 (DisableSSE2 ??_G / ??1), 0x00C555B0 / 0x00C555D0 (MMX): the
  d1 headers emit them EQ at those homes (pk_all --addrs), so a respell batch can follow (with NF447's 0x00C6D350 / 0x00C554C0 / 0x00C55550
  / 0x00C554E0 / 0x00C55570).
- 0x00C6E790 / 0x00C6E820 (NAME_FIXES.tsv), 0x00C6E8B0 Mix and 0x00C6E8F0 Process have no row / a descriptive row.
- 0x00C6ECD0 / 0x00C6ED70 are the SDK's sse2Mix / sse2MixFade (47 v 42, 72 v 67 instructions: one source + near size; not proposed).

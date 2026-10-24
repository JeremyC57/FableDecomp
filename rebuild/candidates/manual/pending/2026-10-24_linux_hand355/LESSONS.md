# Chunk 355 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- /Gr algorithm TUs: a template algorithm needs an explicit `__fastcall`.
  - A non-POD functor or iterator result goes through the hidden pointer in ecx, a pointer `first` goes in edx, and class
    iterators and functors go on the stack (0043f310, 0085a960).
  - An empty POD functor result comes back in al (0084f280).
- STLport `__find_if(first, last, pred, const input_iterator_tag&)`: with class iterators, the tag reference is the second
  register argument, edx (`lea edx,[ebp+13h]` at the front end) (0065f9ab, 0065f96d).
- /Os algorithm TUs (`#pragma optimize("s", on)`) show an unaligned start and a `jmp` to the loop test. The ebp frame is
  VC's own choice there because [ebp+N] parameter references are shorter, so don't fight it (0065f5d2, 0053caa0).
- `list::_Insert(where, first, last, input_iterator_tag)` is a member template looping `insert(where, *first)`.
  - The by-value tag is the unused 4th stack slot.
  - The discarded returned iterator reuses the dead `first` slot (`lea eax,[ebp+0Ch]`) (0053caa0, 005794c3).
- CSmallVector `push_back = resize(m_size + 1); back() = v;`: the never-written `T()` default temporary is homed over a
  live functor slot only when resize is a same-TU `__declspec(noinline)` body that only reads its fill. Out of line or
  inlined, it gets its own `sub esp,8` (006fb640).
- Functor copy-out:
  - A 7-dword functor whose copy-out is retail's scheduled `mov` pairs needs a memberwise user copy ctor over the long
    members; the implicit copy emits `rep movsd` (008065c0).
  - An 8-dword functor's implicit copy is the retail `rep movsd` (006f3530).
- A best-point C3DVector member that retail stores through memory needs a C3DVector user copy ctor, with the functor copy
  left implicit. Otherwise VC register-promotes the point (008e2f10).
- Order of the object load and the argument push:
  - `CThing* target = p.GetPItem(); target->F(m_flag);` evaluates the object before pushing the argument.
  - The direct `p.GetPItem()->F(m_flag)` pushes first and changes the register allocation (0071b590, 0071b690).
- An inline range-test member on the pointee (`ev->IsActiveOnFrame(m_frame)`) gives retail's register choice (event in edx,
  end frame in esi). The open-coded test swaps edx/esi, and named locals don't help (0085a960).
- cmp operand order follows source order: `m_frame < ev->m_frame` gives `mov edx,[ecx+..] ; cmp edx,[eax+4]`, while the
  mirrored form swaps it (00811e20).
- CThing interface model:
  - `HasTC` must return the masked word (`m_presentBits[t>>5] & (1UL<<(t&31))`). A `bool` return emits
    `shr eax,n ; test al,1` instead of `test ch,20h` / `test dword [eax+24h],imm` (0092e540, 00946c60).
  - A predicate with early `return`s gets esi shrink-wrap and a `neg/sbb/inc` bool. Use a single nested if or `&&` chain
    with one trailing `return false` (0092e540, 00946c60).
  - A helper predicate must be `__forceinline`; plain `inline` stayed out of line in these /O2 TUs (00946c60 IsCandle).
  - Slot-48 kind flags read as `(unsigned char)(flags >> n) & 1` give retail's `shr eax,n ; test al,1` (00946520, 0094b9b0).
- For two interface lookups, one non-const and one const, use the non-const LowerBound copy 0x40F020 for the first and the
  const copy 0x4365B0 for the second; that keeps relocation identity (0092e540).
- remove_copy_if hoists the loop-invariant `m_distance * m_distance` onto the x87 stack before the loop (008ab710). The
  remaining dx/dy squaring order did not respond to source order (parked).

Review at landing:
- 006fb640 depends on a same-TU noinline `CSmallVector::resize` stand-in. Check that no landed CSmallVector header
  conflicts, and keep the stand-in TU-local.
- 0082c720 uses engine/CMarkerInfo.h and reads the opaque `PThing[8]` through a `(const CBaseIntelligentPointer*)` cast,
  because the header has no CIntelligentPointer type.
- Local models sit next to possible engine headers, so check `rebuild/include/engine/` before landing:
  - CThing interface model (0090c060, 0092e540, 00946520, 00946c60, 0094b9b0).
  - CBaseIntelligentPointer / CIntelligentPointer, CCountedPointer, C3DVector, CAnimSet, CEnginePrimitiveHandle.
- Byte-identical pairs that land as separate rows: 0043f310 / 0084ad60, 0085a960 / 0085a9c0, and 0053caa0 / 005794c3
  (which differ only in the insert callee).
- Catalog rows hold the wrong function family, so names and leaves were taken from the obj symbol (`objdump -t`). See
  NOTES.md Label corrections for the full list:
  - 008ab710 is remove_copy_if.
  - 008e8a90 / 008e8b80 are count_if.
  - 0084f280 / 0091b890 are for_each.
  - The five operator() rows: 0090c060, 0092e540, 00946520, 00946c60, 0094b9b0.
  - 0065f96d is CIsHornTaskWithThisHorn.
- Already-landed 0x6A2370 is CTCCarrying::OnSetInLimbo, not CEngineLandscapeMap::RenderBackground. Its landed source
  already notes this, but the manifest label still carries the propagated name.
- The five CThing predicate rows share one model, and it went through several revisions across those rows (HasTC
  return type, early returns vs one chain, inline vs `__forceinline`). Per-row grade counts were not tracked
  precisely, so re-grade them all together at landing. The four parked rows stopped at 4 attempts each.

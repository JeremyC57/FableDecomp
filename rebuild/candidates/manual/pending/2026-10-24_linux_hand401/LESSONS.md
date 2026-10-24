# Chunk 401 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Live-slot tag homing.** When retail homes an STLport `__false_type` / iterator tag on a LIVE argument slot (one re-read
  after the call), give the tag-taking callee a same-TU `__declspec(noinline)` body that never touches the tag. A bare
  declaration homes the tag on the dead slot. Seen on `_M_assign_dispatch` 0x00569140 in 0x00841EE0, 0x0083F870 and
  0x0083FB20; the QFE compiler does not change it.
- **Unaliased string local.** Retail caches a string local's `_M_start` across `push_back` / `_M_insert_overflow` and hoists
  the copy's loads above the placement zero stores. That needs EVERY callee receiving the local's address to be a
  NON-template same-TU noinline body that never stores the address (0x0083FC00):
  - `append<char*>` / `append<const char*>` as two plain overloads.
  - `_M_throw_length_error` with a puts/abort body (a declared-only one leaks `this`).
  - Out-of-class `_M_insert_overflow` plus its uninitialized helpers, explicitly instantiated ABOVE the caller.

  Member-template stand-ins instantiated after the caller do nothing, and `/Oa` over-caches the reference parameters too.
- **`__fastcall` free inlines.** Under /Gr, STLport's free inline operators are `__fastcall` (`_STLP_CALL` is empty), and VC7.1
  then evaluates the LEFT operand of an inline `operator==(const string&, const string&)` first. With the default
  convention it evaluates the right operand first (0x0083EE10). Temporaries are destroyed in reverse execution order either
  way.
- **Inlined substr.** For a substr inlined into a caller, retail computes `start + len` AFTER pushing the iterator tag. Build
  the result through `(const char* s, size_type n)` with `_M_range_initialize(s, s + n, forward_iterator_tag())`; the
  (first, last) constructor computes the end pointer before the tag (0x0083EE10). A `__declspec(noreturn)` out-of-range
  thrower is what lets VC7.1 inline substr at all.
- **`resize(n)` zero value.** STLport `vector<char>::resize(n)` forwards `_Tp()` to a `const _Tp&` overload: the zero char is a
  temporary homed at byte 3 of its slot (`lea eax,[esp+13h]`). A by-value `_Tp __x` puts it at byte 0 (0x0083F8B0). Small
  temporaries (tags, bools, chars) sit at byte 3 of their 4-byte slot.
- **Keeping an empty path.** A float epsilon compare that guards an empty-container path keeps its retail layout only when
  the epsilon is a named `extern const float` global (0x0129BA3C). With a literal, VC threads the empty path to the
  `return 0.0f` (0x00851BD0).
- **Exact bool compare.** `return it == end ? true : false;` gives retail's bare `cmp ; sete al`; `return it == end;` adds
  `xor eax,eax` (0x00856570). A loop with no empty-range guard ahead of the first call is a do-while.
- **Pointer locals for member loads.** A manager pointer local taken first gives `mov eax,[edi+40h]` early and
  `lea ecx,[eax+78h]` at the call. A reference local or an inline accessor sinks the load (0x0084E2C0). STLport
  `list::push_back` is `insert(end(), x)`, which reads the sentinel before the node malloc.
- **Pre-call reads.** A member value read before a call needs a local: IsValid's threshold (0x00851B50), and the fps global
  ahead of a virtual call (0x00856250).
- **Fabs float overload.** `inline float fabsf(float x) { return (float)fabs((double)x); }` reproduces retail's
  `fxch st(1) ; fxch st(1)` pair (0x0084CD10).

Review at landing:
- **Header mismatches.** LHHttp2.h, LHHttpHeaderStatus.h and CCreatureAction_PlayAnimationWithLookTurning.h are generated
  from Ego_r, which has 0x1C-byte Dinkumware strings and a 0xC0 class. TLC has 12-byte STLport strings and a 0xB4 class, so
  the LH rows and the three LookTurning constructors use local retail models (Hostname +4, Port +0x10, ProxyHost +0x874,
  LSocket NrLastDataSent +0x90).
- **Grade budgets.** 0x0083FC00 and 0x0083EE10 each used exactly 4 grades, and both matched on the 4th. A compile that failed
  for the environment (env.sh not sourced) or SRC_FAIL was not counted. No row needed more than 4.
- **Already-landed names.** Several landed sources carry wrong names:

  | Address | Landed as | Actually |
  |---|---|---|
  | 0x008368F0 | "ConditionalVirtualDispatchOnLevelUnl" | CThingSoundEmitter::ReleaseLoop |
  | 0x0084F4D0 | "CAxisPointList" | the C3DFrustrum constructor |
  | 0x00841E90 | "CNamedRecord_00841e90" | LAddress::LAddress() |
  | 0x004110D0 | "CStringFillRange" | vector<char>::_M_fill_insert |
- **Chunk-291 attractor run.** 0x0084E1A0, labelled "SetGoToPosition" in chunk 291, is most likely CBase::AddAttractorGoTo.
  The run 0x0084E1A0 / 0x0084E200 / 0x0084E260 is AddAttractorGoTo / AddIncidentalAttractorGoTo / AddAttractorLookAt (lists
  +0x78 / +0x7c / +0x34), ahead of this chunk's AddIncidentalAttractorLookAt 0x0084E2C0.
- **Next try for 0x0084E390.** It is parked with a tag/weight-slot diff; the same-TU `_M_insert_overflow` body (lesson 1) is
  its next attempt.

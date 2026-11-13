# Chunk 2001 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 2001, 0x007891F2) CTCTrapBase::BulletTimeFrameUpdate is `{ FrameUpdate(); }` (vtable slot 10 -> slot 9 tail jump) in a
  size-optimised TU: `#pragma optimize("s", on)` + the shared engine/CTCTrapBase.h (outer FABLE_RETAIL_CTCTRAPBASE branch; define
  FABLE_RETAIL_CTCBASE and FABLE_RETAIL_DEF_CLASS_BASE and include engine/ETCInterfaceType.h first).
- (chunk 2001, 0x00AED600) A halfway select of a multi-bit `unsigned short` bit-field written `t > 0.5f ? other.F : F` narrows the
  arms to byte moves (`mov cl,[edi+40h]` / `mov cl,dl`); retail's full-width arms (`xor ecx,ecx ; mov cl,[..]` / `mov ecx,edx`) are the
  select through FableWin's `GFInterpolateAsFlags(long, long, float)` (both arms widened to long). The one-bit fields go through the
  FableWin GFInterpolate overload of their declared type (unsigned short `YIGGGM` for the +0x40 word, unsigned char `YIEEEM` for the
  +0x44/+0x45 bytes), each `(1 - t) * a + b * t` rounded by GFFloatToLongNear.
- (chunk 2001, 0x00B3C5D0) A local array whose start retail keeps in a callee-saved register across calls that take its address
  (0x00B3C9EA `mov esi,[esp+1Ch]` ... 0x00B3CAC3 `push esi ; call free`, BuildAlphaStippleTexture / BuildHWStipplePatterns in
  between) means the TU saw those callees' bodies: define the same-TU callees noinline AND make every STLport member they reach
  visible (FABLE_RETAIL_STLP_VECTOR for _M_fill_insert); one declaration-only `resize(n)` re-exposes the array (copy loop reloads it).
- (chunk 2001, 0x00B3C5D0) VC7.1 expands STLport `resize(n)` / `resize(n, x)` per element type and site: the plain inline bodies expand
  vector<unsigned long>::resize(0x2000, 0) (retail calls 0x00464931) and RESIZE_OUTOFLINE alone expands vector<CTexture>::resize(4)
  (retail calls 0x0049B7F0); both out of line need the new FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_OUTOFLINE (visible noinline resize(n)).
- (chunk 2001, 0x00B3C5D0) CPixelFormat built with the 4-argument constructor `CPixelFormat(8, 8, 8, 8)` stores no -1 first (the
  default constructor's FormatIndex(-1) store would precede Initialise); retail has none.
- (chunk 2001, 0x00B3C5D0) `if (!rebuild) { CAFile::DoErrorCallback(); return; }` is retail's `mov eax,[13BC9FCh] ; cmp eax,ebx ; je ;
  call eax` (FableWin ?DoErrorCallback@CAFile@@SIXXZ COMDAT sits right after this TU's bodies).
- (chunk 2001, 0x00AF4900) A CCharString temporary compared through the inline `operator==(const CCharString&)` whose PStringData is
  re-read from its frame slot (0x00AF4B39 `mov eax,[esp+40h]`) is an IMPLICIT-conversion temporary: `flag ==
  static_cast<const CCharString&>("DISABLED")`. An explicit `CCharString("DISABLED")` uses the ctor's eax (`mov eax,[eax]`), and
  plain `flag == "DISABLED"` picks the `operator==(const char*)` overload (no temporary at all).
- (chunk 2001, 0x00AF4900) The inline CCharString::GetCRC + CCRC::Calc expansion indexes the text as `[ecx+esi]` only with
  FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN (the ternary gives `[esi+ecx]`), as in 0x00B21A50.
- (chunk 2001, 0x00AF4900) CStringParser(const CCharString&) is inline in retail (COMDAT 0x00404720 called from 0x00AEF8E6, expanded at
  0x00AF4CA1): the new FABLE_RETAIL_CSTRINGPARSER_INLINE_CTOR body reproduces the expansion; the default filename argument
  `CCharString("")` is built before the inline body and destroyed after the full expression.
- (chunk 2001, 0x00A9EB90) Scalar slerp loop: `CQuaternion& rotation = from[i].Rotation; rotation.SetSlerp(rotation, to[i].Rotation, t);`
  gives retail's operand order in both the dot product and the combine (the `from[i].Rotation.SetSlerp(from[i].Rotation, ...)` form
  mirrors every product); the dot / combine spellings themselves are canonicalised (no effect).
- (chunk 2001, 0x00A9EB90) Commutative operand orders in this row move with edits far away (the rdtsc block written as the
  GFGetCycleCount() function instead of the GF_GET_CYCLE_COUNT macro flips the SSE loop's SIB order AND three product orders): a
  generator over neutral spellings with a relocation-masked instruction-diff fitness (probe/search.py, ~0.6 s per variant) is the
  practical way to search such rows; record the best and grade only improvements.
- (tooling) $S/tools/rstr.py has a stale hard-coded Fable.exe path (a removed scratchpad); read strings from $FABLE_EXE instead
  (probe/rs.py).

Review at landing:
- 0x00AF4900 spells the two flag compares `flag == static_cast<const CCharString&>("...")` (an implicit-conversion temporary; see the
  lesson above): plain C++, but check whether a more natural retail spelling exists in a sibling PSC row.
- 0x00B3C5D0 repeats the landed BuildAlphaStippleTexture (0x00B3BE10) and BuildHWStipplePatterns (0x00B3C4B0) bodies as same-TU
  noinline definitions (only their knowledge matters; BuildAlphaStippleTexture's swap is written out and `size()` comparisons cast to
  long so it compiles against the shared CArray). It also defines the 2x2 dither table 0x012A1BB0 as a function-local static
  (descriptive name DitherPatterns; only this function reads it) -- the object defines that data symbol.
- 0x00B3C5D0 carries an `// Inline-budget:` line (two STLport resize opt-ins, one new in stlport/fable_containers.h).
- 0x00AED600 defines the FableWin NParticleEngine::GFInterpolate / GFInterpolateAsFlags overloads row-locally (the landed
  0x00AF9840 does the same for two of them): candidates for one shared NParticleEngine header. GFInterpolateAsFlags's body (select
  past t > 0.5) is inferred from the bytes and the name.
- 0x00A9EB90 (parked) defines GFFastArcCos / GFFastSin and CQuaternion::CalculateSlerpWeights / SetSlerp row-locally (as the landed
  0x00AA3C50 / 0x00AA3390 do); the two 64-bit blend statistics 0x013D2D88 / 0x013D2D90 have descriptive names.
- NAME_FIXES.tsv: 0x00411570 (landed as strcmp_signed), 0x013BC9FC (g_pfnAssertionFailed vs CAFile::m_errorCallback), 0x00464931.
- engine/CTCTrapBase.h structure (outer opt-in wrapping a full class; the inner FABLE_RETAIL_CTCTRAPBASE branch is unreachable).

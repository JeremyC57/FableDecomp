# retry11 (2026-10-24) -- parked near-miss retries + the four landed-but-byte-wrong rows

Lane: Linux Wine, VC7.1 RTM. Grading: `g.sh <addr> <src>` (q.py, catalog/batch oracle), `gx.sh <addr> <src> <len>` (boundsfix g.py,
bytes straight from Fable.exe, for rows whose catalog row is short or missing). Tests: `t.py <addr>...`. Variants in `var/`.

## Matches (3) -- src/ + test/, re-graded at the end, tests PASS
- 0066f7d0 (debakeA, landed as an asm body -> genuine C++ replaces it): CInputTypeXboxPadLeftStickEvent::GetControlDirection.
  `direction.X /= length; direction.Y /= length;` -- VC7.1 folds the two divisions into one reciprocal and multiplies X then
  Y (`fld st0 ; fmulp st2 ; fmulp st2`); a named `scale = 1/length` multiplies Y first whatever the statement order.
- 006c5110 (hand329): a real constructor (`mov eax,esi ; ret`). The manifest's "CIntelligentPointer<const CThing>::ctor" is
  propagated: `.?AV?$CIntelligentPointer@$$CBVCThing@@@@` exists in the exe with its own vtable, but this body stores the
  CIntelligentPointer<CThing> vtable 0x01238C6C into BOTH the temporary and the member, so both are the same instantiation.
  The copy is not elided because it goes through a defaulted `const CIntelligentPointer<CThing>&` constructor parameter of a
  holder member, inlined into the owner's default constructor (named COwner_006c5110 / CThingPointerHolder_006c5110: xref 0).
- 0051a240 (hand332): chunk 333's CReplaceSerialise GetMaxSize model re-graded with q.py (RELOCATION_MATCH); new behaviour test
  (current-version record read straight in, version-3 record upgraded through ConvertToThisVersion and copied).

## Owner-priority landed rows (bounds_landed_differ)
- 00b687d0 BuildThemes 1486v1483: 7 more forms, none closer (`def->` everywhere or a plain pointer out-param reload the def
  after every store -> 1523; unchecked dtor release 1505). Residue: in the 4th mip block retail evicts the def register,
  reuses ebp for the graphic pointer and re-reads the def from its CDefPointer slot without a spill store.
- 00988a50 UpdateCombinedProjectionTransform: both SSE 4x4 multiplies reproduced byte-for-byte as `__asm` (differ/, retail_asm
  PENDING OWNER DECISION: 2 x 68 instructions). Still DIFFER 996v978: `this` spilled (ebx) where retail keeps esi, and the x87
  fallback loops: VC7.1 REASSOCIATES the four-term sums -- the first loop's retail term order is 0,2,3,1 / 3,1,2,0 / 3,1,2,0 /
  2,0,3,1 per column while the second (stack-local operand) is the natural 0,1,2,3; two source orders gave two other scrambles.
  The inlined helper is CMatrix4x4::AlignedSetMultiplyTransposed (FableWin.pdb).
- 00bdc510 CEngineLandscapeMap::UpdateForeground: fresh reconstruction (differ/, `__asm` AABB block, retail_asm PENDING OWNER
  DECISION). 1118v1134 after 7 grades; everything matches except (1) the patch-count multiply operand order, (2) the shape of the
  zero-count allocation arm (`push 4 ; call ::operator new[]` vs the pool allocator, one shared vector-ctor expansion), (3) the
  index/base schedule in the Initialise/LoadHeader loop.
- 00bf4570 RenderForeground (4880 B): not attempted -- needs a full fresh reconstruction.

## Label corrections / identity notes
- 0x006C5110: not CIntelligentPointer<const CThing>::ctor (see above); a default ctor of an unknown owner (descriptive name).
- 0x00BF36C0 (catalogued CEngineLandscapePatch::operator new[]): called with (size, owner) in ecx/edx from UpdateForeground's
  non-zero arm only; the zero-count arm calls MSVCR71 `??_U@YAPAXI@Z` (0x00BFEB22). 0x00BFEB1C is `??_V@YAXPAX@Z` (operator
  delete[]), 0x00BFE9BC is `??3@YAXPAX@Z` (operator delete) -- three distinct imports, do not merge them in sources.
- 0x00993B10 (catalogued CChunkedFileChunk ctor) is ~CMemoryDataInputStream (called on the stream local in UpdateForeground).
- 0x009D68A0 DiscardAsyncData takes the counted pointer (ecx = &ForegroundAsync), not the data object.
- 0x00987700 is landed as a 2-line `return this` stub, yet the retail function containing 0x00987872/0x00987A32 (two SSE-flag
  reads) is a large matrix routine: that landing deserves an audit.
- 0x00632830 is GFSerialiseMapBinaryIn<long,NUISystem::ETableSprites> (catalog: CPersistTraits<list<COpinionTransientOffset>>).

## GOTCHAS one-liners
- Two divisions by the same float local (`X /= l; Y /= l;`) compile to one reciprocal + X-then-Y multiplies; a named
  `scale = 1/l` multiplies Y first (0066f7d0).
- A copy from a same-type temporary is elided even through a template converting constructor (`template<class U> T(const T<U>&)`,
  U = T); only a copy through a (defaulted) const-reference parameter keeps the temporary (006c5110).
- VC7.1 reassociates x87 `a*b + c*d + e*f + g*h` chains; the term order depends on the operand memory classes (member via
  pointer vs stack local), so an inline 4x4 multiply expanded twice gets two different orders (00988a50).
- An `__asm` block that reads inlined reference parameters (`mov ecx, box`) homes them on the stack exactly as retail's
  `mov [esp+14h],ecx ; mov ecx,[esp+14h]` pairs; the surrounding C++ still schedules around it normally (00bdc510, 00988a50).
- A C++ `delete[] p` on a class with a virtual destructor expands to `if (p[-1] == 0) ::operator delete[](p-4) else
  p->vector_deleting_dtor(3)` (00bdc510); `new (memory) T[n]` stores n at `memory` and calls the vector ctor iterator on +4.
- A `count ? A(count*s+4) : ::operator new[](count*s+4)` ternary hoists the size lea above the test; a ternary over two
  `new[]` expressions duplicates the vector-ctor expansion with a folded `push 0` count (00bdc510, open).
- `/Os` moves a member-call `this` load (`mov ecx,[esi+N]`) above the argument pushes but adds the ebp frame and narrows a
  dword bit test to a byte test (the six CTCSpecialAbilities::Setup* rows, still open).
- STLport `vector::DeallocateUnusedMemory` hand-over: the old-start load before the temp loads only appears with an if/else that
  duplicates the three stores (009b5db0 family, open).
- The `lea eax,[ebp-4]` hoisted above `push 1` before a GetDef call (0042f722 / 0042e204) survives a named const CCharString
  local and the 0047d5a3-style noinline GetDef stand-in.
- `q.py` has no oracle for pre-2026-10-24 batches whose rows are not in the catalog: grade those with `gx.sh` (exe bytes, length
  from the park header).

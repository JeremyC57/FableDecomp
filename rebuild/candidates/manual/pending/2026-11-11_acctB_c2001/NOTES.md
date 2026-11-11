# Hand chunk 2001 (CTCTrapBase .. CEngineResourceManager): 4 of 5 byte-exact

All grades were taken with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2001/hdr` (hdr/README.md lists every overlay change). Row
lengths were checked from Fable.exe (first instruction to the final `ret`): all five catalog lengths are right, no SPLITS.tsv.

## What matched (4)
- **CTCTrapBase** -- 0x007891F2 `?BulletTimeFrameUpdate@CTCTrapBase@@UAEXXZ` (MATCH, 1 grade): `{ FrameUpdate(); }`, vtable 0x0126B7FC
  slot 10 jumping through slot 9; size-optimised TU (`#pragma optimize("s", on)`), shared engine/CTCTrapBase.h.
- **NParticleEngine::CParticleEmitter** -- 0x00AED600 `?InterpolateTo@CParticleEmitter@NParticleEngine@@QAEXABV12@M@Z` (RELOCATION_MATCH,
  4 grades): every particle system blended with its partner (CParticleSystem::InterpolateTo 0x00AF4100), the one-bit flags and the
  distances lerped through FableWin's NParticleEngine::GFInterpolate overloads (unsigned short / unsigned char / float; integer ones
  rounded by the shared GFFloatToLongNear asm helper), Priority and Emitter2DLayer taken from the target past t > 0.5 through
  FableWin's GFInterpolateAsFlags(long, long, float) (the long arms give retail's full-width select). Grade `retail_asm` (helper).
- **NParticleEngine::CParticleSystem** -- 0x00AF4900 `?DatabaseSerialise@CParticleSystem@NParticleEngine@@QAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z`
  (RELOCATION_MATCH, 2 grades): SystemName / Enabled / ScaleX-Z / ScaleParticles transfers, then each BEGIN_COMPONENT block: quoted type
  and name, the name's CRC (inline GetCRC + CCRC::Calc, NULLRETURN GetAsCharArray), the DISABLED / VISIBLE words of the flag line
  (inline CCharString / CBasicString<char>::operator== and Compare against implicit-conversion temporaries), the script up to
  END_COMPONENT, AddComponent, an inline-constructed CStringParser + CPersistContext, and the component's virtual DatabaseSerialise
  (new engine/CPSCBase.h, slot 17).
- **CEngineResourceManager** -- 0x00B3C5D0 `?SetupAlphaStippleTexture@CEngineResourceManager@@AAEX_N@Z` (RELOCATION_MATCH, 5 grades):
  stipple / dither textures, the StippleAlphaTexture.dat cache (version 8) read or rebuilt and written. Levers: the same-TU
  BuildAlphaStippleTexture / BuildHWStipplePatterns bodies visible (noinline) and STLport's resize / _M_fill_insert bodies visible
  (FABLE_RETAIL_STLP_VECTOR), both resize forms kept out of line (`// Inline-budget:` line; new RESIZE_ONE_OUTOFLINE opt-in),
  `CPixelFormat(8,8,8,8)`, `CAFile::DoErrorCallback()`.

## Parked for Fable 5.1 (1)
- 00a9eb90 C3DAnimationManager::BlendAnimations DIFFER(1337v1337): 0x00A9EC5A retail `fld dword ptr [eax+edx]` (d9 04 10), ours
  `fld dword ptr [edx+eax]` (d9 04 02) -- the SSE weight loop's from.W load through the to-pointer IV plus the spilled (from - to)
  offset; every other byte equal | tried: 16 graded sources (pass 1: 8, pass 2: 8 -- see STATUS.tsv) plus a generator search
  (probe/search.py, relocation-masked instruction-diff fitness) over ~960 neutral-spelling variants (search1/3/4.log) (rdtsc macro vs GFGetCycleCount() at start
  / end, stats temporary, Position-pointer spelling, named refs to from / to / weights / keyframes in either loop, dot / combine
  spellings, SetSlerp via CalculateSlerpWeights, loop index type, shared index, branch order) | next: the operand order is a
  commutative-canonicalisation outcome that moves with edits far away (GFGetCycleCount() at both ends fixes this SIB but flips three
  product orders); trace c2's canonical ordering of that ADD (c2dbg hook at the strength-reduction / reassociation pass) or extend the
  generator with the levers that flip product orders under the fn/fn base (search4.log: best 16 under that base).

## Skipped
None.

## Names (all proven from two sources unless marked)
- Row names: see LABELS.tsv (PDB name sets + vtable slot / callers / `ret N`).
- Callees: 0x00AF4100 CParticleSystem::InterpolateTo, 0x00AF4530 AddComponent, 0x0049B7F0 vector<CTexture>::resize(size_type),
  0x00464931 vector<unsigned long>::resize(size_type, const unsigned long&), 0x009F3470 CSurface::CopyFromMemoryAutoSwizzle,
  0x009BA390 CStringParser::ReadLineAsString, 0x00994F60 CDataInputStream::ReadWhiteSpace, 0x00411570 CBasicString<char>::Compare
  (body visible), 0x013BC9FC CAFile::m_errorCallback (through DoErrorCallback); all other callees use their bound PDB spellings
  (vland reloc identity: clean, no warnings).
- Descriptive: 0x012A1BB0 the 2x2 dither patterns (function-local static `DitherPatterns`, read only by 0x00B3C5D0); 0x013D2D88 /
  0x013D2D90 `g_BlendAnimationsCycles` / `g_BlendAnimationsKeyframes` (parked row; the two 64-bit blend statistics only it touches;
  no PDB data symbol on this machine). GFInterpolateAsFlags's body (select past t > 0.5) is inferred from the bytes and the FableWin
  name (`?GFInterpolateAsFlags@NParticleEngine@@YIJJJM@Z`).

## Label corrections
- 0x007891F2 catalog `CTCTrapBase::vfunc_10` -> BulletTimeFrameUpdate.
- 0x00AF4900 catalog `CParticleSystem::SerializeComponents` -> DatabaseSerialise (reloc_bindings already had the right name).
- NAME_FIXES.tsv: 0x00411570 (landed symbol strcmp_signed), 0x013BC9FC (g_pfnAssertionFailed), 0x00464931 (CArray<K>::resize spellings).

## Header debt
- engine/CTCTrapBase.h: an outer `#ifdef FABLE_RETAIL_CTCTRAPBASE` wraps a complete PDB-layout class and its `#else` holds the generated
  default with an unreachable inner `FABLE_RETAIL_CTCTRAPBASE` opt-in.
- No shared NParticleEngine GFInterpolate header and no shared GFFastSin / GFFastArcCos header: the rows define them locally.
- CEngineResourceManager.h's stipple members were opaque; the STIPPLE opt-in types them (same offsets).

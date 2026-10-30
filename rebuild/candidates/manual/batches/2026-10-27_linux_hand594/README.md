# Chunk 594 (2026-10-30): 9 of 10 engine-range released pass 9 rows landed

9 RELOCATION_MATCH, all PASS (g++ and VC7.1). Proven from public source plus callers / vtables: DirectShow BaseClasses
(strmbase, compiled from the SDK source with /EHsc) `CBaseRenderer::Pause` 0x00CA67B0 and `::Run` 0x00CA68F0 (renbase.cpp;
`this` is the IBaseFilter sub-object at +0x0C with the full renbase.h virtual order) and `CEnumMediaTypes::Next` 0x00CAB160
(amfilter.cpp, /EHsc /GS, vtable cell 0x012BF6C8); STLport `__unguarded_partition` over pair<unsigned __int64,
CCountedPointer<CActiveEntityScriptBase>> with CKeyPairCompareLess 0x00CBB710 (caller introsort 0x00CBD700); libvorbis
`_ov_64_seek_lap` 0x00C8C580 (vorbisfile.c; its statics must be __declspec(noinline)) and `max_seeds` 0x00C93080 (psy.c;
`seed_chase` as `static __stdcall`; reopens the chunk 447 park). Descriptive (libpk / GameCODA have no PDB):
ComputeAzimuthElevation 0x00C599C0, CSoundEvent::Update 0x00C21A50, CAudioPlatform::ReleaseVoice 0x00C4D070 (retail's
two-pointer pool compare came only from an inline CNodePool::operator==). All 10 manifest labels were wrong (the two
renderer rows were "CAudioBank_Stop/Start").
Parked (../../pending/2026-10-27_linux_hand594/differ/): 0x00C6B260 ConvertFloat128BlockToInt (315v315, one register: retail
`mov ecx,[esi+edx]; push ecx` in the scalar fallback, VC7.1 picks eax; both retail asm blocks, frame and flag tests match;
a retail_asm candidate with its `// Retail-asm:` line already written). Next: a float local / register-pressure variant of
the pushed argument, or the 0x00C6B250 HasSSE getter in the same unit.
NAME_FIXES.tsv: 15 rows (CBaseFilter::Pause 0x00CAABE0, CBaseRenderer::ResetEndOfStream 0x00CA7030 slot 25 of vtable
0x0129D08C, ResetEndOfStreamTimer 0x00CA4D60, `__imp__timeKillEvent@4` 0x014404B0, `@vorbis_window@8` 0x00C90C80 (landed as a
CPoolEntryTable method), CBaseFilter::NotifyEvent 0x00CA7930, six CSoundEvent methods, three CNodePool spellings) + 4
coordinator review rows (0x0143BC2C three landed spellings; 0x00C3EA60 GetOutputMode v GetFirstInfo; IsPlayingVirtual J v H;
the landed _vp_tonemask unit's cdecl seed_chase v the `_seed_chase@12` symbol).
Header gaps: no engine headers for CBaseRenderer, CBaseFilter, CEnumMediaTypes (strmbase), CSoundEvent, CSoundPlayer,
CAudioPlatform, CCodaVoice, CNodePool, C3DVector, CCountedPointer; the SHARED_HEADER_AVAILABLE warning for CBaseObject is a
false positive (strmbase's CBaseObject is not the engine's).
Lessons (LESSONS.md): a same-TU static whose caller keeps an extra `add esp,0Ch` is `static __stdcall` (seed_chase);
vorbisfile statics inline unless `__declspec(noinline)` (393v325); the x87 compare form follows the source spelling (`if (z >
x)` gives `fcom st(1); test ah,41h`, `x < z` does not); same-unit noinline stand-ins under `auto_inline(off)` keep ecx/edx
live across calls.

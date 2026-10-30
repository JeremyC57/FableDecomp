# Chunk 594 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A same-TU libvorbis static whose caller pops nothing after the call needs `static __stdcall`: psy.c seed_chase as cdecl
  leaves an extra `add esp,0Ch` in max_seeds 0x00C93080; `static void __stdcall seed_chase` matches.
- vorbisfile.c statics inline at /O2 and inflate the caller (393v325 at _ov_64_seek_lap 0x00C8C580). Mark ov_info,
  _ov_initset, _ov_initprime and _ov_splice `__declspec(noinline)` (with _ov_splice `static __stdcall`) to restore retail's calls.
- A two-pointer object compare with retail's register/frame layout came only from an inline `bool operator==(const T&) const`
  on the pool class. A fixed-member compare in the caller gave a different frame (CAudioPlatform::ReleaseVoice 0x00C4D070).
- The x87 compare form follows the source spelling: `if (z > x)` emits `fcom st(1); test ah,41h; jne`, while `x < z` emits a
  different compare (0x00C599C0); `y < h` gives `fld st(1); fcomp st(1); test ah,5; jp`.
- strmbase CBaseRenderer methods compile with `this` = the IBaseFilter sub-object (+0x0C). They need the full renbase.h virtual
  order (slots 9..32), with SetRepaintStatus, ResetEndOfStreamTimer and Ready inline (0x00CA67B0, 0x00CA68F0).
- Same-TU `__declspec(noinline)` stand-ins under `#pragma auto_inline(off)` let VC7.1 know callee register use and keep
  ecx/edx live across calls, as retail does (CSoundEvent::Update 0x00C21A50).

Review at landing:
- Land only the graded leaves. 00c93080, 00c8c580 and 00c21a50 carry other bodies (stand-ins and TU siblings) in the same TU.
- The landed _vp_tonemask TU (libvorbis_vptonemask_00c93e30.cpp) has a cdecl seed_chase; the landed seed_chase symbol is
  `_seed_chase@12`, so the landed TU should become `static __stdcall` too.
- 0x0143BC2C has many spellings in landed code (g_pCodaSystem as U and V, g_pAudioMaster, ...). Choose one.
- 0x00C3EA60 is spelled GetOutputMode here and GetFirstInfo in landed code. The use here (`== 2` output mode) favours GetOutputMode.
- The landed AcquireVoiceA/B leaves use CPooledVoice/CVoicePool; the evidence here says CCodaVoice/CNodePool (NAME_FIXES rows).
- IsPlayingVirtual returns J (long) here but H (int) in landed code. Both mangle sizes the same; pick one spelling.
- 0x00C6B260 is parked within one register (ecx vs eax in the scalar fallback). It is a retail_asm candidate and the
  `// Retail-asm:` line is already in differ/.
- No row needed more than 4 grades. No local base model sits next to an existing engine header, apart from the CBaseObject
  name clash noted under header debt.

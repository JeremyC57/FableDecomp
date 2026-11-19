# ESCALATE 0x0066A9E0 CTCSoundPlayer::PlayCollisionSound -- binding question, not a spelling question
Residue: site E (the last InternalPlaySoundEventAtPos, 0x0066ABF1..0x0066AC08). Retail: `lea edx,[esp+30h]; push; lea edx,[esp+38h];
push; call [eax+28h] (GetInstantiationName); mov ecx,eax; call 0x00415DD0` -- the conversion temporary built BEFORE the `push 0; push 0`.
- FableWin (ghidra_out/audio_runtime_trace.c @01E7E0A0) shows all five sites as the same implicit conversion (`first_part + pmaterialN->GetInstantiationName()`),
  plus `if (p1.IsNull() && p2.IsNull()) return;` and ASSERT(pmaterial2.Get()) line 0x59 / ASSERT(pmaterial1.Get()) line 0x5E. Ego_d: no local besides bank / first_part.
- Source with the implicit conversion at site E (src: differ/0066a9e0_fablewin_shape.cpp, or the plain draft with the `name` line replaced)
  grades RELOCATION_MATCH masked (0 instruction hunks), but site E calls `??BCDefString@@QBE?AVCCharString@@XZ`, which reloc_bindings maps to
  0x00415D70; retail calls 0x00415DD0 (bound to ?GetString@CDefString@@QBE). c2 inline trace: ??B c1 17 at f 0.1875/0.0938/0.0938/0.0469/0.0234
  -> scores 27/40/40/66/118 -> expanded at A-D, CALLED at E. Plain VC7.1 heuristics reproduce retail exactly if 0x00415DD0 is the conversion operator.
- Under the current binding (decision 2026-11-01 wave 12: 0x00415D70 = ??B, 0x00415DD0 = GetString) the row needs the forwarder expanded and
  GetString called at E only; FORCEINLINE_FORWARDER makes GetString expand at E too (nested site f=1.0, sc 17, 632 bytes), FORCEINLINE_CONVERSION
  would call GetString at A-D too, and `const CCharString& name = ...GetString()` (draft) leaves one frame slot off (Ego_d has no such local).
- Same pattern in the six "implicit-conversion callers" the 2026-11-01 wave noted (0x0065B951, 0x00795300, 0x0078FD60, 0x008A8C60 ...): each needed
  an artificial forced-forwarder opt-in to call "GetString".
Ask: may the binding of 0x00415D70 / 0x00415DD0 be swapped (or 0x00415DD0 bound to the const conversion operator)? If yes the row is matched as is.
If not, which per-site device is acceptable (no inline_depth value separates ??C->Get from ??B->GetString: both depth 2)?

Update (hand1a): test/0066a9e0.cpp (FableWin-shape body, standalone fixture, CHECK not assert; 8/8 mutants killed incl.
the both-null early return, separator, fallback order, sound-enabled gate). Read-only strict vland on a separate payload
(tmp/payload_66.json, log tmp/vland_0066a9e0.log): RELOCATION_MATCH PASS with two warnings only:
RELOC_CONFLICT ??BCDefString@@QBE?AVCCharString@@XZ -> 00415dd0 here, 00415d70 in landed 00e02e50; RELOC_NAME 00415dd0
labelled 'CDefString::GetString'. These are exactly the binding question under review.

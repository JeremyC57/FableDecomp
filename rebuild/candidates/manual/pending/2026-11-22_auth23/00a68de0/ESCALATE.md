# 0x00A68DE0 CVolumeTexture::LoadFromDataStream -- ESCALATE (AUTH23, 2026-11-22)
Best: `p3_00a68de0.cpp` + overlay `hdr/` (patch `hdr.diff`), stock flags: **DIFFER(923v919)**, 43 register-masked lines (`p3.rmd.txt`), frame 0xAC
as retail. Grades used: 4 (p1 939v919, p2 923, p3 923 / 43 lines, p3 under /Oa 946). The recorded draft could not be regraded: it includes
engine/CVolumeTextureFileInfo.h, which never landed, and on this lane a missing #include makes cl.exe hang instead of failing (mini/h2.cpp).

## What is settled (port onto today's headers + Ego_d)
- Text follows Ego_d.pdb's locals and line table of this function: `file_info`, `pf`, `c1`, `locked_box`, `desc` / `hr` in the compressed arm,
  `overflow_check`, `line_length`, `overflow_check2`, `data_length`, `src_buffer`, `dst = &src_buffer[0]`, `src = locked_box.PMem`,
  `stream.Read(src, data_length)`, `memcpy(dst, src, line_length); dst += RowPitch; src += line_length;`, `dst += SlicePitch`. VC10 /Od sizes
  of p3's lines against Ego_d's (`od/p3_od10.txt` v `od/egod_lines.txt`, the three assert lines 646 / 655 / 664 left out): 26 of 34 equal,
  including line 652 = 53 B, which pins `overflow_check = (unsigned __int64)locked_box.Dimensions.X * GetPixelFormat().GetByteColourDepth()`
  (X on the left; the other order is 51 B). Of the 8 that differ, 4 are a near / far jump over an assert body p3 does not have (642, 653,
  662, 683: each exactly +4 or +3), 1 is the prologue (48 v 38) and 3 are the loop headers below.
- **Not matched by size: the three loop headers** (Ego_d 639 / 674 / 676 = 47 / 47 / 44 B; ours 38 / 35 / 35; a count-up cz / cy loop is 38).
  The c1 header is `for (long c1 = 0; c1 < GetNoLevels(); c1++)` in p3 and retail's bytes agree with it (0x00A68EB7, 0x00A6915C), so Ego_d's
  +9 there is unexplained. For cz / cy retail loads each bound once and counts down (0x00A690DF `test edi,edi ; jle ; mov [esp+10h],edi`,
  0x00A690E7 `mov ebx,[esp+4Ch]`), and a count-up loop re-reads its bound every iteration under VC7.1 (p1; also under /Oa, mini/al.cpp),
  so p2 / p3 spell them as countdowns from the box dimension. I did not decode Ego_d's three headers (each is init + 38 B: the S_BLOCK32
  of c1 / cz / cy starts 9 / 9 / 6 B into its line), so these two spellings are fitted to retail and CONTRADICT Ego_d's sizes as they stand:
  a point to rule on, or to decode first.
- New shared headers engine/CVolumeLockedBox.h and engine/CVolumeTextureFileInfo.h (Ego_d TPI layouts), opt-in
  FABLE_RETAIL_CVOLUMETEXTURE_LOADFROMDATASTREAM in engine/CVolumeTexture.h (three declarations; pin_asserts: nothing moved).

## The residue is one thing: retail treats `locked_box` as a local no call can modify
Retail keeps the box's fields as register variables whose home is the box itself:
- X: 0x00A68FB5 `mov ebx,[esp+48h]` after the first GetByteColourDepth, reused at 0x00A69010 `imul ebp,ebx` after three more calls
  (GetLevelDesc, CPixelFormat::Initialise, GetByteColourDepth). Ours re-reads `[esp+48h]`.
- Z: 0x00A69008 `mov edi,[esp+50h]`, reused for data_length and for the cz loop after the vector constructor and the out-of-line
  ReadWithSrcChunkOverflow; on the inline Read's fast path, where `rep movs` clobbers edi, retail reloads it from the box
  (0x00A690BC `mov edi,[esp+50h]`). Ours reads Z again at the loop.
- Y / RowPitch / SlicePitch / PMem are used straight from the box each time (PMem three times around the Read: 0x00A6905D, 0x00A6908F, 0x00A690CA).
- The same fact decides the registers: retail has `stream` in esi and `this` in edi with `this` homed at [esp+1Ch]; ours has them swapped
  (`this` at [esp+10h]). Under /Oa ours takes retail's esi / edi roles, but /Oa also caches the stream's fields across `rep movs`, which retail
  does not (0x00A690A8 re-reads [eax+0Ch] / [eax+14h] / [eax+4]): 946v919, so retail is not /Oa. Retail trusts the BOX, not memory in general.
VC7.1 3077 never does this for a variable filled through the hidden return pointer. Mini TUs (base flags, not grades):
- mini/kill.cpp, k2.cpp, k3.cpp: `B b = Lock(..)`, `const B b`, `const B& b`, `B b(Lock(..))`, an inlined by-value or by-reference parameter;
  class with / without user constructors; a later call on `this`, on another object, a free function, a virtual __stdcall call: every
  real call kills the fields (the value is re-read). Only __allmul does not.
- `B b; b = Lock(..)` and `B t = Lock(..); B b = t;` do give an unaliased `b` (cse.cpp V=9 shows c2 then keeps fields in registers exactly
  as retail does), but with an eager copy of the fields right after the call and a second 24-byte slot. Retail has neither.
So the TLC text must have made the box a non-address-taken local that still receives Lock's result in place. I found no VC7.1 spelling that does.

## Questions for the advisor
1. Is there a known VC7.1 construct in this code base where a by-value class result is consumed in place AND its fields are then treated as
   unaliased (a landed row with `mov reg,[slot+k]` held across calls after a struct-returning call)? CTexture / CSurface rows that call
   Lock() and then loop over the locked rect are the natural places to look; I did not search them.
2. If not: is this the 0x00CF4EC0 kind of terminal (a c2 decision no text reaches), or a NEXT_CODEX_QUESTIONS item (FableWin's code for
   lines 641..683 would show whether the box is read through a copy)?
Not tried (each is one grade): `stream.Read(locked_box.PMem, ..)` instead of `src` (contradicts Ego_d line 673 = 19 B); the non-ORIGINAL_READ_SCOPE
Read body; `X * GetByteColourDepth()` v the other order for line_length (both 32 B in Ego_d; p2 used X first, no change).

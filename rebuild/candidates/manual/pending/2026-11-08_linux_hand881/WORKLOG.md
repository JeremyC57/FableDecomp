# Chunk 881 worklog

## Extents (xdis)
- 0x00C812F0: `ret` at 0x00C843C3, 8-entry case table 0x00C843C4..0x00C843E3, int3 to 0x00C843EF -> true length 12532
  (catalog 12544 counts 12 pad bytes). One function.
- 0x00C843F0: `ret` at 0x00C87FAC, `lea ecx,[ecx]` pad, 8-entry case table 0x00C87FB0..0x00C87FCF -> 15328 (= catalog).

## 0x00C812F0 ResampleStereoFIR
- probe/gen.py generator (mono FIR taps + stereo output stage: `>>= 14`, add to out1, `>>= 1`, madd by {pan,0}, `>> 14`, add to out2).
- First grade (W1 W2 W3, the ResampleMonoFIR wrap set): DIFFER 12516v12532, only case 0 differs (retail calls `+=` from the
  second k=5 tap; ours one later).
- 128-way wrap enumeration (probe/enum1.txt, 7 features W1 W2 W3 W4 W5 W7 OA): 4 RELOCATION_MATCH (W2W3W4, W2W5, W1W2W7,
  W1W3W4W5W7); OA (`*out = __m128i` without the Is32vec4 wrap) does not compile. Chose W2 W3 W4 (= ResampleMonoRampFIR's
  wrap set). src/00c812f0.cpp RELOCATION_MATCH; probe/callchk.py: all 167 call targets in retail order.
- test/00c812f0.cpp: kernel vs scalar reference, 8 increments x 2 pans; PASS g++ and VC7.1; two mutations FAIL as expected.

## 0x00C843F0 ResampleStereoRampFIR
- Decoded the visible spelling from case 7 (where even the dvec ctors run out of inline budget): out-of-line ctors
  0x00C7B840 = Is32vec4(__m128i), 0x00C7B810 = I32vec4(__m128i) (implicit += argument), 0x00C7B970 = Is16vec8(__m128i)
  (`Is16vec8(sum)` madd input and `gain = Is16vec8(adds)`). Block 2: k=3/4 `t = Is32vec4(_mm_add_epi32(t, madd))` (no inner
  wrap), e/f/sum2 unpacks wrapped; block 1: sum unpacks wrapped, b unwrapped. Output stage: `sum >>= 15; sum =
  Is32vec4(_mm_madd_epi16(Is16vec8(sum), gain)) >> 14; *out1 = Is32vec4(_mm_add_epi32(*out1, sum)); gain = ...; sum >>= 1;
  *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum), panGain)) >> 14));`.
- Then only budget-invisible wraps were free: 256-way (enum2), loop/head variants (no effect), /d2inlS (no effect) and
  /d2inlT meters (coarse, no match), dummy-statement calibration (any code anywhere moves both cutoffs: VC7.1 meters the whole
  function's size), row x k subset searches (enum4/5/6).
- MATCH: W3 W4 W7 G + W5a (a's unpacks wrapped) + block-1 `+=` args of window quads in[2], in[3] wrapped in cases 1..7 and
  only in[2]'s in case 0 (probe/keep_match1.cpp RELOCATION_MATCH; also C0k2 / C0k3 variants).
- src/00c843f0.cpp RELOCATION_MATCH (230 calls in retail order, probe/callchk.py); long gains DIFFER 15308v15328 (probe/long1.cpp).
- The three out-of-line ctors graded RELOCATION_MATCH at 0x00C7B840 / 0x00C7B810 / 0x00C7B970 from the dvec.h spelling (probe/ctor_*.cpp).
- test/00c843f0.cpp: kernel vs scalar reference with gain ramp (fade in / out / saturating), 8 increments x 2 pans; PASS g++ + VC7.1;
  two mutations FAIL as expected.
## Done
gall.sh: both RELOCATION_MATCH; tall.sh / tall.py: both PASS; finish.py: 2 authored.

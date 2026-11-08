# Chunk 881 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1's inline budget is metered on the WHOLE function: adding one statement in case 1 moved case 0's split (probe dd2), and
  prologue code in a colder block does not. Callees cut off at different points by size: in 0x00C843F0 `+=` / `>>=` stop at
  case 0, `>>` slightly later, the tiny dvec constructors only at case 7. Two cut-off points need two independent levers:
  wraps whose constructors expand before the first point move both; a wrap placed only in the later cases moves mostly the
  second (0x00C843F0: the first two block-1 `+=` rounds wrapped in cases 1..7, one round in case 0).
- When even constructors go out of line (calls to the out-of-line `Is32vec4(__m128i)` etc.), the call sequence of that last
  part is a direct transcript of the source's wraps and conversions: decode it first (it showed `Is16vec8(sum)` around the
  pmaddwd input, no inner wrap on the second block's third/fourth quads, `*out = Is32vec4(...)` as a 16-byte dword copy), then
  search only the wraps it cannot see. `I32vec4(x)` as a `+=` argument is identical to no wrap (the implicit conversion
  builds the same I32vec4 temp); `Is32vec4(x)` spends budget.
- /d2inlS has no effect on these kernels; /d2inlT moves the constructor cut-off in coarse steps (T72..T75) and never lands
  both points: not a lever here.
- Enumerations at ~1 s per grade (probe/gen.py, probe/genr.py + probe/cs.py/cs2.py call-signature-per-case summaries and
  probe/callchk.py for the call order) are the way to solve these rows; the per-case call signature is a far better
  objective than byte counts.

Review at landing:
- Grade counts above the 150 per-row budget: 0x00C843F0 logged ~1190 grades, almost all automated ~1 s enumerations
  (enum2 256, enum4 210, enum5 105, enum6 64, enum7 420); 0x00C812F0 131 (128 of them one enumeration).
- NAME_FIXES.tsv: respell the landed callers' ResampleStereoRampFIR declaration to short gains (byte-neutral: they push
  immediates) and rename the three landed out-of-line dvec constructor rows 0x00C7B810 / 0x00C7B840 / 0x00C7B970.
- The landed MixFilteredVoice* header comments still call 0x00C812F0 / 0x00C843F0 "inline-asm bodies": all four FIR
  resamplers are compiler-emitted dvec + SSE2-intrinsic C++.
- Both sources model the dvec.h classes locally (hdr/README.md); the ramp's wrap placement is not uniform across cases
  (case 0 wraps one `+=` round fewer) -- the comment says so; no uniform placement exists in the searched space.

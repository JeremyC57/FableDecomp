# Hand chunk 881 (libpk SSE2 stereo FIR resamplers): 2 of 2 byte-exact

## What matched (2)
- **ResampleStereoFIR 0x00C812F0** (catalog "Audio_ConvolveFilter_LongComplex"; /GS; TRUE EXTENT 12532 bytes = code to
  0x00C843C3 + the 8-entry case table at 0x00C843C4; the catalogued 12544 counts 12 int3 pad bytes; one function).
  ResampleMonoFIR's polyphase FIR (two 16-tap blocks per step, output phases 4..7 then 0..3) into a stereo pair: each block's
  sums `>>= 14` are added into `first`, then `>>= 1`, pmaddwd by the {pan, 0} Is16vec8, `>> 14` and added into `second`.
  SHAPE: the inline split (retail calls `+=` from the second k=5 tap of case 0's second block, 0x00C816A3) falls out of the
  ResampleMonoRampFIR wrap set -- `Is32vec4(...)` around the second block's intrinsic sums and around both blocks' final
  unpacks; found by enumerating 128 wrap combinations (4 match; probe/enum1.txt). 167 calls in retail order.
- **ResampleStereoRampFIR 0x00C843F0** (catalog "Audio_ConvolveFilter_LongFull"; /GS; 15328 bytes = the catalog length = code
  to 0x00C87FAC + `lea ecx,[ecx]` + the 8-entry case table at 0x00C87FB0; one function). ResampleStereoFIR with the gain
  ramp of ResampleMonoRampFIR: per block `sum >>= 15; sum = Is32vec4(_mm_madd_epi16(Is16vec8(sum), gain)) >> 14;` added into
  `first`, gains stepped with paddsw, then `sum >>= 1` and the pan stage into `second`. Gains are `short` (long ones flip the
  first lea and move the split). SHAPE: case 7 runs out of inline budget even for the dvec constructors (calls to
  Is32vec4(__m128i) 0x00C7B840, I32vec4(__m128i) 0x00C7B810, Is16vec8(__m128i) 0x00C7B970), which reveals the spelling of
  everything after its first block's `a` reduction; the two remaining budget points (`+=` stops at the last `+=` of case 0's
  first block, ctors stop at case 7's `b` reduction) are placed by budget-only wraps: `a`'s unpacks and the first two `+=`
  rounds of the first block (only the first round in case 0). 230 calls in retail order.

## Parked for Fable 5.1 (0)
None.

## Skipped
None. Extents: 0x00C812F0 12532 (catalog 12544 incl. int3), 0x00C843F0 15328 (= catalog). No extra function starts.

## Label corrections
- 0x00C812F0 "Audio_ConvolveFilter_LongComplex" -> `ResampleStereoFIR` (`?ResampleStereoFIR@@YAXPBFKKKPAJ1G@Z`, as bound).
- 0x00C843F0 "Audio_ConvolveFilter_LongFull" -> `ResampleStereoRampFIR` (`?ResampleStereoRampFIR@@YAXPBFKKKPAJ1GFF@Z`, short gains;
  reloc_bindings and the landed callers 0x00C70C10 / 0x00C70CB0 / 0x00C70D60 spell `...1GJJ@Z`, long gains: NAME_FIXES.tsv).
- Landed out-of-line dvec constructors (NAME_FIXES.tsv, each graded RELOCATION_MATCH at its address from the dvec.h spelling):
  0x00C7B840 CVectorSSE_00c7b840 -> `Is32vec4::Is32vec4(__m128i)`, 0x00C7B810 CVectorSSE_00c7b810 -> `I32vec4::I32vec4(__m128i)`,
  0x00C7B970 CUnprovenSSEVector_00c7b970 -> `Is16vec8::Is16vec8(__m128i)`. Their siblings 0x00C7B7D0 / 0x00C7B930 have no callers
  (probably M128 / I16vec8 copies of the same TU; unproven, left as they are).

## Names
Descriptive names from the landed callers' declarations (libpk: no PDB, no RTTI; Lionhead in-house code, owner rule).

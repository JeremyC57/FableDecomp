# Hand chunk 878 (CEAXManager .. libpk SSE2 resamplers): 2 of 3 byte-exact

## What matched (2)
- **ResampleMonoFIR 0x00C7BAF0** (catalog "Graphics_RenderTextConditional_ProcessRows"; /GS; 10544 bytes = the catalog length,
  code to 0x00C7E3FF + an 8-entry case table at 0x00C7E400, one function). The libpk mixer's SSE2 polyphase FIR resampler:
  per step, two 16-tap blocks (output phases 4..7, then 0..3) of `pmaddwd` products summed and transposed with
  `punpck[lh]dq`, `>> 14` and added into the mix buffer; `increment` 0..7 picks how many rows already use the next window quad.
  Written with the dvec.h class model the landed 0x00C7B7D0..0x00C7B8F0 rows use (Is32vec4 / Is16vec8) plus SSE2
  intrinsics. SHAPE: retail calls `Is32vec4::operator+=` 0x00C7B870 / `operator>>` 0x00C7B8B0 out of line only once
  VC7.1's per-function inline budget is spent (from the last `+=` of case 1's first block, 0x00C7C053); the budget is
  spent by the inlined ctor chains, so explicit `Is32vec4(...)` wraps around every tap product passed to `+=`, around the
  products inside the intrinsic sums, and around the first block's final unpacks place the split exactly; the two named
  reduction sums give retail's `sub esp,2D4h` frame.
- **ResampleMonoRampFIR 0x00C7E420** (catalog "Audio_ConvolveFilter_LongVariant"; /GS; TRUE EXTENT 11980 bytes = code to
  0x00C812CB + an 8-entry case table at 0x00C812CC; the catalogued 11984 includes 4 bytes of int3 padding). The same FIR
  with a gain ramp for fades: each block's sum `>>= 15`, multiplied (`pmaddwd`) by four 16-bit lane gains
  start + k*step, `>> 14`, added into the buffer; the gains advance by 4*step with `paddsw`. SHAPE: the gains are two
  dvec Is16vec8 eight-short constructors (the 16 word stores before the case jump); the parameters are `short` (long ones
  flip the first lane lea to `[eax+ecx]`, as chunk 874 found for the MMX twin); the inline split (case 0 already calls
  `+=` at its last two taps and `>>=`) is set by the explicit-wrap placement found by enumerating the 128 combinations of
  seven wrap features (six of them match; the chosen one wraps the intrinsic sums, both final unpacks and the buffer add).

## Parked for Fable 5.1 (1)
- 00c4f600 CEAXManager::SetEAXProperty DIFFER(9204v9204): only the three integer listener cases ROOM / ROOMHF / ROOMLF (0x00C5137E..0x00C513DC) load `data` into edx, eax, ecx where ours loads ecx, edx, eax (scratch rotation; c2prio --rotation: our pointer is at ecx with ecx free at the ROOM case, retail acts as if ecx were busy; both resync at ROOMROLLOFFFACTOR); 10 differing lines of 1462 | tried: chunk 874's decode, `bool changed = false` + ONE trailing shared `if (changed && !deferred) ApplyListenerProperties` (fixed every register role: md 756 -> 10 lines), operand order, named value, unsigned / const / folded casts, case / default / NONE label placement, hoisted typed pointer, set-type guard shapes, ALLPARAMETERS spellings (80 grades) | next: find the expression temporary or ecx live range before ROOM that shifts the rotation by one without changing code, else lane gap: scratch-register rotation

## Skipped
None. True extents: 0x00C4F600 9204 (as briefed), 0x00C7BAF0 10544 (= catalog), 0x00C7E420 11980 (catalog 11984 counts 4 int3
pad bytes). No extra function starts inside any row.

## Label corrections
- 0x00C7BAF0 "Graphics_RenderTextConditional_ProcessRows" -> `ResampleMonoFIR` (`?ResampleMonoFIR@@YAXPBFKKKPAJ@Z`).
- 0x00C7E420 "Audio_ConvolveFilter_LongVariant" -> `ResampleMonoRampFIR` (`?ResampleMonoRampFIR@@YAXPBFKKKPAJFF@Z`, short gains;
  the landed callers 0x00C70C10 / 0x00C70CB0 / 0x00C70D60 declare long gains: NAME_FIXES.tsv).
- Behaviour found in 0x00C4F600: a non-deferred EAX 3.0 *buffer* property change also re-applies the listener properties
  (retail jumps from the ApplyVoiceBufferProperties call 0x00C5130E to the listener test 0x00C51900 with `changed` still set).

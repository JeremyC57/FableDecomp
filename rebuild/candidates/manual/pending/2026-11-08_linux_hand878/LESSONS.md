# Chunk 878 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1 inline budget (dvec / ivec kernels): which plain-inline operator sites expand and which are called is a per-function
  budget spent in source order, and the inlined constructor chains of explicit class conversions spend it without emitting
  code. Wrapping an intrinsic result as `Is32vec4(_mm_madd_epi16(...))` before passing it to `+=` / an intrinsic moves the first
  out-of-line call earlier with identical bytes elsewhere (0x00C7BAF0: retail's split inside case 1; 0x00C7E420: inside case 0).
  The effect is not additive (wrapping one reduction moved it, wrapping another did not), so enumerate the wrap placements:
  at ~1 s per grade a 128-way sweep is cheap. `/d2inlT` / `/d2inlS` sweeps never reproduced a mid-case split.
- Named reduction locals (`Is32vec4 sum = ...; *out++ += sum >> 14;`) each add a 16-byte frame slot (0x00C7BAF0 frame
  0x2C4 -> 0x2D4, retail's); writing the shifted value into a named local instead shrinks the frame (0x214) and breaks it.
- A `short` parameter used only through truncating word stores keeps the plain `mov` load but makes `a + b` come out
  `lea [a+b]` in parameter order; a `long` parameter gives `lea [b+a]` (0x00C7E420, as 874 found for 0x00C79440). A `long`
  local copy of the parameter also flips it, but the short signature is the real one (it also matches the inline split).
- A flag set to a constant (`mov bl,1`) right before a jump that skips its own test means the flag is read again later:
  look for a branch tail that jumps into another branch's test. 0x00C4F600's buffer path jumps from its apply call into the
  listener's `if (changed && !deferred)`: the source has ONE trailing shared test after the if/else, and with it the zero
  constant stopped living past the memcmp ladder, so every callee-saved role fixed itself (md 756 -> 10 lines).
- The memcmp intrinsic's `xor reg,reg` before each `repe cmpsd` is its own result register, not the function's zero
  constant; a zero constant kept in a register past such a ladder shows as `cmp al,bl` where retail has `test bl,bl`.
- Scratch rotation residues that resync a few cases later (0x00C4F600 listener ROOM..ROOMLF, resynced by ROOMROLLOFFFACTOR
  whose temp is forced off the busy ecx) point at a register being busy at one block entry, not at a temp count before it.

Review at landing:
- Grades above the 150 per-row budget: 0x00C7E420 logged 300 (two automated 128-combination wrap enumerations, ~1 s each,
  with long and with short gains); 0x00C7BAF0 112; 0x00C4F600 80.
- The landed MixFilteredVoice 0x00C70D60 / MixFilteredVoiceFadeIn 0x00C70C10 / MixFilteredVoiceFadeOut 0x00C70CB0 header comments
  call 0x00C7BAF0 / 0x00C7E420 (and the stereo 0x00C812F0 / 0x00C843F0) "inline-asm bodies": they are compiler-emitted dvec +
  SSE2-intrinsic C++ (both mono rows now match). Their ResampleMonoRampFIR declarations need short gains (NAME_FIXES.tsv).
- Both resampler sources model the dvec.h classes locally, like the landed 0x00C7B7D0..0x00C7B8F0 rows; a shared dvec header
  would be a landing decision (hdr/README.md).
- The stereo FIR kernels 0x00C812F0 / 0x00C843F0 are the next members of this family; start from these two sources and the
  wrap enumeration (probe/gen3.py) rather than from asm.
- 0x00C4F600's differ uses the local SListenerBlock that the landed ApplyListenerProperties 0x00C4E3C0 also defines locally
  (no engine header for CEAXManager exists); the landed row spells its parameter `_EAXLISTENERPROPERTIES`, the shared
  header's type is EAX30LISTENERPROPERTIES (same layout; spelling unresolved, no PDB).

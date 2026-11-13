# Chunk 2019 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x006B42F0) A function-local static's mangled scope number (`?name@?4??Func@...`) is set by the scopes opened before it: function body
  ?1?, an unbraced `if` +2, a braced `if` +3, a bare block +1, a `for` +1, an `if / else if` pair +5, an inline call (GFLimit) or a
  ternary 0. Read the retail static's name (reloc_bindings / the atexit stub) and choose the braces and clamp form that give it.
- (0x006B42F0) A struct holding CArrays copied by its implicit operator= copies them bitwise unless vector::operator= is declared:
  define FABLE_RETAIL_STLP_VECTOR_ASSIGN when retail calls `??4?$vector@..` (0x00454886 / 0x0045EF2D) per member.
- (0x006B42F0) Two-register address SIB order (`[edx+ecx+d]` v `[ecx+edx+d]`) in an unrolled copy loop = which of the loop IV and the
  per-store `_M_start` reload temp has the smaller c2 record id (smaller -> SIB index; tools/c2h/sib.py from agent3005). A fresh IV
  record sits above the temps; a record freed earlier (a folded conversion temporary, a CSE'd duplicate before the loop) is reused by
  the IV and flips the order. Natural lever found: the type of a clamp bound -- `GFLimit(x, 0.0f, 1)` (FableWin GFLimit<float,float,int>)
  instead of `(x, 0.0f, 1.0f)`: identical instructions, one freed record. Dead outer locals, loop-variable types and a preceding
  function in the TU do not move it (record ids are numbered per function); ~76 dead statements or ~140 dead names do.
- (0x006B42F0) FableWin's inline `GetSpellEffectInterface()` (not the member pointer read) fixed the register roles around a hidden-result
  call sequence (`push ebp ; lea edx,[esp+38h] ; rep movsd ; ... push edx`) after an earlier struct copy -- accessor v direct read is
  again the first thing to try for a colouring residue (c2036).
- (0x006B42F0) `X sf = obj.GetX(t);` (copy-initialised, hidden result built in place) then member-by-member stores into a second struct
  with a defaulting constructor (CEngineColourFilterSettings) gives retail's temp-then-8-dword copy: the constructor's default stores are
  dead and dropped.

Review at landing:
- Land the bases of the overlay first: pending/2026-11-11_acctB_c2020 CEnvironment.h (my copy keeps its _SUNLIGHTATTENUATOR opt-in) and
  pending/2026-11-12_acctB_c2030 CIDrawEnvironmentParams.h; then this chunk's opt-ins (hdr/README.md). FABLE_RETAIL_CENVIRONMENT_TICKSTATES
  is mutually exclusive with _WEATHERSCALERS and _SUNLIGHTATTENUATOR (they read GameTickState as bytes).
- The GFLimit bound type (`1` v `1.0f`) is the measured lever for the SIB order, chosen among the PDB-attested FableWin instances
  (MMH; MHM gives the same bytes). No source evidence decides between the two.
- NAME_FIXES.tsv: landed 0x006B5FB0 (top-level CThemeChannel ctor symbol), 0x006B5D80 (CBufferPairTriple leaf), catalog 0x006B5B80 /
  0x008857E0 labels, extra wrong bindings of 0x00454886 / 0x0045EF2D / 0x00A14440.
- vland RELOC_CONFLICT `_$E2`: the TU-local atexit stub name is reused by another landed TU (0x01219F00 binds 0x01228930); the landed
  0x01229390 stub row already uses `_$E2` for this address. Inherent to VC7.1 TU-local names, not a wrong call.
- The landed atexit stub row 0x01229390 has an ABRIDGED owner body; this row's TU is the real owner and can replace it.

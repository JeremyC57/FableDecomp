# Chunk 864 lessons (one line each; candidates for docs/pipeline/GOTCHAS.md)

- (chunk 864, 0x0050A650) **Address exposure decides a vector's `_M_start` web shape.** A local vector filled by a declared-only callee is
  address-exposed; c2 then keeps one memory web from the ctor store through `begin()` to the dtor's reload (nothing in the loop kills it:
  only a call that receives an EXPOSED local's address does), and the dtor reload coalesces with the loop iterator (`mov ebx,[esp+38h]`,
  empty edge threaded past it). Give the filling callee a same-TU `__declspec(noinline)` body (retail's templates live in the header) and
  the vector is a promoted local whose web the allocator splits: the dtor reload takes eax as retail. c2prio metric: the `-1` copy
  preference disappears from the `local <begin line>,<dtor line>` candidate.
- (chunk 864) c2 kills exposed-local memory webs only at calls that receive an exposed local's address (Dg/Dh); calls receiving a temporary's
  address (`_M_insert_overflow`'s `const T&`) or a named local whose address goes nowhere else (Y1) do not kill.
- (chunk 864, 0x00D33730) The owner's 2026-11-08 inline control: a single-site ~CTimer expansion/call split that no plain-C++ form and no
  /d2inl flag reproduces closes with `#pragma inline_depth(0)` ... `#pragma inline_depth()` round the one `if (T) return;` statement
  (inline_depth takes effect at the next call after the pragma, so it scopes to one statement's implicit dtor). Line 2 `// Inline-budget:`.
- (chunk 864, 0x00E76E90) The same device does nothing for a block-layout residue of an expanded cleanup block: /d2inlS2/S6/T80 and
  inline_depth(2..4) are byte-identical to the default; smaller settings change the expansion mix. Not an inline residue.
- (chunk 864, 0x00C52BB0) Compare constants do not create const candidate records (const 3/6/7/8 with 2+ compare refs: refs 0); a switch
  lowering's `sub eax,4` does, and a later `== 4` compare links to it, adding the constant to K of both blocks. That pairing is what
  decides the eax/ecx tie of the family-5 Model load; `Model < 4` wins the tie but emits `cmp 4 ; jae`.
- (chunk 864, 0x0085C7D0) When the first N instructions are byte-identical and a later scratch pair is swapped, read `--rotation`: the
  difference is which earlier load went through the rotation path (ours: the argument load, ptr eax -> edx) and which was a hint
  assignment; the probe shows rotation entries only, so a missing entry is the hint.
- (chunk 864, 0x00D3A4A0) `lea edi,[edx+ecx]` v `shl edi,4 ; add edi,edx` for a vector element address is "three rotation temporaries v
  one": c2 targets the `index << 4` chain into the named reference's register; nine binding spellings keep that.
- (chunk 864, 0x005253F0) FABLE_RETAIL_STLP_MAP (visible `map::operator[]`, key only read) makes a `uid` passed as the map key a register
  candidate (retail's uid-in-ebx shape, md 21 -> 13) -- the same mod/ref lever as above, applied to a scalar.
- (chunk 864, probe hygiene) In a trace, an inlined accessor's local can carry the same name as the caller's (`text`): rename it in a
  diagnostic overlay before attributing priorities. odis.py from/to are absolute addresses.
- (chunk 864, lane) P.sh must not `rm -rf` the shared c2runs dir at exit: it kills a concurrent agent's probe run.
- (chunk 864, 0x0050A650 redo) **A shared-header opt-in body must be retail's algorithm, never a stand-in**: a same-TU template body changes only mod/ref for the caller, but the TU also emits the instances as COMDATs, so a stand-in would ship non-retail code; translate the landed instance rows (0x005185F0 / 0x00518900) into the real `<T, Filter, Container>` templates instead -- the caller's bytes are the same either way (2233 / 2233).
- (chunk 864) A helper declared behind one opt-in guard (`ClipAndConvertWorldMapAreaToLocalArea` under CLIPAREA / ROUGHAREABODY) must have every body opt-in that uses it added to the guard, or the body opt-in SRC_FAILs with C3861 from inside the header; reading the guard beats guessing the define name (two wasted grades).

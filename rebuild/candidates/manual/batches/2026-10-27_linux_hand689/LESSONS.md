# Chunk 689 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1 copy-elides a ternary of two class temporaries assigned to a member (`m = c ? T(a) : T(b)`); when retail shows an extra
  copy ctor and both arm temporaries destroyed *before* `operator=`, the source had an inline accessor returning `const T` by
  value (0x00AB1990 `GetBoneName()`).
- VC7.1's inliner expands a medium STLport `_M_insert_overflow` at 2-3 push_back sites or at none. Retail's one expanded site
  plus out-of-line calls elsewhere is reproduced by a `__forceinline` copy of push_back/overflow (`push_back_expanded`,
  `_M_insert_overflow_expanded`) at the expanded site and plain `push_back` + a same-TU plain definition of
  `_M_insert_overflow` for the called sites. Inside the forceinline copy use the `__fill_len` *parameter* in `(max)` so the
  temp store order matches.
- When a row calls an out-of-line STLport ctor whose memory effects VC must know (vector fill ctor 0x00AD30C0), give the TU the
  real body under `#pragma auto_inline(off)` + `__declspec(noinline)`: the caller then hoists the vectors' start pointers and
  the frame slots fall into retail order (0x00AC9D50). A declaration alone leaves reloads and a swapped slot pair.
- Three identical inline helper calls that share one frame slot for the fetched vector (X on the x87 stack, Y/Z in slots) come
  from one `C3DVector p` local passed by reference to a `__forceinline` helper, with user-written copy ctor and `operator=`
  on the pack(1) C3DVector (member-wise float copies, not the compiler's 12-byte memcpy).
- `cmp edi,ebx` where a constant was expected: VC substitutes a register it knows holds that constant (`side == 1` tested
  against the register still holding `1`), so an immediate compare in retail vs. a register compare in the build is a
  register-allocation residue, not a semantic one.
- A quest-script dtor's `map<CCharString,bool>` model needs a default ctor on the `_Rb_tree` model or the implicit ctor the
  TU must define for the vtable fails (C2512); `vector<bool>` in retail's STLport is the plain 12-byte vector.
- A `pair<const long, CRegion>` node destructor needs the full `engine/CRegion.h` (C2079 otherwise), even though the dtor
  itself only calls `_M_erase`.
- The DirectShow wxutil.cpp llMulDiv matches only from the SDK text with USE_LARGEINT *undefined*; the `/` `%` pair on a DWORD
  divisor becomes a single `_aulldvrm` by itself. The two-byte residue of probes 1-2 was the `bSign` bookkeeping, not the maths.
- CEaxSource::SetAllParameters does not range-check flExclusionLFRatio (retail bytes): keep the asymmetry, do not "fix" it.

Review at landing:
- 0x00613AE0's correct symbol uses `CHeroLogBook::CHeroLogBookEntry::ECategory` (the PDB member is `CategoriesList`); my source
  comment calls the map `map<ECategory, CComponent*>` with a local stub enum -- the NAME_FIXES row has the full mangling.
- 0x00606F80 and both quest dtors restate `CLevelScriptBase`/CTCInventoryBase layouts locally (header debt, NOTES.md); the
  verifier should warn, not reject, since no engine header for those classes exists in the compilable form needed.
- 0x00D3AFF0's landed file (`CLandscapeBackgroundPatch` vector deleting destructor) is misattributed: it is the
  CQ_FireHeartScript scalar deleting destructor (NAME_FIXES.tsv).
- The two parked rows are at 812v812 (operand order) and 782v811; the 0x00A73E50 differ/ file is the 782-byte probe 11 form.

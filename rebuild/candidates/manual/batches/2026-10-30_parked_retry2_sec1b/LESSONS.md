# retry/sec1b lessons (one line each; candidates for docs/pipeline/GOTCHAS.md)

- Retail keeps a counted pointer / def word / key in a register across a call to a template instantiation (vector
  `_M_insert_overflow`, map `operator[]`, `GetDef<T>`, `GetSubDef<T>`, `LowerBound`): give that template a same-TU
  `__declspec(noinline)` body inside `#pragma auto_inline(off)` that only reads its inputs (writes through the out
  pointer); VC7.1 then reproduces retail's register / scheduling / slot sharing (5 matches: 0x0054B4B0, 0x0054DF50,
  0x005DD090, 0x005A45F6, 0x005A731F; big gains on 0x004085F0, 0x005CBC36). It did nothing for non-template callees.
- Check the extent before hand work: a catalog length that runs past a tail-call `jmp` or `ret` into int3 padding or an
  unpadded size-TU neighbour is an over-span (4 rows); grade over the true extent, BOUNDS_FIX.tsv + oracle_extra.tsv
  (and drop the stale row from oracle_extra.tsv: q3 takes the first hit).
- `optimize("s",on)` alone gives the biased `lea ebp,[esp-N]` frame; the parked `("y",off)` turns it into `mov ebp,esp`.
- A `(this, thing)` callee-saved swap can come from where the thing's members are first addressed: reference locals
  taken at function entry gave retail's ebx/ebp map (0x004AFBB0).
- A by-value 8-byte struct (C2DVector) or a one-word pointer wrapper is returned in edx:eax / eax unless it has a user
  ctor or copy ctor; retail's hidden result pointer needs one (C2DVector() {}, CDefPointer copy ctor).
- A dtor on a local adds EH state to a TU whose retail has none: when retail releases by hand, model the release by
  hand (0x005C7452).
- Decode a fastcall operator+ chain by its stack: the right operand is pushed before the left side's calls run, so the
  first pushes belong to the last operator+ (0x0055D180: GetPhotojournalDir() + GetValidProfileName(...) + L"\\" + name
  + L".jpg").
- An inlined class dtor that stores a vtable, calls Close and destroys a member string means the class is the PDB
  layout with a real base (CDiskFileWin32 : CBaseClassNonCopyable, 0x1C bytes); the generated flat header cannot do it.
- Retail calling a getter twice and dropping the first result is in the source (an unused named result keeps the
  calls); do not fold it (0x005590C0).
- A branch whose rare arm is the fall-through wants that arm written first (0x0061AA80: 84 -> 15 lines).
- Behaviour tests are compiled by the verifier with VC7.1 `/Od`: C++98 only, and VC7.1 does not zero `T()` for a class
  with non-POD members.
- Read string literals and float constants from the binary (rdis-style section read) before writing them; guessed
  literals compile to the same relocation but land wrong text.

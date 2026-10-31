# Chunk 624 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `__uninitialized_fill_n` / copy loops over intelligent-pointer records: retail's register assignment (cur in ebx,
  x in edi) needs the `__forceinline` CIntelligentPointer copy constructor to NAME the item:
  `CBaseObject* item = other.GetPItem(); SetPItem(item);`. The unnamed `SetPItem(other.GetPItem())` swaps the two callee-saved
  registers. This fixed 0x00722BD0, 0x00792550, 0x007979B0 and 0x00806C80, plus chunk 622's 0x007AF940, all of them parked
  earlier as a "callee-saved register tie lane gap". The virtual destructor alone does nothing.
- A 16-byte frame holding a 12-byte zero vector with 4 dead bytes is an inlined helper returning `std::pair<C3DVector,long>`
  by value (the long folds into an index). C3DVector needs its user copy constructor, or VC7.1 scalarises the pair and the
  frame shrinks to 0xC (0x00771D50 GetCrowdPos via GetBestVacantPositionForThisPos).
- A `setle dl ; dec edx ; and eax,edx` clamp (result in the VALUE register, stored before the next load) is a BY-VALUE
  `Max(T a, T b) { return a > b ? a : b; }` call `Max(x, 0L)`. Ternary/if/explicit-mask spellings and by-reference std::max
  give `and edx,eax` plus a hoisted load (0x009FD660, twin 0x009FD3A0).
- Under `#pragma optimize("s")`, an explicit-specialisation member defined `inline` (CPersistTraits<CCharString>::TransferBinaryIn)
  is called out of line: mark it `__forceinline` when retail inlines it.
- A `const T&`-returning world accessor followed by `->` on a counted-pointer member gives `add eax,74h ; mov eax,[eax]`.
  Read `.Data` into a named pointer local to get retail's `mov eax,[eax+74h]` (0x006817F0).

Review at landing:
- 0x007AF940 (chunk 622, parked) and 0x009FD3A0 (retry1, parked) match with this chunk's levers: sources in extra/.
  They still need tests, and 0x009FD3A0 needs UpdateAvailableMemory made private.
- 0x00431143 / 0x004310A7: name_review. The nameless `Transfer<T>(T&)` has no PDB symbol (the PDB overloads take a name).
- NAME_FIXES.tsv: the landed 0x009FC070 is non-const (PDB const QBEKK). 0x009FD660 also carries a wrong binding,
  `CGraphicDataBank_009f8200`.
- The CHUNK_BRIEF template names `$S/batch/chunk602_oracle.tsv` for vland. This chunk's oracle is `chunk624_oracle.tsv`.
- Header debt: engine/CTCCreatureOpinionOfHero.h (Dinkumware offsets) and engine/CDataInputStream.h (flattened vptr).

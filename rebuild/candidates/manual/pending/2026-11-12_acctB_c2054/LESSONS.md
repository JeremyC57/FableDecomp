# Chunk 2054 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00A7AF60) Before forcing an inline decision on an STLport helper, grade the row TU's OWN out-of-line copy of that helper
  against its retail address (`q.py <helper addr> <row.cpp> "<helper symbol>"`, free). Here std::__copy<CNavigationLayer*, ..> came
  out 68 bytes against retail's 97 (0x00A85870): the shared vector has no operator= unless FABLE_RETAIL_STLP_VECTOR_ASSIGN is
  defined, so a struct of vectors is assigned with a bitwise `rep movsd`, the helper is too cheap (score 72 < 74), expands into
  erase, erase is then called and the caller's frame grows (0xF4 v 0xE4). With the opt-in the helper is retail's and VC7.1 makes
  retail's call / expand split by itself. Any TU that copies or erases elements holding vectors needs FABLE_RETAIL_STLP_VECTOR_ASSIGN.
- (0x00A7AF60) Whole-object model check: compare EVERY function the row object emits that has a known retail address with the exe
  (tmp/sibs.py in the hand-back, one compile). 39 of 46 helper bodies of this TU equal retail (copy ctors, destroy / uninitialized
  helpers, resize, insert_unique, _M_erase, the inline ~CCachedFile, GFProcessBreath, the CNavRegion ctor); a DIFF there points at a
  wrong shared text before any row grade is spent.
- (0x00A7AF60) A counted loop whose guard is one `xor ; test ; jbe` but whose body sits at HALF the static frequency a plain `for`
  gives (an implicit copy constructor called, not expanded): `{ unsigned long i = 0; if (n != 0) { for (; i < n; ++i) ...; } }`.
  VC7.1 folds the `if` into the loop's rotated guard only when the counter is initialised ahead of the `if`; `for (unsigned long
  i = 0; ...)` inside the `if` keeps two tests and pads the loop head.
- (0x00A7AF60) An inline-expanded constructor shows its member-initialiser order in the store order even when the out-of-line copy
  does not: `: MostRecentSearchID(0) { RegionID = ...; }` stores the zero first in the expansion (0x00A7B07D) while the out-of-line
  0x00A78680 bytes are the same for all three texts.
- (0x00A7AF60) `CCachedFile` has an inline virtual destructor (`{ Close(); }`): the same function calls the out-of-line copy
  0x00826380 on an early exit (frequency 0.5, score 101) and expands it at the final exit.
- (0x00A7AF60) `/Gr` evidence for a TU: the undecorated STLport helpers (`__destroy_aux`, `__copy`) are called with ecx / edx; without
  the flag they become cdecl pushes (2519v2503).

Review at landing:
- The row took 34 full-row grades over three sessions (9 + 13 scout + 12); grades.log has one line each.
- Respell the landed 0x00A78680 CNavRegion::CNavRegion to the row's text (`: MostRecentSearchID(0)` in the init list, RegionID in
  the body) so both TUs share one constructor text; that text grades RELOCATION_MATCH against 0x00A78680 (scout/probe/
  a78680_initlist.cpp and the row TU itself).
- NAME_FIXES.tsv: eleven stale spellings (descriptive class / placeholder names bound by landed rows). The landed 0x00A76340
  "SetBlockedNode" models four globals that are members of CNavQuadTree::BlockedNode (0x013D2D2C + 4 / 8 / 0xC / 0xD).
- Shared-header texts that differ from retail's copies in this TU (not used by the row's bytes, both are called out of line, but
  worth a look): CAFile::ReadVar<int> (129 bytes here v retail 0x00516180) and vector<T>::_M_fill_insert (the CNavigationLayer
  instance is 340 bytes here v 342 at 0x00A85CC0: retail pushes a null distance pointer for __copy_backward; the pointer instance
  differs from 0x00A81890 too). The landed rows of those addresses match with their own local models.
- hdr/ still contains chunk 2051's overlay files (untouched); only the five files listed at the top of hdr/README.md belong to 2054.
- vland prints two RELOC_NAME warnings (0x00A85870, 0x00A83420): the catalog labels are the demangled spelling of the row's symbols.

# 0x00607330 CTCInventoryMap::ConstructLogbookList -- author session 1 (2026-11-12)
Best: src/v18.cpp = differ/00607330.cpp, DIFFER(9232v9124), 71 hunks after masking esp offsets (mask.py). Calls all match except the
CDeletion pair (retail: ~list 0x0042ABCA called at site 1 only, _M_insert_dispatch 0x0042B4C3 called at both sites).
What moved it: full draft from OUTLINE (9312) -> map copy out of line (STLP_MAP_COPY_OUTOFLINE) -> list dtor out of line +
dispatch noinline overlay (9244) -> vector size() as end()-begin() overlay (start loaded first, as retail) -> CDefPointer release
SELFDELETING_POINTEE + CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES (as 4007; no release-count spill) -> `*book_it` instead of a `book`
local (9232) -> book iterator declared before `if (!books.empty())` (retail stores it before the guard).
What did not move: named CRightHandedSet local; book declared outside loop; RELEASE_COUNTREF (GetDef then called out of line);
CDELETION_INLINE (byte-neutral); CLEAR_OUTOFCLASS; vector size via named locals (same regs as end()-begin()).
## session 1 hand-back (grades ~40, probes 3)
Residue (best v18 = differ/00607330.cpp, DIFFER 9232v9124, 71 masked hunks):
1. CDeletion pair: retail site 1 (books.empty block) CALLS ~list 0x0042ABCA, site 2 (category loop, else arm) EXPANDS it; both call
   _M_insert_dispatch 0x0042B4C3. Our plain-inline ~list expands at both; DTOR_OUTOFLINE (used in v18) calls at both (site-2 local
   then lives in memory: `_deletion$13039 = -176`, 8 of the 16 extra frame bytes, /FAs listing). PROVEN: an extra branch level in
   front of site 1 (v28: `if (arrow) if (categories) {...}`) flips ~list1 to a call with dispatch called at both = retail's call
   set (cd.py clean) -> VC7.1 static-frequency inline decision; site 1 is one branch level colder in retail. Tried and NOT it (no
   change): condition-declarations, split `if (entry)` tests (folded before the inliner), `begin()==end()`, `size()==0`,
   `!= 0` spellings, ~list(){clear();} (FableWin ~list 19i/64b; ours 20i/72b), CDELETION_INLINE, CLEAR_OUTOFCLASS.
2. Frame 0x11c v 0x10c: /FAs layout (src/00607330.asm) -- top region matches in size (viewport -64, details_text -60, two 14-byte
   CEngineGraphic temps + 24-byte CRightHandedSet temp; retail puts RHS below the EG temps, ours above); the rest is site-2
   deletion (8) + 4 bytes + an alignment gap at [esp+0Ch].
3. size(): retail loads _M_start first (`mov eax,[ecx+14h]; mov edx,[ecx+18h]; sub edx,eax`, final loop `sub ecx,edx; imul ecx`);
   default text `finish-start` loads finish first; end()-begin() overlay (SIZE_ITERATORS, in v18) loads start first but into the
   wrong register (`sub eax,edx; mov edx,eax`). FableWin says vector::size is 12i/26b = the DEFAULT text, so the overlay is
   probably wrong text; the call-site spelling is unknown.
4. Book loop: retail keeps `book` in memory ([7Ch], read for every fastcall) and caches manager in ebx; a named `long book`
   gets ebx in ours; `*book_it` everywhere (v18) is closer but reads through the iterator.
Best next idea: find the natural extra branch level before site 1 (e.g. how the original guarded the books list: a separate
`bool` / `if` on HasTC that the optimizer merges), then drop STLP_LIST_DTOR_OUTOFLINE (v29 base + that spelling); re-check frame.
Headers (hdr/, README.md): CTCInventoryMap LOGBOOK (CategoriesList retype + decls), CTCInventoryItem STARTINGRHSET,
CHeroLogBook CHEROLOGBOOKENTRY_ACCESSORS (GetAbbreviatedName/GetContent/GetLogEntries, FableWin names), fable_containers
(LIST_INSERT_DISPATCH_NOINLINE, VECTOR_SIZE_* probes, LIST_CLEAR_OUTOFCLASS, LIST_DTOR_CLEARS probe), NUISystem_CDeletion INLINE probe.
Names: no landed name proven wrong.

## session 2 progress (attempt 2)
Best: src/ze.cpp = differ/00607330.cpp, DIFFER(9124v9124), only `lea esi,[eax+ecx]` v retail `[ecx+eax]` at 0x006090E7 differs.
What moved it: (1) site-1 arrow guard `if (entry && arrow)` (redundant entry test folded; VC7.1 then calls ~list at site 1, expands at site 2 = retail; no DTOR_OUTOFLINE);
(2) GetSubDef<T> body visible + noinline (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE + #include engine/CDefClassBase_GetSubDef.h): appearance kept in edi;
(3) `long book = *book_it;` local; (4) CDefPointer RELEASE_POINTEE (not SELFDELETING); (5) THE FRAME: books title/back/books_entry in a SEPARATE
`if (!books.empty())` block after the book loop (scope-based slot packing; 0x11c -> 0x10c); (6) category loop menu entry uses entry.GetAbbreviatedName()
(retail copies entry+4); (7) named `CRightHandedSet orientation` local; (8) VECTOR_SIZE_ITERATORS no longer needed (default size()).
Not it for the lea: index spellings (begin()+(long)i, begin()[i], i+begin(), (*it).second[i]), const_iterator, entries ref (worse), i++/i scope, const& source, 3-arg ctor (worse), size()>i (worse), _M_start[n] operator[] (worse).
Lea residue findings (session 2): the order is NOT set by the category-loop spelling (20+ spellings, all [eax+ecx]); it follows the
site-1 inline decision: with ~list EXPANDED at site 1 (plain `if (arrow)`, p7/s1/t2) the lea is retail's [ecx+eax] but site 1 is then
inline (9140, 6 hunks); with ~list CALLED at site 1 (`entry && arrow`, or DTOR_OUTOFLINE) the lea is [eax+ecx]. Retail has both
"called" and [ecx+eax]: the real cold site-1 spelling is still unknown. Not it: split if(entry), ternary entry, && assignment,
size()==0 / begin()==end() / size()<1 with plain arrow, arrow&&entry / arrow!=0&&entry!=0 / if(arrow) if(entry), CDefPointer plain
inline, dead size() statements, CDELETION_INLINE, CLEAR_OUTOFCLASS, GETSUBDEF_BODY spelling, i declared elsewhere, const_iterator.
## session 2 RESULT: RELOCATION_MATCH (src/final.cpp = differ/00607330.cpp; test src/test.cpp PASS; strict regrade RELOCATION_MATCH; addmatch done)
What made it match (on top of the progress list above): the last residue (`lea esi,[eax+ecx]` v `[ecx+eax]` at 0x006090E7, a
pure IL-order effect insensitive to every category-loop spelling) flipped when the viewport block was written with EIGHT named
locals (`CComponent* basics_desc = categories->FindChild("UI_LOGBOOK_BASICS_DESC"); if (basics_desc) ...AddFakeChild(basics_desc);`
...) instead of one reused `desc`. Byte-neutral elsewhere; it changes c2's symbol numbering.
Fragile: the match also needs CDEFPOINTER_RELEASE_SELFDELETING_POINTEE + CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES (the default
release gives the same bytes except that lea) and the site-1 guard `if (entry && arrow)` (folded; makes VC7.1 call ~list at site 1).
Headers: hdr/ overlay (README.md). Flags: none. Row-local: the inline CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*)
template body (same text as CDefinitionManager.h FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY but inline; retail expands it).
Made-up local names only (descriptive): basics_desc..books_desc, books_list, books_as_list, book, orientation, abbreviated_name.
Names: no landed name proven wrong.

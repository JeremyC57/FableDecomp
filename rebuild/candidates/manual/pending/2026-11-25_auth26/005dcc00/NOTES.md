# 0x005DCC00 CAnimationSet::GetRandomAnim (AUTH26)

Grade: RELOCATION_MATCH (264 B), also with LANDVERIFY_STRICT_HEADERS=1. Grades used: 7 (g0 baseline, g1..g6) + final.
Test: 005dcc00_test.cpp links the compiled row object (vt.py PASS); 10 cases. Mutation controls: `==` -> `<=` on the key (killed by
case J), dropping the anim_to_ignore assignment (killed by C/H), dropping the end() test (killed by F: key 7 continues past end()).
Name gate: FAIL(2) only on the two pick-any helpers whose LANDED rows carry wrong spellings (NAME_FIXES.tsv; see below).

## Route
1. g1: ported onto Ego_d's shape (locals anim_to_ignore_iterator, found_anims, index, start_it; `it` in the for scope; two returns).
2. g2: `it` must not be reloaded after the _M_insert_overflow call and retail homes it over the dead `name` slot: visible out-of-line
   overflow body (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE, the sibling 0x005DCB00 rule).
3. g3: found_anims._M_finish kept in esi across the loop: visible out-of-line reserve (FABLE_RETAIL_STLP_VECTOR_RESERVE). 264v264,
   only the prologue order differed.
4. g4: retail zeroes the vector before reading Anims.end(): anim_to_ignore_iterator declared first without initializer and assigned
   after the vector (Ego_d line 1007, 19 B between the vector ctor 1006 and reserve 1008). RELOCATION_MATCH.
5. g5: the reserve pick-any copy had the plain body (148 B v retail 179 B): the STLport _vector.c form
   (FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY + FABLE_RETAIL_STLP_VECTOR_C, the landed 0x0077E580 owner set). Leaf still
   RELOCATION_MATCH; retcmp: reserve 0x005E15E0 179 B EQ relocs EQ, _M_insert_overflow 0x005E1500 216 B EQ relocs EQ; GFRandom
   0x004CDAE0 and placement new 0x00405C10 copies EQ.
6. g6 (final): g4's declare-then-assign was a fit. FableWin.exe's /Od body of this function (0x01FA1A80) gives the real
   statement order: ASSERT(AnimExists(name, filter)) (assert line 0x3E9 = 1001.., the 200-byte Ego_d line 1004), the vector ctor
   (EH state 3), then anim_to_ignore_iterator CONSTRUCTED from Anims.end() (hidden-return into [ebp-3Ch], EH state 4), reserve(5),
   start_it, the for loop with anim_entry a pointer ([ebp-4Ch] = &it->second; Ego_d type 0x19676 is LF_POINTER), the empty case
   with ASSERT(anim_to_ignore_iterator != Anims.end()) before its return, then index = GFRandom(size, &seed) ([ebp-1Ch]) and
   return found_anims[index]->second. Written that way (asserts via FABLE_RETAIL_ASSERT, ((void)0) by default, byte-neutral):
   RELOCATION_MATCH. Ego_d's S_BPREL list is ordered by frame offset, not declaration (FableWin offsets are identical).
No header changes (all opt-ins already exist).

## Names (two sources each)
- Self ?GetRandomAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@AAKABVCAnimFilterBase@@J@Z: Ego_r.pdb and FableWin.pdb
  publics (identical spelling); Ego_d.pdb S_GPROC32 CAnimationSet::GetRandomAnim (params name, seed, filter, index_to_ignore).
- FindFirstAnimWithName 0x005DC020 ?FindFirstAnimWithName@CAnimationSet@@QBEPBU?$pair@KVCAnimationEntry@@@std@@ABVCCharString@@@Z:
  Ego_r / FableWin publics (VC10 iterator spelling = STLport const pair*), landed row 0x005DC020 + reloc_bindings.
- GFROR13 0x00497890 ?GFROR13@@YIJK@Z: Ego_r / FableWin publics; tracker pdb_spelling (the other landed spelling YIKK is the
  existing split, not added by this row).
- free 0x00BFEA14: CRT (agree_std).
- vector<const pair<unsigned long, CAnimationEntry>*>::reserve 0x005E15E0 and ::_M_insert_overflow 0x005E1500: element type from
  Ego_d (found_anims type 0x3AEEC in both callers GetRandomAnim / GetRandomLoadedAnim) and Ego_r.exe call targets (both callers
  call only the _Vector_const_iterator<pair<unsigned long,CAnimationEntry>> reserve instance). The landed rows at those addresses
  spell vector<const CAnimationEntry*>::reserve (wrong: that instance's overflow is 0x005E1420) and a CUnprovenElement stand-in:
  NAME_FIXES.tsv. The gate reports these two as split FAILs until the coordinator respells the landed rows.
- Parameter / local names: Ego_d (index_to_ignore, anim_to_ignore_iterator, found_anims, index, start_it, it, anim_entry).

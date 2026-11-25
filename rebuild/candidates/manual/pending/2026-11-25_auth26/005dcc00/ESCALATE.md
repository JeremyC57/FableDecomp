# 0x005DCC00 CAnimationSet::GetRandomAnim: ESCALATE (name gate only; the bytes and the test are done)

State: RELOCATION_MATCH (264 B), also with LANDVERIFY_STRICT_HEADERS=1; behaviour test PASS linked against the row object
(3 mutation controls killed); NAME_GATE FAIL (2). Source: 005dcc00.cpp (= g6, FableWin.exe /Od statement order, no fitted text).

## The two FAILs are landed rows' wrong self spellings, not this row's
The row's object emits (and calls) two out-of-line pick-any copies, both byte- and relocation-EQ with retail (retcmp):
- 0x005E15E0 `?reserve@?$vector@PBU?$pair@KVCAnimationEntry@@@std@@V?$allocator@PBU?$pair@KVCAnimationEntry@@@std@@@2@@std@@QAEXI@Z`
  (179 B). The landed row 00/5e/stdvectorconstCAnimationEntry_reserve_005e15e0.cpp spells it
  `?reserve@?$vector@PBVCAnimationEntry@@...` ("Identity (probable)" in its own header). Gate: split_fixable, because that
  landed string is an Ego_r public -- but of a DIFFERENT Ego_r function (0001:7493920), not of this address.
- 0x005E1500 `?_M_insert_overflow@?$vector@PBU?$pair@KVCAnimationEntry@@@std@@...@IAEXPAPBU?$pair@KVCAnimationEntry@@@2@ABQBU32@ABU__true_type@2@I_N@Z`
  (216 B). The landed row 00/5e/stdvector_Minsertoverflow_005e1500.cpp has the stand-in `vector<CUnprovenElement_005e1500*>`
  (tracker: standin T3). Gate: split_unknown.

Evidence for the element type `const std::pair<unsigned long, CAnimationEntry>*` (= VC10's
`_Vector_const_iterator<_Vector_val<pair<unsigned long,CAnimationEntry>>>`, CVectorMap's const_iterator), two sources:
1. Ego_d.pdb locals: the only callers of 0x005E15E0 (E8 at 0x005DCC24, 0x005DCD2D, 0x005DCD44) are GetRandomAnim and
   GetRandomLoadedAnim; both declare found_anims (and found_loaded_anims) as type 0x3AEEC =
   `std::vector<std::_Vector_const_iterator<...pair<unsigned long,CAnimationEntry>...>>`.
2. Ego_r.exe code: GetRandomAnim (0001:2340368) and GetRandomLoadedAnim (0001:2340704) call only the reserve instance
   `?reserve@?$vector@V?$_Vector_const_iterator@...pair@KVCAnimationEntry...` (0001:8337216), never the
   `vector<const CAnimationEntry*>` instance (0001:7493920).
3. Retail: the const CAnimationEntry* vector's overflow is a different address, 0x005E1420 (called by
   GetAnimsWithNameStartingWith 0x005DCB00 and that vector's push_back 0x005E1ECB); 0x005E1500's callers are 0x005DCC9C
   (this row), 0x005DCDB8 / 0x005DCE0D (GetRandomLoadedAnim) and 0x005E1F1A (the pair-pointer vector's out-of-line push_back
   0x005E1EF0). Retail has no ICF, so one address = one instance.

## What is asked
Two self-leaf respells of LANDED rows (NAME_FIXES.tsv lines 1-2): 0x005E15E0 and 0x005E1500 onto the spellings above.
- reloc_bindings.tsv has no line at either address (no landed caller binds them), so no callers move; the only landed spellings
  are those two rows' own `// Leaf:` lines.
- After the respell this row's object defines both functions as pick-any COMDATs under the corrected names, as the owner rows
  will: link-check owner + row together after the respell (GOTCHAS nf135 / nf402 LNK2005 lines) before landing this row.
- payload.json / oracle.tsv / MATCHES.tsv hold the row already, but it must NOT land before the two respells (landing first
  creates exactly the two splits the gate reports).
Then rerun: `FABLE_HDR_OVERLAY=$S/w22/auth26/hdr python3 $S/tools/name_gate.py 005dcc00 $S/w22/auth26/005dcc00/005dcc00.cpp
'?GetRandomAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@AAKABVCAnimFilterBase@@J@Z' --test $S/w22/auth26/005dcc00/005dcc00_test.cpp`
-- expected NAME_GATE PASS (the other four names are OK today).

## RESOLVED (2026-11-25, coordinator: finish as one unit)
AUTH26 claimed 0x005E15E0 / 0x005E1500 and respelled both rows onto the shared header (unit_005dcc00/). With those two leaves in
place the row's gate prints NAME_GATE PASS (unit_005dcc00/NAME_GATE_005dcc00_after.txt). The gate reads landed self leaves from
the tree, so the new spellings, by name:
- 0x005E15E0 ?reserve@?$vector@PBU?$pair@KVCAnimationEntry@@@std@@V?$allocator@PBU?$pair@KVCAnimationEntry@@@std@@@2@@std@@QAEXI@Z
- 0x005E1500 ?_M_insert_overflow@?$vector@PBU?$pair@KVCAnimationEntry@@@std@@V?$allocator@PBU?$pair@KVCAnimationEntry@@@std@@@2@@std@@IAEXPAPBU?$pair@KVCAnimationEntry@@@2@ABQBU32@ABU__true_type@2@I_N@Z

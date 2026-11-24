# Landing nf242 (Account A, 2026-11-22): the CModeSitBench unit with the CModeBase vtable slot fixes (12 rows, one unit)

Agent NF242. Hand-back: `pending/2026-11-22_nf242_handback/` (q12678 step 1 drafts in `q12678_step1/`). q12677 done; q12678 step 1 ready
but held (below); q12679 / q12676 / q12659 not started.
- **Recheck** (`logs/recheck.log`, landed tree): 12 rows, 1 MATCH + 11 RELOCATION_MATCH, every test PASS, 0 RELOC_CONFLICT.
- **Rows:** nf240's held 0x00740040 (CModeSitBench ctor, NCreatureMode_CModeSitFloor.h `_IMPLICIT_DTOR`: its ??_G equals 0x007400E0; new
  linked test with stubs for its 25 undefined symbols, slot 1..11 dispatch and delete order), 0x00739E70 GetMovementSpeed, 0x00739E90
  IsAllowingPlayerSpecialMovementMoves; and the nine emitters 0x008E3720, 0x008E31A0, 0x008E37C0, 0x00741390, 0x00740420, 0x008AC4D0,
  0x0073B0D0, 0x0073AEA0, 0x00743FE0.
- **Why nine more:** mapping every vtable slot onto retail by RTTI (`tools/vtmap.py`) showed the nine local CModeBase models swap slots 9 /
  11 AND lack CModeStandBase's own overrides, so the swap alone would have put the names at wrong addresses. In place, no layout change:
  the slot 9 / 11 names swapped in all nine; seven also define the existing `FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE` (as landed
  0x00743600 / 0x008E3A20); 0x00743FE0 declares its class's AnimUpdate / IsRemoveOnDeactivate overrides (three PDBs; retail slots 2 / 11 =
  0x008D6850 / 0x00743CB0). Each of IsAllowingPlayerSpecialMovementMoves, IsRemoveOnDeactivate and GetMovementSpeed now names one address;
  no address gains a spelling (0x00739EA0 2 -> 1, 0x0073DA00 3 -> 2, 0x00861900 3 -> 2, 0x00862EC0 2 -> 1). A HEAD census of 134 other
  possible emitters found only the two landed siblings binding these names, at the right addresses.
- **Header:** nf240's `FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_IMPLICIT_DTOR` (add-only); equivalence over its 22 includers 0 differ.
- **Pick-any / link:** the one NEQ is `??_GCAnimFilterBase` v 0x0041CCE4 (nf240 ruling 3: debt, as in 0x00743600); a 30-object link gives
  the same 5 LNK4006 as HEAD and 0 LNK2005. Table: the CModeSitBench vtable line onto `??_7CModeSitBench@NCreatureMode@@6B@`.

## HELD: q12678 step 1 (CIEngine.h PDB declarations as the default)
Ready (four declarations default, 4 asserts pinned, ten tests respelled; 108 rows differ in equivalence, 107 recheck clean), but
0x0058578B CTCInventory::OpenInventory (8,081 B) goes RELOCATION_MATCH -> DIFFER on a two-instruction order swap at 0x00585D7A. The PDB
declarations themselves cause it (HEAD header + the four existing opt-ins as /D flags gives the same swap: `q12678_step1/v578b.txt`).
Ruling 3: bisect the four opt-ins on 0x0058578B first; retire whichever do not move it, and hold the one that does for authoring.

## Coordinator rulings on NF242's questions
1. 0x00743FE0's two override declarations in its header-less local class: accepted (three PDBs, retail slots, no layout change).
2. The 0x00740040 test devices (never-constructed CBasicStringProbe accessor, a logging global operator delete, two test-only opt-ins):
   accepted for a test.
3. q12678: (b) bisect first (four compiles), then decide.
4. The 16 off-address CModeBase slot bindings (slots 3-6) in five of the nine vtables: HEAD debt; header ports queued.
5. Self-leaf respells for 0x0073DA00, 0x00861900, 0x008D6850, 0x00743CB0 (two PDB sources each): queued.

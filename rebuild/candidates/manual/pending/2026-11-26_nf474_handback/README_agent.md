# NF474 hand-back (2026-11-26): NF467 b3 rework (r467f) -- 11 CAIStateGroup units land-ready, 11 held on slot respells

No repo writes, no git, no `wineserver -k`. Base: the working tree as read (HEAD + NF473's uncommitted landing).

## Land order
1. `batches/b0_hdr` (9 headers, add-only). Each gets a nested opt-in `FABLE_RETAIL_CAISTATEGROUP_<X>_CTOR` inside its EXISTING retail view
   (NF330/NF429 `_VIRTUAL_MODEL`, or the older full view), declaring only the default ctor; `#error` if defined without the view.
   Moved FABLE_STATIC_ASSERT lines pinned (evidence/pin_asserts.txt, 0 warnings). g++ -E -P identical for every HEAD includer of the 19 headers built
   (62 rows + 17 tests, a superset of the 9 headers' includers; evidence/hdr_equiv.txt); vland --recheck-landed of the 23 HEAD row includers, without and with the overlay:
   identical grade lines, 23/23 PASS, and the objects are ALL_EQ (evidence/vland_includers_*.log, objproof_includers.txt).
2. `batches/b3r_a` (22 rows = 11 units, all ACCEPT). Grade before b0_hdr lands with `FABLE_HDR_OVERLAY=$W/ov9/inc` (ov9/src/compiled holds
   the 11 new ctor texts the thin rows include). Units: MinionHasInformation, MinionListenToStory, ChickenBeg, LookAtInterestingThings,
   SeekPerceivedThing, CreatureRangedCombatUseWillShield, TestTestTest, MinionGuardGreetPlayer, MinionReactToPlayerInteraction (on their
   headers), OpenAndCloseShop and MinionCuriosity (headerless, local class on engine/CAIStateGroupBase.h, Ego_r size 0x18 / 0x20 with the
   PDB members, equal to retail Alloc's `push` size). Apply SELF_SYMBOLS (22), CATALOG_CHANGES (11 thin relabels), TABLE_CHANGES (none).
3. `batches/b3r_hold` (22 rows = 11 units, HOLD r467b): built and slot-checked, not re-gated; header opt-ins in its hdr/ (not in b0_hdr).

The TestTestTest and LookAtInterestingThings `#error` lines also fire when the hiding view (`_PASS27` / `FABLE_RETAIL_FIND_INTERESTING`)
   is defined (edited after the includer runs; same line, no assert moves, inactive for every includer).

## Gates (b3r_a)
- vland HEAD v new: 22/22 RELOCATION_MATCH PASS both, tests PASS; WARN per row never grows (31 -> 11; the 11 left are STANDIN_LEAF on the
  thin rows, gone with the catalog relabel).
- objproof: ctor bytes + relocations equal (only the COMDAT alignment 16 -> 1, r467c); `??_7` 4 -> 56; added `??_G` and
  `??1CAIStateGroupBase` byte- and relocation-equal to retail (0x008FD288 is the landed pick-any thin row); thin rows: the ??_G equal under
  the rename; .drectve unchanged in all 22 (no libcp). MinionCuriosity also emits an unreferenced 3-byte `??0C3DVector@@QAE@XZ` pick-any
  (no binding-table line and no row `// Leaf:` names it: r474b).
- emitters (evidence/table_and_emitters.txt): each table's second retail store is the class's Alloc, which has no catalog row; no other
  row source constructs these classes, so no short copy of the 11 tables remains.
- slots: every one of the 11 tables has 14 slots, each naming the class's override where retail's target differs from the base table and the
  base where it is the same (evidence/slotproof.txt); slot targets checked against each row's Leaf AND self_symbols (evidence/slot_audit.tsv:
  self_symbols.tsv lacks many landed slot rows, so a self_symbols-only check calls them row-less).
- link (11 new ctor objects v HEAD ctor + thin objects, plus HEAD objects of all 46 slot-target rows, 0x008FD255, 0x008FE862): duplicates 0 -> 0;
  unresolved 123 -> 116: the 11 `?v00@<C>@@UAEXXZ` stand-ins gone, 4 added, all slot symbols whose retail target has no row (0x009101C0,
  0x00930480, 0x00926ED0, 0x0091B570).
- census (evidence/fullcensus.tsv): each respelt `??_G` home's only retail reference is its vftable slot 0, bound by the unit's ctor: 11 OK, 0 BAD;
  no other row source names the 11 stand-ins.
- sandbox (shadow, overlay ov9): apply_table --dry 0 misses; bindings_multi 1 -> 1; descriptive_check 2030, 0 FAIL; s_unit_check 199, 0 failed;
  repo table unchanged; tracker dry run: 11 rtti_identified -> agree_pdb, none worse.
- r467c per row (evidence/r467c.txt): every ctor is preceded by the previous function's `ret` with no padding (size-mode packing; 0x008FD880 /
  0x008FE750 / 0x008FE8F0 are 16-aligned by coincidence); without the pragma the ctor still grades RELOCATION_MATCH and its ??_G is 30 bytes.

## Held (b3r_hold, r467b): the slot rows each unit must respell first
Found by the Leaf check (NF467's list was self_symbols-only and missed four): MinionReactToAlert 0x0090F0D0 (long for EScriptAIPriority);
BirdGroundWander 0x009143C0 (free fastcall stand-in); KidWanderNearKids 0x00914C60; KidHurtSelfRunToMother 0x00914EF0; IdleAtHome 0x00917330;
MinionReactToNoise 0x0091CC90 + 0x0091CDC0 (T3 nf320/nf322/nF325: body port); MinionLeaveRegion 0x0091E4B0 (T3 nf319/nf322/nF325: Adapter
alias); Drunk 0x00922530; WifeGiveYouGift 0x00925F80 (T3 nf319 held); AvoidRegionExit 0x0092BCB0; MinionKidGroupTag 0x00934100 (T3 nf319/nf321:
PDB bool return DIFFERs, retail int MATCHes -- needs a ruling) + 0x009340F0 + 0x00934920. NAME_FIXES.tsv lists all of them.
Views needed overrides the existing view lacks (Drunk 5, WifeGiveYouGift 3, MinionKidGroupTag 3, MinionReactToAlert 3): added under the same
`_CTOR` sub-macro, PDB spelling and access from Ego_r + FableWin.

## Task 2 (r467d, CInputProcess): measured only, not built
0x00488D10's HEAD object (RELOCATION_MATCH) emits 68 `??_7` tables, all 4 bytes (only 42 headers carry FABLE_RETAIL_INPUTPROCESS). The
rtti-vtables manifest gives 5..9 slots per table, and for 60 of them slot 1's target is not a catalog row, so most slots would be row-less
unresolved names; each class needs its PDB virtual list through its base chain (CInputProcessGameBase, ...). Not started for context.

## Locks
The 22 b3 ctor locks are owned by NF473 (inherited NF467 -> NF472 -> NF473; no NF467 locks exist). Writing NF474 over them was refused by
the permission system, so they were NOT taken over: the coordinator must transfer them before landing b3r_a. NF474 holds the 11 b3r_a `??_G`
home locks; the 11 held units' `??_G` locks were written back to `released`. LOCKS.txt.

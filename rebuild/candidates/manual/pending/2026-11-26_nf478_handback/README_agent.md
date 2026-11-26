# NF478 hand-back (2026-11-26): respell bT's misspelt slot rows (r467b, NAME_FIXES_QUEUE nf477)

No repo writes and no `wineserver -k`.

**One rule slip:** early on I ran `git log --oneline -5` once (read-only) to see the base. Nothing else touched git.

The base is the working tree as read: HEAD carries b0_hdr, bD and bR, and `.git/refs/heads/FullDecomp` reads 75885e1158. Each batch's `orig/` holds the repo file as read.

**Result:**
- 34 rows ACCEPT, in 6 batches.
- 4 rows HOLD on one ruling (b1m).
- 5 dtor and identity lines left as name fixes or rulings.
- Of bT's 37 misspelt-slot blockers, 33 are now resolved and the other 4 are b1m.

## Land order
The batches are independent of each other.

1. **`batches/b1`: 15 rows, ACCEPT.** The `QAE` ProcessInput rows on local class models move onto `engine/<Class>.h` (retail view + `FABLE_RETAIL_INPUTPROCESS_VTABLE`). The self becomes the PDB `UAE` virtual at retail slot 4.
   - Rows: 0x00687F80, 0x00689C40, 0x0068C880, 0x0068D890, 0x0068F320, 0x00690030, 0x00690390, 0x00690480, 0x00690830, 0x00690B20, 0x00690E70, 0x00690FD0, 0x00691680, 0x00691870 and 0x006A4700.
   - It needs its two add-only header opt-ins (`hdr/engine/`, `HEADERS.tsv`):
     - `CInputProcessGameBase.h`: `_ISEVENTASSIGNABLESPECIALMOVEBUTTON` and `_GETMOUSEWHEELMOVEMENT`, for 0x00690030;
     - `CInputProcessJumpingAndRolling.h`: `_PROCESSPARTS`, for 0x006A4700.
   - On 0x00689C40, the local slot-9 stand-in `Refresh()` is now `GetPInventory()`, retail's slot 9.
   - The tests are unchanged (standalone models).
2. **`batches/b2`: 6 rows, ACCEPT.** Stand-in selves move onto the family opt-in:
   - 0x0068CB70 FreeCamera Init;
   - 0x00690D20 Paralysed Init;
   - 0x0068D830 RightStick Init;
   - 0x00690AC0 CutScene ProcessWhilePaused;
   - 0x006A9170 ControlSpirit ProcessInput;
   - 0x0068EB10 UseRangedWeaponZLock Update.

   Five tests were rewritten to link the row object (pass patterns unchanged). 0x0068CB70 keeps its HEAD test.
3. **`batches/b2d`: 1 row, ACCEPT, with two landing dependencies.** 0x00687DB0 becomes `?IsInputEventToBeSentToThisInputProcess@CInputProcessGameBase@@MBE_NABVCInputEvent@@@Z`.
   - `CATALOG_CHANGES.tsv` renames CInputProcessMain::IsEventFromPlayerDevice. The `build_candidates` Module must be moved to CInputProcessGameBase by hand.
   - `REGISTER_REMOVE.tsv` is a new file: land_scripted does not read it, so apply it by hand. It drops the `descriptive_names.tsv` best-guess line. Without that, descriptive_check FAILs ADDRESS_PROVEN; with it, the sandbox shows 0 FAIL.
   - The test was rewritten and links the row.
4. **`batches/b3`: 10 rows, ACCEPT.** The GetPInventory slot-9 overrides (`mov eax,[ecx+34h] ; ret`) become `NInventory::CTCInventoryBase* <Screen>::GetPInventory() const { return PInventory; }`.
   - Each goes through its screen header plus `engine/CTC<...>.h` with that header's existing opt-ins. RTTI puts `NInventory::CTCInventoryBase` at +0, so there is no adjustment.
   - No header edit was needed. Tests were rewritten to link the row.
5. **`batches/b4`: 1 row, ACCEPT.** 0x0068D840 becomes `?Update@CInputProcessControlCreatureRightStick@@UAEXAAVCProcessedInput@@@Z`.
   - Its callee 0x00687D50 is respelt to the PDB two-argument `?IsGameActionEventInQueue@CInputProcessGameBase@@IBE_NW4EGameAction@@W4EInputButtonEventType@@@Z`, which the existing `FABLE_RETAIL_COMBAT_INPUT_PASS26` declares. 0x0068F470 already calls that spelling.
   - `TABLE_CHANGES`: 1 replace.
6. **`batches/b5`: 1 row, ACCEPT.** 0x00689130 moves off the NativeInput namespace model onto `CInputProcessTargetLockRightStickTargetSelect.h`.
   - The event goes onto `CInputEvent.h` `FABLE_RETAIL_CINPUTEVENT_ACCESSORS`.
   - The target goes onto `CTCTargetingPlayer.h` plus a new add-only opt-in, `FABLE_RETAIL_CTCTARGETINGPLAYER_SETASTARGETNEXTTHINGINDIRECTION` (`hdr/`, `HEADERS.tsv`).
   - `TABLE_CHANGES`: 2 delete lines (GetType and GetJoystickPosition, whose PDB spellings are already bound to the same targets) and 1 replace (SetAsTargetNextThingInDirection 0x008023C0).
   - Resolves T3 nf309 line 1694.
7. **`batches/b1m`: 4 rows, HOLD on R-NF478-1.** Rows 0x0068AE30, 0x0068B2E0, 0x0068B7C0 and 0x00691130.
   - They are fully gated with the row's made-up inline helper GetMousePosition moved to a file-local `static inline`.
   - The self's bytes and relocations are SAME. The unreferenced NOADDR member COMDAT goes and a static one appears.
   - Locks released.

## Gates (all 34 ACCEPT rows)
- **vland HEAD v new:** 34 of 34 PASS on both sides with the test run, and the grades are identical on each side: 13 MATCH, 21 RELOCATION_MATCH.
- **objproof** (`batches/*/evidence/objproof.txt`):
  - every object only swaps its self;
  - the exceptions are the intended callee respells in b4 (1) and b5 (3);
  - in those, the leaf bytes with relocation fields zeroed are SAME and the relocation offsets are SAME (`evidence/leaf_bytes_reloc.txt`);
  - `.drectve` is unchanged everywhere, and no other symbol is added or removed;
  - every new self string-equals NAME_FIXES `correct_symbol` and bT's unresolved name.
  - b2, b2d and b3 objproof identically with no overlay at all (`objproof_no_overlay.txt`).
- **Census, retail side:** every retail reference to the 43 queue addresses is a family vftable slot. 109 `.rdata` dwords were joined to their table and slot (`evidence/retail_xrefs_slots.tsv`); 0x00687DB0 is in 68 tables. The exception is 0x0048CEC0, called once from 0x0048CEA3.
- **Census, landed objects:** 168 landed sources naming a queue class and method, plus 4 users of the 0x008023C0 name, were compiled at HEAD and their full symbol tables read (`evidence/census_binders.tsv`). None defines or references an old or new spelling except the row itself.
- **Census, old identifiers:** a grep of `rebuild/src`, `rebuild/tests` and `rebuild/include` for the old C++ identifiers hits only the rows, their own tests, the untouched dtor rows and `engine/RECONCILE.tsv` line 704. That line is a field-reconcile record on 0x00689B00's local `f`; it goes stale when b3 lands.
- **Census, includers:** no landed file includes any of the respelt sources.
- **Pick-any:** pickany_vs_retail and pk_all HEAD v new, 0 NEQ reach flips (`evidence/reachflip.txt`). The only NEQ copy is `IS_THING_ALIVE` in 0x00690480, reach y on both sides (debt).
- **Header gates:**
  - every opt-in is add-only and `#error`s outside its view;
  - the asserts were already pinned and are unchanged;
  - `g++ -E -P` is identical for all HEAD includers with the macros unset: 127 for GameBase and JumpingAndRolling (b1), 34 for CTCTargetingPlayer (b5).
- **Sandbox** (`tools/sandbox.sh ALL3` = b1, b2, b2d, b3, b4, b5, with the overlays installed):
  - apply_table `--dry`: 0 misses;
  - bindings multi: 1 → 1, nothing new;
  - descriptive_check: 2,030 → 2,029 lines, 0 FAIL;
  - s_unit_check: 205 checked, 0 failed;
  - the repo table is unchanged.
- **Tracker dry run** (`evidence/TRACKER_EFFECT_accept.tsv`), nothing worse:
  - 33 rows go rtti_identified → agree_pdb, and 0x00687DB0 goes agree_descriptive → agree_pdb;
  - 0x00687D50 and 0x008023C0 go split_unknown → split_fixable;
  - 0x00A03B40 goes split_fixable → agree_pdb;
  - 0x00A043A0 stays split_fixable with one name fewer.
  - With b1m, 4 more rows go rtti_identified → agree_pdb.

## T3
- **Resolved** (`T3_SCREEN.tsv`):
  - nf324 lines 2791–2800 and 2806;
  - nf319 / nf323 lines 2343, 2344, 2577 and 2578;
  - nf309 line 1694.
- **Open and untouched:** these are callee bindings, not selves: nf374 3496 (0x0068F320), nf374 3491 (0x00691870) and nf307 1385 (0x0068AE30, b1m).
- **COORDINATION_CLAIMS:** no open claim.

## bT after NF478 (`evidence/bT_unresolved_after_nf478.tsv`)
bT has 145 new slot externals:
- 71 are resolved by HEAD rows, 34 by bR and 33 by NF478 ACCEPT rows;
- 4 are b1m (R-NF478-1);
- 2 are row-less (allowed);
- 1 is `__purecall`.

What still blocks bT:
- R-NF478-1;
- 0x0048D390's 4-byte `??_7CInputProcessJumpingAndRolling` (NF477);
- the link gate, which has not been rerun.

## HOLD reasons
- **b1m:** R-NF478-1.
- **0x0048CEC0:** settled on evidence, not respelt; ruling R-NF478-2. It is `??1CInputProcessZTarget@@UAE@XZ`:
  - the name is FableWin.pdb only;
  - the Ego_r layout matches the teardown order;
  - the sole caller is ZTarget's `??_G`, whose landed row calls it as a free stand-in;
  - the real ~CHitParameters is 0x00789358, already bound under the same name.
- **0x0048CEA0:** waits on R-NF478-2.
- **0x0048D680:** its Combat `??_G` copy is 30 bytes against retail's 40.
- **0x00687DF0 and 0x006A91D0:** thin rows of the ctor TUs 0x00687A30 / 0x006A9140, after the r459a port of those two tables. That is outside bT.

## Rulings needed
R-NF478-1 and R-NF478-2 (`RULINGS_NEEDED.tsv`).

## Locks
- 38 were taken, all from none or released.
- 34 are held by NF478 for the ACCEPT batches.
- 4 were written back to released (b1m).
- bT's `COORD_HOLD_NF477` locks were not touched (`LOCKS.txt`).

## Files
- NOTES.md, STATUS.tsv, NAME_FIXES.tsv (11 lines), RULINGS_NEEDED.tsv, LOCKS.txt, evidence/.
- tools/:
  - port_g1.py: the group-1 transformer;
  - gen_gpi.py;
  - mkrun.py;
  - pk.sh;
  - census_syms.sh;
  - leafcmp.py;
  - NF477's objproof.sh, objdiff.py, runpair.sh, sandbox.sh and hdrequiv.sh, adapted. sandbox.sh now applies REGISTER_REMOVE.tsv.
- Overlays: `ov/inc` holds the b1 headers; `ov5/inc` holds the b1 headers plus CTCTargetingPlayer, and is the one used for b5 and ALL3.

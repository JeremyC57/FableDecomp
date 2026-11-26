# NF456 (2026-11-25/26): r454b self respells landed as caller units

Brief: land NF455's reverted b1 / b2 rows and the b2f FableWin-only ctors as units with every landed caller (advisor rule
2026-11-25: the binding table misses callers, so census by retail call sites + compiled objects), then the remaining `(CThing&)` ctors.
Scratch: `$S/w22/agentNF456`. Tools: NF455's (`tools/`, paths moved to NF456) plus `mkfiles456.py`, `mkfiles_b4.py`.

## Batches (NF455/NF442 layout: rows/, orig/ = exact HEAD copies incl. tests, head/payload.json, RESULTS, SELF_SYMBOLS, UNITS, CHECKS,
## TABLE_CHANGES, HEADERS (empty), BEST_GUESS where needed, evidence/ = vland HEAD/new logs, objproof, link)
| batch | rows | units | verdict | what |
|---|---:|---:|---|---|
| b1u_dtor_units | 6 | 3 | ACCEPT | ~CTCFireheartMinigame 0x007E7ED0, ~CEngineInternalPrimitiveWeaponTrail 0x00BCDE40, ~CLocalDetailPrimitiveMesh 0x00BDFD80: QAE -> UAE (NF455 b1 edit) + each class's deleting destructor (0x007E7EB0, 0x00BCDE20, 0x00BDFD60) now calls `p->X::~X()` (was a free `X_dtor` stand-in that bound nothing). Deleting-dtor selves unchanged. |
| b1p_dtor_unit_particlegroup | 7 | 1 | HOLD r456b | ~CEngineInternalPrimitiveParticleGroup 0x00BBA140 + the 6 deleting dtors that call it (Decal 0x00BBA240, SpriteTrail 0x00BBF180, Sprite 0x00BC2240, Mesh 0x00BC3BE0, Line 0x00BC53E0, 2D 0x00BE1B30): every gate passes but the WARN set swaps RELOC_NAME for SHARED_HEADER_AVAILABLE (local base model; the shared header declares no dtor). |
| b1n_dtor_units_ncreaturemode | 4 | 2 | HOLD r456a | ~CModeStrafe 0x007420E0 + 0x007420C0, ~CModeSleepBed 0x0084B840 + 0x0084B820: the deleting dtor's model must move into NCreatureMode, so its invented self is renamed too (PAU global -> PAV NCreatureMode). Every gate passes. |
| b2u_shot_alloc_units | 6 | 3 | ACCEPT | NThingShot ctors DrainLife 0x004DB15E, ForcePushBall 0x004DCAB9, MultiArrow 0x007BBC65 `(CThing*)` -> `(CThing&)` (NF455 b2 edit) + their factories 0x004DCB3C, 0x004DEB47, 0x004D53BB re-authored on engine/CTCBase.h onto the PDB self `?Alloc@X@NThingShot@@SIPAVCTCBase@@AAVCThing@@@Z` (0x004D393C pattern). |
| b2fu_fwonly_alloc_units | 10 | 5 | ACCEPT (r455c) | the 5 FableWin-only ctors (Smashable Barrier/Window, ActivationReceptor Door/Relay/Chest) + their factories (were `(void*)` stand-ins) on the PDB Alloc self; BEST_GUESS.tsv: 5 ctor lines `one-source:fablewin-pdb`. The Alloc selves are proven (both PDBs + table), not guesses. |
| b4_thingref_alloc_units | 34 | 17 | ACCEPT | 17 of the 23 remaining `(CThing&)` ctors + their factories (same recipe as b2u). TABLE_CHANGES: 17 `replace` (the table bound the old ctor spelling at each factory). |

Totals: 67 rows in 31 units; ACCEPT 56 rows (28 units: 3 + 3 + 5 + 17), HOLD 11 rows (3 units, two rulings).
b1u and b1p were run as one 13-row vland pair (tag b1u): their head/payload.json, vland_b1u{h,n}.log, link and vobjproof files are the same
13-row files in both folders. 0x00BDFD60 and 0x0057832D had no self_symbols.tsv line at HEAD: their SELF_SYMBOLS lines add one (current_line empty).
TABLE_CHANGES: 14 `add` (no table line bound any of those targets before) + 17 `replace`.

## Gates (every row)
- vland `--recheck-landed` HEAD payload (recheck.py) and new payload at one LANDVERIFY_WORK path per batch (tools/runpair.sh): RELOCATION_MATCH PASS
  both sides for all 67; per-row WARN set identical except b1p (above). 0057832d was re-run (tag b4x) after fixing its stale `// Leaf:`
  (NF455's predit leaves a full mangled Leaf untouched; fixed by hand, evidence vland_b4x*.log).
- objproof: tools/objproof.py on vland's own HEAD/new objects (wd_<tag>h / wd_<tag>n, weak externals counted; evidence/vobjproof_*.txt) ALL_EQ
  for all 67 (0057832d / 004d3042 from the b4x re-run); also nf.py proof on the nf.py grade objects: ALL_EQ for all 67, renames exactly the intended ones (pairings checked: callee
  stand-in -> real ??1/??0, factory self -> ?Alloc, ctor PAV -> AAV).
- Link (nf.py link, /FORCE) HEAD set v new set per batch: 0 duplicates in every batch; unresolved: same or fewer (the unit's stand-in
  callees resolve; b4 identical sets because its factories already bound the old ctor spelling).
- Sandbox (`sandbox/`: copies of the three oracle TSVs): all SELF_SYMBOLS applied, every TABLE_CHANGES `--dry` 0 misses and then applied in
  the sandbox (multi-target 1 -> 1), descriptive_check (sandbox copy) 0 FAIL with BEST_GUESS as --extra, self/table clash check 0.
  The repo TSVs were never written (md5 unchanged).
- Ties: dtors `ret` (0x007E7ED0 tail jmp) storing base vftables only; deleting dtors `ret 4` at vftable slot 0 (xref abs refs listed in
  RESULTS); ctors `ret 4` storing their own vftable (emitted ??_7 in objproof); factories `ret`, referenced only by CTCBase::RegisterTCs.

- Vftable ties checked against rebuild/manifest/rtti-vtables.tsv: each of the 11 deleting dtors is slot 0 of its class's RTTI vftable;
  every ctor is `ret 4` and stores its own class's RTTI vftable (MI classes store two; some also store member vftables).
- In-class definition grep (inline ctor bodies / `~X() {`) over rows + headers: hits only for CTCActionUseBuyHouse (held); none for the
  b1/b2u/b2fu classes.

## Census method (per target)
xref.py (retail call/jmp + absolute refs) -> owner_of.py (row, landed, lock) -> HEAD object UND symbols (nf.py syms) -> source greps for
the old/new spelling (definitions incl. in-class inline ones, stand-in names) -> reloc_bindings by target and by symbol -> self_symbols for
the new symbol elsewhere. Retail callers found: dtors only from their deleting dtors; ctors only from their factory; factories only from
RegisterTCs 0x004EE23F (already bound to the PDB Alloc spelling); deleting dtors only from their vftable.

## Held / not done
- 0x007026A0 CTCQuestCard + 0x004D2E44: 0x00702A50 (~CTCQuestCard row) already DEFINES `??0CTCQuestCard@@QAE@AAVCThing@@@Z` ("emission
  only") -> a duplicate definition after the respell (NF455 b2h case). Not changed, locks released.
- 0x0072E690 CTCActionUseBuyHouse + 0x004D321B: 0x0072F040 (??_G row) defines an in-class inline `CTCActionUseBuyHouse(CThing*)` and calls
  `new CTCActionUseBuyHouse(owner)`; it would have to move with the unit. Not changed, locks released.
- 0x005EC8A0 CTCTavernGame: 5 callers (0x007D09C0 locked NF453, 0x007D49E0, 0x007D8DA0, 0x008F1CD0, 0x007D21B8 in no row). Not started.
- 0x007413C0 CModeDrained, 0x007437C0 CModeDragonFlying, 0x00743BE0 CModeIceTrollGroundSmashIcicleAssault: callers outside catalog rows
  (0x00742C27, 0x00743CD7, 0x00743FA7) or a ctor row (0x00743990). Not started.
- `(CThingCreatureBase&)` creature actions: census only, `work/t5_creature_census.tsv` (100 lines from NF455's ctor_pr.tsv; 55 one-argument:
  34 with one landed caller and no table line, 8 with a caller site outside any row, 8+2 with table lines, 1 caller locked NF453).
- Deleting-dtor tests still model their old `X_dtor` stand-ins (standalone, link nothing); left unchanged.
- Deleting-dtor selves stay invented stand-ins: retail ??_G is compiler-generated beside the vftable (the NF424 ctor-owned thin-row route);
  doing that is a later per-class unit (not ALL_EQ).

## Resume point
1. Rulings r456a / r456b (RULINGS_NEEDED.tsv).
2. TavernGame (after NF453 releases 0x007D09C0) and the three creature-mode ctors: find the callers at 0x00742C27 / 0x00743CD7 / 0x00743FA7.
3. QuestCard / BuyHouse units: fold the emitting rows (0x00702A50, 0x0072F040) into the unit.
4. Creature actions from work/t5_creature_census.tsv, starting with the 34 one-caller / no-table lines.

## Locks
NF456 (rows in batches): 67, listed in batches/*/rows.txt. Released: 007026a0 004d2e44 0072e690 004d321b.

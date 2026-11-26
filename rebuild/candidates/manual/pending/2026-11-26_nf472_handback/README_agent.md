# NF472 hand-back (2026-11-26): step 1 -- the four live stand-in binders of 0x008FD288, plus 0x008FD56A

No repo writes, no git, no `wineserver -k`. Step 2 (the 9 held b4 units) and step 3 were not started (step 2 is long; hand-back at step 1 as briefed).

## Land order (five independent units; each lands whole, any order; all after NF471's working-tree landing)
| batch | rows | self respell | T1 binder fixed |
|---|---|---|---|
| batches/bU1_reacttocriminal | 2 (0x008FECD0 owner, 0x008FECEC thin) | 0x008FECEC -> `??1CAIStateGroup_MinionReactToCriminal@@UAE@XZ` | 0x008FECEC `?PairFirstDtor@CSymbolInfo___@@QAEXXZ` |
| batches/bU2_balverinecornered | 3 (0x00900517 owner, 0x00900533 thin, 0x009004E0 r469a decl-only) | 0x00900533 -> `??1CAIStateGroup_BalverineCornered@@UAE@XZ` | 0x00900533 `??1CSymbolInfoTail@@QAE@XZ` |
| batches/bU3_workbase | 4 (0x008FDA24 thin of unchanged 0x009000F2, caller 0x0091A570 with per-row flag /Ob0, TU-mode 0x008FFD3A / 0x009002A5) | 0x008FDA24 -> `??1?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@UAE@XZ`; 0x0091A570 interim (r472b) | 0x008FDA24 `_ForwardTarget_008fd288` |
| batches/bU4_sitonseatbase | 14 (0x008FD56A thin of 0x008FF271; 6 more ??_G callers; 6 ctors r469a decl-only) | 0x008FD56A -> `??1CAIStateGroup_MinionSitOnSeatBase@@UAE@XZ` | (0x008FD56A self, NF471 NAME_FIXES) |
| batches/bU5_flyingthingbase | 2 (0x00913490 thin of the ctor 0x009126C0; 0x009126C0 gets a `// Leaf:` comment only) | 0x00913490 -> `??_GCAIStateGroup_FlyingThingBase@@UAEPAXI@Z` | 0x00913490 `?Dtor@CAIStateGroup_FlyingThingBase@@QAEXXZ` |

25 rows, all ACCEPT. bU3 and bU5 are conditional on ruling confirmations (r472b, r472a; RULINGS_NEEDED.tsv): land them only if granted, else HOLD them
(0x008FD288 then stays split_fixable T1 through the 0x008FDA24 / 0x00913490 lines). Thin rows include their owners' new texts: grade with
FABLE_HDR_OVERLAY=$W/ov/inc (ov/src/compiled holds 0x008FECD0, 0x00900517, 0x008FF271, 0x009126C0 new texts) or with the owners landed first.
Every batch has head/payload.json + oracle.tsv (recheck.py of the HEAD rows), orig/ (+ orig_tests/), rows/, tests/ (new self-contained thin-row
tests print the catalog PassPattern), RESULTS / UNITS / SELF_SYMBOLS / TABLE_CHANGES / CATALOG_CHANGES / CATALOG_MANGLED / T3_SCREEN, evidence/.

## Identities (two sources each; evidence/pdb_dtor_attrs.txt, xref / RTTI)
- Both PDBs flag every destructor here `public virtual compiler-generated` (Ego_r and FableWin type records; control ~CBaseIntelligentPointer `public intro virtual`).
- 0x008FECEC: only caller ??_G 0x008FECD0 = slot 0 of 0x0128B1F4 (RTTI MinionReactToCriminal : CAIStateGroupBase); PHome +0x1C / PHero +0x14 (CIntelligentPointer, ~ = 0x00A01C10), jmp 0x008FD288.
- 0x00900533: only caller ??_G 0x00900517 = slot 0 of 0x0128C0BC (RTTI BalverineCornered); PLastJumpTarget +0x1C / PTarget +0x14, jmp 0x008FD288.
- 0x008FDA24: `jmp 0x008FD288`; only caller 0x0091A573 in ??_G 0x0091A570 = slot 0 of 0x0128FF0C (RTTI BuyFromSeller : WorkBase<CAIStateGroupBase> directly); FableWin name set.
  Twin: 0x008FDA29 is a byte-identical unreferenced jmp (no row). Retail has neither ICF nor unreferenced-COMDAT removal (the twin survives), so BuyFromSeller reaches
  the ~WorkBase copy itself; retail keeps base before derived in this COMDAT run (0x008FD544 / 0x008FD56A / derived jmp 0x008FD57D), so 0x008FDA29 is most likely
  ~CAIStateGroup_WorkAtSchool (RTTI 0x0128BC08 : WorkBase). FableWin's debug order (ai_brain_tables.obj #418 ~WorkAtSchool, #419 ~WorkBase) is derived-first, as it is
  for ~MinionSitOnSeat (#165) before ~MinionSitOnSeatBase (#166), which retail reverses; so donor order does not decide it. Row comment carries this.
- 0x008FD56A: callers = 7 ??_G slot-0 rows (vftables 0x0128B584 / B5D4 / B624 / B674 / B6C4 / CFBC / D00C, RTTI chains checked) + a row-less jmp at 0x008FD57D; POwner +0x34 then jmp ~UsesSeat 0x008FD544.
- 0x00913490: slot 0 of 0x0128F9BC (RTTI FlyingThingBase : CAIStateGroupBase), the vftable ctor 0x009126C0 stores; `call 0x008FD288` (~FlyingThingBase inlined).

## Gates
- vland HEAD v new (evidence/vland_head.log / vland_new.log): 25/25 RELOCATION_MATCH PASS both; no row's WARN grows (totals bU1 4->0, bU2 9->5, bU3 9->6, bU4 23->16, bU5 5->3; per row in RESULTS).
- xr: every respelt call names the symbol of retail's target (0x008FECEC/0x00900533/0x008FD56A/0x008FDA24/0x008FD288/0x00A01C10).
- objproof (evidence/objproof.txt): only the intended symbol changes; .drectve unchanged in all 25 (drectve_and_tumode.txt). TU mode (0x008FFD3A, 0x009002A5): leaf section identical; changed COMDATs all move NEQ -> EQ their retail homes (byte equality with relocation fields masked; the leaves' relocations were resolved with xr) (~WorkBase 0x008FDA24, ??_G WorkInWarehouse 0x008FFD5E / SellToBuyer 0x009002C9 / WorkBase 0x008FDA2E / CIntelligentPointer<..> 0x008FEAE9 / 0x004D6915).
- Copy census (whole symbol tables, 290 objects naming the classes or the stand-ins; census_HEAD / census_AFTER): HEAD 6 NEQ ~MinionSitOnSeatBase, 2 NEQ ~WorkBase, 1 NEQ ~BalverineCornered -> after 0. Remaining NEQ: ??_GCAIStateGroup_MinionReactToCriminal in the ctor 0x008FEC9C (/O2 local model, unchanged, no binder of that name).
- Caller census (fullcensus.tsv, measured on the union of the five units; every retail reference to 0x008FD288 / 0x008FECEC / 0x00900533 / 0x008FDA24 / 0x008FD56A / 0x00913490): 177 OK, 11 row-less, **0 BAD**.
- Link (link_step1*.txt; unit objects without thin rows + 10 HEAD emitters/owners): duplicates 1 -> 1 (pre-existing ??0CAIStateGroup_MinionSitOnSeat local-model pair); unresolved 26 -> 11, 0 new.
- Sandbox (evidence/sandbox): apply_table --dry 0 misses (13 replace, 1 add); bindings_multi 1 -> 1; descriptive_check 0 FAIL; s_unit_check 181 checked, 0 failed; repo table unchanged.
- Tracker dry run (--out in the sandbox), all five: 6 -> agree_pdb, none worse: 0x008FD288 split_fixable, 0x008FD56A / 0x008FECEC / 0x00900533 split_unknown, 0x008FDA24 standin, 0x00913490 rtti_identified.
- Subset bU1 + bU2 + bU4 only (if r472a / r472b are refused; evidence/sandbox_subset_bU1_bU2_bU4): 0x008FD56A / 0x008FECEC / 0x00900533 -> agree_pdb; 0x008FD288 stays
  split_fixable (names 5 -> 3: the 0x008FDA24 and 0x00913490 stand-ins remain); none worse; s_unit 180 checked 0 failed; descriptive 0 FAIL; bindings_multi 1.
  Each of those three units' own binder sites is covered only by its own rows (fullcensus), so they are 0 BAD on their own as well.

## T3 / claims
No T3_PORT_QUEUE line (columns 2/3) or COORDINATION_CLAIMS line names any of the 25 rows (T3_SCREEN.tsv). No T3 line resolved (none applied).

## Not done
- Step 2 (9 held b4 units, slot-target respells) and step 3 (NF467 b3 rework, r467d).
- NAME_FIXES.tsv: the real ??_G names of the free-stand-in owners (need header ctor ports), 0x008FDA2E / 0x008FDA12 WorkBase template spellings.
- Locks: LOCKS.txt (67 addresses, all NF472: NF471's 41 taken over + 26 new).

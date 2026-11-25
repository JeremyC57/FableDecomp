# NF435 hand-back (2026-11-25): CGameScriptInterface round 3 (TC family pilot)

**0 ACCEPT. Two fully gated HOLD batches: `batches/nf435_tc_b1_hold` (11 TC rows, blocked only by ruling R1) and
`batches/nf435_tc_b1_hold_0089ec60` (1 row: R1 + NAME_FIXES 0x0074AC80, lands after the first).**

## What happened
- Piloted one TC row per class (`p1`..`p3`): 12 of 13 attempted rows grade RELOCATION_MATCH on the real
  `CGameScriptInterface::<Method>` with retail's GetTC / PeekTC expansion (`FABLE_RETAIL_CTHING_GETTC_EXPANDED`, engine/CThing.h, unchanged).
- name_gate cannot PASS any TC row: the expansion calls the CThing::Components LowerBound out of line (0x0040F020 non-const /
  0x004365B0 const), and name_gate classes both targets `split_unknown` (established is_std spelling + 20+ landed stand-ins). That is
  pre-existing debt shared by all 1,543 landed GETTC_EXPANDED rows, so the batch is HOLD with ruling R1 (`RULINGS_NEEDED.tsv`). Each row's
  HEAD already binds that target; b1 deletes two of its stand-in table lines.
- 0x0089EC60 also needs NAME_FIXES 0x0074AC80 (the catalog says CTCShop::AddShopper; it is CTCCreatureGenerator::AddCreatureTriggerer).

## batches/nf435_tc_b1_hold (11 rows) + nf435_tc_b1_hold_0089ec60 (1 row); all RELOCATION_MATCH -> RELOCATION_MATCH
| row | method | TC class (interface) | macro set that compiles |
|---|---|---|---|
| 0089a2d0 | IsEntityPickLockable | CTCDoor (0x26), PeekTC | CTCDOOR + _VANDALISM + CONTEXTSENSITIVEITEM_QUERY_PASS28 (existing) |
| 00895090 / 008950e0 | ManuallyTrigger/ResetTrap | CTCTrapBase (0x7D), GetTC | DEF_CLASS_BASE CINTELLIGENTPOINTER CTCBASE CTCTRAPBASE |
| 008a12a0 | GetNumHousesOwned | CTCHeroStats (4), PeekTC | CTCHEROSTATS + _SERIALISE + _PERSONALITY (getters live in the SERIALISE branch) |
| 00895430 | IsDPadButtonHeldForExpression | CTCInventory, PeekTC | the complete-class PASS22 set (see the spec) |
| 00896210 | IsSleepingTime | CTCVillage (0x22), PeekTC | CTCVILLAGE + _TCI + new _ISNIGHTTIME |
| 0089a270 | SetThingAsKilled | CTCInventoryStats (0x14), GetTC | CTCINVENTORYSTATS + new _SETCREATUREASKILLED |
| 0089ec60 / 0089ecc0 | CreatureGeneratorAdd/RemoveTriggerer | CTCCreatureGenerator (0x3D) | CENGINEPERSISTENTPRIMITIVEBASE CTCCREATUREGENERATOR + new _SCRIPTTRIGGERERS |
| 0089d750 | CancelUsingAbility | CTCSpecialAbilities (0xA7) | _FULL + _GUI_DRAW_PASS23 + new _CANCELUSINGABILITY |
| 00895a90 | HeroGoFishing | CTCFishing (0xB1) | CFISHINGDEF_PASS22 CTCFISHING_PASS22 + new _GOFISHING |
| 0089c8a0 | IsDeedWitnessed | CTCHeroOpinionDeedLog (0x51) | CTCHEROOPINIONDEEDLOG + new _ISDEEDWITNESSED |

Shapes (FableWin's /Od body minus its debug-only DoErrorMessage / assert blocks, each checked against retail's bytes):
- script thing: `entity.GetPThing()` (slot 11), sometimes `entity.IsAlive()` first (slot 75) or IS_THING_ALIVE; the creature-generator pair
  has only a debug assertion around its two GetPThing calls, so retail has no alive test there;
- hero: `PlayerManager->GetMainPlayer().GetPControlledCreature()` (or PeekPControlledCreature), IS_THING_ALIVE or a plain null test;
- then `thing->GetTC(&tc)` / `PeekTC(&tc)` and the TC call.

### Header opt-ins (6, add-only, declarations only; base = current repo headers, `orig/hdr_*.h` re-compared just before hand-back)
CTCVillage.h _ISNIGHTTIME (0x00658D07, nested in _TCI), CTCInventoryStats.h _SETCREATUREASKILLED (0x005D66BA, nested in the class block),
CTCCreatureGenerator.h _SCRIPTTRIGGERERS (0x0074AC80 / 0x0074A1E0), CTCSpecialAbilities.h _CANCELUSINGABILITY (0x0072ACF0),
CTCFishing.h _GOFISHING (0x007B1EB0, nested in _PASS22), CTCHeroOpinionDeedLog.h _ISDEEDWITNESSED (0x006C5E90). Every name is in Ego_r.pdb publics and
is FableWin's /Od callee at retail's call target.

### Gates (run tag b1, one work path)
- vland HEAD / new / mutant: grade kept 12/12; tests PASS 12/12; mutants: test FAIL 12/12 (3 swap sibling callees and keep the grade,
  9 DIFFER); 0 LITERAL lines; new WARN: 1 RELOC_NAME (0x0089EC60, the 0x0074AC80 label; HEAD had the same kind on the same target).
- name_gate --test: tests clean; rows FAIL only on the LowerBound use (R1) and 0x0089EC60's RELOC_NAME. HEAD 0x0089A2D0 under name_gate
  shows the same LowerBound FAIL plus 5 more (evidence/ng_head_0089a2d0.log).
- objcmp: 12/12 SELF_EQ; differing pairs are callee respells (stand-ins -> PDB names); no `??_7` / `??_R` emitted.
- pk_all: 0 NEQ; the LowerBound COMDATs (16 non-const, 4 const) EQ and reached; the rest NOADDR unreferenced inline copies.
- /FORCE link (evidence/linkcheck.txt): dup 0 -> 0 (control 1). Unique unresolved 10 removed (stand-ins) / 10 added (8 PDB callee spellings +
  _puts / _abort, the CRT imports of STLport's unreferenced __stl_throw_out_of_range copy that the bitset header emits; /nodefaultlib).
  Census: no other landed definer of any new self.
- apply_table --dry: 6 adds, 6 deletes, 0 misses; multi-target 1 -> 1, none new.
- reserved_check (RESERVED_REFS=""): 0. hdrcheck2: every macro gives the same text (evidence/hdrcheck.txt).
- equiv_check copy against copy (old = copy of rebuild/include, new = old + all 8 NF435 overlay headers incl. the hold-only CTCCombat.h /
  CClassFactory.h): 0 of 57,160 landed rows differ (evidence/equiv.txt).
- /FORCE link WITH the landed definers of every EQ pick-any copy (0x0040DCE0, 0x0040F020, 0x00410DE0, 0x004365B0, 0x0043DBD0, 0x0043DBF0,
  0x006804C0; HasTC 0x004118C0 has no row), built with recheck.py + vland: dup 0 -> 0 (evidence/link_definers/).
- Direct symbol scan of the 24 new objects (12 rows + 12 tests): no `??_7` / `??_R`.
- TABLE_KEPT re-tested by class token: the CTCInventory_Retail / CTCFishing_Retail / Map8a12a0 lines had only this batch's row as
  witness and no other landed source spells the class, so they are deletes (8 deletes, 5 adds). 0089ec60's two ?find@MapT lines are
  deletes in its own batch (MapT is spelled only by 0x0089EC60 / 0x0089ECC0 at HEAD).
- Tests link the row object and call it qualified; the CVectorMap LowerBound comes from the row's own COMDAT (a test-side definition
  collides: LNK2005). Kit: handback_tools/kits.py (@TCKIT@ / @SCRIPTKIT@ / @HEROKIT@ / @HEROPEEK@). Each test covers bit clear, null / dead
  thing and the callee's `this` = the planted component.

## Held drafts (`hold/`): 0x006E72A0 (Validate; waits on the 0x00A36FE0 port) and 0x0089DFF0 (CTCCombat.h branch does not compile).

## Not attempted (released)
20 TC rows, 12 body ports, 5 tc_query rows, 8 T1 caller respells, 0x00899C90 (flatten-local2 unit). See RESIDUE.tsv / T3.txt.
For tc_query: retail keeps only the UsingComponents bit test, which the GETTC_EXPANDED form (an out-of-line LowerBound call) cannot give.

## Judgment calls
- All locks released, including the 12 held rows, as the brief says; both batches stay land-ready once R1 is decided.
- T3: the nf324 port lines for 00895090 008950e0 00896210 0089a270 0089a2d0 0089c8a0 0089ec60 0089ecc0 are resolved by b1 when it lands
  (each asks for exactly this port); none is released now.
- 0089ec60 was split into its own batch so the 11-row batch lands unchanged on R1 alone.

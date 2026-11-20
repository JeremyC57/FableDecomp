# Landing nf191 (Account A, 2026-11-20): game / creature / UI / input member names, second pass (20 rows)

Agent NF191 (round 10) handed back early (context) with 54 entries: 7 done, 8 done-already, 14 blocked, 1 deferred and 24 open
(q11954 to q12212, each with its HEAD census). Its files are in `naming/2026-11-20_agentNF191/`, with the full probe of 1,271
referencing rows (`work/probe_all.tsv`), the classified census (`work/census.tsv`) and q11677's row list. The open entries, the partial
families with rows left to port (q4411 / q6405, q4492, q4522, q6321, q6456, q11920 / q11923, q11936, q11942, q11677) and q6396 (its
binder's lock was NF189's, since landed) go back to the queue unclaimed (36 entries).

- 20 rows land: 2 MATCH (0x004944C0, 0x007AED30) + 18 RELOCATION_MATCH, every test PASS.
- Ports:
  - q4492: nine rows move in place onto the shared CIntelligentPointer<CThing>, binding FableWin's `IS_THING_ALIVE<CIntelligentPointer<CThing>>`
    at 0x004CC340 through a declaration-only template; no row emits a copy of it.
  - q4411: three header-less ??_G rows (0x00739CC0, 0x007F05F0, 0x009704B0) keep their own class token (defined at HEAD) and now derive
    from the shared base their RTTI names.
  - 0x00920960 moves onto the shared headers in the shape of sibling 0x00920DC0; 0x007BDAC7, 0x0068EB20, 0x00890C80 (the last with the
    new FLAT_FINISHCURRENTACTION opt-in).
  - 0x007BDAC7's landed copy local `const CThing* constHit` becomes `CThing* hitThing` (an `// Inferred:` line explains it). The hit
    test then binds FableWin `??$IS_THING_ALIVE@PAVCThing@@...` at 0x0040DCE0, a spelling landed rows already use, instead of the PBV
    instance. Passing `hit` itself by reference takes its address and differs (DIFFER 159v159), so the copy stays. This retypes a local
    that was already landed; it adds no temporary.
- Self renames (5), files moved with them; no landed row is left on an old leaf and no EH funclet includes a moved file:
  - 0x004944C0 `?ClearString@CFrontendGameComponent@@AAEXXZ` (private; both PDB name sets). Its test is rewritten on the shared header
    and reaches the private member through the explicit-instantiation friend idiom (EXTRA_RULES 2026-11-20 scaffolding).
  - 0x007AED30 CTCBoss::IncrementStrikesSinceHitHero (catalog was IncrementBossPhase; B's fix b1120p25E).
  - 0x00957170 the CCreatureAction_BriarRoseShotFarInto ctor (PDB reference form), 0x00A156D0 (const reference), 0x0064BE45 (moved
    into NPlayerGui).
- Headers (add-only, `orig` equals HEAD, default paths token-identical, no landed includer defines the macros, pin_asserts pinned 0):
  - engine/CFrontendGameComponent.h FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CLEARSTRING: splits `_body` into String +0x168 and StringPos
    +0x1E8 (Ego_r layout); the size is unchanged.
  - engine/CThingCreatureBase.h FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_FINISHCURRENTACTION (601 includers, all rechecked).
  - engine/CTCBoss.h FABLE_RETAIL_CTCBOSS_INCREMENTSTRIKESSINCEHITHERO.
- Split gate: 1 CREATED at 0x00857F30 (the CCombatActionBase ctor), a gate artifact (`logs/split_gate_note.txt`). 0x00957170 moves from
  the pointer form to the PDB reference form, and the gate left 0x00906380 ungraded. That landed row already binds the reference form at
  HEAD (HEAD reloc_bindings line and its source), so the address keeps the same two spellings. q11677 (103 pointer-form rows) returns
  to the queue to close it.
- Pick-any (`logs/pickany_check.txt`): seven new COMDATs (CIntelligentPointer<CThing> dtor / `->` / Get, ~CCreatureAction_RiseUp,
  the two GetCreature accessors, ??_GCCreatureAction_BriarRoseShotFarInto). None has a Leaf line in a landed row or a table line. The
  agent graded the ??_G copy RELOCATION_MATCH at 0x00957320, whose landed row carries a stand-in name; its rename is queued.
- Coordinator calls (queued, none blocks the landing):
  - q4522: three in-place forms at 0x005840A6 / 0x0065B2C4 / 0x0065BFFA still differ (retail passes a converted temporary); the next
    form is a whole-row port onto engine/CTCVillage.h after sibling 0x0065B142, which also binds `?Evict@CTCDummyVillager@@QAEXXZ` at
    0x0065F01D (NAME_FIXES).
  - q5192: 0x00C48770 is a compiler-generated copy constructor and the claimed name is an address-named stand-in; identity item queued.
  - q11957: 0x00871D30 is not CTCLook::UpdateLookTimer; NCameraModes::CStringAndCageBase::GetZCoord is in NAME_FIXES (both PDBs).
  - q11942: the four rows move as one family; 0x00833A70 needs a whole-row port first.
  - q11681: header claim on engine/CTCDPhysicalObstruction.h queued.
- Link (`logs/link_landing.log`): 0 LNK2005, as at HEAD (`logs/link_control_head.log`).
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline). Includers and referencing rows 170 good, 0 bad; owner recheck good.
- tablecheck (`logs/tablecheck.txt`): 0x004CC340 9 -> 5, 0x00662650 4 -> 3, 0x00446B10 2 -> 1. One line grows, 0x00436380 3 -> 4:
  0x007BDAC7's `IS_THING_ALIVE_00436380<CThing*>` stand-in on an unchanged source line (SAME as HEAD; the table lacked the line). No
  out-of-image oracle line added.

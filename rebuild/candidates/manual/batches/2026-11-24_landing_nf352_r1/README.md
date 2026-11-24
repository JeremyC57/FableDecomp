# nf352 r1: CCountedPointer<NUISystem::CComponent> Reset / ReleaseData size bodies (2 rows)

Agent NF352 (Account A naming queue). Hand-back: `pending/2026-11-24_nf352_handback/` (NOTES.md, STATUS.tsv, evidence/). This is the
agent's held/r1 text, landed on the NF352 rulings (advisor decision, owner delegated, 2026-11-24; brief NF352 entry):
- Q1: b1 (RESET alone) does NOT stand: there the already-NEQ ReleaseData copy's leaf reach flips n -> y, which the NF340 gate counts as a
  newly leaf-reached NEQ copy. Q1b: yes, the ReleaseData copy 0x004291DE takes the same opt-in (owner sets `#pragma optimize("s",on)`,
  no CompilerFlags entry; pragscan C_OK). Q2: two target COMDATs change, accepted (condition (a) is per opt-in; b1 isolates RESET).

Header `engine/CCountedPointer.h`: add-only block at the end (FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY,
_RESET_SIZE_BODY; per-type `template<> __declspec(noinline) inline` specializations between `optimize("s", on)` and `optimize("", on)`,
Fidelity: scaffolding, not retail). Only these two rows define either macro.

| Row | Function | Grade (repo recheck) | Reset 0x00429C6F | ReleaseData 0x004291DE |
|---|---|---|---|---|
| 0x00605570 | CTCInventoryMap::ConstructRegionDescription | RELOCATION_MATCH PASS (unchanged) | NEQ 103 -> EQ 59 | NEQ 53 -> EQ 44 |
| 0x00626110 | CTCQuickAccessMenu::ContructItemLists (map) | RELOCATION_MATCH PASS (unchanged) | NEQ 103 -> EQ 59 | NEQ 53 -> EQ 44 |

Coordinator verification (objects built from the repo after landing, vland `--recheck-landed`):
- pk_all with `--leaf=<// Leaf:>`, HEAD (agent evidence) v new: the only changed copies are Reset and ReleaseData, both NEQ -> EQ; the only
  reach flip (ReleaseData n -> y) lands on an EQ copy. pickany_vs_retail agrees line for line.
- Agent: leafrel leaf equal; objsec only those two COMDATs change; symbol tables equal; reversed control fails.
- Header closure: `lane_merge/equiv_check.py` old v new over the whole catalog, rerun by the coordinator (see logs/equiv_coordinator.txt),
  plus the agent's run (0 differing of 13,751; planted control 12,211 rows = the real includer set by `cpp -M`). The script's VC7.1
  includer recheck was skipped (8,593 includers; NOINC), because preprocessed-text equality makes it redundant.
- T3: nf340 0x00429C6F closed (T3_CLOSED.tsv; DEBT_BY_COPY lists exactly these two emitters). Queued: nf352 owner ports 0x00429C6F /
  0x004291DE onto the opt-ins. The ctor 0x00429C15 / dtor 0x00429C53 copies stay debt (every form moves the leaf).

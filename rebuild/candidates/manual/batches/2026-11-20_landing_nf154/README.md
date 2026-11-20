# Landing nf154 (Account A, 2026-11-20): CTCBase / CTCPhysicsBase / CTCLook / CTCCreatureNavigation names (23 rows)

Agent NF154 (round 10) handed back 24 entries (15 done, 7 done-already, 1 blocked, 1 deferred); its files are in
`naming/2026-11-20_agentNF154/`.

- 23 rows: 7 self renames (RENAMES section 1; 0x00723730 ported onto engine/CTCPhysicsStandard.h) and 24 caller respellings, each onto
  a spelling other landed rows already bind (NF154's census compiled all 373 referencing rows).
- Header: engine/CTCPhysicsStandard.h, one add-only declaration opt-in; `orig` equals HEAD.
- Advisor decisions (owner delegated):
  - 0x008006D0 keeps its CNavStepVector stand-in and passes it to the PDB `vector&` by reinterpret_cast, marked
    `// Fidelity: cast, not retail`; the real port waits on the packed retail C2DVector (queued).
  - 0x00714730's row-local CThing model now emits an unreferenced GetInterface<CTCCreatureNavigation> COMDAT; no other landed object
    emits that symbol, so there is no second body to disagree with. Accepted.
  - q11661 (0x00751E80 / 0x007525C0): the explicit-instantiation scaffolding (EXTRA_RULES 2026-11-20) was tried in three forms (plain,
    and /d2inlT200 through the catalog): DIFFER 136v306 / 94v115, because the CTCLook.h inline bodies are not retail-shaped
    (`logs/q11661_scaffolding_form_*.log`). The pair is held and queued.
- Recheck (`logs/recheck.log`): 23 RELOCATION_MATCH, every test PASS. rebind: removed 62, added 50; multi-check 1 (baseline); split gate
  0 created; includers 123 good, 0 bad. tablecheck: ten addresses shrink; 0x006A98D0 shows 0 -> 1 only because the table now lists
  0x005DAB10's stand-in binding its HEAD object already had (SAME).
- Link (`logs/link_landing.log`): 3 LNK2005 (CGameEvent(long), the CThingAICreatureInit ctor, CTCLook::SetHeadTurnState); the HEAD
  objects give the same 3 (`logs/link_control_head.log`). CGameEvent was queued by b10; the other two are queued as DEBT.

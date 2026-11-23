# Landing nf306 (Account A, 2026-11-23): the tail of the T1 "other" made-up-name batch (26 rows, 37 table lines)

Hand-back: `pending/2026-11-23_nf306_handback/`; helpers under `naming_batch/agent_tools/nf306/` (mkstayoff.py, so_check.py, tmplcls.py,
symdiff.py, tbladd.py, tblafter3.py, rep.py, st.py). 88 entries with a STATUS line: 16 done, 8 done-partial, 1 done-conditional (NOT
landed), 29 skipped-port (11 `.drectve`, 18 same-TU body), 14 held, 20 open; no header, flag, catalog or test change.

- Unit A (15 binders): name plus the PDB parameter types (pointer -> reference, const, long -> unsigned long, long -> bool with constant
  arguments), and the owner 0x0066B080's `InternalPlayAttachedSample` private with a full Leaf (NF302 Q5; its two other callers already bound
  the private form, no landed row binds the public one). Unit C (9 binders), 0x00602F30 also `GetPMiniMapDisplay` -> `GetMiniMap`; entry 87's
  free function moves out of `NUISystem` to the global scope the PDB gives. Entry 11 (0x005E4430, NF303's identity read): the donor
  Activate calls `GetQuestObjective`, so the binder now calls `GetQuestObjective() const` (it had attached GetQuestName, 0x00702310's name).
- Proof: objsec ALL_EQ (`.drectve` included), each new symbol the entry's PDB spelling; two sources per row (both PDB name sets + a donor
  call or the owner's Leaf); links: nothing HEAD resolved becomes unresolved. Every delete checked against all exe call sites and absolute
  references. Staleness: all 28 orig/ copies equal HEAD at landing (after nf304 / nf305).
- Table: 26 deletes + 11 adds applied; the 2 deletes conditional on Q2 are NOT (`logs/table_conditional_held.tsv`).
- Rulings: Q2 entry 24 (0x00B8D0B0, a 5-byte `xor eax,eax ; ret 8` stub) is held: GOTCHAS NF30 says such stubs are not named on thin
  evidence, and the only donor link is one step indirect (GetPrevFrameResult, inlined by retail); its 2 binders (0x00B6E260 / 0x00BE79D0)
  wait in T3 as identity research. Q3 a binder-only fix where the self row already carries the PDB Leaf is fine (entries 19 / 49).
- Queued: 30 T3 lines (11 drectve units, 18 same-TU-body ports, the entry-24 identity item); NAME_FIXES_QUEUE: owners 0x00BACDD0
  (`AddPrimitive`), 0x00437CE0 (`GetMiniMap`) and 0x00B8D0B0 (marked HELD with entry 24).

## Recheck
`logs/recheck.log` (landed tree): 26 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.

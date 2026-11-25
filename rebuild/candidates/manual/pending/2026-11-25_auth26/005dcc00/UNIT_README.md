# Unit 0x005DCC00 + 0x005E15E0 + 0x005E1500 (AUTH26, 2026-11-25)

Land as ONE unit: the new row 0x005DCC00 CAnimationSet::GetRandomAnim calls (and emits pick-any copies of) the two helpers, and
the helpers' landed self spellings were wrong; landing the row alone would create two naming splits.

| row | grade (also STRICT_HEADERS) | test (vt.py, linked to the row object) | name gate |
|---|---|---|---|
| 0x005DCC00 GetRandomAnim (new) | RELOCATION_MATCH 264 B | PASS, 10 cases, 3 mutants killed | PASS with the respells in place (NAME_GATE_005dcc00_after.txt); FAIL(2) against today's tree (_before.txt) |
| 0x005E15E0 vector<const pair<unsigned long,CAnimationEntry>*>::reserve (respell) | RELOCATION_MATCH 179 B; HEAD source also RELOCATION_MATCH | PASS (new test on the real element type) | PASS |
| 0x005E1500 same vector ::_M_insert_overflow (respell off a stand-in) | RELOCATION_MATCH 216 B; HEAD source also RELOCATION_MATCH | PASS (new test on the real element type) | PASS |

- rows/, tests/: the three sources and tests. orig/: the repo copies of the two landed rows' source and test (by address).
- Identity evidence for the element type: rows/005e15e0.cpp header (Ego_d found_anims type 0x3AEEC in both callers; Ego_r.exe
  call targets; retail's const CAnimationEntry* instance is a different function, 0x005E1420). Also ../NAME_FIXES.tsv.
- reloc_bindings.tsv has no line at 0x005E15E0 or 0x005E1500 today, so no landed caller moves; the only landed spellings were
  the two rows' own `// Leaf:` lines. No other row includes either file by path.
- The respelled helper rows are explicit instantiations of the shared stlport/fable_containers.h bodies (no local vector model
  any more): reserve with FABLE_RETAIL_STLP_VECTOR_RESERVE + _ALLOCATE_AND_COPY + _C (the landed 0x0077E580 set), overflow with
  FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE, i.e. exactly the bodies row 0x005DCC00 sees.
- LINK_CHECK.txt: the three objects link with no duplicate diagnostic (control: row 1's object twice under two names gives
  LNK4006 under /FORCE). The pick-any copies of reserve and _M_insert_overflow in 0x005DCC00's object are byte- and
  relocation-identical to the owner rows' objects (objdump -d -r compare) and to retail (retcmp EQ, relocs EQ).
- NAME_GATE_005dcc00_after.txt comes from gate_after.py, a read-only wrapper that runs name_gate.py with the two landed rows'
  `// Leaf:` lines read from rows/ instead of the repo; nothing else changes. Spellings it binds:
  0x005E15E0 `?reserve@?$vector@PBU?$pair@KVCAnimationEntry@@@std@@V?$allocator@PBU?$pair@KVCAnimationEntry@@@std@@@2@@std@@QAEXI@Z`,
  0x005E1500 `?_M_insert_overflow@?$vector@PBU?$pair@KVCAnimationEntry@@@std@@V?$allocator@PBU?$pair@KVCAnimationEntry@@@std@@@2@@std@@IAEXPAPBU?$pair@KVCAnimationEntry@@@2@ABQBU32@ABU__true_type@2@I_N@Z`.
- RESULTS.tsv / SELF_SYMBOLS.tsv / CATALOG_CHANGES.tsv in the landing formats; the 0x005DCC00 payload / oracle are in ../payload.json,
  ../oracle.tsv. Locks: 0x005E15E0 and 0x005E1500 claimed AUTH26.

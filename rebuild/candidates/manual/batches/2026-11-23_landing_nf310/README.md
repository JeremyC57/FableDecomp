# Landing nf310 (Account A, 2026-11-23): T1 signature, NF307's continuation (38 rows, 1 test, 24 table lines)

Hand-back: `pending/2026-11-23_nf310_handback/` (held/ keeps the measured variants for D1 / D2); helpers under
`naming_batch/agent_tools/nf310/` (nf.py now removes its scratch through clean.py; the inherited nf304 / nf307 copies used rmtree / unlink
on their own work/ scratch: copy nf310's). 74 of 140 entries plus the 0x00713EE0 rider: 18 done, 4 done-partial, 33 skipped-port,
19 skipped-held, no rejects; no header change.

- Rider (NF307 D2): 0x00713EE0 gets 0x00714900's edits (CCreatureNavOutput nested, LookAtPos by reference), so NF307's two rewitness lines
  become deletes. Pointer -> reference / const pointer / `unsigned long` (entries 2, 12 partial with 8 untabled binders, 15, 20, 24, 29, 32,
  33, 62, 70 incl. the untabled 0x00417001, 75); shared enum headers (3 EHeroAbility member retype, 17 ETargetingType self + 4 binders,
  5 / 6 binder 0x00766480 only); return types (71 `bool`, 62 `unsigned long`); access (76 protected const by pointer; 60 protected +
  `friend class CCreatureAction_UseSwitch`, NF305 Q5); 78 `static __fastcall` with its test stub (0x004022B0's test).
- Self fixes with full Leafs (NF304 Q6): 0x0051EC50 AddToUpdateList(CThing&), 0x007F5A70 IsTargetedThingOfType(ETargetingType) const,
  0x006A7930 CThingFilter_InMap::operator()(const CThing&) const (its renamed getter copy byte-equal to 0x0050A650's, NF304 Q1).
- Proof: RELOCATION_MATCH / MATCH at HEAD and new; objsec ALL_EQ (`.drectve` clean, no table emitted, `evidence/proof_all.txt`); symbol
  diffs only the respelled symbols; links: no new duplicate, nothing HEAD resolved left unresolved. Staleness: all orig/ copies equal HEAD
  at landing (after nf308).
- Table: 14 delete (incl. the stale InMap PAV line: witness 0x006186F0 binds ABV, compiled at landing), 10 replace.
- Rulings: D1 the binder-only partial lands (0x00766480 resolves to the byte-equal pick-any COMDATs 0x00839100 / 0x007677E0 emit); the
  selves go T3 for the nf86 / nf88 pick-any form. D2 entry 37 is one unit with 0x005296B0 once that row is free (continuation). D3 agreed: a
  T3 port. D4 the friend lands (both PDB name sets give protected and the only retail caller is that class: NF305 Q5).
- Queued: 36 T3 lines (33 ports, the 0x00766050 / 0x007660C0 pick-any unit, 0x00836A50, the 0x009FB750 `CTexture::Unlock` scaffolding);
  NAME_FIXES_QUEUE: 4 lines + the 0x009F9DE0 `CTexture_Methods` self.

## Recheck
`logs/recheck.log` (landed tree, 38 rows + witness 0x006186F0): WINS 39 (37 RELOCATION_MATCH + 2 MATCH), 0 DIFFER, 0 RELOC_CONFLICT; 0x006186F0 binds the ABV InMap operator (`logs/witness_head.log`).

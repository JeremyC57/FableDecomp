# Landing nf152 (Account A, 2026-11-20): CEngineResourceManager getters and CEngineShadowRenderer names (8 rows)

Agent NF152 (round 10) handed back 17 entries (8 done, 6 done-already, 1 separable self port, 2 blocked); its files are in
`naming/2026-11-20_agentNF152/`.

- 8 rows, all self renames (RENAMES section 1) onto both-PDB spellings that landed callers already bind, with file moves to the new names.
  0x00B3B0A0 GetIndexBufferMemPool lands as the separable self port (advisor decision, owner delegated): its old leaf (a free
  AddChildPrimitive) is bound by no landed row, and nf149 already respelled the last stand-in binder 0x00BE91E0, so the address now has one
  spelling. 0x00B4FC10 calls the shadow renderer's OnReleaseDefaultPoolResources as the virtual override (both PDBs).
- Headers: CEngineResourceManager.h and CEngineShadowRenderer.h add-only; overlays' `orig` equal HEAD.
- Recheck (`logs/recheck.log`): 3 MATCH + 5 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  tablecheck: 0x009BEDE0, 0x01436E1C (GPDisplayManager stand-ins 6 -> 5) and 0x00B4B830 shrink.
- Includers (`logs/includer_recheck.txt`): 207 good; 2 bad, the July landscape rows at their HEAD grades (0x00BE7D70, 0x00BFE050).
- Link (`logs/link_landing.log`): the 8 objects, no owner rows: 0 LNK2005.
- q4444 / q4733 stay blocked on the 0x00418DCA CMainGameComponent ctor port; its header claims are queued.

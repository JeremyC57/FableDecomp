# Landing nf287 (Account A, 2026-11-23): T1 standin_alias, NF285's continuation (14 rows, 1 header opt-in, 22 table-only fixes)

Redone from the repo tree after the 2026-11-23 restart. Hand-back: `pending/2026-11-23_nf287_handback/`; the agent's 33 helpers are
committed under `naming_batch/agent_tools/nf287/` (objbind.py: what a binder's HEAD object really references; qq.py: capstone-free grade).
97 of 272 ENTRIES reached; 175 go to NF289.

- **GFVYToAY / GFAYToVY / GFVXToAX / GFAXToVX (0x009E1E40, 0x009E1DD0, 0x009E1E10, 0x009E1DA0):** every converter caller censused
  against the exe call sites. 0x009DDFF0 / 0x009DDA50 / 0x009DD210 / 0x009DCA80 declared GFVYToAY as GFAYToVY (at HEAD 0x009DD210 and
  0x009DCA80 linked into 0x009E1DD0's body), 0x0048F230 declared it `__stdcall`; all now `__fastcall GFVYToAY`. Same lines: GFVXToAX in the
  same five rows, and 0x00438AA0 called 0x009E1DA0 as GFVXToAX (no table line): now GFAXToVX (decision 1, accepted). Each converter now has
  one spelling and each spelling one address (both PDB name sets).
- **0x0097D580 AIAction_LookAtPos (partial):** new add-only opt-in `FABLE_RETAIL_CAISTATEGROUPBASE_FLAT_AIACTION_LOOKATPOS` in
  `engine/CAIStateGroupBase.h`'s flat view (declaration of the protected PDB member); equiv_check over 1,970 includer rows: 0 differ, the
  planted-token control flags 82. Binder 0x0094A6D0 uses it; 0x00968230 needs a port (T3 queue).
- **0x0041A410 GetStaticMapDir:** four binders (three had no table line, found from exe call sites). **0x00AE18C0
  CParticleSimulation::WriteVectorArray:** two binders, 0x00AE2230 with `friend class CParticleProgram;` as the shared header has it.
- **0x00AF8B00 CDecal::operator new (decision 3, accepted):** binder 0x00AF5380's inline `__cdecl` wrapper around the free stand-in becomes
  the out-of-line `__fastcall` PDB member (`??2CDecal@NParticleEngine@@SIPAXI@Z`, both PDBs); its object stops emitting the wrong-spelling
  pick-any copy `...SAPAXI@Z`, which no other landed object references; every remaining COMDAT and the vtable SAME; the owner row defines SI.
- **22 table-only entries (done-already):** each witness's HEAD object already binds the PDB spelling (stale witness); no landed source binds
  the stand-in; their stale lines deleted, PDB lines added where missing. Plus 0x005BD82E (one witness table-only, the other a port).
- 68 skipped-port (each binder opened and read; queued T3). Held: 0x0090DEE0 (decision 2: the respell adds a `/alternatename` line to
  `.drectve`; not an unchanged object, T3).
- Table: 30 lines deleted, 13 added, 3 respelled; no symbol gains a second target.

## Recheck
`logs/recheck.log` (landed tree): 14 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT. Rule slips: owner rows locked for link checks
before their tests were prechecked; a recheck payload piped into `head` graded HEAD (discarded, rerun); GOTCHAS not re-read after the
restart. The NF289 prefix repeats the rules and asks for orig/ copies (NF287 kept none; staleness checked by git history instead: none of
the 14 sources changed since the restart).

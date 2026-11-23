# Landing nf304 (Account A, 2026-11-23): T1 signature, NF300's continuation (61 rows, 1 test, 46 table lines)

Hand-back: `pending/2026-11-23_nf304_handback/`; helpers under `naming_batch/agent_tools/nf304/` (new: census_addr.py lists every exe caller
of an address, bindaddr.py what a row binds at an address; caveat: `xr` / `bind.py` take the first symbol containing a bare Leaf, so a `-`
result needs an objdump check). Entries 1-60 reached: 40 done, 1 done-partial, 17 skipped-port, 1 skipped-held, 1 deferred (55); no header change.

- 61 rows respell binder signatures onto the PDB spellings in their local models; five self rows move with every binder HEAD resolved
  against the old self spelling (0x00A10DC0 with its test, 0x00555440, 0x005556F0, 0x00492F69, 0x006CB780; units: entry 27 / 28 C2DVector
  to the global `::C2DVector` in 6 rows, entry 37 SetText self + 8 binders, entry 54 self + 3, entry 19).
- Proof: RELOCATION_MATCH at HEAD and new; objsec ALL_EQ (`.drectve` included, tables unchanged); a per-row HEAD v new symbol diff shows
  only the respelled method symbols (references in binders, definitions only in the 5 selves); links HEAD v new with controls: no new
  duplicate, nothing HEAD resolved left unresolved; the name census finds no new spelling defined anywhere but its own self. Every delete
  checked against every exe caller of its target (compiled, or read where another agent held it). Staleness: all 62 orig/ copies equal HEAD.
- Table: 35 deletes (5 stale-witness deletes with no row change: entries 5, 10 x2, 32, 45), 11 replaces.
- Rulings (standing brief): Q6 the five self signature fixes land (NF302 Q5 widened to PDB-signature self fixes with every binder moving
  together; NF282 D2 is about re-scoping a self onto a made-up owner). Q1 a re-scope whose only renamed definitions are pick-any copies of the
  type's own implicit members or local inline getters, now in the PDB spelling, lands when each copy's body equals the landed PDB definition or
  no landed strong definition of that symbol exists (otherwise T3, NF300 entry 2); entries 1-3 and 8 go to the continuation. Q2 retyping a
  local model's `long` member and parameter to the shared 4-byte enum (`EOpinionDeedType`) lands under NF300 Q1 (entries 56 / 57 to the
  continuation). Q4 agreed: `SetDisplacementParams` (0x00884F90) is in neither name set, so it stays. Q5 entry 9 is queued as a unit with
  the T3 item 0x004498C0 (the GetPlayer witness already moved to 0x00449BF0 at nf302).
- Queued: 13 T3 lines (11 ports, entry 9's unit, 0x00B32000 as identity research: probable `CEnginePrimitiveRenderer::UpdateRepeatedZSprite`,
  unproven); NAME_FIXES_QUEUE: 8 self rows left on stand-ins + the 0x00A76A80 `CSmallVector::resize` copy note.
- Slip noted: one `rm -f` on a glob in the agent's own work/ (matched nothing); clean.py otherwise.

## Recheck
`logs/recheck.log` (landed tree): 61 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.

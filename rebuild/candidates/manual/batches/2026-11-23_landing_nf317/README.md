# Landing nf317 (Account A, 2026-11-23): 45 rows from the in-place rejects of the scripted routes (7 table replaces)

Hand-back: `pending/2026-11-23_nf317_handback/`; helpers under `naming_batch/agent_tools/nf317/`. Entries: the 112 rejects of
`access_fix.py` (access 1-4, binders 1). Outcome: 47 done, 56 skipped-port, 9 skipped-held.

- **Landed: 45 rows.** That is 40 entries plus 5 unit binders, moved so no new split appears. 6 tests moved with their rows (LINK_FAIL).
  - **Proof (agent, re-done at landing):** precheck; objsec ALL_EQ with `.drectve` included, the COMDAT sets the same, and no new vftable;
    the graded symbol is the PDB symbol; link check.
  - **The script's rejects were often artifacts.** 9 objsec rejects differed only in the `?A0x` anonymous-namespace hash, because HEAD and
    new had been compiled in different directories. Gotcha recorded.
  - **13 `access1` Q->U / Q->M entries** landed with `virtual`, under NF299 Q3, with the vptr slot taken from the RTTI vtables.
- **Rulings on NF317's questions** (recorded in the standing brief):
  - **Q1 (T3, entries 24, 26, 27, 28, 41, 42, 56 held; 8 rows not landed):** const overloads added to local inline container stand-ins
    (CLongVector, global `vector`, CRetailVector), whose pick-any helper COMDATs then carry renamed stand-in symbols. These are NF309 Q3:
    const forced onto same-TU stand-ins.
  - **Q2 (lands under R1, entries 29, 37, 45):** const on a force-inline lookup (GetTC / GetInterface) or a virtual-slot call of the local
    model. The PDB-const body forces it, and no symbol is emitted.
  - **Q3 (port):** the 23 CCreatureAction_*::Clone entries are a port onto engine/CCreatureActionBase.h. A stand-in vftable that grows is
    the brief's vftable change, so it is not allowed.
  - **Q4 (keep the retail bytes):** the 14 return-type entries return the int / byte width retail's bytes show, while the PDB says bool /
    void. The retail code wins, and these are recorded as a PDB revision difference; no respell.
- **Withdrawn by the agent: entry 108 (0x008C63C0).** It compiled ALL_EQ, but a T3 line records that the donor
  TrollTunnel::PerformAction never calls WhackGround, so the binder's identity is in doubt.
- **Table:** 7 replaces applied (`TABLE_CHANGES_applied.tsv`). The 8th line, entry 56's witness, is held with its entry. Every witness
  object references the new spelling (checked at landing).
- **Queued:**
  - T3 lines `nf317` for the 56 ports and the 7 entries held by Q1;
  - NAME_FIXES_QUEUE: 0x00958C00 / 0x0096D2D0 (self rows define free `__fastcall` stand-ins), 0x006A4CF0 (row CParticleSystem_Kill, RTTI
    CThingPhysical::OnDie), 0x008B0650 (stand-in self, 6 spellings).
- **Agent slip:** one stray file outside the scratchpad (`/tmp/x_rc`, an address list). It holds no repo content and was removed.

## Recheck
`logs/recheck.log` (landed tree, 45 rows): 45 MATCH / RELOCATION_MATCH PASS.

# nf363 b1: 0x0042AF3C respelled in place onto ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z

NF360's b1 text (hand-back `pending/2026-11-24_nf360_handback/`) finished by agent NF363 (`pending/2026-11-24_nf363_handback/`). The made-up
self `?GetDef_0042af3c@...` becomes the FableWin.pdb symbol (reloc_bindings line witnessed by 0x007CBDC0; 119 retail direct calls) as a
strong `template <>` specialization on the local model, the 0x0042B040 form.

Rulings (advisor decision, owner delegated, 2026-11-24): R1 the strong in-place form lands, although the link adds one duplicate against a
different body: the ten landed strong copiers already define the PDB name with seven different NEQ bodies (NF363 compiled all ten:
b1/evidence/copiers363.tsv; queued T3 nf360). R2 the `reinterpret_cast<CDefPointer<const CDefClassBase>&>(*out) = def` assignment is marked
`// Fidelity: scaffolding, not retail`; it goes with the nf349 header port (T3 nf349 0x0042AF3C, still open).

Agent gates: vland RELOCATION_MATCH PASS HEAD and new; objsec OTHER_SAME, symbol delta 0; checkers PASS with reversed / planted controls;
reach flips 0; drop test clean; link with owner binder 0x004CA010: the PDB name now resolves, 0 duplicates.
Coordinator from the repo (`$S/v363.sh`): RELOCATION_MATCH PASS (SHARED_HEADER_AVAILABLE warnings only); pk_all HEAD (agent) v repo: 0 new
NEQ; pickany_vs_retail 0 new NEQ; reachflip 0. T3 nf349 / nf360 0x0042AF3C lines parked during the landing and restored in place, not
released. self_symbols refreshed; NAME_FIXES lines for 0x0042AF3C closed (NAME_FIXES_CLOSED.tsv). File rename to
`CDefinitionManager_GetDefCThingBaseDef_0042af3c` proposed only (engine/CDefinitionManager.h line 242 cites the old path).

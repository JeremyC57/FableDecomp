# NF432 hand-back (2026-11-25): NF430's identified ports

| batch | verdict | rows | what |
|---|---|---:|---|
| batches/n1 | ACCEPT | 7 | 5 NPlayerGui draw ctors onto engine/NPlayerGui_CDrawBase.h (CTOR + VSLOTS) + 4 new class headers; 0x00442760 -> ClearPrimitives (NAME_FIXES, RENAMES); 0x00644627 ported to the PDB ClearPrimitivesIfNecessary (needed: otherwise slot 0's name has no definer); add-only opt-in CENGINEPRIMITIVEHANDLE_CLEAR_DECL_ONLY |
| batches/g1 | ACCEPT | 3 | CScriptBase::RegisterMain / Main / Init (0x00CBD4B0 / C0 / D0) respelt from the shared gapscan stand-in `T::Hook`: the cause of most "wrong slots" on NF430's debt rows. Outside the listed rows; found while measuring Task 3 |
| batches/hold_n1d | HOLD | 1 | 0x0064D3DA CDrawDamage ctor: STLport list ctor copy NEQ to 0x00655B18 |
| batches/hold_d1 | HOLD | 1 | 0x00C3C840 CCodaObjectBase ??_G (D3): ruling R-NF432-1 |
| batches/hold_g1o | HOLD | 1 | 0x00CBD4E0 CScriptBase::OnPersist: pick-any clash with two local models |

Not ported: Task 2 (CEntityScriptBase, 0x00EB2000 / 0x00DEF6A0, CSP_UndeadTrigger) and Task 3 (the 102 debt rows), for context budget;
both have resume notes in T3.txt. Pilots for R-NF430-1 are in NOTES.md. Method, sources and every gate: NOTES.md; per-row: batches/*/CHECKS.tsv.
Tools: tools/ (mkroots.sh, runpair432.sh, ngate432.sh, mut432.sh, mkdefs.sh / hobj.sh, gen432n.py, vt432.py + coffload.py, copies of
gate425 / pkgate425 / linkgate425 / link377 / nf.py / nf387.py repointed here).

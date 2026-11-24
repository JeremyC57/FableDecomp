NF342 helpers (2026-11-24): stale-line sweep of rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv. Read-only on the repo; absolute paths into
w22/agentNF342 (common.py holds R / S / A / W). Run every python with PYTHONDONTWRITEBYTECODE=1 (verify_and_land is imported from the repo).
  common.py     loaders: queue (line no + sha1), PDB name sets, reloc_bindings by target, catalog rows (bounded blocks), Leaf, OPEN.tsv, locks,
                landed_by index (batches/2026-11-2*/{rows.txt,RESULTS.tsv,TABLE_CHANGES.tsv,README.md} + pending/*_handback/{RENAMES,APPLIED,TABLE_CHANGES*})
  classify.py   every queue line -> work/lines.tsv: kind (self / binder / binder2 / unit / table / other), exact PDB target P (text, tracker
                pdb_spelling / rtti, or the one PDB name with the same ?name@scope@@), named binders, cheap pre-filter
                (self: Leaf / tracker self_leaf / self_symbols.tsv when the source is older than it; binder: table witnesses; nf303: its census
                evidence; unit: whole target agrees in the table)
  pre_rows.sh   clang precheck (nf.py pre) of HEAD source + test per row -> work/pre.log ; zs_list.sh: D1 cl /Zs (+ NOWCHAR clang) -> work/pre_zs.log
  census.py pay|read + cen_run.sh   recheck.py payload -> vland --recheck-landed (background, timeout 900 on its own line) -> per row grade,
                graded symbol, DEF / UND, and every relocation of the graded function resolved against the retail bytes (target -> symbol)
                -> evidence/census/<addr>.json ; skips rows locked by another agent or without a passing precheck ; cleans wd with clean.py
  judge.py      lines.tsv + census + work/manual.tsv -> STALE_T3.tsv, PARTIAL.tsv, work/verdicts.tsv
                self STALE = DEF P, graded symbol = P, grade MATCH / RELOCATION_MATCH + PASS (no named stand-in symbol left in DEF / UND)
                binder STALE = every named binder that binds the target binds only P there (UND, not a strong DEF), the table has no
                non-P spelling at the target, and the target row itself defines P (else PARTIAL with what is left)
  nf.py         NF339's helper (NF323 lineage) retargeted to agentNF342 (pre / zs / coff / ext_syms / flags_of)

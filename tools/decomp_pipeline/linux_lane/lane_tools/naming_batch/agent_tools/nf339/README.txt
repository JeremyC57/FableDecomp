NF339 helpers (2026-11-24), all with absolute paths into w22/agentNF339; cwd = work/ for anything that imports verify_and_land.
  triage.py              scripted re-read of ENTRIES.tsv (no compiles) -> work/tri.tsv, work/tri2.tsv (adds self_symbols.tsv + row declaration)
  mktriage.py            TRIAGE.tsv + STATUS.tsv skeleton from tri2 + work/census_def.tsv + work/triage_override.tsv
  pre_rows.sh / pre_new.sh   clang prechecks (nf.py pre) of HEAD sources / tests and of rows/ drafts -> work/pre.log
  zs_list.sh             D1 cl /Zs prechecks (nf.py zs) -> work/pre_zs.log
  census.py pay|read + cen_run.sh   HEAD census: vland --recheck-landed over landed rows, then per row: graded symbol, DEF of the PDB symbol,
                         UND of the old / new callee spelling (binder lines) -> work/census_def.tsv
  medit.py               zero-line-delta replacements into rows/<a>.cpp and rows/<a>_test.cpp (key <a>_test), originals to orig/ (+ orig/tests/)
  ver.py pay|read + ver_run.sh   HEAD v new proof at ONE vland work path: objsec under renames, external symbol delta, graded symbol, grades
  assemble.py            land_scripted run dir batches/<bN>/ from a ver.py proof (RESULTS / UNITS / rows / orig / tests / head + HEAD_FROM_REPO)
  nf.py                  NF336's helper retargeted (pre / zs / syms / src_of / test_of ...)

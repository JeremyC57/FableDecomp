NF293 helpers (paths set to w22/agentNF293). Pipeline per entry group:
  nf.py census <idx..>            exe scan -> work/census.tsv          nf.py scen <idx..>  source census (heuristic; crashes on ??1 names)
  pdbsets.py <idx..>              exact pdb_spelling in egor / fablewin name sets -> evidence/pdb_namesets.tsv
  sigdiff.py <idx..>              signature triage: each wrong spelling flagged STANDIN (class in neither PDB name set) or INPLACE?
  pre_tests.sh <files.tsv> <done> clang precheck of every landed TEST, BEFORE the locks (files.tsv: addr owner src test)
  lock.sh (repo toolkit) ...      pre_list.sh <changed> <controls> <done>   clang prechecks (NOWCHAR via work/nowchar.txt)
  cpp_err.sh <file..>             gcc cpp #error pass before any cl /Zs     zsq.sh <list> <done>   D1 / D1b / D1c cl /Zs (background)
  grade_lane.sh <tag> <changed> <controls> <done>   one row per `timeout 900` grade line (run 4 in parallel)
  bind.py <objdir> <idx..>        what each HEAD object binds at the entry address + the self's DEF  (the work list)
  defscan.py <objdir> <idx..>     census-row objects that DEFINE the entry's method (same-TU strong copies -> T3)
  gl.sh <addr> <regex>            non-comment source lines of a landed row (declaration / call sites)
  fix.py / rep.py / medit.py      zero-line-delta edits (medit: JSON {addr: [[old,new,count]..]}, saves orig/ + rows/)
  nf.py proof <tag> <rows..>      objsec HEAD v new;  drectve_eq.py <rows..>  .drectve equality with the ?A0x path hash mapped
  gen_ren.py <rows..>             RENAMES pairs from the objects (prefix match; ?A0x path hashes skipped)
  link_g.sh                       HEAD v new /FORCE link with controls;  nf.py link <tag> <watch> <objs> -- <objs> (ad hoc pairs)
  nf.py pay work/<tag> <rows..> + recheck.sh <tag> [overlay]   vland --recheck-landed -> work/recheck_<tag>.log
  assemble.py                     STATUS / APPLIED / RENAMES / TABLE_CHANGES / rows.txt from work/st.tsv ren1 ren2 tc_extra
NF286 / NF288-only (kept for reference): links.sh, run_grades.sh, pre_all.sh, recheck_final.sh, recheck_g3.sh, recheck_g4.sh, mism.py.
NF293 additions:
  decl.py <idx..>   (DECL_HITS_ONLY=1 DECL_MAX=n) per census row: non-comment declaration lines of the method / ctor, header-only rows counted
  declsum.py <decl output>   compact: only the declaration lines with a pointer parameter (pointer -> reference triage)
  calls.py <addr> <Name>     call / temporary / mem-initializer sites of Name( in a landed row (not declarations)
  ins.py <spec.json>         line-level insert / replace (for one-line #define opt-in users and tests: rows/<addr>_test.cpp via key <addr>_test)
  NF_TREE=<full include tree> for nf.py pre / pre_list.sh; FABLE_HDR_OVERLAY=<same tree> for grades, zs and recheck.sh <tag> <tree>
  nf.py blk(): now bounded to the row's own [pscustomobject]@{ ... } block (NF294's catalog-lookup bug class)

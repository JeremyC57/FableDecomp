NF288 helpers (paths set to w22/agentNF288). Current pipeline, per entry group:
  nf.py census <idx..>            exe scan -> work/census.tsv          nf.py scen <idx..>  source census (heuristic)
  lock.sh (repo toolkit) ...      pre_list.sh <changed> <controls> <done>   clang prechecks (NOWCHAR via work/nowchar.txt)
  cpp_err.sh <file..>             gcc cpp #error pass before any cl /Zs     zsq.sh <list> <done>   D1 / D1b cl /Zs (background)
  grade_lane.sh <tag> <changed> <controls> <done>   one row per `timeout 900` grade line (run 4 in parallel)
  bind.py <objdir> <idx..>        what each HEAD object binds at the entry address + the self's DEF  (the work list)
  mism.py <scen> <idx..>          source-census mismatches with the declaration lines (hints only)
  fix.py / rep.py                 zero-line-delta edits;  nf.py proof <tag> <rows..>  objsec HEAD v new
  gen_ren.py <rows..>             RENAMES pairs from the objects;  link_g.sh  HEAD v new /FORCE link with controls
  nf.py pay + recheck_final.sh / recheck_g3.sh   vland --recheck-landed;  assemble.py  STATUS/APPLIED/RENAMES/TABLE/rows.txt
NF286-only (kept for reference, hard-wired to its batch): links.sh, run_grades.sh, pre_all.sh.

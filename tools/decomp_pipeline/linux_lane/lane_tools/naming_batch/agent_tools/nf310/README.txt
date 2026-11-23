NF310 helpers (paths set to w22/agentNF310). Pipeline per entry group:
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
NF310 additions:
  decl.py <idx..>   (DECL_HITS_ONLY=1 DECL_MAX=n) per census row: non-comment declaration lines of the method / ctor, header-only rows counted
  declsum.py <decl output>   compact: only the declaration lines with a pointer parameter (pointer -> reference triage)
  calls.py <addr> <Name>     call / temporary / mem-initializer sites of Name( in a landed row (not declarations)
  ins.py <spec.json>         line-level insert / replace (for one-line #define opt-in users and tests: rows/<addr>_test.cpp via key <addr>_test)
  NF_TREE=<full include tree> for nf.py pre / pre_list.sh; FABLE_HDR_OVERLAY=<same tree> for grades, zs and recheck.sh <tag> <tree>
  nf.py blk(): now bounded to the row's own [pscustomobject]@{ ... } block (NF294's catalog-lookup bug class)
NF310 (2026-11-23) additions:
  show.py <idx..>   per entry: PDB spelling demangled, every WRONG spelling with its table witness, and for the self + each census row the
                    non-comment lines naming the method / ctor (declarations and calls, continuation lines joined): the one-screen triage
  work/offlimits.txt is built from the brief's held rows + the still-reserved Codex p28 rows + every T3_PORT_QUEUE address (decl.py / show.py tag OFF)
NF310 (2026-11-23, this pass) additions / changes:
  show.py           now handles operator names (??R, ??2, ...), member templates (??$); SHOW_WIT=1 prints only the self + the rows that witness a WRONG spelling
  pickany.py        (copied from agent_tools/nf298) <obj> <symbol> <owner addr>: COMDAT body v Fable.exe at the owner (reloc fields zeroed) + each reloc's retail target v the table
  cpp_err.sh        honours NF_TREE (full include tree) before hdr/
  work/offlimits.txt + work/other_agents_entries.txt: built from the brief's held rows, Codex p28 RESERVED (still reserved), T3_PORT_QUEUE addresses, NF299 / NF301 ENTRIES
NF310 (2026-11-23, T1 signature continuation) additions:
  census_addr.py <addr..>      exe census (calls / jumps / absolute dwords) for ANY retail address (a respelled callee outside ENTRIES)
  bindaddr.py <objdir> <target> <rows..>   what each row's object binds at a target address (nf.py xr)
  workflow per group: work/g<N>_changed.txt + g<N>_controls.txt (selves + the FULL exe census of every entry touched, minus other
  agents' entries / locks), pre_tests.sh BEFORE the locks, lock.sh, medit.py spec, pre_list.sh (+ NOWCHAR reruns for L"" / wchar_t
  failures), grade_lane.sh x4 in the background, then bind.py on hb/ (HEAD) and nb/ (new) symlink dirs, nf.py proof, a `nf.py syms`
  diff per row (DEF renames!), link_g.sh, gen_ren.py, assemble.py (+ work/tc_extra.tsv for table-only lines)
  CAVEAT: nf.py xr / bind.py pick the FIRST symbol containing a bare Leaf (`Open` matched ?SetOpened@...): confirm a `-` with objdump
NF310 (2026-11-23, this hand-back) additions / changes:
  mkoff.py                     work/offlimits.txt, work/other_agents_entries.txt AND work/other_agents_witnesses.txt (every (addr) in the
                               NF305 / NF306 spellings column); RESERVED rows only while still reserved
  cmpcomdat.py <sym> <obj1> <obj2..>   the COMDAT defining <sym> in each object v the first: bytes + relocation list (pick-any census, ruling NF304 Q1)
  assemble.py                  reads work/tc_skip.tsv (old symbol, target): skips the generated delete / replace for that pair (a hand
                               `rewitness` line in tc_extra.tsv replaces it when another binder still binds the old spelling)
  lock.sh needs `export S` (not only `source env.sh`)
NF310 (2026-11-23, T1 signature continuation of NF307) additions / changes:
  cstat.py <idx..>      per ENTRIES line: the self + every census row flagged OFF / OE (NF308 / NF309 entry) / OW (their witness) / LOCK=<owner> / MINE
  aflags.py <addr..>    the same flags for arbitrary rows (controls, riders, binders outside ENTRIES)
  ppdiff.sh <addr..>    gcc -E -P HEAD v rows/<addr>.cpp, whitespace-normalised diff (NF300 Q1 evidence: only the enum / types / literals change)
  mkoff.py              retargeted to NF308 / NF309 (entries + witnesses)
  nf.py                 link / grade / zs scratch now removed through the toolkit's clean.py (no os.unlink / shutil.rmtree of a variable path)
  check before a SELF rename: `nf.py syms` every compiled census object for a DEF of the new name (pick-any COMDAT copies from a shared
  header inline make a strong self a new duplicate: entries 5 / 6 here)

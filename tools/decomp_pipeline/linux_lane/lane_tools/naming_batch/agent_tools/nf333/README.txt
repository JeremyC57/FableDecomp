NF323 / NF325 helpers, retargeted to w22/agentNF333 by NF327 (2026-11-24). Pipeline per entry group:
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
NF323 additions:
  decl.py <idx..>   (DECL_HITS_ONLY=1 DECL_MAX=n) per census row: non-comment declaration lines of the method / ctor, header-only rows counted
  declsum.py <decl output>   compact: only the declaration lines with a pointer parameter (pointer -> reference triage)
  calls.py <addr> <Name>     call / temporary / mem-initializer sites of Name( in a landed row (not declarations)
  ins.py <spec.json>         line-level insert / replace (for one-line #define opt-in users and tests: rows/<addr>_test.cpp via key <addr>_test)
  NF_TREE=<full include tree> for nf.py pre / pre_list.sh; FABLE_HDR_OVERLAY=<same tree> for grades, zs and recheck.sh <tag> <tree>
  nf.py blk(): now bounded to the row's own [pscustomobject]@{ ... } block (NF294's catalog-lookup bug class)
NF323 (2026-11-23) additions:
  show.py <idx..>   per entry: PDB spelling demangled, every WRONG spelling with its table witness, and for the self + each census row the
                    non-comment lines naming the method / ctor (declarations and calls, continuation lines joined): the one-screen triage
  work/offlimits.txt is built from the brief's held rows + the still-reserved Codex p28 rows + every T3_PORT_QUEUE address (decl.py / show.py tag OFF)
NF323 (2026-11-23, this pass) additions / changes:
  show.py           now handles operator names (??R, ??2, ...), member templates (??$); SHOW_WIT=1 prints only the self + the rows that witness a WRONG spelling
  pickany.py        (copied from agent_tools/nf298) <obj> <symbol> <owner addr>: COMDAT body v Fable.exe at the owner (reloc fields zeroed) + each reloc's retail target v the table
  cpp_err.sh        honours NF_TREE (full include tree) before hdr/
  work/offlimits.txt + work/other_agents_entries.txt: built from the brief's held rows, Codex p28 RESERVED (still reserved), T3_PORT_QUEUE addresses, NF299 / NF301 ENTRIES
NF323 (2026-11-23, T1 signature continuation) additions:
  census_addr.py <addr..>      exe census (calls / jumps / absolute dwords) for ANY retail address (a respelled callee outside ENTRIES)
  bindaddr.py <objdir> <target> <rows..>   what each row's object binds at a target address (nf.py xr)
  workflow per group: work/g<N>_changed.txt + g<N>_controls.txt (selves + the FULL exe census of every entry touched, minus other
  agents' entries / locks), pre_tests.sh BEFORE the locks, lock.sh, medit.py spec, pre_list.sh (+ NOWCHAR reruns for L"" / wchar_t
  failures), grade_lane.sh x4 in the background, then bind.py on hb/ (HEAD) and nb/ (new) symlink dirs, nf.py proof, a `nf.py syms`
  diff per row (DEF renames!), link_g.sh, gen_ren.py, assemble.py (+ work/tc_extra.tsv for table-only lines)
  CAVEAT: nf.py xr / bind.py pick the FIRST symbol containing a bare Leaf (`Open` matched ?SetOpened@...): confirm a `-` with objdump
NF323 (2026-11-23, this hand-back) additions / changes:
  mkoff.py                     work/offlimits.txt, work/other_agents_entries.txt AND work/other_agents_witnesses.txt (every (addr) in the
                               NF305 / NF306 spellings column); RESERVED rows only while still reserved
  cmpcomdat.py <sym> <obj1> <obj2..>   the COMDAT defining <sym> in each object v the first: bytes + relocation list (pick-any census, ruling NF304 Q1)
  assemble.py                  reads work/tc_skip.tsv (old symbol, target): skips the generated delete / replace for that pair (a hand
                               `rewitness` line in tc_extra.tsv replaces it when another binder still binds the old spelling)
  lock.sh needs `export S` (not only `source env.sh`)
NF323 (2026-11-23, T1 signature continuation of NF307) additions / changes:
  cstat.py <idx..>      per ENTRIES line: the self + every census row flagged OFF / OE (NF308 / NF309 entry) / OW (their witness) / LOCK=<owner> / MINE
  aflags.py <addr..>    the same flags for arbitrary rows (controls, riders, binders outside ENTRIES)
  ppdiff.sh <addr..>    gcc -E -P HEAD v rows/<addr>.cpp, whitespace-normalised diff (NF300 Q1 evidence: only the enum / types / literals change)
  mkoff.py              retargeted to NF308 / NF309 (entries + witnesses)
  nf.py                 link / grade / zs scratch now removed through the toolkit's clean.py (no os.unlink / shutil.rmtree of a variable path)
  check before a SELF rename: `nf.py syms` every compiled census object for a DEF of the new name (pick-any COMDAT copies from a shared
  header inline make a strong self a new duplicate: entries 5 / 6 here)
NF323 (2026-11-23, this hand-back: T1 signature, NF310's continuation) additions / changes:
  mkoff.py        retargeted to NF309 / NF311; witnesses = every 8-hex inside the spellings column's parentheses (incl. `(a,self)` /
                  `(a,b)`), PLUS every reloc_bindings.tsv witness of an NF309 / NF311 target (the column is truncated) and their rows.txt
  whose.py <addr..>      which NF309 / NF311 ENTRIES line names an address (target / witness), their rows.txt, and the lock owner
  spells.py <idx..>      full (untruncated) spellings of an entry: PDB, self Leaf, every table line with the first differing position
  portcensus.py <idx..>  NF299 Q4 census form: per wrong table line, witness flags, local STL model (namespace std / global template
                         vector / pair), stand-in classes (in neither PDB name set), shared headers declaring the method -> evidence/port_census.tsv
  rdis.py <addr> [n]     capstone listing of a landed row's catalog bytes (screen only; never write its output into a hand-back file)
  check before a binder respell: does the binder define a same-TU stand-in BODY under the callee's name (0x007786F0)? then the respell
  makes a second strong definition of the PDB name: T3
NF323 (2026-11-23, NF312's held entries) changes:
  mkoff.py / whose.py   retargeted to NF314 / NF315; mkoff also takes every 8-hex address in T3_PORT_QUEUE column 3 (unit targets,
                        NF309 Q5) and the neighbours' rows/*.cpp (before they write rows.txt)
  recheck.sh            cds to my work dir, never the repo (a verify_and_land import can create a `D:\...` directory relative to cwd)
NF323 (2026-11-23, header-route rejects of alias1 / flatten-switch1) additions:
  triage.py [idx..]        how engine/<X>.h declares the ENTRIES member: #if stack (guard dropped), DECL / INLINE -> work/triage.tsv
  prec.py <idx..>          landed precedent rows defining a member of X under the same class-model switch (NPREC=n), with their preamble
  flat.py <idx..>          flatten-switch rows: HEAD free `R __fastcall X_M(P*)` -> `R' X::M() [const]`, every other HEAD line kept
  hoist.py <src> <dst>     move every `#define FABLE_RETAIL_*` that follows an #include up above the first #include
  port.py <idx> [prec]     alias rows: precedent define / include block (never a _PASS<nn> precedent) + the draft's own lines
  retfix.py <idx..>        the X::M definition takes the header's return type; an enum return keeps its literal as (E)N
  chunkfix.py <idx..>      chunk classes: FABLE_RETAIL_CCHUNKEDFILECHUNK, local CCharString model dropped, header return type
  tidy.py <addr..>         drop emptied alias classes / stale alias comments, one provenance line after the Leaf
  libcp.py <tag> <addr..>  objsec + the 2026-11-23 libcp ruling check (HD= / ND= vland work dirs, else work/obj): one DEF rename,
                           UND / DEF sets, no ??_7/??_R, sections equal, .drectve equal or HEAD + exactly /DEFAULTLIB:"libcp"
  defgrep.py               other landed sources defining X::M( or Leafing the PDB name -> work/defgrep.tsv
  inclass.py               local models of X elsewhere with an in-class M(...) { body (possible COMDAT emitters)
  st.py <addr> <status> <reason>   one STATUS.tsv line;  rec.py <tag>   STATUS lines from a batch's vland + libcp evidence
  assemble2.py             RENAMES (section 1 from the libcp evidence), rows.txt, REJECTED, empty TABLE_CHANGES / NAME_FIXES headers
  Pipeline per batch: drafts -> nf.py pre (new + HEAD) -> nf.py pay work/<t>n + repo recheck.py -> work/<t>h_payload.json ->
  recheck.sh <t>h / <t>n (flat tags!) -> libcp.py with HD=work/wd_<t>h ND=work/wd_<t>n -> rec.py <t> -> nf.py link (HEAD v new sets)
NF323 (2026-11-23, alias-sig signature ports, this hand-back) additions:
  mkdraft.py <idx> [--defs D1,D2] [--prec <landed src>] [--decl <hdr line>]   draft from orig/<a>.cpp: alias struct dropped, definition
                     head = the header declaration's types + HEAD's parameter names, full PDB Leaf + provenance line -> rows/<a>.cpp
  addpre.py <addr> [-d DEF] [-i include] [-r old new]   add prerequisite switches / includes before the engine include; literal edits
  mkpay.py <tag> <addr..>    HEAD payload via the repo's recheck.py -> work/<tag>h_*; the new payload = same entries with rows/ + Leaf
  runpair.sh <tag> [tree]    vland --recheck-landed HEAD then new under ONE LANDVERIFY_WORK path (wd_cur renamed to wd_<tag>h / n);
                             [tree] = FABLE_HDR_OVERLAY for the NEW side only
  recdone.py <tag>           STATUS done lines from libcp evidence + both recheck logs (asserts new DEF == pdb_spelling)
  libcpN.py <tag> <addr> old=new ...   libcp.py with an explicit rename map (NF304 Q1 pick-any COMDAT renames)
  abscensus.py               census of undeclared members: flat GENERATED / no header / class model + base declares M -> work/abscensus.tsv
  vslot.py <vtable> <slot..> retail vtable slot targets + RTTI labels;  vtcmp.py <obj> <??_7 sym> <vtable>  emitted vftable v retail, slot by slot
  assemble3.py               RENAMES / APPLIED / rows.txt / REJECTED from STATUS + libcp evidence
NF323 (2026-11-24, Clone family / add-only declarations) additions:
  clonetri.py <Class..>          retail vftable (rtti-vtables.tsv) slot count + override slots (labels outside the RTTI base chain)
  ovlist.py <Class..>            the override slots with full labels, targets and Ego_r / FableWin presence -> evidence/ovlist_*.txt
  mkmodel.py <Class..>           CCreatureActionBase-derived class models from ovlist (flat header -> #ifdef _VIRTUAL_MODEL / #else old; none -> new file)
  mkclone.py <addr> <Class> <defines|-> [hdr]   Clone draft on the shared headers (orig/ saved)
  cloneproof.py / cloneproof_all.sh <tag> <addr:Class:vtva..>   NF320 Q1 objsec (Clone COMDAT, DEF allow-list, ??_7 slot-for-slot, ??_G body v retail, .drectve)
  runpair2.sh <tag> <new tree> <old tree>    runpair with the HEAD side on a frozen old tree
  runone.sh <tag> <payload dir>  one vland run (controls)
  link_clone.sh / linkdiff.py    /FORCE link HEAD v new + controls; summary (dups, resolved-by-new, newly-unresolved)
  ovowners.py <linkdiff>         landed owner rows of the newly referenced vftable slot targets (NAME_FIXES input)
  hdredit.py / probe.py / equiv3.py   NF319's, retargeted (probe strips every new opt-in macro name)
NF327 (2026-11-24) additions / changes:
  wrapnew.py <model.h> <generated.h> <out.h>   NEW class header in the NF323 Q1 opt-in form: gen_class_headers.py flat output = default branch,
                     the model under #ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL / #else / #endif; then pin_asserts.py with the generator tree as "old"
  xdis.py <va> [n]   capstone listing of retail Fable.exe at any VA with RTTI-port labels (screen only; never into a hand-back file)
  vprobe3.py         vprobe2.py + PROBE_TREE (run the slot probe on a control tree, e.g. a pure-slot model with `= 0` removed)
  plant327.py        planted-control tree work/eq_ctl from work/eq_new (anchors: add-only #ifdef line, or first line of the _VIRTUAL_MODEL #else)
  link327.sh <tag> "<head dirs>" "<new dirs>" "<control dirs>"   /FORCE link HEAD v new + controls (nf.py link) -> evidence/link_<tag>.txt
  hdrcheck327.py     hdrcheck.py, a NEW header checked against the generator output (work/gen0) instead of orig/hdr_<X>.h
  namecensus.py      skips special members (??0 / ??1 ... selves) instead of crashing
  mkoff.py / whose.py  retargeted to NF324 / NF326
  ref_scan.py (repo) is run from work/rsroot (symlinks tools -> repo/tools, rebuild -> repo/rebuild) so nothing it or vland creates lands in the repo cwd
NF332 (2026-11-24) additions:
  mkoff332.py (off-limits lists), tri332.py <idx..> (entry triage: PDB / Leaf / table lines / census; NOPDB=1 skips the name-set lookup),
  selfv.py <addr..>, files332.py, ed332.py (medit with line deltas), bindchk.py <idx..> (which objects DEF / UND each spelling), ctx.py,
  proof332.py <tag> <addrs> [addr:old=new] (objsec with renames paired by ?Name@Scope@@ prefix, explicit maps for renamed names),
  tedit332.py <spec.json> (zero-line-delta TEST edits -> rows/<addr>_test.cpp, HEAD copy in orig/<addr>_test.cpp),
  assemble332.py (STATUS / APPLIED / RENAMES / TABLE_CHANGES / REJECTED / rows.txt from work/st.tsv, ren1, ren2, tc.tsv)
NF333 (2026-11-24) additions:
  mkoff333.py        mkoff332 with agent NF334's ENTRIES / table witnesses / rows in place of NF330 (work/offlimits*.txt, other_agents_*.txt)
  g1_deletedata.py   CCountedPointer Reset binders: free Destroy_<addr> stand-in -> NF282 hybrid (DELETEDATA_DECL_ONLY + engine/CCountedPointer.h)
  sacensus.py <idx..>  standin_alias census (NF299 Q4): per wrong table line, stand-in scope, witness self, PORT(self on stand-in) / helper-class / free
  tcgen.py <idx..> [skip witnesses]   TABLE_CHANGES lines for the wrong lines of an entry: replace (no PDB line yet) or delete (PDB line exists)
  lblproof.py <addr> [old=new..]      objsec with compiler-local $L labels compared by (section, value) (jump-table label renumbering)
  zs_tests.sh <files.tsv> <list> <done>   D1 cl /Zs of landed tests whose clang precheck fails only in VC7.1 headers

# hand4f -- egod lane author brief (rows reserved in rebuild/candidates/manual/pending/2026-11-18_hand1/RESERVED.tsv; the 2026-11-17 Ego_d lane's docs are in pending/2026-11-17_egod_parked/; this file OVERRIDES older briefs where they differ)

## THIS SESSION (2026-11-19, hand4f) -- read this first; it OVERRIDES everything below where they differ
Owner: "continue working on parked rows. do not repark, keep working until they match. if a function resists matching for more than
100 tries, then send it to main chat to be tried on the fable 5.2 advisor."
- NO PARKING. Every row ends MATCH / RELOCATION_MATCH + behaviour test PASS in the strict read-only vland check. No other end state.
- 100-try rule: one line per grade in ATTEMPTS.log; past 100 grades of yours on a row, SendMessage to "main" starting `ESCALATE <addr>:`
  (residue retail v ours at exact addresses, best source path, LEVELS tried, best guess at the original shape) + /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/tmp/ESCALATE_<addr>.md,
  then continue other rows. Before 100, change the LEVEL of the idea when a residue stops moving.
- LESSON of the last round (hand3a, 9 rows closed; docs/pipeline/GOTCHAS.md "Same-TU callee visibility"): three long-parked rows closed only
  once the same-TU callee's REAL body was defined earlier in the row source (mod/ref + inlining). Check egod_tu.txt / the Ego_r / FableWin
  object tables for same-TU callees of every row and try that level early. Read /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand3a/LESSONS.md and NOTES.md (closed rows, idioms).
- Include order is an accepted lever (EXTRA_RULES 2026-11-19 owner): for a register / reload tie with no source-shape cause, try header
  order variants; land with an `// Include-order:` note.
- Before calling a source "right", compare /Od sizes of your draft and its helpers against FableWin / Ego_d debug lengths
  (ghidra_out/egod_carved_functions.tsv column 3; ghidra_out/fablewin_obj_functions.tsv): a gap names a missing source element.
- Donors (Ego_d / Ego_r / FableWin) are VS2010 builds of a DIFFERENT source revision: structure, names, TU membership and call targets carry
  over; literal types and code do not (FableWin's double literals are NOT retail's). Verify every donor-derived change against retail bytes.
- LESSONS of today's lane (all in docs/pipeline/GOTCHAS.md; read its 2026-11-19 lines): (a) most closes came from PORTING the parked local-model
  draft onto shared headers in the FableWin /Od statement order with Ego_d / FableWin local names -- several matched on the first grade; do
  that before anything else. (b) Same-TU callee real bodies defined first. (c) VC6 for-scope idiom: if FableWin opens a loop with
  `xor reg,reg ; je`, spell it `if (false) {} else for (...)` (shifts c2 record ids, no bytes). (d) A residue that is c2's 81-tuple
  scheduling-region cut can be moved by no-code PAREN tuples; put such brackets in SHARED helper text as a new nested opt-in (row-side
  redundant wraps like `((f()))` are rejected), regrade every landed user of that helper, and mark it `// Inferred:` with the measurement.
- OWNER RULES added today (EXTRA_RULES, read its 2026-11-19 sections): (1) grouping brackets are accepted -- mark `// Bracket:`, prefer natural
  grouping, helper brackets as nested opt-ins with every landed user regraded; (2) Inferred scope / lifetime constructs fixed by frame size or
  slot sharing land without a review flag; fitted tie-breakers stay `// Inferred:` with evidence; (3) forced inlines of sourced helpers are
  `// Inline-budget:`. Grade each same-TU helper body you define at ITS OWN retail address too (q.py <helper addr> <row src>): hand4a found two
  that matched the row but were not retail's own text.
- Retail defs (enum headers, compiled defs): /tmp/claude-0/retail_defs/Fable The Lost Chapters/data/ (read-only; never copy into the repo).
- LESSONS from hand4b (10/10 closed) and hand4c: (e) making a search / container helper's body VISIBLE in the row TU (the existing
  CThingSearchTools *_EGOD opt-ins: ROUGHAREA_EGOD, ALLTHINGSBODY_EGOD, TCSINROUGHAREA_EGOD, FOREACHINROUGHAREA_EGOD) lets VC7.1 keep
  begin/end in registers -- check those opt-ins first on any CThingSearchTools row; (f) a donor-named inline wrapper (e.g.
  GetStaticCameraFocalPos() returning GetCameraFocalPos(1.0f)) moved a 42-instruction residue: call the helper the donor calls, not its
  expansion; (g) x87 compares: `!(a < b)` gives `test ah,41h ; je`, `a >= b` gives `jp` -- read the flag test retail uses;
  (h) declaration order of locals moves slots (follow Ego_d's record order). (i) A row's `// Flags:` line must equal the payload's
  "flags" (the catalog's CompilerFlags is what the real build reads); omit both when none.
- LESSONS from hand4c / hand4d (today): (j) the GameCODA 2.10 SDK (Sensaura audio, CODAApi.lib / CODAApiDbg.lib with CodeView, CAGE + Toolkit
  source) is at /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/sdk/gamecoda/tree/CODA (read-only; never copy into the repo). hand4c's tools: w24/hand4c/tools/arx.py
  (extract members) and its objmatch script. If a row turns out to be byte-identical to an SDK or other library object, REPORT it (do not author it):
  the toolchain_library question for such rows is open with the owner. (k) A catalog label on a template instance is often a byte-twin
  propagation; IDENTIFY from Ego_r / FableWin object tables + callees before writing (hand4d b4, b5). (l) Rows whose `// Flags:` is none: omit
  "flags" from the payload entirely (never ["none"]).
- LESSONS from hand4e (10/10 in 20 grades): IDENTIFY first; FableWin order + Ego_d locals + retail literal types closed most rows on the
  first grade; check calling conventions against the PDB decoration even when a no-argument body cannot show them (0x006379D0).
- Payload: "module" = the proven namespace / class (e.g. `std`, `CWorld`), "name" = the proven short name (no `std::` prefix, no catalog
  label); omit "flags" when none.
- Made-up names (_global::..., descriptive): identify per the IDENTIFY section (two sources) and hand back NAME_FIXES.tsv / LABELS.tsv.
- Data on disk: Ego_d.pdb + types.txt in /tmp/claude-0/egod_pdb/; ghidra_out/egor_obj_functions.tsv, fablewin_obj_functions.tsv,
  egor_publics_noproc.tsv, fablewin_publics_noproc.tsv. FableWin.exe / Ego_r.exe / Ego_d.exe are in /tmp/claude-0/egod_pdb/ (donor code to
  read, never to copy bytes from).
- Wine needs /run/user/0 (`mkdir -p /run/user/0; chmod 700 /run/user/0`). Other agents (hand3c, hand4d) share the Wine lane: one compile at
  a time of yours, never kill their processes, never wineserver -k.
- Tools: $S/tools/q.py, xdis.py, xref.py; oracle $S/batch/hand4f_oracle.tsv.

## Hand authoring first (owner, 2026-11-18 -- the most important rule of this round)
Hand authoring is often better than automated tools. For every row and every stuck residue: read the retail disassembly around the
differing site and the LANDED siblings of the same TU (egod_tu.txt marks them), decide what statement the Lionhead programmer
actually wrote, and write that block by hand -- Ego_d's locals (egod_locals.txt: every record of the name; pick the one whose
parameters fit the row) in their scopes and declaration order, the source's natural expression shapes. Variant generators, flag
sweeps and spelling permutations only answer ONE specific question you have already formed; they are never how the source is
found. Last round's matches came from careful reading (a plain X, Y, Z dot product; a vector built member by member).

## Your rows (parked near misses within 12 bytes of retail's length; best grade 'DIFFER(ours v retail)'), smallest first
1. 0x004C5B90 map<void*::operator[] (161 bytes; extent scan 161; best DIFFER(173v161); residue: )
   ref/004c5b90/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
2. 0x0053848F CList::AddActionOnInactive (78 bytes; extent scan 78; best DIFFER(67v78); residue: )
   ref/0053848f/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
3. 0x005384DD CList::AddActionOnInactive (78 bytes; extent scan 78; best DIFFER(67v78); residue: )
   ref/005384dd/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
4. 0x00554030 std::map<NUISystem::ETableSprites,CCountedPointer<NUISystem::CComponent> >::operator[] (150 bytes; extent scan 150; best DIFFER(161v150); residue: )
   ref/00554030/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
5. 0x005698A0 _global::RBTree_SearchOrCreate (150 bytes; extent scan 150; best DIFFER(161v150); residue: )
   ref/005698a0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
6. 0x005DC340 CAnimationSet::GetAnim (66 bytes; extent scan 66; best DIFFER(56v66); residue: )
   ref/005dc340/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
7. 0x006CA8C0 CEngineTextureMorphSet::AddEngineMorph (138 bytes; extent scan 138; best DIFFER(128v138); residue: )
   ref/006ca8c0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
8. 0x00A0CA90 CWParsedItem::InitAsFloat (187 bytes; extent scan 187; best DIFFER(176v187); residue: DIFFER(176v187) -- x87 allocation: retail keeps only the integer part and fraction on the FPU stack and the divisor in the push-ecx frame word (fld/fmul/fstp per digit), ours keeps all three on the st)
   ref/00a0ca90/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
9. 0x00B6DF90 CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader (54 bytes; extent scan 54; best DIFFER(43v54); residue: )
   ref/00b6df90/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
10. 0x00B6DFD0 CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader (54 bytes; extent scan 54; best DIFFER(43v54); residue: )
   ref/00b6dfd0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
Notes: 0x00B6DF90 / 0x00B6DFD0 are byte twins (one body, two addresses?) -- IDENTIFY both first. 0x00554030 / 0x005698A0 / 0x004C5B90 are std::map
operator[]-shaped instances with the same 161v150 / 173v161 residue: solve one, then the others. 0x00A0CA90 CWParsedItem::InitAsFloat is the wide
twin of the landed 0x009B9770 CParsedItem::InitAsFloat (hand4f b3: FableWin order + Ego_d locals + retail literal types).

## What is new for these rows: Ego_d.pdb (read rebuild/candidates/manual/pending/2026-11-17_egod_parked/README.md first)
Every row here is a PARKED row: earlier agents got close and stopped. Ego_d.pdb (a VC10 /Od debug build of the same TLC source
tree) adds source facts none of them had:
- `ref/<addr>/egod_locals.txt`: the function's own parameter and local NAMES, their TYPES and their SCOPE BLOCKS (`{ ... }` =
  an `if` / loop / plain block where locals were declared). Use them as source evidence: declare the same named locals in the same
  scopes with the same types (`float` vs `double`, reference vs value, `CCharString` copy vs `const CCharString&`), name every local
  as the source did, keep the source's statement structure. Named locals are the known lever for register / frame / x87 residues.
  Frame offsets in it are VC10 debug, NOT retail; unnamed temporaries are not recorded.
- `ref/<addr>/egod_tu.txt`: every function of the same object file (TU) in source order, with retail addresses where the catalog
  knows the name. Same-TU callees retail could see (templates, inline members, file statics) may need their real bodies defined in
  the row (this closed 0x006D0830 after eleven failed passes: a same-TU template COMDAT that was only declared).
- Tools: `python3 -I rebuild/candidates/manual/pending/2026-11-17_egod_parked/lane/egod_locals.py /tmp/claude-0/egod_pdb/Ego_d.pdb '<Class::Name>'`
  (any function, e.g. callees and siblings) and `python3 -I .../lane/egod_type.py /tmp/claude-0/egod_pdb/types.txt <hexindex>...`
  (resolve a type index). Full type records: `grep -n -A12 '^ *0x<INDEX> |' /tmp/claude-0/egod_pdb/types.txt` (150 MB: grep only).
  Class layouts / members / static members / nested typedefs of the TLC source: grep that same types.txt for "LF_CLASS ... `<Name>`".
- STL in Ego_d is VC10 Dinkumware (`_Tree`, `_List_val`, `_Vector_val`): never copy STL internals from it; retail is STLport, spell
  STL from rebuild/include/stlport/. Game classes, members, enums, statics and function names carry over.
- IDENTIFY rows (made-up current names): find the function in `egod_tu.txt` of its likely object by its order next to the named
  neighbours and by its parameters / locals vs the retail body, then CONFIRM the name from a SECOND source (Ego_r / FableWin name
  tables `ghidra_out/egor_pdb_names.tsv` / `fablewin_pdb_names.tsv`, retail strings, FSE, callers, vtable slots). Put the proven
  name (exact decorated spelling) in NAME_FIXES.tsv and LABELS.tsv; if no second source exists say so in NOTES.md and keep the
  name descriptive. Then match the row like any other.

## Environment (every Bash call starts with these two lines; shell state does not persist)
    export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
    source $S/wine/env.sh; export W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f FABLE_HDR_OVERLAY=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/hdr; cd /home/user/FableDecomp
- Write ONLY inside `/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f` (src/, test/, differ/, hdr/, tmp/, probe/). The repo is READ-ONLY for you. NO git commands at all.
- Never kill a process you did not start (another agent shares this Wine lane): kill a hung cl.exe of yours by explicit PID only;
  never pkill / killall / wineserver -k. Wrap long commands in `timeout`. One compile at a time.
- Preflight a new or re-ported source before grading: `g++ -E -D_MSC_VER=1310 -I /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/hdr -I rebuild/include <src> > /dev/null`
  (cl under Wine spins until the timeout on a missing include or an #error instead of printing it).
- Tools ($S/tools): `python3 $S/tools/q.py <addr8> <src.cpp> [leaf] [--flags ...]` grades one source against retail (relocations
  masked; side-by-side disassembly of the differing hunks; the oracle is $S/batch/hand4f_oracle.tsv); `xdis.py <hexva> <hexlen>`
  disassemble retail; `xref.py <hexva>` callers / references; `rstr.py 0x<va>` string; `iat.py 0x<slot>` import; `fun.py 0x<va>` EH info.
  Log every grade as one line in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/ATTEMPTS.log (addr, idea, grade). There is no fg.py / vt.py / addmatch.py / fwsize.py here.
- Behaviour test + relocation check of a matched row (read-only; the lander's real check): add the row to /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/payload.json
  (`{"result": {"authored": [{"address": "<addr8>", "module": "<Module>", "name": "<Name>", "source_cpp": <src text>, "test_cpp": <test text>,
  "pass_pattern": "<addr8>_TEST PASS"}]}}`, plus `"flags": [...]` when the row has a Flags line (the key verify_and_land reads; "compiler_flags" is silently ignored)), copy its line from
  $S/batch/hand4f_oracle.tsv into /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/oracle.tsv (true length in the length column), then
  `LANDVERIFY_WORK=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/lv LANDVERIFY_STRICT_HEADERS=1 timeout 3000 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/payload.json /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/oracle.tsv > /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/tmp/vland.log 2>&1`
  (NEVER with --land / --recheck-landed). A row is matched only with `RELOCATION_MATCH PASS` (or MATCH PASS) there; fix every RELOC_IDENTITY; delete /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f/lv after.
- Tool paths under /opt/fable_lane/... named in older notes do NOT exist on this machine. c2 measuring tools that are in the repo:
  rebuild/candidates/manual/pending/2026-11-09_linux_hand892/tools/ (copy what you need into your directory).
- PDB-derived evidence in the repo: ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv, egod_pdb_names.tsv (name SETS keyed by
  donor addresses, never by retail address), pdb_class_keys.tsv, struct_layouts_egor.tsv, naming_stragglers/FableWin.exe.signatures.tsv,
  rebuild/oracles/reloc_bindings.tsv (retail address -> bound symbol), rebuild/include/engine/<Class>.h. A third, weaker source
  (names of locals / original file; PowerPC build): `llvm-pdbutil dump ... $S/anniv_dbg/WellingtonGame-Xbox360-Test.4k.pdb` (large:
  always pipe through grep; rule in docs/re_reference/FABLE_ANNIVERSARY.md).

## Read first (large files in parts; grep rather than read whole where you can)
0. rebuild/candidates/manual/pending/2026-11-17_egod_parked/lane/LESSONS_EGOD.md (what closed this lane's rows so far; short)
1. /home/user/FableDecomp/CLAUDE.md
2. docs/pipeline/ASM_RULES.md
3. tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md (later sections override earlier ones)
4. tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/BRIEF.md -- the sections "Rapid fire" and "Stay on task" are your working
   style: every tool call ends in a grade, short loop, one row at a time until it matches, no side quests.
5. rebuild/candidates/manual/pending/2026-11-13_acctB_FINAL/lane/CHUNK_BRIEF_lessons.md -- the lessons of ~60 earlier chunks on rows
   exactly like yours (what closed each residue class). Read ALL of it once; it is the most valuable file here.
6. docs/pipeline/MATCHING_PLAYBOOK.md (measured c2 rules; before calling anything a "compiler tie")
7. rebuild/candidates/manual/pending/2026-11-13_acctC/README.md (the rules that keep the hand-back landable) and
   rebuild/candidates/manual/pending/2026-11-13_acctC/lane/WF_RULES.md (owner decisions and tips; its LATER sections override earlier ones)
8. grep docs/pipeline/GOTCHAS.md for your classes and callee addresses; read the nearest LANDED sibling rows of your class
   (`grep -rl "<Class>" rebuild/src/compiled | head`) and copy their idioms.

## Rules (each is re-verified; a row that breaks one is rejected)
- MATCH or RELOCATION_MATCH under VC7.1 RTM for the exact file in src/<addr>.cpp, graded against TODAY's rebuild/include plus your
  hdr/ overlay, AND test/<addr>.cpp printing exactly `<addr>_TEST PASS` in the vland check. The test must exercise the function's real
  logic (a mutant with one decision removed should fail).
- Plain portable C++: no `__asm`, `naked`, `_emit`, the word `register`, bare `__forceinline` (write FABLE_FORCEINLINE), and
  `volatile` only per ASM_RULES. No dead padding, no steering declarations a Lionhead programmer would not have written; every
  statement whose result is unused cites the retail bytes that require it. Inline-budget devices only per EXTRA_RULES 2026-11-08.
- Shared headers only: model every class with `engine/<Class>.h`; never a local padded struct or a second header for a class that
  has one. A header change is an ADDITION under a NEW `#ifdef FABLE_RETAIL_<CLASS>_<WHAT>` opt-in made on a copy of the CURRENT repo
  header placed in your hdr/ (same relative path). Never reuse an existing opt-in name for different content, never edit the body
  of an existing opt-in, never change a default path, never move a FABLE_STATIC_ASSERT line (insert additions so existing asserts
  keep their line numbers where possible, and say so in hdr/README.md when one moved). hdr/README.md: one line per opt-in (header,
  name, what, evidence). After ANY overlay header change regrade every row you already matched.
- Names right the first time: the row, every callee, global, class and member in the exact PDB spelling (class key, namespace,
  const, access, calling convention), two sources each. One retail address, one name: grep rebuild/oracles/reloc_bindings.tsv for
  every callee / global address before declaring it. A made-up name only when the original is found nowhere; then descriptive,
  consistent, listed as descriptive in NOTES.md. Wide text is `unsigned short`, never wchar_t.
- Line 1 of a source: `// <Module>::<Name>(<params>) -- <what it does>`; a `// Leaf:` line with the exact object symbol (check with
  `objdump -t`); exactly one `// Flags:` line if flags are needed; `// Retail:` address and length.
- No retail bytes leave the lane: nothing you write may embed retail byte dumps (disassembly excerpts in notes are fine).

## Working style
Rapid fire (BRIEF.md): port the draft onto today's headers, grade, read the hunks, one hypothesis, edit, grade. Grades are not
rationed; ten grades with the same residue mean change the LEVEL of the idea (callee body visibility, helper texts, the
real structure re-derived from the disassembly, identities, TU flags), not an eleventh spelling. Do not rebuild compiler-hook
tooling. The moment a row matches: write its test, run the read-only vland check, next row.

## Hand-over (all inside /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f)
src/<addr>.cpp + test/<addr>.cpp (matched); differ/<addr>.cpp (best source of an unmatched row, line 2 `// Grade: DIFFER(<ours>v<retail>) -- <residue>`);
hdr/ + hdr/README.md; payload.json + oracle.tsv (matched rows only); STATUS.tsv (`address grade bytes test grades_used notes`);
NOTES.md (per matched row: what made it match, headers / flags; per open row: the exact residue with retail addresses and every
idea tried); LESSONS.md (one line per new lesson); NAME_FIXES.tsv (`address correct_symbol evidence wrong_spellings_seen`);
LABELS.tsv (`address name evidence`). Keep NOTES.md and STATUS.tsv current after every matched row (your context may end at any
time and the files are the only channel). Final message: rows matched / in progress / untouched, grades used, header changes,
names proven vs descriptive, and anything the coordinator must decide.

## Owner decisions that apply (details in acctC lane/WF_RULES.md)
- Dead release asserts (shared fable_assert.h `FABLE_RETAIL_ASSERT` only) are accepted at any count IF the row source carries an
  `// Inferred:` note: count, the residue they close (addresses), evidence. Asserts inside an inline helper that retail plausibly
  asserted in are accepted too (owner 2026-11-16), still only with byte match + test + note.
- Hand-authoring often beats automatic tools: read the retail disassembly and the matched siblings and write the block the way a
  programmer would; sweeps (k locals, assert counts, flag ranges) are for measuring one question, not for finding the source.
- Unused locals (device c, k <= 7) need a `// Fidelity:` line; inline controls (/d2inlT, scoped inline_depth / auto_inline) only for a
  pure inline-decision residue with an `// Inline-budget:` line and the smallest N. No `__assume`, no home-made macros, no invented
  helper functions, no local class models where a shared header exists.

## Owner's requirements for this lane (2026-11-18; they override every older budget or parking rule)
"I want every function it works on to reach a match. No exceptions." Every row in your list must end MATCH or RELOCATION_MATCH, with a
passing behaviour test, in plain portable C++ that behaves exactly like retail. There is NO grade or time budget and NO parking: a row
is finished only when the read-only vland check (strict headers) says RELOCATION_MATCH PASS or MATCH PASS.
- The three goals hold together: retail's exact bytes, retail's exact behaviour, and portability (plain C++; no __asm / naked /
  _emit / register; volatile only per ASM_RULES; no tricks a Lionhead programmer would not have written; header changes only as new
  opt-ins with the default path unchanged). A match that breaks behaviour or portability is not a match.
- Hand authoring first, always: read the retail disassembly around each differing site and the landed same-TU siblings, decide
  what the programmer wrote, write it by hand. Sweeps only answer a question you have already formed.
- Work order: closest row first. When a row stops moving, change the LEVEL of the idea (source structure -- duplicated arms,
  hoisted stores, scopes, inline helper bodies, same-TU callee bodies, the real class / header model, identities), not the spelling.
- Fable advisor: when you have tried several levels on a row and believe you need another view, send the row to the main chat for
  the Fable advisor: use SendMessage with to "main" and a message that starts `ESCALATE <addr>:` and gives the residue (retail v
  ours at the exact addresses), your best source path, what you tried (levels, not spellings) and your best guess at the original
  shape. Also write the same into tmp/ESCALATE_<addr>.md. Then keep working on your OTHER rows; the coordinator runs the review and
  sends its candidates back to you as a message. Grade them, then continue the row until it matches. If you have no SendMessage
  tool, write the file, keep working on the other rows, and when only escalated rows are left end your turn with the list of
  ESCALATE files: you will be resumed with the review.
- Keep NOTES.md / STATUS.tsv / ATTEMPTS.log current after every grade batch (your context may end at any time).
- Your final message comes only when every row is matched (or when you stop to wait for escalations); list per row the grade,
  what closed it, header opt-ins, flags, and names proven.

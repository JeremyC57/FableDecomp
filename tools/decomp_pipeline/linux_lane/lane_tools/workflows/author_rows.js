export const meta = {
  name: 'author-rows',
  description: 'Byte-match ten parked rows: scout identities, author each row until it matches (fresh agent with notes if needed), verify with two independent lenses, repair once',
  phases: [
    { title: 'Scout', detail: 'prove each row\'s name, signature, callees and visible callee bodies', model: 'opus' },
    { title: 'Author', detail: 'rapid-fire port + grade until the row matches (up to 3 agents per row, notes carried over)', model: 'opus' },
    { title: 'Verify', detail: 'mechanical lens + evidence lens on every matched row', model: 'opus' },
    { title: 'Repair', detail: 'fix verifier findings while keeping the match, then re-verify', model: 'opus' },
  ],
}

const S = args.scratch   // the lane scratch directory ($S)
const WF = args.wf   // the run's directory name under $S/w22 (e.g. 'wf2')
const ROWS = args.rows
// Owner rule (2026-11-12, latest): every stage runs on Opus. Pass args.authorModel = 'fable' ONLY for rows that Opus authors could
// not match after their three attempts (the 'absolutely necessary' case); the coordinator decides that per row, never by default.
const AUTHOR_MODEL = args.authorModel === 'fable' ? 'fable' : 'opus'
const MAX_ATTEMPTS = 3
const ENV = `Start EVERY Bash call with exactly:\n  export S=${S}; source $S/wine/env.sh >/dev/null 2>&1; export S; export W=$S/w22/${WF}/ADDR; cd /home/user/FableDecomp\n(with ADDR replaced by your row's 8-hex address). Every Bash call must exit 0 (end pipelines with \`; true\`) and must not print a Python traceback. The repo /home/user/FableDecomp is READ-ONLY for you: never run git, never write under it, never land anything. Write only under $W.`

const SCOUT_SCHEMA = { type: 'object', properties: {
  address: { type: 'string' }, module: { type: 'string' }, name: { type: 'string' }, mangled_leaf: { type: 'string' },
  name_proven_by_two_sources: { type: 'boolean' }, summary: { type: 'string' } },
  required: ['address', 'module', 'name', 'mangled_leaf', 'name_proven_by_two_sources', 'summary'] }
const AUTHOR_SCHEMA = { type: 'object', properties: {
  address: { type: 'string' }, status: { type: 'string', enum: ['MATCH', 'DIFFER'] }, grade: { type: 'string' },
  grades_used: { type: 'integer' }, module: { type: 'string' }, name: { type: 'string' },
  flags: { type: 'array', items: { type: 'string' } }, headers_changed: { type: 'array', items: { type: 'string' } },
  test_vc71: { type: 'string' }, strict_regrade: { type: 'string' },
  residue: { type: 'string' }, ideas_tried: { type: 'string' }, next_idea: { type: 'string' }, what_made_it_match: { type: 'string' } },
  required: ['address', 'status', 'grade', 'grades_used', 'residue', 'ideas_tried', 'next_idea'] }
const VERDICT_SCHEMA = { type: 'object', properties: {
  ok: { type: 'boolean' }, issues: { type: 'array', items: { type: 'string' } }, details: { type: 'string' } },
  required: ['ok', 'issues', 'details'] }

function scoutPrompt(r) {
  return `You are the IDENTITY SCOUT for one row of the FableDecomp byte-matching lane (CLAUDE.md describes the project). Row: retail function 0x${r.addr.toUpperCase()}, ${r.length} bytes, manifest label \`${r.module}::${r.name}\` (a hint only -- labels are often propagated or made up). Its old parked draft is $W/draft_original.cpp and its residue history is in ${r.pending}/ (NOTES.md, STATUS.tsv, README.md).
${ENV.replace(/ADDR/g, r.addr)}
You do NOT author or compile. In about 25 tool calls, find the facts an author needs and write them to $W/DOSSIER.md (plain, compact, each fact with its evidence). The last eight chunks of this lane matched rows almost only by getting these facts right, so be precise and cite sources:
1. Retail shape: disassemble with \`python3 $S/w17/xdis.py ${r.addr} 0x${(r.length + 16).toString(16)}\` -- calling convention, \`ret N\`, frame pointer or not (\`push ebp ; mov ebp,esp\` means the TU was built with \`#pragma optimize("y",off)\` / /Oy-), SEH prologue (/EHsc), size-optimised idioms (\`optimize("s")\`).
2. The real NAME, class and full mangled leaf, from two sources: ghidra_out/egor_pdb_names.tsv, ghidra_out/egor_signatures.tsv, ghidra_out/_fablewin_api.tsv, ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv, the vtable slot (rebuild/manifest/rtti-vtables.tsv), and the callers (\`python3 $S/w23/xref.py ${r.addr}\`; read the landed callers' sources). Say plainly if the manifest label is wrong. Signature: parameter types, const, return type.
3. Every CALLEE (each call / jmp target and each vtable slot called): its landed row file (rebuild/src/compiled/<aa>/<bb>/*_<addr>.cpp), its landed name, and its PDB name -- flag every one where the old draft's spelling disagrees with the PDB.
4. Every MEMBER offset the function reads or writes: which shared header (rebuild/include/engine/<Class>.h) names it, the member's real name and type (ghidra_out/struct_layouts_egor.tsv), and which \`FABLE_RETAIL_*\` opt-in macros that header already offers for it. List the classes the old draft models LOCALLY that have a shared header.
5. Callee-body VISIBILITY (the lesson of chunk Q32): which callees were probably visible to retail's compiler -- inline members that retail emitted out of line (unaligned address among COMDATs), landed functions at neighbouring retail addresses in the same class / source file (same TU, defined earlier), STLport helpers (map::operator[], tree ctor/dtor) called out of line. Name the opt-in macros that exist for them (grep rebuild/include/stlport and the engine headers).
6. SIBLING landed rows: 3-5 landed rows of the same class or the same retail address neighbourhood, with the pragmas, flags (\`// Flags:\`) and opt-in macros they use -- the author will copy their idioms.
7. The old draft's likely model errors, judged against the disassembly (wrong control flow, wrong loop form, wrong types, missing inline accessors).
Return the structured result; \`summary\` = the five most important facts for the author in a few sentences.`
}

function authorPrompt(r, attempt, history) {
  const prev = history.length
    ? `This is attempt ${attempt} on this row: ${history.length} earlier agent(s) ran out of budget on it. Their last reports:\n` +
      history.map((h, i) => `  attempt ${i + 1}: ${h.grade}; residue: ${h.residue}; tried: ${h.ideas_tried}; their best next idea: ${h.next_idea}`).join('\n') +
      `\nRead $W/NOTES.md and $W/ATTEMPTS.log first and do NOT repeat those ideas. Start from $W/differ/${r.addr}.cpp (their best source) and change the LEVEL of the idea: callee-body visibility, the real structure re-derived from the disassembly, identities, the TU's flags.`
    : `This is the first authoring attempt on this row. Start from $W/differ/${r.addr}.cpp (the old parked draft).`
  return `You are a rapid-fire AUTHOR agent for one row of the FableDecomp byte-matching lane (CLAUDE.md describes the project). Row: retail function 0x${r.addr.toUpperCase()}, ${r.length} bytes, manifest label \`${r.module}::${r.name}\` (a hint). Row directory: $W = ${S}/w22/${WF}/${r.addr}.
${ENV.replace(/ADDR/g, r.addr)}
Read ${S}/w22/WF_BRIEF.md once, quickly, and follow it exactly (above all "Rapid fire", "The lesson of chunk Q32" and "Stay on task"), then read $W/DOSSIER.md (the scout's proven facts for this row).
${prev}
The user's instructions for this lane, in their words: go FAST; no grade limit; rapid fire quickly until matches are found; keep working on the row until it matches -- don't switch, don't give up early. So: every tool call makes an edit AND grades it with \`W=$W python3 $S/tools/fg.py ${r.addr} '<mangled leaf>' v1.cpp [v2.cpp v3.cpp] --note "<idea>"\` (up to 3 variants per call); short hypotheses, no essays, no helper-building, no surveys; first grade within 5 tool calls; when ten grades leave the same residue, change the level of the idea.
Hard rules that speed never buys: plain C++ only (no __asm / naked / _emit / register / bare __forceinline; volatile only where retail used it); never a local model of a class that has rebuild/include/engine/<Class>.h -- add an \`#ifdef FABLE_RETAIL_*\` opt-in to a copy of that header under $W/hdr/ instead (full copy of the CURRENT repo header plus additions only; one line per opt-in in $W/hdr/README.md); never a stand-in body for another retail function (a callee's REAL landed body may sit in the row file); exact PDB names proven from two sources, made-up names only when found nowhere and then listed in NOTES.md.
When the row grades MATCH / RELOCATION_MATCH: write a small but REAL behaviour test (a standalone program that exercises the function's logic through a copy of the model and prints exactly \`${r.addr}_TEST PASS\` on success / \`${r.addr}_TEST FAIL\` + return 1 on failure), check it with \`python3 $S/tools/vt.py ${r.addr} <test.cpp>\` (must print PASS), regrade once with \`LANDVERIFY_STRICT_HEADERS=1\` set on the fg.py call, then run \`python3 $S/tools/addmatch.py $W ${r.addr} '<module = class>' '<name = METHOD ONLY>' <src.cpp> <test.cpp> [--flags ...]\`. The source must carry a header comment (what the function does, retail address and length, callees by address), an exact \`// Leaf: <mangled>\` line and, if needed, a \`// Flags:\` line.
If your working budget is nearly spent before a match: save the best source as $W/differ/${r.addr}.cpp with line 2 = \`// Grade: DIFFER(ours v retail) -- <exact residue>\`, and append to $W/NOTES.md every idea you tried and your best next idea.
Either way append a short section to $W/NOTES.md (what made it match or what blocks it; headers / flags it needs; any landed name you proved wrong, with evidence) and return the structured result. \`status\` is MATCH only if the final fg.py grade of the final source is MATCH or RELOCATION_MATCH.`
}

function verifyPrompt(r, lens, a) {
  const common = `You are an independent VERIFIER (${lens} lens) for one matched row of the FableDecomp byte-matching lane (CLAUDE.md describes the project and its rules). Row 0x${r.addr.toUpperCase()}, directory $W = ${S}/w22/${WF}/${r.addr}. An author agent reports it MATCHED as \`${a.module || r.module}::${a.name || r.name}\`${a.flags && a.flags.length ? ' with flags ' + a.flags.join(' ') : ''}; its hand-back is $W/payload.json + $W/oracle.tsv (+ $W/hdr/ overlay, $W/NOTES.md, $W/DOSSIER.md).
${ENV.replace(/ADDR/g, r.addr)}
Do not trust the author's report: check the hand-back yourself and try to REFUTE it. You must NOT edit the row, payload.json, oracle.tsv or hdr/; write scratch files only under $W/verify_${lens}/. Report every problem as one concrete sentence in \`issues\`; \`ok\` is true only if you found none that must be fixed before landing.`
  if (lens === 'mechanical') return common + `
Checks (run them, do not reason about them):
1. Extract source_cpp and test_cpp of the row from $W/payload.json into $W/verify_mechanical/. Regrade the SOURCE with the lander's rules: \`LANDVERIFY_STRICT_HEADERS=1 W=$W python3 $S/tools/fg.py ${r.addr} '<leaf from the // Leaf: line>' $W/verify_mechanical/src.cpp [--flags <payload flags>]\` -- it must print MATCH or RELOCATION_MATCH. Then grade it once more with \`FABLE_HDR_OVERLAY\` pointing at an EMPTY directory to learn which overlay headers the row really needs (a row that only matches with the overlay is fine; list the headers it needs). Then run the LANDING verifier as a dry run (no --land; it writes nothing under the repo): \`FABLE_HDR_OVERLAY=$W/hdr LANDVERIFY_WORK=$W/verify_mechanical/vl LANDVERIFY_STRICT_HEADERS=1 python3 tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/payload.json $W/oracle.tsv > $W/verify_mechanical/vland.txt 2>&1\` and read the row's lines: it must say MATCH or RELOCATION_MATCH with PASS. fg.py normalises jump targets and does NOT read jump tables; vland does. A RELOC_JUMPTABLE / RELOC_IDENTITY / NOT_GENUINE line is a blocking issue (a switch whose cases map to the wrong bodies still 'matches' in fg.py): report the table entries it prints.
2. Test: \`python3 $S/tools/vt.py ${r.addr} $W/verify_mechanical/test.cpp\` must print PASS; the test must print exactly \`${r.addr}_TEST PASS\`.
3. Purity, with // comments stripped: no \`__asm\`, \`_emit\`, \`naked\`, \`register\`, \`__forceinline\`, \`volatile\` in source or test.
4. Shared headers: list every \`class X {\` / \`struct X {\` DEFINED in the source for which rebuild/include/engine/X.h exists -- each one is an issue (a local model of a header class). Classes with no shared header are fine.
5. Stand-in bodies: every function body in the source other than the row's own function must be the REAL body of that function -- if it is a landed function, diff it against its landed row (rebuild/src/compiled/<aa>/<bb>/*_<addr>.cpp) and report differences in the code (not comments); a body described as a stand-in, or empty, or shorter than retail's is an issue.
6. Overlay: each file under $W/hdr/ must equal the current repo header plus ADDED lines only (\`diff rebuild/include/<h> $W/hdr/<h> | grep -c '^<'\` must be 0, unless hdr/README.md explains a rewritten \`#if\` line), and every addition must sit inside an \`#ifdef FABLE_RETAIL_*\` opt-in so the default path is unchanged. Run \`python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/hdrcheck2.py $W/hdr\` and report any \`default=DIFF\`.
7. Formats: payload address = 8 lowercase hex, \`name\` = the method only (no \`Class::\`), pass_pattern = \`${r.addr}_TEST PASS\`; oracle.tsv has its header row and the bytes do not end in 0xCC / 0x90; \`python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/reserved_check.py $W/payload.json\` reports 0 reserved rows.
8. The source has the header comment and an exact \`// Leaf:\` line whose mangled name equals the symbol fg.py graded.`
  return common + `
Checks (read the evidence yourself):
1. NAME and class: prove \`${a.module || r.module}::${a.name || r.name}\` and the \`// Leaf:\` mangled symbol from two independent sources (ghidra_out/egor_pdb_names.tsv / egor_signatures.tsv, ghidra_out/_fablewin_api.tsv / naming_stragglers/FableWin.exe.signatures.tsv, the vtable slot in rebuild/manifest/rtti-vtables.tsv, the callers via \`python3 $S/w23/xref.py ${r.addr}\`). A name with one source, or a signature (const, parameter types, return type, access) that disagrees with the PDB spelling, is an issue.
2. CALLEES: for each callee the source declares or calls, check the spelling against the PDB extracts and against the retail target address in the trailing comment (disassemble with \`python3 $S/w17/xdis.py ${r.addr} 0x${(r.length + 16).toString(16)}\`); a callee spelled with a made-up or address-suffixed name while a PDB name exists is an issue; a made-up name that is not listed as such in $W/NOTES.md is an issue.
3. MEMBERS: spot-check five member accesses against ghidra_out/struct_layouts_egor.tsv / the shared headers (right member at that offset, right type).
4. The TEST is a real behaviour test: it must exercise the function's actual logic (branches, loop, result) through a model copy and be able to FAIL -- a test that prints PASS unconditionally, or checks only constants, is an issue. Say in one line what behaviour it checks.
5. FAITHFULNESS: read the source as a reviewer -- anything that exists only to steer the compiler and that a Lionhead programmer plausibly would not have written is allowed only if the comment says which retail bytes require it; flag constructs that look arbitrary AND are unexplained. Check that each real callee body carried in the file is justified in the comments (which retail fact shows the body was visible) and names the landed row it copies.
6. NOTES: every landed name the author says is wrong must come with its evidence (address, correct symbol, two sources).`
}

function repairPrompt(r, a, issues) {
  return `You are a REPAIR agent for one matched row of the FableDecomp byte-matching lane (CLAUDE.md describes the project). Row 0x${r.addr.toUpperCase()}, directory $W = ${S}/w22/${WF}/${r.addr}; it grades MATCH as \`${a.module || r.module}::${a.name || r.name}\` and is in $W/payload.json, but independent verifiers found these problems that must be fixed before it can land:
${issues.map((x, i) => `  ${i + 1}. ${x}`).join('\n')}
${ENV.replace(/ADDR/g, r.addr)}
Read ${S}/w22/WF_BRIEF.md ("Rapid fire" and the Rules / Output sections) and $W/NOTES.md. Fix every listed problem while KEEPING the byte match: edit the source / test / overlay, regrade after every edit with \`W=$W python3 $S/tools/fg.py ${r.addr} '<leaf>' <src.cpp> --note "repair: <what>"\` (it must stay MATCH / RELOCATION_MATCH, also with \`LANDVERIFY_STRICT_HEADERS=1\`), recheck the test with \`python3 $S/tools/vt.py ${r.addr} <test.cpp>\`, then rewrite the hand-back with \`python3 $S/tools/addmatch.py $W ${r.addr} '<module>' '<method only>' <src.cpp> <test.cpp> [--flags ...]\`. A problem you believe is not real: do not change the row for it, explain why in $W/NOTES.md under "Verifier findings not applied" with evidence. Same hard rules as the author (plain C++, no local model of a header class, no stand-in bodies, exact PDB names, nothing written under the repo, no git). Return the structured result; \`status\` is MATCH only if the final source still grades MATCH / RELOCATION_MATCH.`
}

async function verifyBoth(r, a, round) {
  const vs = await parallel(['mechanical', 'evidence'].map(lens => () =>
    agent(verifyPrompt(r, lens, a), { label: `verify-${lens}:${r.addr}${round > 1 ? '#' + round : ''}`, phase: 'Verify', schema: VERDICT_SCHEMA, model: 'opus' })))
  const issues = []
  vs.forEach((v, i) => { if (!v) issues.push(`the ${i === 0 ? 'mechanical' : 'evidence'} verifier did not finish`); else if (!v.ok) issues.push(...v.issues) })
  return issues
}

const results = await pipeline(
  ROWS,
  async (r) => {
    const s = await agent(scoutPrompt(r), { label: `scout:${r.addr}`, phase: 'Scout', schema: SCOUT_SCHEMA, model: 'opus' })
    return { scout: s }
  },
  async (prev, r) => {
    const history = []
    for (let attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
      const a = await agent(authorPrompt(r, attempt, history), { label: `author:${r.addr}#${attempt}`, phase: 'Author', schema: AUTHOR_SCHEMA, model: AUTHOR_MODEL })
      if (!a) { history.push({ grade: 'agent did not finish', residue: 'unknown (read $W/NOTES.md and ATTEMPTS.log)', ideas_tried: 'see ATTEMPTS.log', next_idea: 'see NOTES.md' }); continue }
      if (a.status === 'MATCH') return { scout: prev && prev.scout, author: a, attempts: attempt, history }
      history.push(a)
      log(`${r.addr}: attempt ${attempt} ended ${a.grade} after ${a.grades_used} grades -- ${attempt < MAX_ATTEMPTS ? 'a fresh agent continues with its notes' : 'attempt cap reached'}`)
    }
    log(`${r.addr}: NOT matched after ${MAX_ATTEMPTS} author agents -- left in $W/differ with notes (no silent drop: it goes back to the coordinator's pool)`)
    return { scout: prev && prev.scout, author: null, attempts: MAX_ATTEMPTS, history }
  },
  async (prev, r) => {
    if (!prev || !prev.author) return { addr: r.addr, status: 'DIFFER', attempts: prev ? prev.attempts : 0, last: prev && prev.history.length ? prev.history[prev.history.length - 1] : null }
    let a = prev.author
    let issues = await verifyBoth(r, a, 1)
    if (issues.length) {
      log(`${r.addr}: verifiers raised ${issues.length} issue(s) -- repairing`)
      const fixed = await agent(repairPrompt(r, a, issues), { label: `repair:${r.addr}`, phase: 'Repair', schema: AUTHOR_SCHEMA, model: AUTHOR_MODEL })
      if (fixed && fixed.status === 'MATCH') {
        a = Object.assign({}, a, fixed)
        issues = await verifyBoth(r, a, 2)
      } else {
        issues = issues.concat([`the repair agent did not keep the match (${fixed ? fixed.grade : 'did not finish'})`])
      }
    }
    return { addr: r.addr, status: issues.length ? 'MATCH_WITH_OPEN_ISSUES' : 'VERIFIED', attempts: prev.attempts, module: a.module, name: a.name, flags: a.flags || [], headers_changed: a.headers_changed || [], what_made_it_match: a.what_made_it_match || '', open_issues: issues }
  }
)
const out = results.map((x, i) => x || { addr: ROWS[i].addr, status: 'PIPELINE_ERROR' })
log(`done: ${out.filter(x => x.status === 'VERIFIED').length} verified, ${out.filter(x => x.status === 'MATCH_WITH_OPEN_ISSUES').length} matched with open issues, ${out.filter(x => x.status === 'DIFFER').length} not matched, ${out.filter(x => x.status === 'PIPELINE_ERROR').length} errors`)
return out
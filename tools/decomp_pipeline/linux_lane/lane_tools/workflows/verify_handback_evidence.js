export const meta = {
  name: 'verify-handback-evidence',
  description: 'Independent evidence check of matched rows in an author hand-back before landing (names, callees, members, tests that can fail, faithfulness)',
  phases: [
    { title: 'Verify', detail: 'one Opus verifier per matched row, prompted to refute the hand-back', model: 'opus' },
  ],
}
const S = args.scratch
const W = args.dir
const VERDICT = { type: 'object', properties: {
  address: { type: 'string' }, ok: { type: 'boolean' },
  name_proven: { type: 'boolean' }, test_can_fail: { type: 'boolean' },
  issues: { type: 'array', items: { type: 'string' } }, details: { type: 'string' } },
  required: ['address', 'ok', 'name_proven', 'test_can_fail', 'issues', 'details'] }
const out = await parallel(args.rows.map(r => () => agent(
`You are an independent VERIFIER (evidence lens) for one matched row of the FableDecomp byte-matching lane (CLAUDE.md describes the project and its rules). Row 0x${r.addr.toUpperCase()}, reported as \`${r.module}::${r.name}\`, ${r.length} bytes. The author's hand-back is in ${W}: payload.json (the row's source_cpp and test_cpp are the entry with this address), NOTES.md, LABELS.tsv, NAME_FIXES.tsv, hdr/ (header overlay) and hdr/README.md.
Start EVERY Bash call with: export S=${S}; source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp
Every Bash call must exit 0 (end pipelines with \`; true\`) and print no traceback. You are READ-ONLY: do not edit the hand-back, never run git, never write under the repo; scratch files only under ${W}/verify_evidence/${r.addr}/. Do not compile -- the coordinator regrades and runs the tests separately.
Do not trust the author's report: read the evidence yourself and try to REFUTE the hand-back. Checks:
1. NAME, class and signature: prove \`${r.module}::${r.name}\` and the source's \`// Leaf:\` mangled symbol from two independent sources (ghidra_out/egor_pdb_names.tsv, ghidra_out/egor_signatures.tsv, ghidra_out/_fablewin_api.tsv, ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv, the vtable slot in rebuild/manifest/rtti-vtables.tsv, the callers via \`python3 $S/w23/xref.py ${r.addr}\`). One source only, or a signature (const, parameter types, return type, access, virtual) that disagrees with the PDB spelling, is an issue.
2. CALLEES: disassemble retail (\`python3 $S/w17/xdis.py ${r.addr} 0x${(r.length + 16).toString(16)}\`) and for each call target check that the source spells that callee with its PDB name (or its landed name when no PDB name exists) and the right retail address in the trailing comment. A made-up or address-suffixed spelling where a PDB name exists is an issue; a made-up name not listed as such in NOTES.md is an issue.
3. MEMBERS: spot-check five member accesses against ghidra_out/struct_layouts_egor.tsv and the shared headers (right member at that retail offset, right type).
4. LOCAL CLASSES: for each class the source defines locally, confirm rebuild/include/engine/<Class>.h does NOT exist (if it exists, that is an issue) and that its members carry PDB names.
5. REAL BODIES: every function body in the source other than the row's own must be the real body of that function. If it is a landed function, diff the code (not comments) against its landed row rebuild/src/compiled/<aa>/<bb>/*_<addr>.cpp; if it is an inline with no out-of-line copy, check it against the PDB name and the bytes it expands to in the row. A stand-in, empty or made-up body is an issue. The comments must say which retail fact shows the body was visible to the compiler.
6. The TEST is a real behaviour test: it must exercise the function's actual logic (branches, loops, results) through a model copy and be able to FAIL if that logic were wrong. A test that prints PASS unconditionally or checks only constants is an issue. State in one line what it checks.
7. FAITHFULNESS: flag constructs that exist only to steer the compiler AND are not explained by a comment naming the retail bytes that require them. Where the author says retail's behaviour looks odd (kept "as is"), confirm from the disassembly that retail really does that.
8. NAME_FIXES.tsv / LABELS.tsv entries that concern this row's callees: each needs the address, the correct symbol and evidence; say whether the evidence holds.
Return the verdict. \`ok\` is true only if nothing must be fixed before landing; put each required fix in \`issues\` as one concrete sentence, and observations that need no fix in \`details\`.`,
  { label: `evidence:${r.addr}`, phase: 'Verify', schema: VERDICT, model: 'opus' })))
return out.map((v, i) => v || { address: args.rows[i].addr, ok: false, name_proven: false, test_can_fail: false, issues: ['the verifier did not finish'], details: '' })
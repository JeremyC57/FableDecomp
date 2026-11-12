export const meta = {
  name: 'name-fix-queue-pass',
  description: 'Short naming pass over the name-fix queue: one Opus agent applies proven fixes in scratch, an independent Opus verifier attacks the hand-back, one repair pass',
  phases: [{ title: 'Apply' }, { title: 'Verify' }, { title: 'Repair' }],
}
const S = args.scratch, N = `${S}/w22/${args.agent}`
const LANE = `Every Bash call starts with: export S=${S}; source $S/wine/env.sh >/dev/null 2>&1; export S; cd /home/user/FableDecomp   and must exit 0 (end pipelines with "; true"; never pipe a grader into head -- redirect to a file). Never run git, never write under /home/user/FableDecomp, never land anything.`
const APPLY_SCHEMA = { type: 'object', properties: {
  applied: { type: 'number' }, rejected: { type: 'number' }, partial: { type: 'number' },
  rows_changed: { type: 'number' }, self_renames: { type: 'number' },
  headers_changed: { type: 'array', items: { type: 'string' } },
  gaps: { type: 'array', items: { type: 'string' } }, rule_slips: { type: 'array', items: { type: 'string' } },
  summary: { type: 'string' } }, required: ['applied', 'rejected', 'rows_changed', 'summary'] }
const VERIFY_SCHEMA = { type: 'object', properties: {
  ok: { type: 'boolean' },
  findings: { type: 'array', items: { type: 'object', properties: {
    address: { type: 'string' }, severity: { type: 'string', enum: ['blocker', 'fix', 'note'] }, problem: { type: 'string' } },
    required: ['address', 'severity', 'problem'] } },
  rows_regraded: { type: 'number' }, rows_failing: { type: 'array', items: { type: 'string' } }, summary: { type: 'string' } },
  required: ['ok', 'findings', 'summary'] }

phase('Apply')
const applied = await agent(`You are name-fix agent ${args.agent} for the FableDecomp byte-matching project (CLAUDE.md describes it). Your full brief is ${N}/BRIEF.md -- read it first and follow it exactly; the queue is ${N}/QUEUE.tsv (44 entries), the addresses you must not touch are in ${N}/BUSY.txt. ${LANE}
Work in ${N}/ (rows/, hdr/ with a README.md, RENAMES.tsv, STATUS.tsv, APPLIED.tsv, REJECTED.tsv, NOTES.md, LESSONS.md as the brief's Hand-back section says). Header overlay: a FULL copy of the CURRENT repo header under ${N}/hdr/<path> with #ifdef additions only; grade with FABLE_HDR_OVERLAY=${N}/hdr. Keep the pass short and mechanical: verify each claim with two sources, respell every landed binder you can find, regrade every changed row (it must stay MATCH / RELOCATION_MATCH and its test must still pass: python3 $S/tools/vt.py <addr> <test.cpp> when a test changes). An entry you cannot prove goes to REJECTED.tsv with the reason; an entry too large for a short pass is 'partial' with the gap written in NOTES.md. Before finishing, regrade ALL rows in rows/ once more in one loop with the final overlay and write the result lines to ${N}/FINAL_GRADES.txt. Return the structured summary.`,
  { label: 'apply name fixes', phase: 'Apply', model: 'opus', schema: APPLY_SCHEMA })
if (!applied) return { status: 'APPLY_FAILED' }

const verifyPrompt = (round) => `You are an independent, adversarial VERIFIER of a naming hand-back in the FableDecomp byte-matching project (CLAUDE.md describes it and its naming rules: exact PDB spelling from two sources, one address one name, made-up names only when found nowhere and declared descriptive). ${LANE}
The hand-back is in ${N}/ (BRIEF.md = what the agent was asked; QUEUE.tsv = the claims; rows/<addr>.cpp = changed landed rows; hdr/ = header overlay; RENAMES.tsv, STATUS.tsv, APPLIED.tsv, REJECTED.tsv, NOTES.md). This is verification round ${round}. Assume the agent made mistakes and try to find them:
1. REGRADE every row in rows/ yourself with the overlay: for each <addr>.cpp take the Leaf from its '// Leaf:' line and flags from '// Flags:' (or the catalog entry in rebuild/build_candidates.ps1) and run  FABLE_HDR_OVERLAY=${N}/hdr timeout 240 python3 $S/tools/q.py <addr> ${N}/rows/<addr>.cpp '<leaf>' [--flags ...]  one at a time, results to a file. Any row that is not MATCH / RELOCATION_MATCH is a blocker. For rows with a changed test (rows/<addr>_test.cpp) run python3 $S/tools/vt.py <addr> <test>.
2. For every 'done' entry in APPLIED.tsv check the new spelling against the PDB name sets yourself (ghidra_out/egor_pdb_names.tsv, ghidra_out/fablewin_pdb_names.tsv, ghidra_out/_fablewin_api.tsv; grep the mangled name) AND against the retail target (objdump -dr of the graded object in $S/wine/q/<addr>/<addr>.obj shows which symbol each relocation binds; rebuild/oracles/reloc_bindings.tsv has symbol -> target). A new spelling that binds one symbol to two different addresses, or two symbols to one address, is a blocker.
3. Header overlay: each file in hdr/ must be the CURRENT repo header (rebuild/include/<path>) plus #ifdef additions only -- diff them; any removed or changed existing line outside a new #ifdef is a blocker (run python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/hdrcheck2.py ${N}/hdr and report any macro that is not 'same' and already has users: grep -rlw MACRO rebuild/src rebuild/tests rebuild/include).
4. No row in rows/ may be in BUSY.txt; no new local model (class/struct X { ...) of a class that has rebuild/include/engine/X.h compared with the landed version of the row; no __asm / naked / _emit / register / bare __forceinline; RENAMES.tsv section 1 has the exact header the brief gives, addresses with 0x prefix, name column = method only.
5. Sample five REJECTED entries: was the rejection reasoned with evidence, or lazy?
Report findings with severity (blocker = must not land as is; fix = small correction; note = for the record). ok = true only with zero blockers and zero fixes.`

phase('Verify')
let verdict = await agent(verifyPrompt(1), { label: 'verify hand-back', phase: 'Verify', model: 'opus', schema: VERIFY_SCHEMA })
let repaired = null
if (verdict && !verdict.ok && verdict.findings.filter(f => f.severity !== 'note').length) {
  phase('Repair')
  repaired = await agent(`You are the REPAIR agent for a naming hand-back in the FableDecomp byte-matching project (CLAUDE.md). ${LANE}
The hand-back is in ${N}/ (read BRIEF.md for the rules and formats). An independent verifier found these problems:
${JSON.stringify(verdict.findings.filter(f => f.severity !== 'note'), null, 1)}
Fix each one: correct the spelling / header / file, or, when a row cannot be kept matching with the correct name, REVERT that row (delete rows/<addr>.cpp and its test, remove its RENAMES / STATUS lines, mark the queue entry 'partial' or 'rejected' in APPLIED.tsv with the reason). Regrade every row you touch (FABLE_HDR_OVERLAY=${N}/hdr timeout 240 python3 $S/tools/q.py <addr> <file> '<leaf>' [--flags ...]) and finally regrade ALL rows in rows/ once, writing the result lines to ${N}/FINAL_GRADES.txt. Return the structured summary of the hand-back as it now stands.`,
    { label: 'repair hand-back', phase: 'Repair', model: 'opus', schema: APPLY_SCHEMA })
  verdict = await agent(verifyPrompt(2), { label: 'verify hand-back (2)', phase: 'Verify', model: 'opus', schema: VERIFY_SCHEMA })
}
return { applied, repaired, verdict }

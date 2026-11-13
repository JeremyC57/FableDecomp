#!/usr/bin/env python3
# Build the acctB variant of lane_tools/workflows/author_rows.js: same pipeline (Opus scout -> Fable authors (up to 3) -> two Opus
# verifiers -> one Opus repair -> re-verify), adapted for rows that may have no draft yet and that sit on acctB's own reserved list.
import os
ROOT = "/home/user/FableDecomp"
SP = "/tmp/claude-0/-home-user-FableDecomp/ec5e96c7-c2a3-5632-9fee-64af8822e954/scratchpad"
s = open(ROOT + "/tools/decomp_pipeline/linux_lane/lane_tools/workflows/author_rows.js").read()

def rep(old, new, count=1):
    global s
    assert s.count(old) >= 1, old[:80]
    s = s.replace(old, new, count)

rep("name: 'author-rows',", "name: 'author-rows-acctB',")
rep("description: 'Byte-match ten parked rows: scout identities, author each row until it matches (fresh agent with notes if needed), verify with two independent lenses, repair once',",
    "description: 'Byte-match acctB reserved rows: Opus scout (identity dossier, plus an outline where no draft exists), Fable author until the row matches (fresh agent with notes if needed, up to 3), two independent Opus verifiers, one Opus repair',")

# --- scout
rep("Its old parked draft is $W/draft_original.cpp and its residue history is in ${r.pending}/ (NOTES.md, STATUS.tsv, README.md).",
    "${r.has_draft ? 'Its best draft so far is $W/draft_original.cpp (= $W/differ/' + r.addr + '.cpp); earlier agents\\' notes, grade log and scout files are in $W/prior/.' : 'There is NO draft of this row yet: a Fable author will write the whole function from the disassembly, your dossier and your outline.'} Read $W/HINTS.md first (the coordinator's pointers: earlier material in $W/prior/, family models, lane lessons); $W/dis_all.txt is the full retail listing.")
rep("You do NOT author or compile. In about 25 tool calls, find the facts",
    "You do NOT author or compile. In about ${r.has_draft ? 25 : 45} tool calls, find the facts")
rep("7. The old draft's likely model errors, judged against the disassembly (wrong control flow, wrong loop form, wrong types, missing inline accessors).",
    "7. ${r.has_draft ? 'The draft\\'s likely model errors, judged against the disassembly (wrong control flow, wrong loop form, wrong types, missing inline accessors).' : 'Because there is no draft: also write $W/OUTLINE.md, the map the author writes the function from -- frame facts (prologue form, frame size, saved-callee order, `ret N`, EH states, /GS cookie), the frame map (each stack slot / range with the local it holds, its type and the constructor / destructor that touches it), then block-by-block pseudo-C++ of the WHOLE function in retail order with the retail address of every block start, branch target and call, every call and global resolved to a name, the inline expansions you recognise (STLport containers, CIntelligentPointer, CCharString / CWideString, vector maths) with the header or landed row where the same expansion already matches, cold blocks placed after the epilogue, and jump tables with their case values.' + (r.has_dossier_material ? ' Earlier Opus scouts already wrote $W/prior/scout/outline.md and headers.md for this row: verify them against the listing, fix what is wrong, and build OUTLINE.md and the dossier on them instead of starting over.' : '')}")

# --- author
rep(": `This is the first authoring attempt on this row. Start from $W/differ/${r.addr}.cpp (the old parked draft).`",
    ": (r.has_draft ? `This is the first authoring attempt of this workflow on this row. Start from $W/differ/${r.addr}.cpp (the best draft of earlier agents; $W/HINTS.md gives its baseline grade and where their notes, grade log and candidates are -- read $W/prior/NOTES.md and do NOT repeat their ideas). The overlay $W/hdr/ was re-based by the coordinator: headers marked STALE in $W/hdr/README.md are old copies that you must re-base onto the current repo header (current text + your additions only) before the hand-back.` : `This is the first authoring attempt on this row and there is NO draft yet: write the WHOLE function (every block, no stubs) from $W/OUTLINE.md, $W/DOSSIER.md and the retail listing $W/dis_all.txt within your first ten tool calls, grade it, save it as $W/differ/${r.addr}.cpp, and then rapid-fire. Close the big structural differences first (missing or extra code, inline v call decisions, frame size, block layout), then the residues.`)")
rep("then read $W/DOSSIER.md (the scout's proven facts for this row).",
    "then read $W/DOSSIER.md (the scout's proven facts for this row) and $W/HINTS.md (the coordinator's pointers: family models with matched sources, this lane's lessons and the FableWin debug-size tool).")
rep("If your working budget is nearly spent before a match: save the best source as $W/differ/${r.addr}.cpp with line 2 = \\`// Grade: DIFFER(ours v retail) -- <exact residue>\\`, and append to $W/NOTES.md every idea you tried and your best next idea.",
    "The project owner wants every row to have at least a close draft at all times: after every improvement copy the best source to $W/differ/${r.addr}.cpp with line 2 = \\`// Grade: DIFFER(ours v retail) -- <exact residue>\\`, and add two lines to $W/NOTES.md every ten grades (best grade, what moved it, what did not), so nothing is lost if your session is cut off. If your working budget is nearly spent before a match: make sure differ/ holds the best source, and append to $W/NOTES.md every idea you tried and your best next idea.")

# --- verifiers
rep("\\`python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/reserved_check.py $W/payload.json\\` reports 0 reserved rows.",
    "the row's address is listed in rebuild/candidates/manual/pending/2026-11-11_acctB/RESERVED.tsv (this lane works its OWN reserved rows, so reserved_check.py is not used here) and the payload holds no other address.")

# --- repair: HINTS
rep("Read ${S}/w22/WF_BRIEF.md (\"Rapid fire\" and the Rules / Output sections) and $W/NOTES.md.",
    "Read ${S}/w22/WF_BRIEF.md (\"Rapid fire\" and the Rules / Output sections), $W/NOTES.md and $W/HINTS.md.")

open(SP + "/author_rows_acctB.js", "w").write(s)
open(SP + "/chk_author.js", "w").write(s.replace("export const meta", "const meta", 1))
print("ok", len(s))

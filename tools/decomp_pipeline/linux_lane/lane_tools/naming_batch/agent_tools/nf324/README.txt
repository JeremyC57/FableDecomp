NF324 tools (2026-11-24). Run each from the repo root with the lane env sourced and S exported. Every output path is under the agent folder.

flatten_pilot.py   v4 of the repo's flatten_pilot.py: the deliverable. Modes:
                     gen    identity + generation + clang precheck  -> <work>/GEN.tsv, PRECHECK.tsv, gen/, pre/
                     grade  vland HEAD/new per chunk at one path + objcheck -> <work>/<run>/GRADE.tsv
                     export batch dir for land_scripted.py -> RESULTS.tsv, rows/, head/payload.json, TABLE_CHANGES.tsv
                     objcheck  the grade's object comparison alone, for controls
                   The full usage is in the docstring.
hdr_rtti_decls.py  the add-only header layer for gen's `header` bucket. It writes FABLE_RETAIL_<CLASS>_RTTI_DECLS blocks into a full tree
                   copy (work/tree) and into hdr/, keeps the originals in orig/, pins moved asserts, and runs an override probe with a
                   made-up-name control.
triage_v3.py       replays the v3 tool's candidate filter and its flat4 verdict next to the v4 verdict -> TRIAGE.tsv.
assemble.py        STATUS / RENAMES / APPLIED / TABLE_CHANGES / rows.txt / rows/ from the triage GEN.tsv and the exported batches.
lockall.sh         lock.sh (repo toolkit) over a file of addresses, claim or release.
eqrun.sh           lane_merge/equiv_check.py, repo include v a new tree, with S pointed into this folder.
treesync.py        keeps work/tree (the header overlay) equal to the repo's rebuild/include except the edited headers; reports STALE
                   when an orig/ copy no longer equals the repo file (rebase that header and regenerate its rows).
linkcheck.sh       /FORCE /NODEFAULTLIB DLL link of HEAD objects and of new objects: LNK2005 duplicates and the unresolved-symbol sets.
fincmp.py          final-regeneration check: final gen source v the graded source per row (SAME / DIFF / FINAL_SKIP).
inclass_census.py  new-name census: in-class bodies on local models, out-of-line bodies and Leaf lines naming a row's new symbol.
hdr_prune.py       keeps in hdr/ / orig/ only the edited headers an accepted header-route row uses (the rest go to work/hdr_unused/) and
                   writes HEADERS.tsv (address, header, opt-in macro) into each header-route batch.
evcopy.py          copies a graded run's proof into evidence/ without bytes (GEN / PRECHECK / GRADE tsv, vland status lines only).
verify_final.py    hand-back checks: done == ACCEPT in the last final run, rows/ == graded source, landed source unchanged since the grade,
                   NF324 locks == rows.txt.
bytescan.py        byte-dump scan of the hand-back (run from the agent folder): hex-pair runs, 0x-lists and \x strings in every file and
                   every payload source / test; prints hits (0 at hand-back).
access_fix.py, land_scripted.py, ref_scan.py   unchanged copies of the repo's (read-only reference; land_scripted.py was never run).

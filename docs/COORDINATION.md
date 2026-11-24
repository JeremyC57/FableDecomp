# Two-account coordination (owner decision, 2026-11-19)

Two Claude accounts work this repo at the same time. They cannot message each other directly: Claude's cross-session messaging only reaches sessions
of the same account. **The repo is the channel.** Everything in this file is how the two sides stay out of each other's way.

| | Account A | Account B |
|---|---|---|
| Branch | `FullDecomp` | `FullDecomp-B` |
| Owns | the **naming queue**: NAME_FIXES respells, catalog labels, reloc bindings, shared-header renames | **authoring**: the rows handed off on 2026-11-19 and any parked rows it claims |
| Starting point | `docs/HANDOFF.md` | `rebuild/candidates/manual/pending/2026-11-19_authoring_handoff/README.md`, then each agent's `HANDOFF.md` |

Owner order (2026-11-19): "save their authoring work for the second account. so we can do naming queue work on this account while the other account does
authoring". Only the owner merges into `main`.

## 0. Coordination restarted (owner, 2026-11-24): pull from the other branch, then commit, after every piece of work

Owner order (2026-11-24): "I want to start the coordination between main branch and b branch ... Each should pull from the other then commit
when they finish a piece of work." Here "main branch" is Account A's `FullDecomp` and "b branch" is Account B's `FullDecomp-B`; `main`
itself stays owner-merged only.

**After every finished piece of work** (a landing, a hand-back applied, a tools or docs change), in this order:
1. **Pull from the other side:** `git fetch origin <other branch>` then `git merge --no-edit FETCH_HEAD`. Use a merge, never a rebase.
2. **Resolve conflicts** by the table in section 4. Then recheck what the merge touched:
   - after a header merge, that header's landed users;
   - a `build_candidates.ps1` entry both sides changed.
3. **Commit** your own finished work. If the merge changed anything, commit the merge too, then push your branch:
   `git push -u origin <own branch>`.
4. **Read** `docs/journal/2026-11/COORDINATION_LOG.md` for new entries from the other side and answer there.

The session-start sync below still applies. This rule adds a sync at the end of every piece of work, so neither branch drifts for more than
one landing.

## 1. Sync: merge the other branch, never rebase

- **At the start of every session**, before claiming anything and before every landing, run `git fetch origin <other branch>` then
  `git merge --no-edit FETCH_HEAD`, and push your own branch.
- **Merge commits only.** Never rebase, amend or force-push either branch.
- **First sync** (2026-11-19): `FullDecomp` merged `FullDecomp-B` at f2fb6101c9. Account B: merge `origin/FullDecomp` first. It carries this file, the
  hand-off and 117 commits.

## 2. Claim before you work: `rebuild/candidates/manual/COORDINATION_CLAIMS.tsv`

Append one line per claim, on its own commit, and push it before starting the work. Append only: never rewrite another side's line. To close a claim,
append a new line for the same item with status `done` or `released`. The **newest line for an item wins**.

Columns:
- `date` (YYYY-MM-DD)
- `account` (A / B)
- `item`: an address, a header path or a named task
- `kind`: author / naming / header / other
- `status`: claimed / done / released
- `note`

Row status in `rebuild/candidates/manual/UNMATCHED_ROWS.tsv` must agree. B marks a row it starts as `reserved (acctB <agent>, <date>)`; rows already marked
`handed off (2026-11-19, ...)` are B's without a further claim.

## 3. Who may touch what

**Account A (naming):**
- may respell landed rows, catalog labels and `rebuild/oracles/reloc_bindings.tsv` entries;
- must not touch any row whose UNMATCHED status or newest claim is B's.

**Account B (authoring):**
- lands new rows;
- edits an already-landed row only for its own re-port, and claims that row first. A has naming work queued against many landed rows.

**Shared headers (`rebuild/include`), both accounts:**
- additive opt-ins only, copied from the CURRENT merged header;
- run `lane_merge/pin_asserts.py` at every merge; after every header merge, recheck that header's landed users;
- a rename inside a shared header (naming) is a claimed `header` item, because it can break B's open overlays.

**NAME_FIXES that B finds:** B records them in its batch NAME_FIXES.tsv and does not apply them. A applies them at its next naming pass.

## 4. Conflict hot spots

| File | Resolution |
|---|---|
| `rebuild/build_candidates.ps1` | Keep both sides' entries. One address has one entry: if both changed it, keep the one whose source file is in `rebuild/src/compiled`, then recheck it. |
| `rebuild/oracles/reloc_bindings.tsv`, `auto-re-candidates.tsv` | Union of lines; for the same symbol, the respelled (naming) side wins. |
| `UNMATCHED_ROWS.tsv` | Union of rows; for the same address, the more advanced status wins: landed (removed) > handed off / reserved > parked. |
| `README.md` progress, `rebuild/COVERAGE.md` | Never resolve by hand. Take either side, then run `tools/write_decomp_dashboard.py` and `tools/update_readme_progress.py`. |
| `docs/HANDOFF.md`, `docs/ROADMAP.md` | Keep both sides' lines. |

## 5. Messages between the accounts: `docs/journal/2026-11/COORDINATION_LOG.md`

An append-only mailbox. Dated entries `YYYY-MM-DD A->B:` or `B->A:`, one paragraph each.
- **What goes there:** questions, decisions that affect the other side, warnings ("respelling CFoo.h tomorrow").
- **Reading:** read it after every sync, and answer with a new entry. Never edit the other side's entries.
- **Owner decisions** still go to EXTRA_RULES / HANDOFF as before, with a pointer entry here.

# lane_merge: landing rows another lane verified against its own header copies

Used on 2026-11-13 for the acctB hand-over (`rebuild/candidates/manual/pending/2026-11-13_acctB_FINAL/`): 140 rows that matched retail
against header overlays taken at older states of `rebuild/include`, while this branch kept changing the same headers.

- `acctb_setup.py`, `acctb_baseline.py`: one scratch folder per row (payload, overlay, oracle) and a baseline grade of every row against
  its OWN overlay. The hand-over index only carried length estimates; the oracle length comes from the row's `// Retail:` line.
- `acctb_land_v2.py <run> <addr>...`: the gated landing driver, one row at a time. It merges only the headers the row includes
  (3-way against the repo version the overlay came from, found by `find_base.py`), keeps both blocks of a both-added conflict (trying
  the placements of shared `#endif` lines until the file balances), drops an incoming opt-in the repo already has, grows a repo
  opt-in when the incoming one is a superset, renames an incoming opt-in whose name this branch used for something else (`_B`), and
  keeps the repo's header when both lanes wrote it independently. Gates: balanced `#if`s, the default path preprocesses unchanged
  (`coordinator/hdrcheck2.py`), landed files that use a changed opt-in are rechecked, then the strict lander. A row that fails any
  gate is left unlanded with its headers restored. `ACCTB_BUDGET=<seconds>` stops between rows; `ACCTB_DRY=1` only compiles.
- `acctb_stage.py`: the same driver against a PRIVATE copy of `rebuild/include` (`ACCTB_INC`, `ACCTB_STAGE`), for hand merges done
  outside the repository (`fix/<header>`, `fix/src.cpp` in the row's scratch folder). `BRIEF.md` is the brief the merge agents got.
  `acctb_stage_verify.py` re-verifies every staged row against the final private tree.
- `acctb_integrate.py <group>`: merges a private tree's header changes into the current repo headers (base = the commit the private
  tree was copied at), applies the same gates, lands the staged rows and rechecks the landed users.
- `acctb_show.py`, `acctb_versions.py`: read-only views of a row's header merges and of the distinct overlay versions of one header.

Lesson: never commit while the driver is in the middle of a row (it copies headers in before it verifies and restores them on
failure); run it in the foreground with a time budget and commit between runs.

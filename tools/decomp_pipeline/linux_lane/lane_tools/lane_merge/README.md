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

## The proof for any header change (use it for every edit of `rebuild/include`, not only merges)

1. Keep a copy of `rebuild/include` from before the edit (`git archive <commit> rebuild/include | tar -x -C <old>`; give `<old>/rebuild`
   a `src` symlink to the repo's `rebuild/src` so funclet rows find their parents).
2. `pin_asserts.py <old>/rebuild/include rebuild/include <changed headers>`: every `FABLE_STATIC_ASSERT` the edit moved to another line
   is respelled `FABLE_STATIC_ASSERT_AT(<old line>, ...)`. The plain macro names its typedef `fable_static_assert_<__LINE__>`, and
   VC7.1's stack-slot order in a few rows depends on those names (row 0x007E2A60 flips when six comment lines are added to
   `CPlayer.h`), so moving an assert is not a neutral edit. Check the tool's warnings; an assert inside a block that was folded into
   another may need its pin set by hand to the name the EARLIER users compiled with.
3. `equiv_check.py <old>/rebuild/include rebuild/include <out.tsv>`: preprocesses every landed source and test against both trees
   (gcc cpp, about 25 minutes for 56k rows) and lists the rows whose text differs. The comparison is exact, assert names included
   (`EQUIV_LOOSE=1` restores the old behaviour, which hid the names). Identical text means the compiler sees the same input.
4. `recheck_rows.py <addr-file> <outdir> [shards]`: real VC7.1 recheck (byte match and behaviour test) of the differing rows and of
   every row whose source or test was edited. Everything in `good.txt`, nothing in `bad.tsv`, is the acceptance test.

`nesting_check.py <old-commit>` is informational (noisy). Landing fy ran step 3 with the names normalised; landing fz found the gap,
closed it (steps 2 and 3 as above) and measured it: a random sample of 240 header-heavy landed rows all still match at HEAD.

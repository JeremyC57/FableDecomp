# Authoring hand-off to the second account (2026-11-19)

**Owner order (2026-11-19):** "have each agent finish and match their current function then have them end. have them note their progress, and save their
authoring work for the second account. so we can do naming queue work on this account while the other account does authoring"

Each folder is one author's workspace as it stood when it stopped. **Start with its `HANDOFF.md`**, then `NOTES.md` / `STATUS.tsv`.

| Folder | Agent | Landed on this account (2026-11-19) | Still open (UNMATCHED_ROWS status "handed off") |
|---|---|---|---|
| `hand3c/` | not-plain-C++ rows (workspace was w24/hand2c) | b1-b23, ~51 rows + re-ports | 50 reserved rows: 7 touched with residue (00986210, 00d8a240, 00a4d770, 0063f4b0, 00ac7eb0, 008a8260, 00989230 escalated); the rest baseline only. Un-landed overlays: CAIStateGroup_OpinionReactionRoot.h, CQ_HerosOldHouseScript.h, C3DAnimation2.h, C3DMesh2.h. Queued: the CFileInstaller Pause / StartReadingFile / StartWritingFile shared-header ports |
| `hand4d/` | parked near misses | 9 rows (b1-b9) | 0x008185B0 (78 grades, one inlined site), 0x00774D20 BuyItem (49 grades, register swap), GameCODA 0x00C77800 / 0x00C88E60 (not started) |
| `hand4f/` | parked near misses | 8 rows (b1-b4) | 0x006CA8C0, 0x00A0CA90 (untouched; next ideas in HANDOFF.md) |

The finished authors hand4b, hand4c and hand4e left no open rows; their lessons are in GOTCHAS and EXTRA_RULES.

## What was left out
These copies (staged with `tools/stage_ws.py`) leave out:
- retail bytes (oracle.tsv files) and retail disassembly listings: regenerate both with the lane tools (`$S/tools/xdis.py`, mk_rows.py `ref/`);
- binaries, build objects and bulk scratch;
- files over 600 KB.

The GameCODA SDK is never in the repo: fetch it per `docs/BUILDING.md` "GameCODA 2.10 audio SDK".

## Rules that apply
- Read `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`, especially its 2026-11-19 sections: brackets, Inferred scopes, `__assume`
  as an unreachable marker, and GameCODA library copies.
- Read `docs/pipeline/ASM_RULES.md`.
- Overlay headers must be re-copied from the current repo before reuse. Run `lane_merge/pin_asserts.py` at every header merge.

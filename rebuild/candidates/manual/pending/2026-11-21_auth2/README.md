# AUTH2 (Account A authoring agent, round 2, 2026-11-21): 0x00A4EFC0, 0x0093D270, 0x00C73050

Still 0 of 10 released rows matched. Brief: `BRIEF.md`; state: `STATUS.tsv`; per-row `LOG.md`, `ESCALATE.md` / `RESUME.md`; header overlay as
`00a4efc0/hdr.diff`. Grader outputs and objects are left out.

- **Finding (affects every Ego_d size calibration):** Ego_d AND FableWin are VC10 builds (FableWin's `ghidra_out/fablewin_obj_functions.tsv`
  sizes equal Ego_d's: SetScaling 34, GetBoneName 49, C3DVectorWord::operator== 86, GetMaximumAbsoluteComponent 259, AllocateType 371).
  A VC7.1 /Od line size can bound a text against them but not select it; only exact agreements (SetupIdentityFlag 7/54/15/2/15/4) are evidence.
- 0x00A4EFC0: DIFFER(3491v3491), 16 lines with candidate AllocateType text V1 (`a2.cpp`, overlay `00a4efc0/hdr.diff`); rule-clean fallback
  `pending/2026-11-21_auth1/00a4efc0/a1.cpp` (26 lines). AUTH1's AllocateType text (named u64 local) breaks 3 of 5 landed users: fitted,
  rejected. V1 holds for 8 of 9 class-branch users; 0x00A50510 breaks under every if-shaped body (its x87 order is self-declared fitted).
  NOTE: the overlay reuses the macro name FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD for V1; rename it before any landing.
- 0x0093D270: DIFFER(305v295). No Ego_r record of GetProximityForNavigation in the repo dumps; Ego_d: public, float by value. c2 budget-path
  forcing on the m2 mini leaves the bytes identical (compile ORDER untested). Probe data: `0093d270/m2_*.json`, hook `c2/c2force.py`.
- 0x00C73050: DIFFER(672v672), 3 lines. Confirmed with the c2 hook (`c2x/c2prio_x.py`, C2X_FORCE): colouring queue (#9, prio 23) after
  #22/#31/#32 gives a RELOCATION_MATCH object (`objcmp.py`). A natural text needs queue's priority <= 20. Per-block shares in `00c73050/blk.txt`.
  No CAudioSource record in Ego_d / Ego_r: the text is unconstrained and the catalogue name stays (descriptive).

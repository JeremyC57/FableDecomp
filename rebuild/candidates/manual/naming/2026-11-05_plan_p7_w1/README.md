# Plan phase 7 (2026-11-05 late): old-spelling respells, the CThing leftovers and the CIntelligentPointer leftovers

This README covers `2026-11-05_plan_p7_w1` (wave 1, plus the phase's tools, APPLY.md, NEXT.md, AGENT_NOTES.md,
strict_classes.txt and idxpost/), `2026-11-05_plan_p7_w2` (wave 2) and `2026-11-05_plan_p7_w3` (wave 3). One plan agent did the
work on the HEAD 248ea936 index. The coordinator applied and regraded it.

| Wave | Directory | Changed and passing | Deferred |
|---|---|---|---|
| w1, old-spelling binders + CScriptThing::NullCoord + CThing::PeekPlayer | `plan_p7_w1` | 87 | 8 |
| w2, CThing leftovers onto engine/CThing.h | `plan_p7_w2` | 123 | 2,308 |
| w3, CIntelligentPointer leftovers onto engine/CIntelligentPointer.h | `plan_p7_w3` | 388 | 131 |

- **Headers:**
  - engine/CScriptThing.h gains FABLE_RETAIL_CSCRIPTTHING_NULLCOORD: the protected static `?NullCoord@CScriptThing@@1VC3DVector@@A`
    at 0x0143E8E0.
  - engine/CThing.h gains FABLE_RETAIL_CTHING_MOREACCESS (14 out-of-line members) and FABLE_RETAIL_CTHING_OPERATORNEW (the
    protected static `??2CThing@@KIPAXI_N@Z` at 0x004C7380).
  - hdrcheck2 against HEAD reads `default=same` and every existing macro `same`. Only the new sub-macros read DIFF.
- **Applied** with `apply_w35c.py` in the order w1, w2, w3. w2 and w3 rewrote 5 files each on w1's text. The coordinator checked
  every overlap file against w1 with `git merge-file`. The two w1 lines missing from w2 are there in the header's form:
  - 0074e560: `GetTCGraphicAppearance()->SetLightingChannel`;
  - 0085bd70: the header's `PeekPlayer()`.
- **BINDINGS:** w1 retired 12 and added 2, w2 retired 1.
  - `bindings_multi_check.py` reports 1: NullCoord at 0x0143E8E0 / E4 / E8. Those targets are the X / Y / Z members of one
    12-byte object, so this is not one symbol bound at two addresses.
- **Queue:**
  - lines 4451 and 5150 are closed;
  - lines 249 140 206 277 1659 1893 1979 2223 3902 4988 4223 6222 and 220 are narrowed (every line_sha1_12 checked);
  - queue_mark.py marked the 40 `satisfied` lines of idxpost/QUEUE_STATUS.tsv as index-verified.
- **Coordinator regrade from the working tree** (`logs/regrade_*`): the 588 changed rows plus every other landed source whose
  includes reach engine/CScriptThing.h or engine/CThing.h, 6,778 in all.
  vland `--recheck-landed` passed 6,778 / 6,778 (1,456 MATCH, the rest RELOCATION_MATCH) and every behaviour test passed
  (`logs/regrade_vland.tsv`, `logs/regrade_addrs.txt`, `logs/regrade_run.sh`). The agent's own gate runs are in each wave's `logs/`.
- **Not done:** wave 4 (CTCBase 1,223, CThingCreatureBase 1,071). `NEXT.md` is the phase 8 brief. It lists:
  - the wave 1 naming decisions: 0x004BFFC0 ClearHighlighted vs ClearMeshEffect, the semantic multi-spelling targets, the 4
    NullCoord reads outside CScriptThing, and the CThing::operator new placeholder stand-ins;
  - the w2 deferral classes (local_std 567, header_lacks 525, closure_model 461, ...).
  - It also lists the 9 w2 `grade` rows, deferred because the catalog length differs (their landed text grades the same).
- **Tools** (`scripts/`):
  - the phase 6 tools, plus hdrmig7.py, ipmig7.py, w7_respell.py, w7_nullcoord.py, w7_targets.py and includers.py;
  - includers.py takes header paths relative to rebuild/include (`engine/CThing.h`);
  - the tools read `S` from their own text: set it to the current lane scratch before use.
- **idxpost/** comes from the 248ea936 index: MULTINAME.tsv and QUEUE_STATUS.tsv (satisfied 40, partial 2,855, open 652,
  unparseable 1,615).

# nf357 h1: the three NF357 rows held on NF358's census locks (0x006096E0, 0x005E5F30, 0x00607330)

Same change as `../2026-11-24_landing_nf357_b1/README.md` (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CAPPEARANCEDEF_SIZE_BODY; the headers
landed with b1). The run dir is the three rows taken verbatim from NF357's b2a (0x006096E0) and b2r (0x005E5F30, 0x00607330); orig re-diff
against the repo passed. They were held only because NF358 held their locks; NF358 released them (lock files read `released`).
The T3 line `nf357 0x006096E0,0x005E5F30,0x00607330 held (lock)` is removed, since this landing resolves it.

Coordinator verification from the repo (`$S/v357h.sh`, the v357 script on these three addresses):
- vland `--recheck-landed`: 3/3 RELOCATION_MATCH PASS.
- pk_all `--leaf`, agent HEAD v repo: the only change is the GetSubDef<CAppearanceDef> copy NEQ -> EQ; no new NEQ; reachflip: 0 NEQ reach flips
  (3 pairs). pickany_vs_retail: no new NEQ line.
- The rows' other NEQ copies (Reset / ctor of CCountedPointer<CComponent>, GetDef<CThingObjectDef>, map / list copies) are unchanged
  pre-existing debt (NF358 census: the 0x00429C6F, 0x0042B040 lines reopened).
The 0x005E5F30 / 0x00607330 pair is Account B's 2026-11-12/13 stale staging with no open claim (NF357 ruling Q5); B was told in the log.

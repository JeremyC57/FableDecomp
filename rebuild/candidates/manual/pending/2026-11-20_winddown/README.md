# Account B wind-down, 2026-11-20: matched work not yet landed

The owner stopped the session. Everything here is source text only. Oracle files (retail bytes) and binaries were left out, so rebuild the
oracle with `$S/stage/audit_roll/tools/mkland.py`, or vland's own oracle path, before landing. Landing procedure: docs/BUILDING.md and the
2026-11-20 batch READMEs (strict vland `--land`, hdrcheck2, bindings_multi_check, fidcheck).

| folder | rows | state | what is left |
|---|---|---|---|
| hand3c3_b12 | 0x0078D250 CTCBerserk::FrameUpdate | matched | audit: the CRGBColour INTERPOLATE_FORCEINLINE opt-in (device question); the CHitParameters COMDAT is 242 v 178 B. The new CThingFilter_CanBeHitByEpicSpell.h flags 7 rows |
| hand3c3_parked_006e5a00 | 0x006E5A00 CScriptConversation::FrameUpdate | draft, does not compile | see its README |
| followup_navedge | 8 landed or handed-off nav rows incl. hand3c2 b5 0x00A793D0 / 0x00A79680, plus new uncatalogued 0x00A75B60 | matched, strict PASS, pragma-free | audit; add 0x00A75B60 to rebuild/manifest/boundary-starts.tsv and run crawl/manifest_add_boundaries.py --write before landing |
| reports_b4 | 0x00BACE10, 0x00BB1740 (stand-in rewritten as the real ??_G) | strict PASS | audit (the scaffolding on the 0x00BCA060 pattern); PassPattern becomes `00bb1740_TEST PASS` |
| reports_b5 | 8 CPSCSingleSprite rows (3 stand-ins rewritten) | strict PASS | audit |
| reports_b6 | 0x00B0A140 ??_G | snapshot | decide whether it lands with the 0x00B0A160 port |

- Open lists:
  - hand3c3_STATUS.tsv / hand3c3_NOTES.md / hand3c3_HANDOFF.md: 5 rows of hand3c chunk 3 never started;
  - reports_REMAINING.md: the PSC pair's 13 rows, and the CVsyncCallbackTimer set, which needs the owner's header-contract decision
    (options A/B/C);
  - followup_NOTES.md: 0x007E2A60 regression re-author, the 0x00A4D920 real-object test, 0x0040F0E0 ResetDimensions.
- Name fixes for Account A: hand3c3_NAME_FIXES.tsv (64 lines) and each batch's NAME_FIXES.tsv.

# Landing acctB_b24e (Account A, 2026-11-24): three rows staged by Account B

The owner ruled on 2026-11-24 that B verifies and stages and A lands. B's sections are in `pending/2026-11-24_acctB_b24/README.md`.

| row | function | flags | header overlay |
|---|---|---|---|
| 0x00839940 | CThingGameObject::OnHit | stock | CThing.h, CCreatureDef.h, CHeroExperienceDef.h |
| 0x00DE9690 | NScript::CQ_RansomVictimScript::CRansomHelper::Main | stock | none |
| 0x00D04AE0 | NScript::CQ_BanditCampBossBattleScript::CAssassin1::Main | /d2inlS15 | CQ_BanditCampBossBattleScript.h |

## How A checked them
A re-graded each row on its own tree with B's overlay:
- `logs/<addr>.grade_overlay.log`: RELOCATION_MATCH PASS before landing.
- `logs/verify.log`: RELOCATION_MATCH PASS in the landing run.
- `logs/recheck.log`: RELOCATION_MATCH PASS again from the repo. There are no RELOC_CONFLICT lines. OnHit calls 0x006EA430 as
  CCombatManager::SetAttackerAsBeenStruck, which landed in nf341_b2x.

keypayload changed only the test texts of 0x00DE9690 and 0x00D04AE0.

## Headers
Every overlay was based on today's repo header (`logs/<addr>.hdrcheck.log`).
- **CThing.h:** one line changes. The existing `#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT` becomes
  `#if ONHIT && ONHIT_HASTC_CLASSTYPE && !GETTC_EXPANDED ... #elif ONHIT`. The default branch and every existing macro check `=same`.
- **CCreatureDef.h and CHeroExperienceDef.h:** they gain additive STATGETTYPENAME opt-ins. Only those new macros check DIFF.
- **CQ_BanditCampBossBattleScript.h:** it gains the ASSASSIN1 section.
- **Correction (coordinator, after the landing; advisor review):** hdrcheck2 read `[0](cpp-err)` for this header and for the default
  branches of CCreatureDef.h / CHeroExperienceDef.h. That is a failed preprocess, not an includer count, so those checks proved nothing.
  The headers have 9 / 26 / 7 landed includers (41 rows). All 41 were rechecked from the repo after the landing: 10 MATCH PASS,
  31 RELOCATION_MATCH PASS (`logs/includer_recheck.log`). The diffs are add-only.

## Pick-any
Both checkers ran on A's objects:
- `pickany_vs_retail.py` (`logs/<addr>.pickany.log`);
- `pk_all.py` with the row length, source a = the leaf's own relocations (`logs/<addr>.pk_all.log`).

No row has a leaf-called NEQ copy. The NEQ copies left are not leaf-called and are bound at HEAD, so they are debt already queued:

| copy | address | queued as |
|---|---|---|
| `_Rb_global<bool>::_M_increment` | 0x004292C0 | table |
| `??_GCEnginePrimitiveHandle` | 0x0041C500 | self_symbols |
| ~CIsThingAlive | 0x00E241B0 | |
| ~CScriptGameResourceObjectMovieBase | 0x006E7B80 | nf340 |
| ??_GCBaseClass | 0x0099A470 | |
| ??_GCScriptThing | 0x004AB170 | nf328 |

OnHit's GetDef<CMaterialDef> copy is EQ retail 0x0066C340 through `FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD` (nf341_r2).

## Dead asserts in 0x00839940
FableWin.exe's debug OnHit (0x027EC280) holds eleven DoErrorMessage call sites, counted by A. The row carries all eleven:
- three as dead arms (`FABLE_RETAIL_ASSERT_DEAD_ARM`), which emit no code;
- eight as release success forms.

This follows the rule of the 0x00975BB0 landing.

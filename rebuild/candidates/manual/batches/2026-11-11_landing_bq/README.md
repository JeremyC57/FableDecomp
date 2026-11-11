# 2026-11-11 landing bq -- chunk H40 (Fable 5.2, the 40 longest parked rows): 5 NScript quest-script Main bodies

Source: `pending/2026-11-11_linux_handH40/` (STATUS / NOTES / LESSONS / LABELS / SPLITS / BOUNDS_FIX / differ). vland --land: 5 RELOCATION_MATCH, tests PASS.
- 0x00E5B3B0 CV_BeggarAndChildScript::CBeggarBully::Main, 0x00E58D40 CLookoutPointBeggar::Main (true extent 9616, BOUNDS_FIX),
  0x00DD6A60 CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Main, 0x00DE4CB0 CQ_RansomVictimScript::CVictimsFather::Main: the
  predicate lever (chunk 786 "blocker 1"): `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor (and dtor) keeps the nested
  CScriptThing copy 0x004ABE90 / dtor 0x004AA840 called, and FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (_MSC_VER only; the same lever as the
  2026-10-26 predicate filter rows) removes the leftover base ctor call. Only rows whose retail body has no base-vtable store may use it.
- 0x00D5E0C0 CQ_GuildTrainingWillScript::CTheRealGuildmaster::Main (15 KB): timers in a block, `if (T) break;` loop exit, in-body
  `#pragma inline_depth(0)` round the final return / last movie, guarded do-while for the final answer wait.
Headers (base 43674f96e, hdrcheck2 default=same): CPredicateBase (NOVTABLE), CV_BeggarAndChildScript (ENTITY_MAINS), CQ_PrisonEscapeRescueMotherScript
(MOTHERTORTURE, carried from hand811); new CQ_GuildTrainingWillScript.h (hand819). Overlay files serving only parked drafts were not installed.
Parked 35 (14 worked, 21 untouched; differ/ + STATUS.tsv). Grade budget overrun reported: 0x00E84B80 38, 0x00D5E0C0 28, 0x00E5B3B0 20.

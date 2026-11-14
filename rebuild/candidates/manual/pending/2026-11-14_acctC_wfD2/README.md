# acctC workflow hand-back wfD2 (unworked rows round 2, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00E27E90 | NScript::CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze::Main | 3,595 | RELOCATION_MATCH | PASS |
| 0x00DC5F80 | NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageWoman::Main | 4,245 | RELOCATION_MATCH | PASS |
| 0x00EE57B0 | NScript::CV_TourGuideScript::CTourGuideGuide::Main | 4,246 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). The verifier's audit of each row follows; its review
items (inline-budget devices, fitted spellings, label warnings) are for the owner / landing account.
`open/` holds the best drafts of the batch's unmatched rows (residues in NOTES.md).

## 0x00E27E90
Verifier audit: none. Review item (allowed device, not a violation): 2 scoped `#pragma inline_depth(0)` regions, (a) round the row-local CIsThingAlive ctor (FABLE_RETAIL_CPREDICATEBASE_NOVTABLE predicate idiom, landed in 0x00D62480 / 0x00D706E0 / 0x00D8BCD0) and (b) round the talk-arm `seh_hero` declaration (retail calls ctor 0x007E72A0 at 0x00E28424 but expands it in the hit arm and for seh_me). The `// Inline-budget:` marker on line 2 names the sites and the evidence, and records the /d2inlT30..150 and /d2inlS5..20 sweeps that fail to split it. Strict-header check clean: no HEADER_POLICY or SHARED_HEADER_AVAILABLE. vland.log has no RELOC_* warnings for this row. No __asm, naked, _emit, volatile or `if (0)`, and no local class model: only local enum EScriptAIPriority (landed idiom) plus row-local inline ctor bodies for header-declared members. NOTES says no header changes. The test is a real behaviour model: 4 scenarios that assert the event log, PieceOver, Logbook == 130 and Frame.

Fitted items: none marked FITTED. The NOTES call the first wait loop `for (;; Game->NewScriptFrame())` EVIDENCED (landed script-row idiom); the inline_depth regions are the Inline-budget device.

## 0x00DC5F80
Verifier audit: none. Review item (allowed device): 1 scoped `#pragma inline_depth(0)` region round the row-local CIsThingConscious ctor and dtor (FABLE_RETAIL_CPREDICATEBASE_NOVTABLE landed predicate idiom). Retail expands the predicate ctor and dtor but calls the nested CScriptThing copy 0x004ABE90 and ~CScriptThing 0x004AA840. The `// Inline-budget:` marker on lines 2-5 gives the evidence and says no /d2inlT or /d2inlS value splits it (chunk 835). Strict-header check clean. No RELOC_* warnings. No asm, volatile or dead code. Local enums ETextGroupSelectionMethod, EHeroAbility and EScriptEntityMoveType redefine enums that have engine headers; this is a common landed idiom (215 landed rows define EHeroAbility locally) and the strict check did not flag it. NOTES says no header changes. The test is a real model: 5 scenarios asserting the event-log strings and the AttackedByHero / VisitorsAttacked flags, including the heal-life case that is not an attack and early termination. NAME_FIXES.tsv carries the class spelling fix (the catalog had truncated `CSecretPass` to the full CSecretPassageWoman).

Fitted items: none marked FITTED. The NOTES call the second loop `for (;; Game->NewScriptFrame()) { if (T) return; ... }` a landed idiom; the inline_depth region is the Inline-budget device.

## 0x00EE57B0
Verifier audit: none. Warning to report: RELOC_ALIAS: `_rand -> 00bfeb16 is the import thunk 'MSVCR71.DLL::rand'`. This is only an alias note, not RELOC_IDENTITY, RELOC_CONFLICT or OUTSIDE_MANIFEST. No flags, pragmas or inline controls. Strict-header check clean. No asm, volatile or dead code. Local enums EScriptAIPriority and EScriptEntityMoveType are a landed idiom, though EScriptEntityMoveType has an engine header. Row-local inline bodies for CTimer ctor/dtor, CScriptThing ctor/dtor and GetPos match landed rows (112 / 20 landed copies). Uses the header opt-in FABLE_RETAIL_CV_TOURGUIDESCRIPT_GUIDE. The test is a real model: Initialise waypoint choice and restart, non-repeating PickResponse, and 3 frame-loop scenarios asserting Steps, Walked, Faded, Released and Frame.

Fitted items: Main loop spelled `for (; true; Game->NewScriptFrame())` (FITTED, ordinary spelling; source line 13 and NOTES). The explicit `true` changes VC7.1's static block-frequency model so the exit pads route as retail. The author cites landed twin 0x00EB7B00 (`while (true)` v `for (;;)`) and an inline-trace diagnosis.

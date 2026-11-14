# acctC workflow hand-back wfD1 (unworked rows round 2, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00D82640 | NScript::CQ_HeroSoulsBriarScript::CBriarRose::Main | 3,260 | RELOCATION_MATCH | PASS |
| 0x00DEA3F0 | NScript::CQ_RansomVictimScript::CVictimsBrother::Main | 3,076 | RELOCATION_MATCH | PASS |
| 0x00D7FE90 | NScript::CQ_HeroSoulsArenaScript::WatchForRoundsInArena | 3,148 | RELOCATION_MATCH | PASS |
| 0x00D0A830 | NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main | 2,720 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). The verifier's audit of each row follows; its review
items (inline-budget devices, fitted spellings, label warnings) are for the owner / landing account.
`open/` holds the best drafts of the batch's unmatched rows (residues in NOTES.md).

## 0x00D82640
Verifier audit: none. Review item (allowed, owner rule 2026-11-08): scoped #pragma inline_depth(0) round the row-local inline CIsThingAlive ctor/dtor definitions plus FABLE_RETAIL_CPREDICATEBASE_NOVTABLE. The // Inline-budget: line is on line 2 and gives the evidence: the landed predicate lever of chunk H40 (CFishermansWife::Main 0x00D8BCD0); retail calls 0x004ABE90 / 0x004AA840 for the nested CScriptThing. No asm, no volatile, no if(0), no local pad struct. Only repo FABLE_RETAIL_* opt-ins are used. Behaviour test is real: three scenario event-log strcmp checks plus deliveredSoul/frame state.

Fitted items: No FITTED marks in the source or NOTES. The closing idle loop shape is marked EVIDENCED (landed twin CHeroThunder::Main 0x00D7B510). The inline_depth predicate lever is a device, not a fitted spelling. NOTES: two local enums were replaced by the shared engine/EHeroAbility.h and ETextGroupSelectionMethod.h.

## 0x00DEA3F0
Verifier audit: none. Review item (allowed): the same scoped #pragma inline_depth(0) predicate lever plus FABLE_RETAIL_CPREDICATEBASE_NOVTABLE. The Inline-budget marker on line 2 gives the evidence: retail expands ~CIsThingAlive one level and calls ~CScriptThing 0x004AA840 (GOTCHAS chunk 822). EScriptAIPriority is a row-local enum, which NOTES says matches the landed script rows (minor review). No asm, no volatile, no dead code. Behaviour test is real: three scenarios checked by log strcmp plus victimKilled/missionSucceeded/morality/killable state.

Fitted items: No FITTED marks. The loop shape is marked EVIDENCED (landed same-TU sibling CVictimsBrotherHelper::Main 0x00DEB280, TRetVoid _Fail_Val generator pattern). NOTES: the made-up enumerator MOVE_TYPE_RUNNING was replaced by ENTITY_MOVE_RUN from the shared EScriptEntityMoveType.h.

## 0x00D7FE90
Verifier audit: none. Review item (allowed): per-row flag // Flags: /d2inlT57 with the Inline-budget marker on line 2. The only residue is the per-site inline split of std::vector<CScriptThing>::~vector (COMDAT 0x008AC970) across 47 sites, with c2 trace scores given. T57..T64 all match and T57 is the smallest; T56 fails because the L45 / L122 exits get called instead of expanded. The earlier fitted always-called ~vector specialisation declaration was dropped. Warning to report: RELOC_ALIAS _rand -> 00bfeb16 is the import thunk MSVCR71.DLL::rand. This is benign; it is neither RELOC_IDENTITY nor RELOC_CONFLICT. Behaviour test is real: spawn/jack/cutscene event-log checks over two scenarios.

Fitted items: No FITTED marks. The rounds-loop shape `while (round < 3)` is marked EVIDENCED by the inline trace. The /d2inlT57 flag is the declared inline-budget device.

## 0x00D0A830
Verifier audit: No hard-rule violation: no asm, no volatile, no if(0), no unused locals, no local pad struct, and only repo opt-ins (NOTES: no overlay change). Review items: (a) in-body scoped #pragma inline_depth(0) ... inline_depth() round the final `Terminate: return _Fail_Val.Return();`. The Inline-budget line on line 2 cites 0x00D0B27A (movie dtor expanded) and the shared cold tail 0x00D0B297 (~CTimer 0x00CD4470 / ~seh_me 0x007E74D0 called), and notes that /d2inlT and /d2inlS sweeps cannot split the sites. (b) Several heavy fitted control-flow devices: nested do{}while(0) statement-macro wrappers round `if (IsActiveThreadTerminating()) goto Terminate;` at lines 145 and 147, including a double-wrapped do{do{...}while(0);}while(0) used only to lower c2 block frequency; a do{}while(0) movie scope (line 155); and goto Terminate. All are marked FITTED with measurements, and behaviour equals plain returns. The coordinator should judge whether the double do-while(0) wrapper counts as an acceptable fitted ordinary spelling or as a frequency 'ruler'; WF_RULES says dead guards / rulers are not acceptable. Behaviour test is real: three scenarios with taunt/beg/movie/pause event-log ordering and destructor-order (~BEG;~SEH;~TAUNT) checks.

Fitted items: FITTED (header lines 6-12): (1) one do { } while (0) as the speech-movie scope (CEndGameScythe-style frequency lever); (2) the two speech-wait terminate checks leave through `goto Terminate`, wrapped in statement-macro do-while(0): once inside the IsPerformingScriptTask loop, twice after it; (3) `Terminate: return` at the end of the function under inline_depth(0). The plain-return source grades 2700 vs 2720.

# Landing 007175d0 (2026-11-19, hand2b): NTCScriptedControl::CActionSummonerLightningAttackTarget::Update

vland --land, strict headers: RELOCATION_MATCH + PASS (10 scenarios incl. multi-frame, 8/8 mutants). Landed as ::Update (catalog `vfunc_1`;
FableWin + Ego_r S_GPROC32 and header vtable slot 1; `NAME_FIXES.tsv`). Matched on the first grade once written from the FableWin donor
(tc_scripted_control.cpp lines 2638..2690) on shared headers: two scopes each declaring `CThingCreatureBase& parent_creature` and
`const CTCCreatureModeManager* ptc_cmm`; the second PeekTC is the release-evaluated assert (result untested). The parked register-colouring
residue came from the old local-struct model. New opt-ins: FABLE_RETAIL_CACTIONSUMMONERLIGHTNINGATTACKTARGET_PTARGET (PTarget as
CScriptThing), FABLE_RETAIL_CCREATUREACTION_SUMMONERLIGHTNINGORBINTO_CLASS (class over CCombatActionBase, ctor 0x008CBD90, POrb +0x114,
sizeof 0x11C).

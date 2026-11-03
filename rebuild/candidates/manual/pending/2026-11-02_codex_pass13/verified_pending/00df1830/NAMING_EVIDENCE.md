# 00df1830 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 44. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00df1830/NAMING_EVIDENCE.md

# 00df1830 identity and evidence

Current status: VERIFIED_MATCH, independent main44,2139bytes,121relocations,646scenarios per compiler. Earlier parked discussion below is preserved historical evidence.

Original identity is `NScript::CQ_SummoningTheShipScript::CSTS_BriarRose::Main`, private virtual Leaf `?Main@CSTS_BriarRose@CQ_SummoningTheShipScript@NScript@@EAEXXZ`. Original Ego_r TPI0x10e8f/0x10eea in evidence/00df1830_types.json gives the complete owner/nested data and access. Both original name sets contain the Leaf. Function-local extraction is evidence/00df1830_function_locals.json: Ego_r stream24 record0x25bf6c, FableWin stream23 record0x26ce84.

Independent native factory00df16b0 allocates0x20 and writes its own vtable012de1ac, Game+4 from parent+0x40, Me+8, ParentClass+0x14 and PMasterData+0x18. Vtable slot1 contains00df1830; slot2 is independently named Init00df1760 and slot5 is the empty OnPredicateFail00df1800. Native Main writes BrainState+0x1c and reads parent SummonersAlive+0x48, CurrentAttackWave+0x4c and SummonerAttacksStarted+0x51, agreeing with donor field order after the base shrinks from0x5c to0x48. The new complete owner header has all seven original data members, all six nested entity classes and each nested field through its full size. Owner size0x5c and target nested size0x20 compile-time assertions pass.

The old neighbour Init alias at00df2090 is the original private `FollowHero(bool)`: original TPI and FableWin donor Main name the callee exactly, while native toggles the follow/unfollow interface operations on its own Me. Parent MakeBriarRoseComment00df20f0 is public bool(const CCharString&) in namespace NScript; the older relocation spelling omitted NScript. Original public/global records are in evidence/00df1830_original_publics.json, including exact float fastcall GetDistanceBetweenThings(const CScriptThing&,const CScriptThing&) and the original CGameScriptInterfaceBase* pDSTGame.

Original locals are SummonerCountComment, BriarMode, CurrentAIState, PreviousAIState (long), AttackedByEnemyTimer (CTimer), Enemy, Summoner, Minion (CScriptThing), SummonerCountCommentString (CCharString), and TRetVoid _Fail_Val. The readonly original FableWin function at038a4f80 establishes the loop construction and calls IsDead in the loop-carried enemy check. Native004aa890 implements IsDead as a virtual call to IsAlive followed by boolean inversion; this accounts for the middle virtual call that a direct !Enemy.IsAlive source would incorrectly devirtualize. The selected source uses the original IsDead method and a constant-false-negated generated for-loop with frame increment and leading termination return. The initial CurrentAIState and PreviousAIState zeros are also visible in the donor. Native evidence governs the shipped behavior and layout.

Native distance evaluation computes Summoner distance first, then Minion distance and compares them; the selected `GetDistanceBetweenThings(Me,Summoner)>GetDistanceBetweenThings(Me,Minion)` reproduces this order and chooses Summoner for equal or unordered distances. Native enemy attack commentary checks both summoner and minion in one timer-zero block, so both may speak and reset the timer in a single frame. The hero attack comment is outside that timer gate. All three magic tests exclude healing from SCRIPT_NAME_HERO, including the enemy-source tests.

The2139-byte extent is exact: RET00df208a, five int3 bytes00df208b..00df208f, then FollowHero00df2090. The selected Leaf occurs once in the COFF table and strict extraction succeeds. Both native and selected frames are0x94. Normalized raw-instruction comparison differs only at two assignment sites: native expands pDSTGame->SetTimer(slot89), while the selected compiler calls the original CTimer::operator=. Those two sites account for20 bytes, yielding DIFFER(2119v2139). Every121 selected relocation and14 virtual identities is audited in RELOCATIONS.json, with the two native-inline/object-call bindings explicitly categorized. No full match is claimed.

The exact selected Main body passes646 scenarios on GNU64 and actual VC7.1 /GR-. Tests cover start waiting, objective and follow setup, original count comment labels/ranges/retries, AI transitions, all128 hit/magic/heal combinations, both enemy comments in one timer gate, independent hero commentary, target selection/reacquisition, equal and NaN distances, negative timer suppression,100 termination checkpoints in5 modes, and timer/thing cleanup. Selection40 retains the donor-proven shape after source families, helper contexts and late flag probes; failed and superseded receipts remain immutable.

## Expert timer follow-up

Original FableWin donor0360a190, name-set exact public member `??4CTimer@@QAEAAV0@J@Z`, independently reads argument long, TimerIndex+0, global pDSTGame, calls virtual displacement0x164(slot89), then returns saved this. Ego_r name-set also contains the same exact member. This confirms the existing assignment body without inferring an annotation. Selected stronger form expands precisely the two remaining native assignments; every other native instruction already agreed. All121 actual COFF operands now bind positionally to native targets/payloads,14 virtual identities were independently rechecked, native frame0x94 and RET00df208a/padding agree.646 scenarios pass GNU C++03 and VC7.1 /GR-, including the genuine assignment body through an instrumented interface.

Main44 uses the corrected private CIsThingAlive member access and integrated original owner header. Byte parity and all exact target identities reconfirmed. Tests rerun from root. No out-of-scope helper completion or original inline keyword proof is claimed.

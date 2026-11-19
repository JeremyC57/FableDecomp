# Landing 0x006E89B0 CDefendingCombatantInfo::CCombatWheel::GetAttackerPlacementVector (2026-11-19, 344 bytes) -- Inferred, owner decision

hand2a (escalated twice to the Fable advisor). vland --land, strict headers: RELOCATION_MATCH + PASS (`logs/vland_land.log`); test kills
4/4 mutants. New opt-in FABLE_RETAIL_CCOMBATRING_ISOUTERRING (engine/CDefendingCombatantInfo.h: CCombatTypeDef forward declaration +
IsOuterRing declaration; Ego_r 0x00711110 / FableWin 0x024103D0). Structure from FableWin.exe's debug copies (ASSERT(pring), the
IsOuterRing test, arm 1 `prong * pref_dist`, arm 2 `/ 2.0f`, helper tail `separation += distance`), each checked against retail.

Inferred (owner, 2026-11-19: "land it as inferred with the double type and float cast"): the inlined helper's `distance` is a `double`
compared as `(float)distance < DELTA_NOTIONAL_ZERO`. No donor shows the double (FableWin types it float). Measured alternatives, all
DIFFER: float with / without the cast / `* 1.0f` (344v344, pop esi before fmul at 0x006E8A7A); double compared directly or `* 1.0f`
(348v344, fld st(1); fcompp). The source's `// Inferred:` block records all of this.

Queued naming debt (NAME_FIXES.tsv; the landing added the proven bindings and so left two names on 0x006E7FD0):
- 0x006E7FD0 is `?GetProngVector@CProng@CDefendingCombatantInfo@@QBE?AVC3DVector@@M@Z` (landed row CProng_GetRotatedPosition_006e7fd0 on a
  local model) and 0x006E8810 is `?GetAttackerProngVector@CCombatRing@CDefendingCombatantInfo@@QBE?AVC3DVector@@ABVCThingCreatureBase@@@Z`
  (landed row CCombatRing_GetAttackerProngPosition_006e8810). Both need re-authoring on engine/CDefendingCombatantInfo.h -- which first
  needs a layout decision: the shared header (Ego_r TPI) puts CProng::Prong at +4, the landed 0x006E7FD0 row reads it packed at
  +1 / +5 / +9 from retail's bytes.
- 0x0129BA3C is `?DELTA_NOTIONAL_ZERO@@3MB` (label still `g_NormaliseEpsilon`).

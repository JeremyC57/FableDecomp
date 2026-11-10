# 2026-11-10 landing ag -- chunk 1010b, retry of the CGameScriptInterface rows (3 rows)

Source: `pending/2026-11-10_linux_hand1010b/`. vland --land: 3 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- 0x00896390 OpenChest: the nested positive form fixed the block order (711v711 before).
- 0x0089B5B0 StartScriptingEntity: rebased on the retail CScriptGameResourceObjectScriptedThing class, CTCScriptedControl::IsLocked,
  CThingAICreature::GetCurrentStateGroupScriptPriority; the script ID read through FableWin's inline GetCurrentScriptID().
- 0x008A8C60 EntitySetAsSittingOnFloor: three new shared headers + the SitDown opt-in; visible noinline CCountedPointer<CModeSitBase> ctor /
  dtor bodies (FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR); the CDefString conversion forwarder expanded per TU
  (FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION, an `// Inline-budget:` row: retail calls GetString 0x00415DD0 where VC7.1 would call ??B).
- Manifest names set to the plain method names.

Parked (differ/): KickOffQuestStartScreen DIFFER(437v445) (exit-block placement only), EntityWillTeleportToArea DIFFER(618v620) (this /
thing tie for ebx), ScreenFilterFadeTo DIFFER(347v363). Unworked: 0x0089C910, 0x0089CE70, 0x008A7A70, 0x008A8260, 0x0088EDC0.
Queued `land1011ag`: 8 name fixes (OnQuestStartScreenDisplayed 0x004B2FE0, CCountedPointer<CModeSitBase> ctor / DeleteData, ...).

# 2026-11-11 landing bn -- agentNF12: 7 GetSubDef / PeekDefinitionManager binders moved onto the shared headers

Source: `naming/2026-11-11_agentNF12/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES). vland --recheck-landed here: 7 RELOCATION_MATCH,
VC7.1 tests PASS (g++ PREFAIL on 0x0078809B / 0x0083DF20 / 0x007310C0 is pre-existing: their landed tests embed old-source copies).
- GetSubDef<CAppearanceDef>: 0x007884F5 CTCMeleeTrap::ResetTrap, 0x00788406 TriggerTrap, 0x0078809B InitialiseAnimation (now protected /
  private as in Ego_r), 0x007310C0 CTCDoor::UpdateOpenStatusTriggerTypeScript (invented CTCDoor_Methods class dropped).
- CThing::PeekDefinitionManager: 0x008338D0 CThingAICreature::InitialiseFromDef, 0x0083DF20 CThingPhysicalSwitch::UntriggerMe, 0x0083DDC0
  TriggerMe (CThingCreatureBase& parameter, as in the PDBs). Stand-in GetSubDef / GetDef / LowerBound / PlayAttachedSample bodies removed.
Headers (base 3123ac48b, opt-ins only, hdrcheck2 default=same): CSoundInit, CTCDoor, CTCEntityEvents, CTCSoundPlayer, CTCTrapBase (retail
polymorphic opt-in), CThingAICreature, CThingCreatureDef, CThingPhysicalSwitch; new CTCMeleeTrap, NEntityEvents_CEventTriggered.
Not installed: CInventoryDef DOLLS opt-in + new CTCDNone.h (only the parked 0x005B37F7 uses them; kept in naming/.../parked/).
Bindings: 24 removed, 12 added (logs/). Queued land1011bn: 16 name fixes incl. DOOR_OPEN / DOOR_CLOSE globals 0x013BAD34 / 0x013BAD38.

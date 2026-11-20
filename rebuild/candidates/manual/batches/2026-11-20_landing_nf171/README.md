# Landing nf171 (Account A, 2026-11-20): CThing / player GUI names (17 rows; 0x00446B10 follows in nf171b)

Agent NF171 (round 10) handed back early (heavy context) with 18 entries: 9 done, 2 done-already, 1 blocked, 2 set aside for
locks, 4 open. Its files are in `naming/2026-11-20_agentNF171/`. The 6 unfinished entries go back to the queue unclaimed.

- 17 rows. Self renames (8): CThing::GetJoystickDeviceNumber 0x004C7CA0 (an in-place const; the header port q11924 asked for is
  queued), CTCMapwho::PlaceInMapwho / MoveInMapwho 0x0071A710 / 0x0071A750 (moved together onto the shared headers),
  CMiniMapDisplay::Initialise 0x0082A310, CThingGameObject::ConstructFromLevelScript 0x00838930 (the old-style row re-authored
  under the PDB virtual), CGameScriptInterface::ClearAllRumbles 0x00892DF0, CScriptGameResourceObjectScriptedThing::
  ClearAllActions / ClearAllActionsIncludingLoopingAnimations 0x00903920 / 0x00903970. Five tests were rewritten on the shared
  headers.
- Headers (declaration-only opt-ins, `orig` equals HEAD): CMiniMapDisplay.h (INITIALISE), CGamePlayerInterface.h
  (ISOTHERQUICKACCESSITEMBUTTONHELD), CScriptGameResourceObjectScriptedThing.h (CLEARALLACTIONS; 8 existing asserts line-pinned).
- Coordinator calls:
  - 0x0088E1C0, 0x00903920 and 0x00903970 static_cast CThing* to CThingPhysical* / CThingCreatureBase* after the type-bit test.
    Both sides are PDB classes, the idiom of landed 0x0085FA00. Accepted.
  - 0x00440E40 / 0x00441670 (q5132) are identical unreferenced ~list<T> bodies. T is unprovable, so they keep their stand-ins.
  - 0x00446B10's self rename was held out of this run, then re-checked against the self-rename rule (nf120 / nf139). Its old leaf
    (`CGamePlayerInterface_Methods@@QBE...J@Z`) is bound by no landed row, and the new one is bound by the rows this landing respells.
    So it lands on its own in nf171b, right after.
- Recheck (`logs/recheck.log`): 17 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 247 good, 0 bad. tablecheck: 0x006A5D90 is now recorded once, under the PDB virtual (0x00838930 dropped a forwarding
  stand-in); ten addresses shrink, among them 0x00446B10 3 -> 1.
- Link (`logs/link_landing.log`): 5 LNK2005 (CVectorMap::LowerBound), the same as HEAD (`logs/link_control_head.log`).
- Queued: the ~list twins, the q11924 header port, the EInputButtonEventType enumerator for value 2, and 0x00687D50's self
  signature family.

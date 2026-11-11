# NF11 notes

All 12 changed rows were regraded after the container restart (`work/final.sh`, --all): RELOCATION_MATCH,
the target BIND OK, g++ and VC7.1 tests PASS, catalog PassPatterns present (`work/pp.py`). No PREFAIL rows were touched.

## Left, with reasons
- 004bfd10 CTCGraphicAppearance::CatchMessage (GetSubDef): parked in work/parked/004bfd10.cpp. Residue does not close:
  our def pointer stays in ebp, retail reloads it from the slot. Tried PeekBaseDef / GetPBaseDef, no BODY,
  RELEASE_POINTEE, SELFDELETING. The landed row is unchanged and its header opt-in was withdrawn.
- 00665510 CThingCreatureBase::UseAbility (PeekDefinitionManager): calls CThingCreatureBase virtual slot 72
  (SetActionForMelee); CThing's virtual slots are not declared in the shared headers. Its GetDef 0x0043FFA0 is
  GetDef<CCreatureAbilityDef> (NAME_FIXES).
- 008d1130 CGameScriptThing::MsgIsHitByWithWeapon: needs the event-filter templates and the quest manager headers.
- Not attempted (time), GetSubDef: 005b37f7 005cd767; 00779d20 (orb: SetMainGraphic then `or [+0x59],2`, field not
  identified); CTCDoor 007306c0 00730c00 00730dd0 007310c0; 007502d0 CTCChest (needs the CTCContainer chain,
  COpenCloseHelper, mesh bank); traps 0078809b 00788406 007884f5 (need a retail CTCTrapBase opt-in, a CTCMeleeTrap
  header, and the size-optimised TU with an out-of-line bitset::test).
- Not attempted, PeekDefinitionManager: 00664a50 006a9ef0 0082d3b0 00832e60 008338d0 00833a70 00836260 0083cc10
  0083ddc0 0083df20 008cb780. 0083ddc0 / 0083df20 carry same-TU stand-ins for GetDef<CThingPhysicalSwitchDef>
  0x0083E0D0, LowerBound and CTCSoundPlayer::PlayAttachedSample and need a CThingPhysicalSwitch header plus
  CSoundInit / CSoundPair on shared headers first.
- Item 3 (GetDef<CThingBaseDef> 0x0042AF3C / CTCBase::GetDefinitionManager 0x006869D0 leftovers and NF10's "left"
  list) was not reached; NF10 STATUS.tsv still describes it.

## Landing notes
- Each row turns on the opt-ins it needs with `#define FABLE_RETAIL_...` lines at its top (no `// Flags:` line is used).
- 00732850 and the static initializer 0x0121ED40 name the CCharString at 0x013BAD3C differently (NAME_FIXES).
- 0x0070D920 is CAnimateThing::Init(const CAnimInit&); the Ghidra label CTCAnimation::SetMainAnim gives a RELOC_NAME warning.

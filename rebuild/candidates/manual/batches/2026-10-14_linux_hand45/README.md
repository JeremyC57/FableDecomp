# 2026-10-14_linux_hand45 — hand-authored chunk 45 of the class-labelled 17–96-byte pool

Coordinator-authored (no sub-agent) on the Linux VC7.1 lane. Chunk 45 (`mkchunk.py 45`, 40 rows: 29 `CGameScriptInterface`
entry points, 11 `CGameScriptThing`-labelled rows): **18 of 40** landed as `RELOCATION_MATCH`/`MATCH` with behaviour tests.
18 rows were not attempted: they are component-map lookups whose retail body loads the component with `mov eax,[eax+4]`
(the gap documented in `_hand44/README.md`; the classifier is the byte after the `mov eax,esi` of the find tail), and 4
were parked after their attempts.

## Landed shapes
- Component rows retail loads straight into ecx: `DeactivateBoastUi` (hero with no null/dead test), `IsThingTurncoated`,
  `SetTavernTableAvailableForUse` (null-tested component), the region exit/entrance rows (no map lookup: the object sits at
  the thing's +0x5c; the exit row also refreshes the player GUI through the global at 0x013B8790).
- `CGameScriptThing` over its `CIntelligentPointer` at +0xc (`GetPItem` @ 0x00A01B50 re-called for every access): the two
  module twins of `SetToKillOnLevelUnload` (0x0066AD90 / 0x0066ADD0 guard on the global at 0x013B8394), `UpdateThingAttachment`
  (the 64-bit UID at +0x18: the struct needs `#pragma pack(4)` or the `__int64` pushes the members past the vptr),
  `SetDataString` (the thing's string assigned through an inline setter: the argument is pushed before the pointer fetch),
  `GetCurrentStateGroupType` (tail jump), `IsEqualTo` (`sub ; neg ; sbb ; inc`), `GetCurrentMapName` / `GetHomeMapName`
  (CCharString by value from `CWorldMap::GetLevelScriptName` or the empty string).
- Three 0x00C2/0x00C3/0x00C5 bodies carrying propagated CGameScriptInterface labels (a scoped named lock around a
  `__stdcall` queue processor, an observer attach, a validate/reset pair), named by address.

## Parked (4; sources in the lane's `differ/`)
- 0x006AF520 `GetPThing`: retail keeps the null result in edi (`xor edi,edi` at entry, `mov eax,edi` at the exit) around the
  three `GetPItem` calls; the lane re-materialises `xor eax,eax` (zero-register retention gap).
- 0x008A96F0 / 0x008A97D0 `SetIsGossipForPlayer` (static `__stdcall`, by-reference and by-value overloads): retail homes the
  CCharString temporary for `map::operator[]` in the dead argument slot; the lane allocates a frame word (dead-argument-slot
  homing gap).
- 0x008D03E0 `GetFocalPos`: register choice (`this` in esi and the pointer address in edi in retail, the reverse in the lane).

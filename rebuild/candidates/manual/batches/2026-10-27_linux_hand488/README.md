# Chunk 488 (agent488, 2026-10-27): 38 of 40 single functions byte-exact

Fourth queued 40-row chunk of single functions under 128 bytes (address order). One Opus 5.5 agent at high effort.

- **Landed:** 38 rows with `vland.py --land` (verify.log: 18 MATCH, 20 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY).
- **Parked (1, pending/2026-10-27_linux_hand488/differ/):** 0x00680840 CSimVoice::GetRandomSampleIndexVariation
  (114v114: only the stack slot of the last by-value copy of `type` differs, retail `[esp+14h]` vs ours the dead `seed`
  slot `[esp+18h]`).
- **Skipped (1):** 0x006C9C60 carries the catalog label `Catch@004ab920` but is an ordinary thiscall function
  (`sub esp,10h` ... `ret 8`); the label is wrong and the row should be re-queued as a function.
- **Identities:** 18 proven (ghidra_out/labels_chunk488_2026-10-27.tsv), 17 probable, 3 descriptive (NOTES.md).

Label corrections (NOTES.md): 0x00687D40 is CInputProcessGameBase::GetMouseWheelMovement (not operator_new); 0x006924B0
CCreatureActionBase::GetRandomAnimFromName (not BeginInputLoading); 0x006CB6A0 CTCCreatureNavigation::GetNavigatorType;
0x0073A6E0 CTCCreatureModeManager::GetCurrentMode (not CPixelShader::CountReferences); 0x00751A80
CTCLook::GetLookAngleRelToBodyXY (not CMap::DrawGetSeaLevel); 0x007122C0 CTCScriptedControl::IsActionListEmpty; callee
0x00445920 (landed as `_Cons_val`) is CGamePlayerInterface::GetMouseWheelMovement, 0x005BC0D3 ~CEngineTextureMorphSet,
0x00886430 CEnvironmentThemePriorityStackEntry::SetDefaultThemeWeight.

Review at landing: the RELOC_CONFLICT is again landed 0x00896EE0 binding GetPControlledCreature to 0x00487DD0
(PeekPControlledCreature); landed stand-ins: 0x006F1280 `CThingCompareMap::Insert`, SElement/SRecord in 0x00743430,
0x006F4470, 0x006F44E0. Header debt: eight rows use a local polymorphic CTCBase (engine/CTCBase.h flattens the vptr).

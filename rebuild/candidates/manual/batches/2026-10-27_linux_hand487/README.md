# Chunk 487 (agent487, 2026-10-27): 40 of 40 single functions byte-exact

Third queued 40-row chunk of single functions under 128 bytes (address order). One Opus 5.5 agent at high effort.

- **Landed:** all 40 rows with `vland.py --land` (verify.log: 12 MATCH, 28 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). Nothing parked or skipped.
- **Identities:** 23 proven (ghidra_out/labels_chunk487_2026-10-27.tsv), 13 probable, 4 descriptive (NOTES.md).

Label corrections (NOTES.md): 0x0057A59B is CTCHeroStats::AddRewardMoney (not CTCCoopSpirit::AddExperience); 0x005DA530
is CTCWeapon::IsProjectileWeaponClass (not CComponent::HasState); 0x00661FC0 is CThingCreatureBase::IsIndestructible
(inherited slot 67) and landed 0x00661FD0 is CThingCreatureBase::IsWillable; landed 0x006804C0 is the
EReactionSpeechType map's const LowerBound (all six callers are CSimVoice members), not the ETCInterfaceType one;
callees 0x0055B040 / 0x0055A890 / 0x0054E5D0 are CClickable / CDraggable / CViewport::GetDataFromDef, 0x0066E73D is
NHeroInformationScreens::CManager::Close, 0x004830A1 is CTCInGameMenu::OpenInGameMenu(), 0x00698F50 is
CGameCameraManager::PushSpecialCameraMode.

Review at landing: the 3 RELOC_CONFLICT warnings agree with family pass 6 (GetPControlledCreature 0x00487DC0); landed
0x00896EE0 is the one to rebind (PeekPControlledCreature 0x00487DD0). 0x00661FC0 is declared non-virtual (QBE) where
the PDB has UBE (bytes identical; the CThingCreatureBase header stores the vptr as data). Header debt: CWorldMap,
CSurface, CThingCreatureBase headers flatten the vptr; no headers yet for CViewport, CScrollableList, CClickable,
CDraggable, CMinimapData, CThingCreatureDef, CThing.

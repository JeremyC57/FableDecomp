# Landing 2026-11-07w: authoring chunk 846 (2 of 6 landed, 4 parked)

| Row | Grade | Note |
|---|---|---|
| 0x0051FD80 CThingManager::LoadSingleThing | **RELOCATION_MATCH + PASS, landed** | Codex pass-12 source rebased onto our headers (61 compile errors on our tree before) |
| 0x00520D00 CThingManager::LoadEntitiesFromScript | **RELOCATION_MATCH + PASS, landed** | the same rebase plus the CLevelLoader LOADALL and CCharString operator< opt-ins |
| 0x00657580 NPlayerGui::CDrawQuickAccessIconsPC::Draw (was vfunc_2) | DIFFER(1057v1057), 24 | x87 operand order of one C2DVector sum |
| 0x00816410 CPhysicalPrimitiveCylinder::GetClosestPointToPos2D (was vfunc_10) | DIFFER(1122v1174), 194 | frame 0x6c v 0x60 (slot reuse) |
| 0x00D0B3E0 CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main | DIFFER(3802v3802), 36 | CScriptThing copy split + per-exit ~CTimer |
| 0x00DB3FA0 CQ_NewOakValeIntroScript::CNOVI_BookTrader::Main | DIFFER(4106v4042), 53 | one-level ~CIsThing blocker |

- **Headers landed (all additive opt-ins, default path textually unchanged):** CClassFactory ALLOCATECLASS, CLevelLoader LOADALL,
  CPersistContext ISBINARY, CQuestManager ONACTIVATEENTITY, CThing LOADSINGLE, CThingManager LOADSINGLE, CEntityRuntimePersistence DEAD,
  fable_string CCHARSTRING_LESS_INLINE, fable_map STLP_TREE_CONSTFIND. The agent rechecked 35 landed includers with the overlay (all
  still matching). The CGamePlayerInterface / CEnginePrimitive2DSprite / CQ_NewOakValeIntroScript opt-ins serve only parked rows and stay
  in `pending/2026-11-07_linux_hand846/hdr/`.
- **Names:** labels `labels_landing_2026-11-07w.tsv` (the two placeholders, base CDrawQuickAccessIcons::Draw 0x00652FA4, GetRootsNumerically
  0x008185B0, CGamePlayerInterface::GetNumberOfQuickAccessItemButtons 0x00445950); queue `land0711w`: the `_M_find` member templates at
  0x00527D80 / 0x00527D20, 0x00445950, 0x008185B0.

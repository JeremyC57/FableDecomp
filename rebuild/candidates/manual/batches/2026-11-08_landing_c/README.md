# Landing 2026-11-08c: authoring chunk 852 (0 of 6 landed, 6 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00D07E50 NScript::CQ_BanditCampBossBattleScript::CCampHostage::Main | DIFFER(2016v2068), 25 lines, 10 attempts | was placeholder Quest009_Gamesmaster_Hostage_Setup; documented predicate-dtor blocker (MovieBase dtor called at 3 exit pads, retail expands it) |
| 0x00D067A0 NScript::CQ_BanditCampBossBattleScript::CBCGameMaster::Main | DIFFER(2644v2628), 52 lines, 10 attempts | was placeholder Quest009_Gamesmaster_ResidentialBanditCamp_Setup; caller-size inline budget (matches only under a diagnostic budget flag) |
| 0x00677970 CPersistTraits<CReactionFrequencyTraitsArray>::TransferIn | DIFFER(2273v2272), 484 lines, 6 attempts | was placeholder TOpinionTransientOffsetList_TransferParsing (wrong: ret 0xC, body allocates the three CReactionFrequencyTraits kinds); register allocation |
| 0x00E01900 NScript::CQ_TraderEscortScript::MakeTraderComment | DIFFER(2641v2697), 110 lines, 8 attempts | retail expands CScriptThing::operator= at the first site only |
| 0x00981410 CAIStateGroupBase::AIAction_InteractWith | DIFFER(2491v2703), 890 lines, 4 attempts | first full body; frame 0x358 v 0x364, loop-local layout, LowerBound expanded in the loop but called in cleanup |
| 0x00BB2540 CEngineClothRenderer::RenderPolygons | DIFFER(2726v2905), 834 lines, 4 attempts | first full body; local cloth / particle models (chunk 850's CParticleProgram opt-in is still pending) |

- 0x00981410 and 0x00BB2540 got under 10 attempts (large bodies, structural residues): candidates for a fresh pass.
- **Headers:** CQ_BanditCampBossBattleScript (HOSTAGE / GAMEMASTER opt-ins) and C3DVector (GETMAGNITUDEANDNORMALISE) serve only parked rows;
  kept in `pending/2026-11-08_linux_hand852/hdr/`.
- **Names:** labels `labels_landing_2026-11-08c.tsv` (the three proven placeholder names); queue `land0811c` (0x00677970).

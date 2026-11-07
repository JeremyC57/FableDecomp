# Landing 2026-11-07v: authoring chunk 845 (1 of 6 landed, 5 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00E57E60 NScript::CV_BeggarAndChildScript::Main | **RELOCATION_MATCH + PASS, landed** | new header `engine/CV_BeggarAndChildScript.h` |
| 0x00D67890 CQ_GuildTrainingWoodsWillScript::Main | DIFFER(3472v3426), 120 | retail inlines `resize(n)`; the choice depends on the preceding wait loops |
| 0x00D04AE0 CQ_BanditCampBossBattleScript::CAssassin1::Main | DIFFER(3847v3623), 183 | caller-size inline budget |
| 0x00E54E90 CV_BookCollectingScript::CBS_Teacher::Main | DIFFER(3534v3534), 35 | an entry zero store (a conditional-temporary flag home) and one return pad |
| 0x00E97070 CV_HeroDollsScript::CBS_Teacher::Main | DIFFER(3619v3619), 60 | the same entry flag store |
| 0x00ED8680 CV_TalentlessBardScript::CTB_Bard::Main | DIFFER(3793v3652), 196 | CIsThingAlive nested-copy blocker |

- **Names:** all six in the PDB name sets (the brief wrongly said the CBS_Teacher / CTB_Bard classes had none). Labels
  `labels_landing_2026-11-07v.tsv` (LookForBook, DoMission, CBeggarBully::Alloc, WatchForResetWanderCentre, GetRandomSpeech, the std::map dtor at
  0x00CDBFB0). Queue `land0711v`: 0x00E55CE0 AskForBook is bound without its namespaces.
- **Headers:** only the new `engine/CV_BeggarAndChildScript.h` landed. The CScriptBase default-region opt-in, the CAssassin1 opt-in and the four
  other new script headers serve only parked rows; they stay in `pending/2026-11-07_linux_hand845/hdr/`.

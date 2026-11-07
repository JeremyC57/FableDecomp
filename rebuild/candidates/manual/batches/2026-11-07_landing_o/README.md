# Landing 2026-11-07o: chunk 838 (2 of 6 rows landed, 4 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00D9DAF0 NScript::CQ_HobbeToothContestScript::CSpawnNewHobbe::Main | **RELOCATION_MATCH + PASS, landed** | row-local `__forceinline CScriptThing::operator=` (established, chunk 805) for the if-chain CreateCreature assignments; header opt-in `FABLE_RETAIL_CQ_HOBBETOOTHCONTESTSCRIPT_CSPAWNNEWHOBBE` copied in (additive, no landed definer) |
| 0x00EB7B00 NScript::CV_MayorsInvitationScript::CWiseVillager::Main | **RELOCATION_MATCH + PASS, landed** | written from the retail disassembly; MOVIEBASE_OUTOFLINE_CTOR; idle loop `while (true) { NSF; if (T) return; }`; header opt-in `FABLE_RETAIL_CV_MAYORSINVITATIONSCRIPT_WISEVILLAGER` copied in (additive) |
| 0x00D33730 NScript::CQ_EndGameBossBattleScript::CTheRealGuildmaster::Main | DIFFER(3292v3292), one byte | one jump target: the talk branch's first thread-check return goes to the expanded ~CTimer exit, retail to the shared ~CTimer call pad (per-exit ~CTimer split) |
| 0x00D21B40 NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::Main | DIFFER(3013v2965), 66 | movie / cast-map frame slots swapped; ~MovieBase called at one pad |
| 0x00DEE2E0 NScript::CQ_SecretPassageScript::CMotherTorture::Main | DIFFER(3134v3102), 313 | frame one dword short (CTimer at 0x14 v 0x18) and its knock-ons |
| 0x00F0A960 NScript::CQ_AmbushTradersScript::CAT_BanditContact::Main | DIFFER(2142v1985), 181 | CIsThingAlive / by-value CScriptThing copy blocker |

- **Names:** all PDB names; no fixes. **Headers:** the CQ_AmbushTradersScript, CQ_EndGameBossBattleScript (CTheRealGuildmaster) and
  NScript_CQ_SecretPassageScript opt-ins serve only parked rows; they stay in `pending/2026-11-07_linux_hand838/hdr/` (with its bcheck.py in tools/).
- **Also at this landing:** 0x00EC4A00 relabelled CRTFE_RockTroll::Main (was a placeholder) and cut into chunk 842.

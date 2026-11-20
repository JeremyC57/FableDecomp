# Landing nf162 (Account A, 2026-11-20): NScript quest script names, CQ_/CV_ A-M (25 rows)

Agent NF162 (round 10, finished in a resume session after a container restart) handed back 26 entries (21 done, 2 done-already,
1 rejected, 2 blocked); its files are in `naming/2026-11-20_agentNF162/`.

- 25 rows, no header changes (every opt-in the rows use already exists).
- Self renames (19, RENAMES section 1), among them CV_ChapelOfEvilScript, CV_AmbushScamScript, CV_FishingCompetitionScript,
  CQ_FireHeartScript::CFH_Prisoner and CV_TempleOfLightScript members. Also the four CPersistContext_TransferableComponent<T>::TransferOut
  rows (0x00E9E400, 0x00E38F60, 0x00EB6730, 0x00EB68E0, exact FableWin.pdb symbols) and Transfer<BriarRoseOpinion> 0x00E9E270
  (Ego_r + FableWin spelling; its only binder 0x00E9E110 already used it, which removes a split).
- Landed together as required: 0x00E9E270 with 0x00E9E400 (its object defines the vtable naming that TransferOut), 0x00E6A7F0
  with 0x00E6A9E0, 0x00E7F7D0 with 0x00E7F730, 0x00E7F410 with 0x00E80F10.
- 0x009E2560: four rows moved from the stand-in TransferOut_009e2560 to CPersistTraitsSLONG::TransferOut, shrinking a split.
  CV_MazeResearchScript (existing header-less local class) moved onto the shared CScriptBase in 0x00EA8130.
- Recheck (`logs/recheck.log`): 25 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 77 good, 0 bad. tablecheck: 0x00EDC5A0 is now recorded once, under the PDB spelling (0x00EDC450 dropped its stand-in);
  nine addresses shrink.
- Link (`logs/link_landing.log`): 4 LNK2005 against 9 at HEAD (`logs/link_control_head.log`). The CScriptThing and
  CBasicStringChar clashes are at HEAD too. This landing removes the TransferOut_009e2560 (x3), DoMission and
  AssignMoralityEtcForEvilEnding clashes.
- Queued: the engine/CSpawnedFunc1.h include fix (five landed rows fail to compile at HEAD), the CSpawnedFunc1 `Add` stand-ins
  (q6380) and 0x00E6AAC0 (q6274), 0x00E9E350's port (NAME_FIXES).

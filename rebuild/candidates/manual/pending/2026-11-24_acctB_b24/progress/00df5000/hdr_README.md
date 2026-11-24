# 0x00DF5000 overlay (B24S2)
- engine/CQ_TentacleKrakenBossFightScript.h: NEW header (no repo header exists), copied whole from
  rebuild/candidates/manual/pending/2026-11-08_linux_hand853/hdr/engine/ (Ego_r.pdb layout: MissionSucceeded +0x48 / MissionFailed +0x49 /
  KrakenAutoSaveCheckPoint +0x4a / TheKraken +0x4c, 0x58 bytes). FableWin.pdb also lists WatchForTermination 0x038A9C70 and EndMission
  0x038AB910 for the class, not declared in this header (not used by the row).

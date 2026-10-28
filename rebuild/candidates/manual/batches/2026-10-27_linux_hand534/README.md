# Chunk 534 (agent534, 2026-10-28): 4 of 12 re-queued large functions landed (527-920 bytes)

Last re-queue chunk: 12 rows of 527-920 bytes (0x00A3A410..0x00E12330). One Opus 5.5 agent at high effort (finished
before a container restart; landed after it).

- **Landed:** 4 rows with `vland.py --land`, all RELOCATION_MATCH, tests PASS under g++ and VC7.1: 0x00A3B9D0
  CVideoSys::Init (/Zc:wchar_t), 0x00A68370 CVolumeTexture::Initialise, 0x00DF9B80
  CQ_TraderConflictEvilScript::UpdateLiveEnemies, 0x00E12330 CQ_WaspBossScript::WatchForCutscene.
- **Parked (5, differ/):** 0x00A614B0 `_Rb_tree<CWideString,...CChecksumCacheFile::CEntry>::insert_unique` (one register),
  0x00A68690 InitialiseSlicesFromTextureList (frame overlap), 0x00A868C0 CIPhysicsFacesSegmentBase::Calculate (VC inlines
  all three push_backs, retail one), 0x00AB1CC0 C3DAnimationBuilder::BuildBoneInfoTree (tag temporary slots),
  0x00B61DD0 CEngineSkyRenderer::BuildCloudMesh.
- **Skipped (3):** 0x00A3A410 CWADBank::AddFile and 0x00A3EF00 the CSoundManagerLH ctor (not attempted, budget);
  0x00C400D0 over-spans past its ret into the jump table and index bytes (catalog needs a re-split).

Name fixes (NAME_FIXES.tsv, 34 rows, deferred to the naming pass). Lessons in LESSONS.md (non-POD hidden-return in ecx for
a `__fastcall` static, ternary CCountedPointer initialiser, same-TU real STLport helper bodies).

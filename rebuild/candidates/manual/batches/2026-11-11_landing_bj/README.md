# 2026-11-11 landing bj -- chunk 1024: 5 of 8 fresh rows

Source: `pending/2026-11-11_linux_hand1024/` (NOTES, STATUS, LESSONS, LABELS, NAME_FIXES, SPLITS, ATTEMPTS, differ/). vland --land: 5 wins
(RELOCATION_MATCH), tests PASS. Each matched on its first official grade.
- 0x00966DA0 CAIStateGroup_TavernServer::GetServingPoint (signed long GFRandom index); 0x00967D40 CAIStateGroup_TavernServer::Update (vtable
  slot 12; same-TU WanderWithinTavern / FreeServingPoint / WalkToBar / WalkToServingPoint bodies).
- 0x006B3B80 CEnvironment::LightingFlash (catalog placeholder CEnvironment_UpdateThunder).
- 0x006E2ED0 CTCTalk::GetCurrentLipSyncFrameData (catalog placeholder CViewBase_ProcessLipSync).
- 0x007CD390 CTCMultiStrike::CTCMultiStrike, 796 bytes (catalog 832 split; BOUNDS_FIX): a visible same-TU GetEffectFromString body; an
  explicit `case 0` beside an identical `default` gives retail's jump table starting at 0.
Coordinator fix at landing: the 0x006B3B80 test's `g_emitter` renamed `g_soundEmitterThing` (no identifier may contain `_emit`).
Parked as structural drafts, never graded (header work first; NOTES.md): 0x0080CD40 CTCDExplosion::UpdateHitThings, 0x006CBAC0
CTCCreatureNavigation::GetNextNavPosOnRoute (creature slots 79-86, navigator slots 3-5), 0x006CBE80 GetNearestFleePositionBehindCover.
Headers: base da1e0a174, clean merge, hdrcheck2 default=same; new CTCTavernTable, CTCTavernJug, CLipSyncSampleData, CActiveSoundInfo,
CSpecialAbilitiesMultiStrikeDef, CTCDParticleEmitter (some enumerator / member names descriptive, marked). Queued land1011bj: name fixes
(0x0065D55F IS_THING_ALIVE folds several pointer instances -- pick one name; 0x006A4DD0 label wrong, unproven).

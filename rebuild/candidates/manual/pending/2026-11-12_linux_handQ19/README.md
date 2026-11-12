# Chunk Q19 (Q18's 14 parked rows, Opus 5.5)

5 of 14 rows matched and landed in landing di, all newly ported onto the shared headers:
- 0x0065C5BB CTCVillage::UpdateMirroredCrimeData (`#pragma optimize("y",off)` + `("s",on)`, PlayersCrimes typed std::set<long>)
- 0x0079B6F0 CTCSummonSpell::ResetMinionPos (new C3DVector GFFASTONEOVERSQRT_NEWTON_SQUARE_PAREN)
- 0x008C4FD0 CCreatureAction_ScorpionSummon::CreateBabies (noinline GetDef<CCreatureGenerationFamilyDef>, do-while creature loop)
- 0x008CDD40 CCreatureAction_ReleaseTimeSpell::FinishAction (real class, named `CCharString animation` local)
- 0x008E16A0 NCreatureMode::CModeScreamerSuperAttack::FrameUpdate (CVECTORMAP_INSERT_FORCEINLINE)

9 are in differ/ with the residue on line 2:
- 0x008780D0 and 0x006FE7C0 (680v680): the same template body with one reload-order residue. Five c2prio-probed spellings kept
  the same split-piece order.
- 0x00837220 (716v716): three scheduling residues; every Newton spelling puts `mov edx,17Ch` one tuple late or one early.
- 6 not ported (00A78B20, 00540320, 00649E8B, 006C1250, 007BD845, 0096CD70): chunk Q20 takes them.

Records: STATUS.tsv, NOTES.md, LESSONS.md, LABELS.tsv, NAME_FIXES.tsv, ATTEMPTS.log, hdr/README.md.

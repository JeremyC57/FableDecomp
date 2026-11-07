# Chunk 833 notes

Result: 1 of 6 matched.
- Matched: 0x00E19D80 is RELOCATION_MATCH + PASS (vland, read-only).
- Parked: 5 rows in differ/, each with a g++-PASS state-model test.

All six row names are Ego_r.pdb spellings, proven by the entity vtable slot read from retail .rdata plus the PDB symbol. No descriptive
names were introduced: every callee spelling in the sources is a reloc_bindings.tsv / PDB / header-bound name.

Common environment for every row: FABLE_RETAIL_NSCRIPT / CLEVELSCRIPTBASE / BASE_OBJECT / CCHARSTRING, plus the row's header opt-in.

Shared spellings and evidence:
- Predicates:
  - CIsThingConscious (vtable 0x012C2FE8) and CIsThingAlive (0x012C32F0) use a row-local inline ctor / dtor.
  - The Thing copy calls 0x004ABE90 and the dtor is 0x004AA840.
  - SetPredicate is 0x00F35B10 and IsActiveThreadTerminating 0x00F35B30.
- STB (CScriptGameResourceObjectScriptedThingBase):

  | Function | Address |
  |---|---|
  | dtor | 0x007E74D0 |
  | IsValid | 0x00CD23B9 |
  | Release | 0x00CD2770 |
  | GetScriptThing | 0x007E7490 |
  | Speak | 0x007E7390 |
  | IsPerformingScriptTask | 0x007E7450 |
  | PlayAnimation | 0x007E73D0 |
  | ClearAllActions | 0x007E7400 |
  | MoveToPosition | 0x007E72F0 |
  | implicit operator= | CBaseObject::operator= 0x0099A3B0 + the CCountedPointer share (COMDAT 0x008ABD10 when not expanded) |

- MovieBase: ctor COMDAT 0x006E7B60, dtor COMDAT 0x006E7B80, PImp dtor spec 0x006E7AB0.
- Cast map std::map<CCharString, STB> (FABLE_RETAIL_STLP_MAP): operator[] 0x00CD3D2E, dtor 0x00CDBFB0. RunCutsceneMacro is
  0x00CBFB7D (__fastcall, engine/fable_script_cutscene.h).
- GameScriptInterface slots used:

  | Slot | Function |
  |---|---|
  | 284 | IsQuestActive |
  | 361 | ClearThingHasInformation |
  | 371 | FixMovieSequenceCamera |
  | 421 | IsCameraPosOnScreen |
  | 465 | EntityAttachToScript |
  | 526 | EntitySetAsToAddToComboMultiplierWhenHit |
  | 527 | EntitySetAsToAddToStatChangesWhenHit |
  | 529 | EntitySetAsUseMovementInActions |
  | 844 | SetIsPushableByHero (by value) |

  These are all from the header vtable order, and each one checked against the retail call displacement.
- CScriptThing slots: 7 GetHomePos, 21 MsgIsHitBy, 27 MsgIsTalkedToBy, 41 MsgIsHitBySpecialAbilityFrom,
  42 MsgIsHitByAnySpecialAbilityFrom, 67 SetFriendsWithEverythingFlag.

## 0x00E19D80 NScript::CQ_WhiteBalverineWWScript::CKG_Chief::Main: RELOCATION_MATCH (src/00e19d80.cpp)
Shape, in order:
1. Predicate CIsThingConscious(Me), then NSF / T.
2. Make him unkillable; combo / stat counting off; SetFriendsWithEverythingFlag(true).
3. Take scripted control of seh_me (Release / Start loop), then T, then EntityAttachToScript(Me, GetActiveQuestName()).
4. Loop `while (!ParentClass.MissionSucceeded) { NSF; T; ... }`. Talk branch:
   - Balverine dead: seh_hero, then the cast map in ONE `do { } while (0);` (HERO, CHIEF), then movie + pause, FixMovieSequenceCamera(true),
     RunCutsceneMacro("CS_WBK_CHIEF4", map, 0, 0, false, true), FixMovieSequenceCamera(false), MissionSucceeded = true.
   - Otherwise a paused speech movie: GONE_FIRST the first time (then `InitialChatted = false`: retail writes ebx = 0 at 0x00E1A208, a script
     quirk kept as is), GONE_SECOND after that.
5. Hit branch (no heal-life spell): BEEN_ATTACKED, then NSF / T / SetFriends(true).
6. After the loop: T, then ClearThingHasInformation(Me).

Strings read from retail:

| String | Address |
|---|---|
| HERO | 0x01255174 |
| CHIEF | 0x012CB38C |
| CS_WBK_CHIEF4 | 0x012E0F44 |
| GONE_FIRST | 0x012E0F1C |
| GONE_SECOND | 0x012E0EF0 |
| BEEN_ATTACKED | 0x012E0BB4 |
| SCRIPT_NAME_HERO | 0x0125D1C8 |
| "" | 0x0122D70E |

Lever: a single-site heat do-while (line-3 `// Inline-split:` note). Without it both operator= stay COMDAT calls (a1, 292 lines).

Relocations: all agree with reloc_bindings.tsv (bin/bindchk.py).
- The vland RELOC_NAME warning on 0x00CDBFB0 is a label problem (see LABELS.tsv), not a binding problem.

## 0x00E17840 NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main: DIFFER(2501v2515), 97 lines (differ/00e17840.cpp)
Shape:
- Predicate CIsThingAlive(Me), then NSF / T.
- Combo / stat counting off, not pushable, unkillable, no information marker.
- `while (!T)` loop:
  - Talk: a paused speech movie with the line for BalverineState: 2 IN_FIRST_ATTACK, 3 AFTER_FIRST, 4 IN_SECOND, 6 IN_THIRD, and 7
    AFTER_THIRD only while Q_WhiteBalverineWW is not active.
  - Hit (no heal-life spell) with state != 7: BEEN_ATTACKED, then SetFriendsWithEverythingFlag(true).

Strings:

| String | Address |
|---|---|
| IN_FIRST_ATTACK | 0x012E0C98 |
| AFTER_FIRST_ATTACK | 0x012E0C68 |
| IN_SECOND_ATTACK | 0x012E0C38 |
| IN_THIRD_ATTACK | 0x012E0C08 |
| AFTER_THIRD_ATTACK | 0x012E0BD8 |
| Q_WhiteBalverineWW | 0x012E0A3C |

Residues; none has a plain-C++ lever found in 12 grades:
1. Mixed MovieBase ctor. The talk movie ctor is expanded (vtable 0x01260EF4) and the hit movie calls 0x006E7B60. This is the chunk 823
   blocker.
2. CScriptThing copy split. The predicate's Thing copy calls 0x004ABE90, while the by-value SetIsPushableByHero argument copy is
   expanded:
   - a declared-only copy calls both;
   - an inline / implicit copy expands both;
   - definition order makes no difference (a9 / a10, new).
3. The CWideScreenMagicPauseEntities dtor (slot 0x5EC) is duplicated per return pad, and seh_me's dtor is expanded only at the natural
   loop exit. A row-local inline STB dtor (a11, new) does not split it that way.

Parked per the documented blockers, plus new attempts a9-a11.

## 0x00DB1F00 NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWoman::Main: DIFFER(2858v2714), 135 lines (differ/00db1f00.cpp)
Shape:
1. Predicate CIsThingConscious, then take scripted control.
2. AffairWife / AffairMan = GetThingWithScriptName("NOVI_AffairWife" 0x012D82E8 / "NOVI_AffairMan" 0x012D830C).
3. Main loop, until IsDistanceBetweenThingsUnder(Me, AffairWife, 5) (0x00CBE2FF __fastcall):
   - walk home while IsDistanceFromThingToPositionOver(Me, home, 0.1) (0x00CBE45C __fastcall);
   - on a hit: ON_HIT 0x012D8C78 + AddBadDeed(2) (0x00DAEA70);
   - on a talk: BUSY 0x012D8C58, with the man turned to the hero and back;
   - TalkingToWoman is set around both;
   - RECEIVE_KISS 0x012D8C48 / RECEIVE_HUG 0x012D8C3C when she is idle.
4. Run to AffairWomanRunOffPoint 0x012D8C24, wait until off screen, then RemoveThing.

Residue:
- Per-exit ~CScriptThing split. Retail calls 0x004AA840 at every return pad. With the row-local inline dtor, ours expands AffairWife /
  AffairMan at the walk-home wait-loop return.
- The predicate is homed at [esp+0x6c], against retail's 0x60 with the vtable in edi.

10 grades.

## 0x00D17F20 NScript::CQ_BountyHuntScript::CBH_GangDeputy1::Main: DIFFER(3035v3068), 78 lines (differ/00d17f20.cpp)
Shape: this chunk's differ test header.
- Spawns WatchForDeputy1GruntsKilled 0x00D18B20 through CSpawnedFunc +0x34 (new header opt-in member).
- The grunt spawner is the landed SpawnBanditGangGrunts 0x00D17470.

Residue: both MovieBase dtor sites call 0x006E7B80 where retail expands them. The same source is RELOCATION_MATCH under the diagnostic
/d2inlS15, so the residue is the caller-size inline budget. This is a park per the brief: no /d2 flag in a row. 19 grades.

## 0x00D19BD0 NScript::CQ_BountyHuntScript::CBH_Hostage1::Main: DIFFER(2807v2918), 188 lines (differ/00d19bd0.cpp)
Residue: the implicit STB operator= is called at 5 cast-map sites, and the MovieBase dtor is called. Only a row-local `__forceinline`
operator= plus /d2inlS15 matches (d3), and both are ruled out (owner 2026-11-07, brief). The WW do-while lever does nothing at 2.9 KB.
11 grades.

## 0x00D1A740 NScript::CQ_BountyHuntScript::CBH_Hostage2::Main: DIFFER(2662v2743), 167 lines (differ/00d1a740.cpp)
The parked source is plain. Same residue class as Hostage1. With the forbidden forceinline operator= and /d2inlS15, only 4 lines remain:
retail keeps a post-loop T test at 0x00D1AA9F after the GreatwoodLake / FishComplete wait. 14 grades.

## Labels to correct (LABELS.tsv)
- 0x00CDBFB0 should be `std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map`. labels_manifest_sync_2026-11-03 put back
  the wrong `CRetailTree_erase00cd344a::~CRetailTree_erase00cd344a`, which chunk 584 and naming wave 1 had already corrected.
- 0x00E15500 should be `NScript::CQ_WhiteBalverineKnotholeGladeScript::WatchForTermination` (PDB, header and landed 0x00E13F10 binding). It
  is currently the autoname `_global::CScriptGameResourceObjectScriptedThingBase_HandleWhiteBalverineQuestObjective`.

## Header debt
None required. All four headers carry opt-in blocks only (hdr/README.md).
- The BountyHunt and KG CKG_Chief opt-ins also serve the parked rows, so they can land with WW or wait.
- No change to engine/CScriptThing.h. The copy-split residue would need a per-site choice, not a header switch.

## Owner-rule notes
- Hostage1 / Hostage2 / GangDeputy1 match only with the diagnostic /d2inlS15 (and, for the Hostages, a forceinline operator=). They are
  parked, and no /d2 flag or forceinline helper is left in any source.
- The only lever in the landed row is a single-site `do { } while (0)` with its `// Inline-split:` note.

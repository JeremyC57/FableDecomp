# Row-by-row final selections

Status and selection are hash-bound. Byte lengths are lengths, not the number of equal bytes.

## 00c60ff0 — RELOCATION_MATCH

- Exact Leaf: `?Set3DBuffer@CEAXBuffer@@AAEHKPAXK@Z`.
- Selected attempt: 11; total grades: 11; distinct source families reported: .
- Built/retail length: 1417/1417. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: bytes and every named/internal relocation checked; original SDK identities corrected
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00c60ff0/PROBES.md and NAMING_EVIDENCE.md.

## 00490be0 — PARKED

- Exact Leaf: `?GetGraphicFromEmbeddedTag@CSubtitleRenderer@@ABEJABVCWideString@@@Z`.
- Selected attempt: 27; total grades: 27; distinct source families reported: 15.
- Built/retail length: 1306/1646. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1306/1646: automatic inlining expands only a prefix of28 string comparisons; remaining original operator calls and repeated cleanup exits differ.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00490be0/PROBES.md and NAMING_EVIDENCE.md.

## 00b3be10 — RELOCATION_MATCH

- Exact Leaf: `?BuildAlphaStippleTexture@CEngineResourceManager@@AAEXJAAV?$CArray@E@@@Z`.
- Selected attempt: 19; total grades: 19; distinct source families reported: .
- Built/retail length: 1684/1684. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00b3be10/PROBES.md and NAMING_EVIDENCE.md.

## 00f29530 — RELOCATION_MATCH

- Exact Leaf: `??1CScriptDef@@UAE@XZ`.
- Selected attempt: 6; total grades: 6; distinct source families reported: 1.
- Built/retail length: 1756/1756. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; worker masked match and named operands verified; independent main review pending
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00f29530/PROBES.md and NAMING_EVIDENCE.md.

## 00a1c010 — PARKED

- Exact Leaf: `?CullWayPoints@CNavigatorAStar@@AAEXJ@Z`.
- Selected attempt: 71; total grades: 71; distinct source families reported: 20.
- Built/retail length: 1773/1773. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Four x87 product instructions reverse potential-point XY squared-length order; four unmasked byte differences
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00a1c010/PROBES.md and NAMING_EVIDENCE.md.

## 00d321c0 — PARKED

- Exact Leaf: `?JackOfBladesBossFightCutscene@CQ_EndGameBossBattleScript@NScript@@QAEXXZ`.
- Selected attempt: 19; total grades: 19; distinct source families reported: 13.
- Built/retail length: 1122/1136. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Frame0x60 correct. Retail inlines scripted-handle assignment into base assignment plus counted-pointer assignment; candidate keeps outer assignment call.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00d321c0/PROBES.md and NAMING_EVIDENCE.md.

## 00b2fd60 — PARKED

- Exact Leaf: `?SetupGamut@CEngineCamera@@AAEXXZ`.
- Selected attempt: 28; total grades: 28; distinct source families reported: 17.
- Built/retail length: 1796/1812. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Initial far-Z lifetime and redundant Z-store elimination; scratch-register scheduling
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00b2fd60/PROBES.md and NAMING_EVIDENCE.md.

## 00ae0710 — RELOCATION_MATCH

- Exact Leaf: `?CalculateAveragePatchSize@CParticleProgram@@AAEXABVCParticleSimulation@@@Z`.
- Selected attempt: 18; total grades: 18; distinct source families reported: .
- Built/retail length: 1863/1863. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: See the main review and naming evidence.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ae0710/PROBES.md and NAMING_EVIDENCE.md.

## 0092f310 — PARKED

- Exact Leaf: `?GetGossipPhrase@CAIStateGroup_MinionReactToPlayerInteraction@@QAE_NAAK@Z`.
- Selected attempt: 26; total grades: 26; distinct source families reported: 17.
- Built/retail length: 1929/1945. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1929/1945 with exact58 frame; temporary/key slots, typed PeekTC/GetDialogueVoice automatic inlining and cleanup ordering differ.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/0092f310/PROBES.md and NAMING_EVIDENCE.md.

## 00c66c90 — RELOCATION_MATCH

- Exact Leaf: `?GetEAX4Source@CEAXBuffer@@QAEHKPAXKPAK@Z`.
- Selected attempt: 10; total grades: 10; distinct source families reported: .
- Built/retail length: 1944/1944. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: bytes and every named/internal relocation checked; original SDK identities corrected
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00c66c90/PROBES.md and NAMING_EVIDENCE.md.

## 00a86110 — PARKED

- Exact Leaf: `?CalculateDividingPlane@CIPhysicsFacesSegmentBase@@KI?AVCPlane@@ABV?$CArray@VC3DVector@@@@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@G@@J@Z`.
- Selected attempt: 34; total grades: 34; distinct source families reported: 17.
- Built/retail length: 1967/1967. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1967/1967 bytes; only final pop ebx and first x87 discard are interchanged, three byte positions differ at+0x7a1..+0x7a3.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00a86110/PROBES.md and NAMING_EVIDENCE.md.

## 00775350 — PARKED

- Exact Leaf: `?UpdateStock@CTCShop@@QAEXXZ`.
- Selected attempt: 20; total grades: 20; distinct source families reported: 13.
- Built/retail length: 2004/1988. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Two inline Drift sites have direction-first instead of snap-distance-first comparisons and different scalar spill slots; original 63-byte Drift source instead inlines all five sites (2164 versus1988). Selected direction-first version naturally gets retail two-inline/three-call split and frame0x54 but is2004 versus1988. No prohibited control used.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00775350/PROBES.md and NAMING_EVIDENCE.md.

## 00417a58 — PARKED

- Exact Leaf: `?InitSound@CMainGameComponent@@IAEXXZ`.
- Selected attempt: 39; total grades: 39; distinct source families reported: 13.
- Built/retail length: 1960/2097. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Frame7c recovered;4 CWideString comparisons remain called instead of native null/wcscmp/STL expansion; dependent register and temporary slots differ
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00417a58/PROBES.md and NAMING_EVIDENCE.md.

## 00821010 — RELOCATION_MATCH

- Exact Leaf: `?LoadBrushesFromFile@CMap@@QAEXAAVCAFile@@@Z`.
- Selected attempt: 11; total grades: 11; distinct source families reported: 2.
- Built/retail length: 2101/2101. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: complete2101-byte function and all40 operands independently audited.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00821010/PROBES.md and NAMING_EVIDENCE.md.

## 00ca2bb0 — RELOCATION_MATCH

- Exact Leaf: `@floor1_encode@16`.
- Selected attempt: 6; total grades: 6; distinct source families reported: .
- Built/retail length: 1440/1440. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; instruction bytes, named relocations and internal case table checked
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ca2bb0/PROBES.md and NAMING_EVIDENCE.md.

## 006932b0 — RELOCATION_MATCH

- Exact Leaf: `?RegisterActionEventTypes@CCreatureActionBase@@SIXXZ`.
- Selected attempt: 6; total grades: 6; distinct source families reported: .
- Built/retail length: 2176/2176. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; all relocated operands individually audited
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/006932b0/PROBES.md and NAMING_EVIDENCE.md.

## 00cb48a0 — RELOCATION_MATCH

- Exact Leaf: `?ImeUi_ProcessMessage@@YIJPAUHWND__@@IIJPA_N@Z`.
- Selected attempt: 14; total grades: 14; distinct source families reported: .
- Built/retail length: 2160/2160. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None in selected leaf: all bytes and137 named/internal relocation operands checked
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00cb48a0/PROBES.md and NAMING_EVIDENCE.md.

## 00cb38e0 — RELOCATION_MATCH

- Exact Leaf: `?ImeUi_RenderUI@@YIX_N0@Z`.
- Selected attempt: 10; total grades: 10; distinct source families reported: .
- Built/retail length: 2644/2644. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None in selected leaf: all bytes and158 named/internal relocation operands checked
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00cb38e0/PROBES.md and NAMING_EVIDENCE.md.

## 0043a570 — PARKED

- Exact Leaf: `??1CPlayerGui@@UAE@XZ`.
- Selected attempt: 27; total grades: 27; distinct source families reported: 15.
- Built/retail length: 2754/2770. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Primitive handle notification plus implicit release is 16 bytes short overall; native retains a second cleanup copy. Clear-based original-source hypothesis outlines the handle, yielding2706. The missing indirect-call block also rotates scratch registers through following counted-owner teardown.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/0043a570/PROBES.md and NAMING_EVIDENCE.md.

## 00df5000 — PARKED

- Exact Leaf: `?PhaseOne@CQ_TentacleKrakenBossFightScript@NScript@@QAEXXZ`.
- Selected attempt: 18; total grades: 18; distinct source families reported: 13.
- Built/retail length: 2969/3071. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Frame0x94 matches, but original loop-first Thing cleanup expands in retail and remains called with current genuine boundaries; other frame/reference-count/inlining differences remain.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00df5000/PROBES.md and NAMING_EVIDENCE.md.

## 00e8a2b0 — RELOCATION_MATCH

- Exact Leaf: `?FightWarmupRounds@CTyler@CV_FisticuffsClubScript@NScript@@AAEXXZ`.
- Selected attempt: 14; total grades: 14; distinct source families reported: 5.
- Built/retail length: 3263/3263. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; fresh exact target bytes and every relocation equal prior independently verified main receipt; zero active forced-inline attributes; updated dual tests PASS. Renewed main review pending.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e8a2b0/PROBES.md and NAMING_EVIDENCE.md.

## 00a9bbe0 — RELOCATION_MATCH

- Exact Leaf: `??0C3DMeshFileLoader@@QAE@XZ`.
- Selected attempt: 8; total grades: 8; distinct source families reported: .
- Built/retail length: 3281/3281. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; all relocation operands, support helpers and emitted vtables audited
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00a9bbe0/PROBES.md and NAMING_EVIDENCE.md.

## 00402510 — RELOCATION_MATCH

- Exact Leaf: `?GFMain@@YIJPAUHINSTANCE__@@PADH@Z`.
- Selected attempt: 29; total grades: 29; distinct source families reported: .
- Built/retail length: 3952/3952. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None in bytes/operands; eight explicitly descriptive native-only names remain under README exception
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00402510/PROBES.md and NAMING_EVIDENCE.md.

## 00c01750 — RELOCATION_MATCH

- Exact Leaf: `_deflate_slow`.
- Selected attempt: 3; total grades: 3; distinct source families reported: .
- Built/retail length: 1033/1033. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; all instruction bytes and named/internal relocations audited
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00c01750/PROBES.md and NAMING_EVIDENCE.md.

## 00ca4160 — RELOCATION_MATCH

- Exact Leaf: `@vorbis_lpc_to_lsp@12`.
- Selected attempt: 3; total grades: 3; distinct source families reported: .
- Built/retail length: 1028/1028. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; instruction bytes, named relocations and internal case table checked
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ca4160/PROBES.md and NAMING_EVIDENCE.md.

## 00b2d9f0 — BYTE_MATCH_NAME_REVIEW

- Exact Leaf: `?UpdateDiskCache@CEngineSurfaceCompositionManager@@QAEXABV?$CArray@VCEngineTextureMorph@@@@_NMPBV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@@Z`.
- Selected attempt: 41; total grades: 41; distinct source families reported: 19.
- Built/retail length: 807/807. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: No byte residue. Exact const std::_Rb_tree::find member-template versus non-template mangling remains conditional on unretained original STLport extension configuration. This is a naming hold, not an accepted match.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00b2d9f0/PROBES.md and NAMING_EVIDENCE.md.

## 00ae0070 — RELOCATION_MATCH

- Exact Leaf: `?ApplyTransform@CParticleProgram@@QAEXABVCMatrix3x4@@@Z`.
- Selected attempt: 137; total grades: 137; distinct source families reported: 17.
- Built/retail length: 1090/1090. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: all1090 bytes equal after4 separately verified relocation operands.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ae0070/PROBES.md and NAMING_EVIDENCE.md.

## 00defb10 — RELOCATION_MATCH

- Exact Leaf: `?SwitchDoorsOnAndOff@CQ_SecretPassageScript@NScript@@QAEXXZ`.
- Selected attempt: 8; total grades: 8; distinct source families reported: 2.
- Built/retail length: 1094/1094. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; fresh exact target bytes and every relocation equal prior independently verified main receipt; zero active forced-inline attributes; updated dual tests PASS. Renewed main review pending.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00defb10/PROBES.md and NAMING_EVIDENCE.md.

## 007602f0 — PARKED

- Exact Leaf: `?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z`.
- Selected attempt: 38; total grades: 38; distinct source families reported: 20.
- Built/retail length: 1125/1126. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1125 vs1126 bytes; correct0x54 frame and entire CFG, residual angle/copy scheduling and early [edi+0xc] first-component load before pointer advance.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/007602f0/PROBES.md and NAMING_EVIDENCE.md.

## 00e247f0 — RELOCATION_MATCH

- Exact Leaf: `?Init@CQR_EscortTraderScript@NScript@@UAEXXZ`.
- Selected attempt: 8; total grades: 8; distinct source families reported: 5.
- Built/retail length: 1073/1073. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: all1073 bytes match after relocation operands are masked; all75 operands independently audited.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e247f0/PROBES.md and NAMING_EVIDENCE.md.

## 00d3a4a0 — PARKED

- Exact Leaf: `?Main@CFH_Prisoner@CQ_FireHeartScript@NScript@@EAEXXZ`.
- Selected attempt: 18; total grades: 18; distinct source families reported: 13.
- Built/retail length: 1129/1140. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1129/1140; correct frame 0x28 and scoped IsValid receiver cache. Ordinary CSpawnedFunc constructor remains called rather than expanded; associated saved-register roles and tail cleanup differ. No forced-inline workaround.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00d3a4a0/PROBES.md and NAMING_EVIDENCE.md.

## 00ae2e20 — RELOCATION_MATCH

- Exact Leaf: `?OptimiseSpringOrder@CParticleProgram@@QBEXAAV?$CArray@VCParticleDistanceConstraint@@@@@Z`.
- Selected attempt: 14; total grades: 14; distinct source families reported: .
- Built/retail length: 1293/1293. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: all1293 bytes and20 relocated operands independently checked
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ae2e20/PROBES.md and NAMING_EVIDENCE.md.

## 00c9f8c0 — RELOCATION_MATCH

- Exact Leaf: `@mapping0_inverse@8`.
- Selected attempt: 3; total grades: 3; distinct source families reported: .
- Built/retail length: 1282/1282. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; instruction bytes, named relocations and internal case table checked
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00c9f8c0/PROBES.md and NAMING_EVIDENCE.md.

## 00e0a820 — RELOCATION_MATCH

- Exact Leaf: `?TurnToBalv@CQ_TraderEscortScript@NScript@@QAEXVCScriptThing@@@Z`.
- Selected attempt: 7; total grades: 7; distinct source families reported: 1.
- Built/retail length: 1304/1304. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; fresh exact target bytes and every relocation equal prior independently verified main receipt; zero active forced-inline attributes; updated dual tests PASS. Renewed main review pending.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e0a820/PROBES.md and NAMING_EVIDENCE.md.

## 00e9b690 — RELOCATION_MATCH

- Exact Leaf: `?DoMission@CV_HeroDuelScript@NScript@@QAEXXZ`.
- Selected attempt: 5; total grades: 5; distinct source families reported: 1.
- Built/retail length: 1319/1319. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; byte, naming, literal and dual behavior checks passed; independent main review pending
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e9b690/PROBES.md and NAMING_EVIDENCE.md.

## 00ea2720 — PARKED

- Exact Leaf: `?WatchForCutscene1@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ`.
- Selected attempt: 16; total grades: 16; distinct source families reported: 13.
- Built/retail length: 1112/1334. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1112/1334; frame 0x74 vs 0x78. Three scripted-handle assignments remain calls rather than fully expanded base/count-pointer ownership; their missing data spill also changes slots. Automatic expansion, ShareData forms, real ctor visibility and temporary scopes tried.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ea2720/PROBES.md and NAMING_EVIDENCE.md.

## 00ee3140 — PARKED

- Exact Leaf: `?Main@CTH_Whisper@CV_TravellingHeroesScript@NScript@@EAEXXZ`.
- Selected attempt: 18; total grades: 18; distinct source families reported: 13.
- Built/retail length: 1350/1366. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: External copy restores predicate boundary but native value-argument copy is inline; scripted-handle Release receiver reloads instead of ebx cache.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ee3140/PROBES.md and NAMING_EVIDENCE.md.

## 00507070 — PARKED

- Exact Leaf: `?BuildStaticMapGenerationVisibilityInfo@CWorldMap@@QAEXAAVCStaticMapGenerationVisibiilityInfo@@@Z`.
- Selected attempt: 23; total grades: 23; distinct source families reported: 16.
- Built/retail length: 1447/1432. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1447/1432 with correct aligned44 frame; callee-saved register roles, temporary slots, range-copy reloads and exit layout remain.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00507070/PROBES.md and NAMING_EVIDENCE.md.

## 00de4710 — PARKED

- Exact Leaf: `?DisappearFollowers@CQ_RansomVictimScript@NScript@@QAEXXZ`.
- Selected attempt: 19; total grades: 19; distinct source families reported: 13.
- Built/retail length: 1013/1428. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1013/1428: missing four full native inline vector-cleanup blocks; selected body routes all exits through genuine external vector destructors. Main body through native de4ad0 otherwise aligns except cleanup branch destinations and swapped loads at de49ff/de4a03. Retain alternate8 at1870 bytes with too many inline cleanup copies; 52 normalized diff lines is not a byte-match metric.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00de4710/PROBES.md and NAMING_EVIDENCE.md.

## 00da8bf0 — PARKED

- Exact Leaf: `?Main@CMinionSpawnAdv@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ`.
- Selected attempt: 34; total grades: 34; distinct source families reported: 16.
- Built/retail length: 1359/1447. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: First1323bytes/407instructions align except relocations and six exit-destination displacements. Native two final inline vector cleanup loops remain one shared out-of-line destructor; exact50byte vector helper proved. Main1359v1447, not matched.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00da8bf0/PROBES.md and NAMING_EVIDENCE.md.

## 00ec4a00 — PARKED

- Exact Leaf: `?Main@CRTFE_RockTroll@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ`.
- Selected attempt: 27; total grades: 27; distinct source families reported: 15.
- Built/retail length: 1476/1540. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Ordinary CSpawnedFunc1 constructor and pause constructor remain called where native expands; frame0x50 versus0x5c, mixed handle cleanup and register web. Native callee identities retained; no byte parity.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ec4a00/PROBES.md and NAMING_EVIDENCE.md.

## 00e71f10 — PARKED

- Exact Leaf: `?Main@CDF_DressAsBandit@CV_DemonDoorsScript@NScript@@EAEXXZ`.
- Selected attempt: 17; total grades: 17; distinct source families reported: 13.
- Built/retail length: 1604/1605. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Prefix and original0x68 frame match. Nested CWideScreenMagicPauseEntities ctor remains a call instead of native inline pause; trailing cleanup layout differs.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e71f10/PROBES.md and NAMING_EVIDENCE.md.

## 00e482f0 — PARKED

- Exact Leaf: `?CheckForLeaderDead@CV_BanditTollScript@NScript@@QAEXXZ`.
- Selected attempt: 22; total grades: 22; distinct source families reported: 13.
- Built/retail length: 1600/1627. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1600/1627; correct frame0x44 and improved asymmetric loop shape. First Thing assignment remains called instead of native inline; later IsAlive calls expand instead of native calls and inner wait receiver LEA scheduling differs. Named reference aliases remain source-shape inferences.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e482f0/PROBES.md and NAMING_EVIDENCE.md.

## 00eeaef0 — PARKED

- Exact Leaf: `?Main@CExpression_PickpocketScript@NScript@@UAEXXZ`.
- Selected attempt: 16; total grades: 16; distinct source families reported: 13.
- Built/retail length: 1835/1906. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Frame0x48 and opening match. Native one assignment-result temporary destructor inline while candidate calls all; SpottedTimer/PickpocketTimer slots swap.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00eeaef0/PROBES.md and NAMING_EVIDENCE.md.

## 00e63890 — RELOCATION_MATCH

- Exact Leaf: `?Main@CSpectator@CV_ChickenKickingScript@NScript@@EAEXXZ`.
- Selected attempt: 9; total grades: 9; distinct source families reported: 3.
- Built/retail length: 1925/1925. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None:1925 bytes match;101 operands checked, including the two corrected conversion-operator calls at0099e4c0.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e63890/PROBES.md and NAMING_EVIDENCE.md.

## 00ee4dd0 — PARKED

- Exact Leaf: `?Main@CTourGuideFollower@CV_TourGuideScript@NScript@@EAEXXZ`.
- Selected attempt: 15; total grades: 15; distinct source families reported: 13.
- Built/retail length: 1751/1946. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Correct0x54 frame and opening sequence; mixed inline/called CScriptThing destruction across loop exits remains, selected guarded-do shape.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ee4dd0/PROBES.md and NAMING_EVIDENCE.md.

## 009cf3a0 — RELOCATION_MATCH

- Exact Leaf: `?WriteSymbolHeaderFile@CBankFile@@QAEXXZ`.
- Selected attempt: 9; total grades: 9; distinct source families reported: 5.
- Built/retail length: 1991/1991. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None after relocation masking; all145 operands and2 virtual calls verified.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/009cf3a0/PROBES.md and NAMING_EVIDENCE.md.

## 00d0ee70 — PARKED

- Exact Leaf: `?CheckForFirstAreaMassacre@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ`.
- Selected attempt: 33; total grades: 33; distinct source families reported: 16.
- Built/retail length: 1838/1989. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Correct frame0x78, initial stack slots and hot-path prefix; native mixed vector destruction, two unsigned-vector destructor calls and cold return layout remain.1838/1989 is not a151-byte-only residue.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00d0ee70/PROBES.md and NAMING_EVIDENCE.md.

## 00ddd430 — PARKED

- Exact Leaf: `?Main@CPrisonRaceStartingGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ`.
- Selected attempt: 17; total grades: 17; distinct source families reported: 13.
- Built/retail length: 1900/2044. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Frame0x5c correct and prefix through cast-map initialization exact; two resource assignments are called instead of retail inline expansion.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ddd430/PROBES.md and NAMING_EVIDENCE.md.

## 00d07e50 — PARKED

- Exact Leaf: `?Main@CCampHostage@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ`.
- Selected attempt: 16; total grades: 16; distinct source families reported: 13.
- Built/retail length: 2016/2068. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Three late implicit MovieBase destructor expansions become calls; frame0x70 and other main-body instructions match.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00d07e50/PROBES.md and NAMING_EVIDENCE.md.

## 00677970 — PARKED

- Exact Leaf: `?TransferIn@?$CPersistTraits@VCReactionFrequencyTraitsArray@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCReactionFrequencyTraitsArray@@@Z`.
- Selected attempt: 36; total grades: 36; distinct source families reported: 17.
- Built/retail length: 1844/2272. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Frame0x1c versus0x18; three comparisons expand while fourteen CCharString comparisons remain out-of-line. No native out-of-line binding for the latter is proven.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00677970/PROBES.md and NAMING_EVIDENCE.md.

## 00d067a0 — PARKED

- Exact Leaf: `?Main@CBCGameMaster@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ`.
- Selected attempt: 15; total grades: 15; distinct source families reported: 13.
- Built/retail length: 2548/2628. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 2548/2628; correct frame0x38 and long exact prefix. MovieBase and CWideScreenMagicPauseEntities constructors remain called where native expands; their receiver/register and cold cleanup differences remain. Original ordinary constructor, body visibility, scoped temps and exit forms tried; no forcing.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00d067a0/PROBES.md and NAMING_EVIDENCE.md.

## 00daadd0 — PARKED

- Exact Leaf: `?Init@CQ_NewOakValeIntroScript@NScript@@UAEXXZ`.
- Selected attempt: 22; total grades: 22; distinct source families reported: 13.
- Built/retail length: 3406/3306. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Native expands22 then calls20 real _Construct helpers; object expands all42, +100 bytes. Smaller alias alternative3342 has more register/stream differences.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00daadd0/PROBES.md and NAMING_EVIDENCE.md.

## 00e5ebd0 — RELOCATION_MATCH

- Exact Leaf: `?MakeHenchmanComment@CV_BodyGuardScript@NScript@@QAE_NABVCCharString@@JW4EHENCHMAN_COMMENT@12@ABVCScriptThing@@@Z`.
- Selected attempt: 13; total grades: 13; distinct source families reported: 3.
- Built/retail length: 1064/1064. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; worker checks pass, independent main review pending.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e5ebd0/PROBES.md and NAMING_EVIDENCE.md.

## 00e5e5a0 — PARKED

- Exact Leaf: `?MakeHenchmanPerformIdleComment@CV_BodyGuardScript@NScript@@QAE_NXZ`.
- Selected attempt: 20; total grades: 20; distinct source families reported: 15.
- Built/retail length: 1590/1580. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Native expands script-thing dtor on bored/good exits but calls it on question/loop/evil paths. Compiler expands all and merges good/evil calls;1590 vs1580, correct frame0x2c.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e5e5a0/PROBES.md and NAMING_EVIDENCE.md.

## 007c0690 — PARKED

- Exact Leaf: `?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1PBV4@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z`.
- Selected attempt: 26; total grades: 26; distinct source families reported: 13.
- Built/retail length: 972/1088. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Ordinary typed component lookups remain outlined; direct original vector-field predicate restores the native nonzero expansion without special controls. Targeting-result copy and scratch registers still differ.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/007c0690/PROBES.md and NAMING_EVIDENCE.md.

## 007bfe30 — PARKED

- Exact Leaf: `?UpdateCollision@CTCShotDefinitionBase@NThingShot@@MAEXAA_N@Z`.
- Selected attempt: 24; total grades: 24; distinct source families reported: 14.
- Built/retail length: 1620/1648. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Ordinary typed GetTC and pause query change expansion, producing1620 versus1648 bytes; receiver/condition registers, frame slots and x87 scheduling differ. Prior equal-size candidate used unsupported new inline-control annotations and is rejected.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/007bfe30/PROBES.md and NAMING_EVIDENCE.md.

## 00c5ffb0 — RELOCATION_MATCH

- Exact Leaf: `?AddToChangesList@CEAXListener@@QAEXW4IDENTIFIER@@KPAXJ@Z`.
- Selected attempt: 8; total grades: 8; distinct source families reported: .
- Built/retail length: 1120/1120. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: bytes and every named/internal relocation checked; original SDK identities corrected
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00c5ffb0/PROBES.md and NAMING_EVIDENCE.md.

## 0057e2f1 — RELOCATION_MATCH

- Exact Leaf: `?OnSerialise@CTCHeroStats@@MAEXAAVCPersistContext@@_N@Z`.
- Selected attempt: 18; total grades: 18; distinct source families reported: 5.
- Built/retail length: 2373/2373. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: See the main review and naming evidence.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/0057e2f1/PROBES.md and NAMING_EVIDENCE.md.

## 00de6730 — PARKED

- Exact Leaf: `?PlayCutscene@CQ_RansomVictimScript@NScript@@QAEXVCCharString@@0@Z`.
- Selected attempt: 26; total grades: 26; distinct source families reported: 14.
- Built/retail length: 936/1252. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Five resource assignments remain outlined where retail expands them; corrected original case-insensitive NULL sentinel and actual _stricmp import. No byte parity claimed.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00de6730/PROBES.md and NAMING_EVIDENCE.md.

## 00e78100 — PARKED

- Exact Leaf: `?PlayCutscene@CV_ExposeMayorScript@NScript@@QAEXVCCharString@@_N0@Z`.
- Selected attempt: 21; total grades: 21; distinct source families reported: 14.
- Built/retail length: 1059/1323. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Four resource assignments remain outlined where retail expands them; corrected original case-insensitive NULL sentinel and actual _stricmp import. No byte parity claimed.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e78100/PROBES.md and NAMING_EVIDENCE.md.

## 00e76e90 — PARKED

- Exact Leaf: `?HeroFlashesLamp@CV_ExposeMayorScript@NScript@@QAEXJ@Z`.
- Selected attempt: 22; total grades: 22; distinct source families reported: 13.
- Built/retail length: 1590/1612. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1590 vs1612; early TimesLeft register caching, frame sharing and mixed inner timer expansion differ; recursion setting remains experimental
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e76e90/PROBES.md and NAMING_EVIDENCE.md.

## 00e7ab50 — PARKED

- Exact Leaf: `?PhaseFour@CV_ExposeMayorScript@NScript@@QAEXXZ`.
- Selected attempt: 22; total grades: 22; distinct source families reported: 16.
- Built/retail length: 1283/1275. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Two pause constructor calls remain instead of inline native calls, frame0x24 versus0x20, constant/register web differs. Own logic14976 cases passes GNU64 andVC7.1.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e7ab50/PROBES.md and NAMING_EVIDENCE.md.

## 009fde80 — PARKED

- Exact Leaf: `?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z`.
- Selected attempt: 27; total grades: 27; distinct source families reported: 15.
- Built/retail length: 1073/1265. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Ordinary inline leaves original TouchFrame/TouchResource calls outlined where retail expands them;1073 versus1265 bytes. Prior equal-size candidate used unsupported new inline-control annotations and is rejected.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/009fde80/PROBES.md and NAMING_EVIDENCE.md.

## 0051fd80 — BYTE_MATCH_NAME_REVIEW

- Exact Leaf: `?LoadSingleThing@CThingManager@@AAEPAVCThing@@AAVCPersistContext@@J@Z`.
- Selected attempt: 33; total grades: 33; distinct source families reported: 15.
- Built/retail length: 1320/1320. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: No byte residue. Two original STLport _M_find specializations need exact target-config identity adjudication; both current spellings use the original default member-template form.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/0051fd80/PROBES.md and NAMING_EVIDENCE.md.

## 00520d00 — BYTE_MATCH_NAME_REVIEW

- Exact Leaf: `?LoadEntitiesFromScript@CThingManager@@QAEXABVCCharString@@JJABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@PAVCASuspendableProcess@@JAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@4@@Z`.
- Selected attempt: 16; total grades: 16; distinct source families reported: 4.
- Built/retail length: 1338/1338. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None in relocated bytes. Private STLport _M_find member-template spelling matches official default4.6 configuration, but target-specific NO_EXTENSIONS configuration is unproven; see NAMING_EVIDENCE.md.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00520d00/PROBES.md and NAMING_EVIDENCE.md.

## 005253f0 — PARKED

- Exact Leaf: `?LoadGameState@CThingManager@@QAEXAAVCPersistContext@@@Z`.
- Selected attempt: 24; total grades: 24; distinct source families reported: 14.
- Built/retail length: 1314/2263. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Ordinary-inline Replace and ReadType remain out of line;1314 vs2263. Historical2276 source used unproven stronger inline attributes and is rejected. Genuine helper visibility and lifetime alternatives did not recover retail expansion.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/005253f0/PROBES.md and NAMING_EVIDENCE.md.

## 00522a20 — RELOCATION_MATCH

- Exact Leaf: `?InitThingTypes@CThingManager@@QAEXXZ`.
- Selected attempt: 15; total grades: 15; distinct source families reported: 2.
- Built/retail length: 2589/2589. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: See the main review and naming evidence.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00522a20/PROBES.md and NAMING_EVIDENCE.md.

## 008edff0 — RELOCATION_MATCH

- Exact Leaf: `??0CWillButtonHelperTimer@@QAE@W4EHeroAbility@@AAVCThing@@@Z`.
- Selected attempt: 23; total grades: 23; distinct source families reported: 7.
- Built/retail length: 1201/1201. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: See the main review and naming evidence.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/008edff0/PROBES.md and NAMING_EVIDENCE.md.

## 007677e0 — PARKED

- Exact Leaf: `?AttachAugmentationParticles@CTCObjectAugmentations@@AAEXXZ`.
- Selected attempt: 25; total grades: 25; distinct source families reported: 15.
- Built/retail length: 1462/1467. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Frame matches0xa0; cached zero in EBX versus native num/EBP finish caching causes multiple register/reload/block differences.1462/1467 bytes.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/007677e0/PROBES.md and NAMING_EVIDENCE.md.

## 0058e99f — RELOCATION_MATCH

- Exact Leaf: `?OpenInventoryShortcut@CTCInventory@@QAEXJ@Z`.
- Selected attempt: 14; total grades: 14; distinct source families reported: 1.
- Built/retail length: 1498/1498. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: See the main review and naming evidence.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/0058e99f/PROBES.md and NAMING_EVIDENCE.md.

## 0058e208 — PARKED

- Exact Leaf: `?ConstructItemEntryPC@CTCInventory@@QAEXJJPAVCList@NUISystem@@PAVCComponent@3@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z`.
- Selected attempt: 34; total grades: 34; distinct source families reported: 13.
- Built/retail length: 1942/1943. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1942/1943; frame0x70 agrees. Early assignment-icon receiver registers, cached menu receiver reload timing, def-index compare scheduling and later scalar-parameter setup differ. Distinct source strategies13; no byte-match claim.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/0058e208/PROBES.md and NAMING_EVIDENCE.md.

## 0058a763 — PARKED

- Exact Leaf: `?OpenPCStatusMenu@CTCInventory@@QAEXXZ`.
- Selected attempt: 23; total grades: 23; distinct source families reported: 10.
- Built/retail length: 1895/1965. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Ordinary-inline equality and typed GetTC remain calls, frame0x28 vs0x38. /O2 expands them but2210 vs1965. Original forced annotation not proven.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/0058a763/PROBES.md and NAMING_EVIDENCE.md.

## 0058f9bc — PARKED

- Exact Leaf: `?OpenPCPhotojournal@CTCInventory@@QAEXXZ`.
- Selected attempt: 27; total grades: 27; distinct source families reported: 13.
- Built/retail length: 2027/2012. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Frame0x2d0 vs0x2cc, EDI/EBX roles and temporary slot scheduling;2027 vs2012. Visible map operator[], both string destructors and CAction constructor did not close it.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/0058f9bc/PROBES.md and NAMING_EVIDENCE.md.

## 00590f0b — PARKED

- Exact Leaf: `?GetItemsListUIComponentPC@CTCInventory@@QAEPAVCComponent@NUISystem@@PAV23@@Z`.
- Selected attempt: 30; total grades: 30; distinct source families reported: 13.
- Built/retail length: 2894/2907. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Compliant direct destructor now expands naturally:2894/2907. Two escaped-handle Object clear stores remain absent; retaining them crosses VC7.1 automatic-inline threshold and restores2752. Saved-register roles, frame-slot ranking and final AddParam expansion still differ.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00590f0b/PROBES.md and NAMING_EVIDENCE.md.

## 0041d21b — RELOCATION_MATCH

- Exact Leaf: `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@J@Z`.
- Selected attempt: 27; total grades: 27; distinct source families reported: 9.
- Built/retail length: 1677/1677. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None after relocation masking. Jump-table destinations individually verified, not merely masked. Existing wrong ledger binding forCDraggableInto at52cc50 corrected in NAME_FIXES; no shared bindings changed.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/0041d21b/PROBES.md and NAMING_EVIDENCE.md.

## 00427216 — PARKED

- Exact Leaf: `?CreateTickSlider@CManager@NUISystem@@QAEPAVCComponent@2@ABVCWideString@@ABVC2DCoordI@@JJABVCRGBFloatColour@@MM_N32M@Z`.
- Selected attempt: 49; total grades: 49; distinct source families reported: 19.
- Built/retail length: 2983/2995. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Direct original component construction hypothesis: ghost colour return-temp copy is elided (12bytes), plus two state-key scheduling differences. Ordinary original conversion operator/getter variants remain outlined. Exact original call-site conversion spelling is not proven.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00427216/PROBES.md and NAMING_EVIDENCE.md.

## 009bc120 — RELOCATION_MATCH

- Exact Leaf: `?TransferVectorOfSubComponentsLoadText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableVectorOfSubComponentsGenericType@@@Z`.
- Selected attempt: 13; total grades: 13; distinct source families reported: 3.
- Built/retail length: 1639/1639. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: See the main review and naming evidence.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/009bc120/PROBES.md and NAMING_EVIDENCE.md.

## 00ac2be0 — RELOCATION_MATCH

- Exact Leaf: `?PartitionSpace@C3DMeshPartitionBuilderElement@@AAEJPBVC3DVertex2@@JM@Z`.
- Selected attempt: 34; total grades: 34; distinct source families reported: .
- Built/retail length: 2791/2791. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: complete2791-byte function relocation-matches and all33 operands are individually proven.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ac2be0/PROBES.md and NAMING_EVIDENCE.md.

## 00c66330 — RELOCATION_MATCH

- Exact Leaf: `?GetEAX4FXSlot@CEAXListener@@QAEHKKPAXKPAK@Z`.
- Selected attempt: 7; total grades: 7; distinct source families reported: .
- Built/retail length: 2008/2008. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: bytes and every named/internal relocation checked; original SDK identities corrected
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00c66330/PROBES.md and NAMING_EVIDENCE.md.

## 00ae8d50 — RELOCATION_MATCH

- Exact Leaf: `?Load@CParticleEmitterDatabase@NParticleEngine@@IAEXAAVCAFile@@ABVCEmitterDatabaseSerialiseInfo@2@@Z`.
- Selected attempt: 7; total grades: 7; distinct source families reported: 2.
- Built/retail length: 828/828. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: exact828 bytes and64 independently checked operands.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ae8d50/PROBES.md and NAMING_EVIDENCE.md.

## 00a68de0 — PARKED

- Exact Leaf: `?LoadFromDataStream@CVolumeTexture@@QAEXAAVCDataInputStream@@@Z`.
- Selected attempt: 32; total grades: 32; distinct source families reported: 15.
- Built/retail length: 691/919. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 691/919 under ordinary original CDataInputStream::Read: stream reads remain outlined instead of native expansion. Previous923/919 candidate relied on an unproved stronger annotation and is rejected; retained only in history. Original21-byte serialized layout proved; /Zp1 is reconstruction configuration, not recovered original build text.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00a68de0/PROBES.md and NAMING_EVIDENCE.md.

## 00642aa0 — PARKED

- Exact Leaf: `?PopulateMinimapThemeList@CMinimapData@@QAEXABVCWorldMap@@J@Z`.
- Selected attempt: 25; total grades: 25; distinct source families reported: 17.
- Built/retail length: 1344/1340. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: 1344/1340 with exact0x120 frame; scratch-register rotations, map-call scheduling, endpoint/loop slots and ray-copy ordering remain. Equal-size variants preserved.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00642aa0/PROBES.md and NAMING_EVIDENCE.md.

## 00ef71e0 — RELOCATION_MATCH

- Exact Leaf: `?Main@CArcherMan@CPersonalScript_EmptyScript@NScript@@EAEXXZ`.
- Selected attempt: 5; total grades: 5; distinct source families reported: 1.
- Built/retail length: 1051/1051. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; exact bytes after audited relocations
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ef71e0/PROBES.md and NAMING_EVIDENCE.md.

## 00ef80a0 — RELOCATION_MATCH

- Exact Leaf: `?Main@CChatter2@CPersonalScript_EmptyScript@NScript@@EAEXXZ`.
- Selected attempt: 5; total grades: 5; distinct source families reported: 1.
- Built/retail length: 1070/1070. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; exact bytes after audited relocations
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ef80a0/PROBES.md and NAMING_EVIDENCE.md.

## 00cf9610 — RELOCATION_MATCH

- Exact Leaf: `?OnPersist@CQ_AmbushTradersScript@NScript@@UAEXAAVCPersistContext@@@Z`.
- Selected attempt: 6; total grades: 6; distinct source families reported: 1.
- Built/retail length: 1084/1084. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; exact bytes after audited relocations
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00cf9610/PROBES.md and NAMING_EVIDENCE.md.

## 00d23dc0 — PARKED

- Exact Leaf: `?Main@CQ_BowerstoneTownLifeIntroScript@NScript@@UAEXXZ`.
- Selected attempt: 20; total grades: 20; distinct source families reported: 15.
- Built/retail length: 1076/1079. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: One second CScriptThing assignment remains outer wrapper call; retail expands add eax,4 and calls CCountedPointer assignment.1076vs1079. Other instruction stream and frame match.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00d23dc0/PROBES.md and NAMING_EVIDENCE.md.

## 00d13130 — RELOCATION_MATCH

- Exact Leaf: `?OnPersist@CQ_BountyHuntScript@NScript@@UAEXAAVCPersistContext@@@Z`.
- Selected attempt: 4; total grades: 4; distinct source families reported: 1.
- Built/retail length: 1097/1097. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; exact bytes after audited relocations
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00d13130/PROBES.md and NAMING_EVIDENCE.md.

## 00c71870 — RELOCATION_MATCH

- Exact Leaf: `?Update@CSalOpenALReverb@@UAEXPBVCCodaEnvironment@@@Z`.
- Selected attempt: 3; total grades: 3; distinct source families reported: .
- Built/retail length: 1115/1115. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; instruction bytes, named relocations and internal case table checked
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00c71870/PROBES.md and NAMING_EVIDENCE.md.

## 00ee42a0 — RELOCATION_MATCH

- Exact Leaf: `?Init@CV_TourGuideScript@NScript@@UAEXXZ`.
- Selected attempt: 4; total grades: 4; distinct source families reported: 1.
- Built/retail length: 1130/1130. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; exact bytes after audited relocations
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ee42a0/PROBES.md and NAMING_EVIDENCE.md.

## 00c5e650 — RELOCATION_MATCH

- Exact Leaf: `?CreateBuffer@CSalOutputKS@@UAE_NII@Z`.
- Selected attempt: 5; total grades: 5; distinct source families reported: .
- Built/retail length: 1144/1144. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None: bytes and all named/internal relocation operands checked
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00c5e650/PROBES.md and NAMING_EVIDENCE.md.

## 00ddae40 — PARKED

- Exact Leaf: `?Main@CQ_PrisonRaceScript@NScript@@UAEXXZ`.
- Selected attempt: 31; total grades: 31; distinct source families reported: 20.
- Built/retail length: 1180/1180. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Exact1180 length; sole reordered fstp [esp+18] precedes mov ecx,[esp+20]/cmp ecx,ebp instead of following them. All77 operands independently proven.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ddae40/PROBES.md and NAMING_EVIDENCE.md.

## 00ef7be0 — RELOCATION_MATCH

- Exact Leaf: `?Main@CChatter1@CPersonalScript_EmptyScript@NScript@@EAEXXZ`.
- Selected attempt: 6; total grades: 6; distinct source families reported: 2.
- Built/retail length: 1204/1204. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; exact bytes after audited relocations
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ef7be0/PROBES.md and NAMING_EVIDENCE.md.

## 00d27460 — RELOCATION_MATCH

- Exact Leaf: `?Main@CQ_EndGameScript@NScript@@UAEXXZ`.
- Selected attempt: 7; total grades: 7; distinct source families reported: 1.
- Built/retail length: 1244/1244. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; exact bytes after audited relocations
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00d27460/PROBES.md and NAMING_EVIDENCE.md.

## 00e39b40 — RELOCATION_MATCH

- Exact Leaf: `?Main@CV_BordelloScript@NScript@@UAEXXZ`.
- Selected attempt: 8; total grades: 8; distinct source families reported: 1.
- Built/retail length: 1250/1250. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; exact full1250 bytes after relocation masking and strict101-operand target/literal audit.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e39b40/PROBES.md and NAMING_EVIDENCE.md.

## 00ef4b40 — PARKED

- Exact Leaf: `?Main@CTestPerson@CPersonalScript_EmptyScript@NScript@@EAEXXZ`.
- Selected attempt: 20; total grades: 20; distinct source families reported: 15.
- Built/retail length: 1241/1268. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Mixed constructor expansion: inner ScriptedThingBase ctor expands where retail calls; frame0x7c vs0x80; zero cache, slot assignment and cleanup-return layout differ. External MovieBase constructor declaration is a header visibility hypothesis.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ef4b40/PROBES.md and NAMING_EVIDENCE.md.

## 00e377d0 — RELOCATION_MATCH

- Exact Leaf: `?Main@CV_AssassinAttacksScript@NScript@@UAEXXZ`.
- Selected attempt: 9; total grades: 9; distinct source families reported: 1.
- Built/retail length: 1294/1294. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; exact bytes after audited relocations
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e377d0/PROBES.md and NAMING_EVIDENCE.md.

## 00e92a50 — RELOCATION_MATCH

- Exact Leaf: `?Main@CV_HiddenBootyScript@NScript@@UAEXXZ`.
- Selected attempt: 11; total grades: 11; distinct source families reported: 2.
- Built/retail length: 1294/1294. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: None; fresh exact target bytes and every relocation equal prior independently verified main receipt; zero active forced-inline attributes; updated dual tests PASS. Renewed main review pending.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00e92a50/PROBES.md and NAMING_EVIDENCE.md.

## 00efa4f0 — PARKED

- Exact Leaf: `?Main@Cbandit_level_cutsceneScript@NScript@@UAEXXZ`.
- Selected attempt: 20; total grades: 20; distinct source families reported: 16.
- Built/retail length: 1353/1362. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Main instruction stream agrees until cleanup copies; one MovieBase destructor fully expands counted release where retail calls its destructor and merges different return copies.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00efa4f0/PROBES.md and NAMING_EVIDENCE.md.

## 00ed3b30 — PARKED

- Exact Leaf: `?Main@CV_StatueMasterScript@NScript@@UAEXXZ`.
- Selected attempt: 23; total grades: 23; distinct source families reported: 15.
- Built/retail length: 1102/1402. GNU64 PASS; VC7.1 RTM PASS
- Residue or limitation: Original GFGetAngleXYTo remains called4x where retail expands; helper itself matches existing91-byte native dependency. Other bindings/lookups/cleanup closely aligned; no forced inline used.
- Selection basis: frozen final review index, independently validated.
- Full attempts and findings: rows/00ed3b30/PROBES.md and NAMING_EVIDENCE.md.

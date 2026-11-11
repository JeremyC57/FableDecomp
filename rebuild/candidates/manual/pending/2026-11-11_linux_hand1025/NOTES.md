# Chunk 1025 hand-back (agent1025)

**Header overlay:** every grade (gg.sh / gall.sh / q.py) ran with `export FABLE_HDR_OVERLAY=$A/hdr`; see hdr/README.md for the opt-ins
and the three new headers (CTargetingDef.h, CAtmosProcess.h, CThingAICreatureInit.h).

## Result
| Row | Grade | Grades | File |
|---|---|---|---|
| 0x007BF000 NThingShot::GetCreatureFirePosAndOrientation | RELOCATION_MATCH | 4 | src/ + test/ |
| 0x008028D0 `anonymous namespace'::GetTargetingRangeForTargetType | RELOCATION_MATCH | 1 | src/ + test/ |
| 0x006B1D30 CAtmosProcess::SetAtmosSoundsAsMuted | RELOCATION_MATCH | 3 | src/ + test/ |
| 0x00802B60 `anonymous namespace'::GetTargetingRangeForTarget | DIFFER(410v424) | 4 | differ/ |
| 0x007C0AD0 NThingShot::CreateShot | DIFFER(936v938) | 3 | differ/ |
| 0x0062FE60 NUISystem::CUIDef::CUIDef | DIFFER(829v817) | 3 | differ/ |
| 0x00716B30 CActionTalkBase::SetUpTextAndSpeech | not attempted | 0 | none (budget) |
| 0x00BD2D90 CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations | not attempted | 0 | none (budget) |

## Identities (two sources each; LABELS.tsv)
- 0x007BF000 = ?GetCreatureFirePosAndOrientation@NThingShot@@YIXABVCThing@@AAVC3DVector@@AAVCRightHandedSet@@@Z (Ego_r 0x007BDEF0, FableWin 0x026FB140; ret 4; caller CreateShot 0x007C0B56).
- 0x008028D0 / 0x00802B60 = the anonymous-namespace pair of the player-targeting TU (?A0x58ed6966; Ego_r donors 0x007FD8C0 / 0x007FDB70 after GetTargetingRange 0x007FD880 = retail 0x00802890).
- 0x006B1D30 = ?SetAtmosSoundsAsMuted@CAtmosProcess@@QAEX_NM@Z (Ego_r 0x006EE0D0; callers 0x0049E616 / 0x004A5C99).
- 0x007C0AD0 = the def-index CreateShot overload (Ego_r donor 0x007BF280; ret 20h: index/shooter in registers, 8 stack words incl. const C3DVector* / const CRightHandedSet*).
- 0x0062FE60 = ??0CUIDef@NUISystem@@QAE@XZ (stores vtable 0x01259F8C, RTTI NUISystem::CUIDef, the landed ~CUIDef 0x006301A0's).
- Callees named on the way: 0x005DA890 CTCWeapon::GetTargetingRange, 0x006B2660 GetDef<CSoundThemeDef> by name, 0x013BACB4/B8
  SHOT_CREATION_DUMMY/POINT (NAME_FIXES.tsv for the three landed placeholder spellings), 0x009ACCE0
  CDefinitionManager::GetDefClassTypeNameFromGlobalIndex, 0x009D5080 CDefString::SetDefaultString, 0x004FACD0 CThingAICreatureInit ctor,
  0x0073AE10 CTCCreatureModeManager::AddMode (mode 57; NCreatureMode::EMode enumerator name not on disk), TCI_ENTITY_MODE_MANAGER 49.
- Descriptive (no original name found anywhere): g_bShotsEnabled 0x0137573F (spelling of the landed 0x007C0E80), CAtmosProcess::CMyAtmosMap
  slot names GetBlockValue / GetBlockHeight (inherited CASoundManager::CAtmosMap names), the CreateShot float parameter names.

## What each unmatched row needs next
- **0x00802B60** (4 grades): body/blocks/tails identical; only allocation. Retail: def in ebx, targeter homed to [esp+0x10] (frame 8,
  key2 shares that slot); ours def esi, targeter ebp, frame 4. c2prio: def priority 37 must drop below target's 28 so the map value takes
  esi, target edi, def ebx, const 0 ebp. Invariant under: flags local, `const CTargetingDef&` local, decl order, def.Get(), default
  PeekTC (Find) branch, same-TU GetDef/GetSubDef bodies, single/multiple returns. Next: try the def copy taken by value from a
  non-inline GetPTargetingDef, or the scripted-hook branch written first (`if (def.IsNull()) {...} else`).
- **0x007C0AD0** (3 grades, differ = g3 source): levers found: `CCharString class_name = ....GetString()` (in-place), the created
  creature held in `shot` (no separate local), `CThing* shot = 0` first (coalesces with retail's ebx zero), same-TU
  CBasicString<char>::operator==(const char*) body (class-name data in esi). Left: (1) block order: with the body in the TU, VC7.1 lays the
  null-string CREATURE test and creature body right after the null OBJECT test (retail: OBJECT body first, non-null OBJECT call between);
  g2 (no body) had retail's order but re-tested null. Try the CCharString::operator==(const char*) inline spelled with the non-null arm
  first (needs an overlay opt-in in fable_string.h -- an overlay COPY of fable_string.h breaks the include guard pairing, so add the
  opt-in in the repo header at landing time). (2) c2prio shot_def 71 > shot 66 swaps ebx/ebp.
- **0x0062FE60** (3 grades, differ = g3 source): base ctor expansion, vtable, all STLport containers, CWideStrings and the font call
  match (needs FABLE_RETAIL_DEF_CLASS_BASE_CTORS + FABLE_PDCB_TYPED_MEMBERS + FABLE_RETAIL_CVECTORMAP_CTOR). Left: the scalar member stores
  after the font: retail copies one zero register for every TextWindow/Up/Forward/RotationAxis component and stores PositionOffset straight
  from ebx; try member-wise stores for PositionOffset (`PositionOffset.X = PositionOffset.Y = 0`), C2DBoxF/C3DVector copy forms, and the
  initialiser-list for the scalar members.
- **0x00716B30** (not attempted): decode notes. CActionTalkBase (Ego_r layout, CArray Messages 12 bytes in retail): CurrentMessageIndex
  +0x3c, GameTurnDuration +0x48, NextAttitude / NextAnimation / NextCameraTagIndex +0x4c/+0x50/+0x54, FramesUntilNextGesture +0x5c,
  UseCameraTags +0x6f, CameraTagsWereUsed +0x70, SoundIn2D +0x72, SoundStartedPlaying +0x73, SkippedCurrentLine +0x8f; statics
  0x013BAD08..14 (speaker/listener UIDs), 0x01380C34, 0x013BAD00, DefaultCameraTags 0x013BAD18 (landed InitialiseDefaultCameraTags
  0x007167E0); callees 0x006E3580, 0x006E2A50, 0x006E2840, GFGetTime 0x009E1BC0, random 0x004978A0, 0x00686A70, 0x00697AE0, 0x00712E40,
  SetUpCamera 0x00715A70; a GFFloatToLongNear-style rounding (needs the shared retail-asm helper and a Retail-asm line).
- **0x00BD2D90** (not attempted): SSE arm = GF_SSE_MATRIX_MULTIPLY (engine/CMatrix4x4AlignedSSE.h, g_SSEAvailable 0x013D2880) on homed
  pointer locals, aligned frame (`and esp,-16`), x87 fallback arm whose per-column term order is the open chunk-748 residue
  (0x00989230 / 0x00AE1320 / 0x00AE29D0); retail_asm grade at best. Callees 0x00B83750, 0x00AA0090 (bone evaluation), 0x00A5C910 (alloc
  of count*0x40), PBoneTransforms +0x120.

## Extents
All six worked rows end at `ret` + int3 padding inside the catalog length (no SPLITS.tsv needed): 0x007BF000 679 (+9),
0x008028D0 648 (+8), 0x00802B60 424 (+8), 0x006B1D30 507 (+5), 0x007C0AD0 938 (+6), 0x0062FE60 817 (+15).

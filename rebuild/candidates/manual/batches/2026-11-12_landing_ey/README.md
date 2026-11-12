# 2026-11-12 landing ey -- author workflow wf13 (Opus only): seven of ten rows landed, three in repair

Source: `pending/2026-11-12_linux_handWF13/` (workflow `lane_tools/workflows/author_rows.js`, 58 agents, all Opus; no row needed
Fable). All ten rows byte-matched in about 130 minutes. Landed (all RELOCATION_MATCH, test PASS under VC7.1,
LANDVERIFY_STRICT_HEADERS=1): 0x007F0F40 CTCKrakenTentacle::ReturnToDesiredPos, 0x006B08F0 CTCPhysicsBase::OnSerialise (two author
passes), 0x00731EA0 CTCSpotLight::OnAppearanceDraw, 0x009341C0 CAIStateGroup_MinionKidGroupTag::TagTarget, 0x00754050
CTCLook::UpdateTurningAnims, 0x00951D60 CAIStateGroup_JackDragon::IntoHover, 0x009CCEF0 CBankFile::FindIndicesByFilename.

**Not landed yet (matched; each is in a repair + two-verifier workflow, `lane_tools/workflows/repair_and_verify_row.js`):**
- 0x00918640 CAIStateGroup_Gaze::FindFreeParticularGazePos: the source carries a local model of a class that has a shared header;
  it is being ported onto an opt-in of `engine/CAIStateGroup_Gaze.h`.
- 0x00881A40 NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags: an undocumented `#if` rewrite in the
  `stlport/fable_map.h` overlay, a `__declspec(noinline)` whose retail evidence is not cited, and a truncated name fix.
- 0x00492973 CSubtitleRenderer::SetText: carried callee bodies whose provenance is not stated in the source (RemoveMessage spelled
  as a second strong definition, the list ctor/dtor names), and three `#pragma`s without retail evidence.

**Mechanical checks:** reserved_check 0 (0x009341C0 only declares the reserved 0x009578A0; no body), purity clean (the one
`__declspec(noinline)` is the established spelling for the out-of-line retail GetSubDef<T> instance, body = landed 0x008E3300), no
local model of a header class in a landed row source.
**Headers** (14; additions under new opt-ins, all unused elsewhere, defaults unchanged per hdrcheck2): C3DVector.h (3-way merge;
GETMAGNITUDEANDNORMALISE, ISNONZERO_ZYX_SQUARED), CWorld.h, CTCSpotLight.h, CSpotLightDef.h, CIntelligentPointer.h
(FORCEINLINE_CTOR, with its Inline-budget line), CAIStateGroup_MinionKidGroupTag.h, CTCCreatureModeManager.h, CAnimationEntry.h,
CAIStateGroup_JackDragon.h, CJackDragonDef.h, CBankFile.h, fable_string.h (GETASLOWERCASE), stlport/fable_containers.h
(CONSTRUCT_BYVAL), stlport/fable_map.h (MULTIMAP_EQUAL_RANGE).
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011ey): the rows' name fixes (0x004C7580, 0x007324A0, 0x004C4160, 0x008137B0, 0x00950C30, 0x0094FDF0, 0x0099ED10).

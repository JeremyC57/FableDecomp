# Chunk 648 -- 1 of 10 landed (2026-10-31)

The first chunk cut from the fixable bucket of the re-ranked pool (811-1144 byte rows, same length as retail). Landed via
`coordinator/land_chunk.sh 648` (vland --land 1/1 RELOCATION_MATCH + PASS, multi-check 0; new bindings GetAllThingsInRoughArea<const
CThingBuilding, vector<const CThingBuilding*>, CThingFilter_None> const 0x006B2720, GetDef<CSoundThemeDef> 0x006B25E0,
CAtmosProcess::ApplyReverbSettings 0x006B1600, CThingBuilding::GetReverbSettings 0x0082D8C0): CAtmosProcess::UpdateInsideBuildings
0x006B1F30 -- (1) the camera callee respelled to return a whole 0x38-byte CCamera by value gives retail's 0x7c frame (a C3DVector return
packs it to 0x44); (2) a same-TU GetAllThingsInRoughArea stand-in that never reads the empty filter puts the filter on the existing
`checked` byte; (3) a same-TU GetDef<CSoundThemeDef> stand-in that only fills its out-parameter puts the loop's CCharString and
CDefPointer in retail's slots; (4) the region def read as the raw `PDef.Object` word with the count raised/released by hand (a CDefPointer
copy or .Get() leaves a dead `add eax,24h`); (5) CWorldMap / CThingSearchTools / CDefinitionManager / CRegion through the `#define X XLayout`
include pattern, CWorld and CAIGameCameraBase local because their headers flatten the vptr. The landed ctor 0x006B1960 bound 0x006B1600
as `LoadMainTheme_006b1600`; respelled to the private ApplyReverbSettings and rechecked (`verify_caller_respell.log`). Nine rows stay DIFFER
in `pending/2026-10-27_linux_hand648/differ/` (00912840 AddRandomBezierCurveToPath interleaved 12-dword push_back copy, 009552c0
MazeShieldedChase::Update 868v861 def field order, 00881c90 InsertScreenFilterEntry loop registers / zero-store order, 0079c880
CTCBulletTime::FrameUpdate two frame slots, 00b17030 CPSCOrbit::DatabaseSerialise name in the dead context slot, 00bf0740
SetupOutlineStencilStates prologue `push edi` before the global load, 007d00f0 ShoveHaPenny::FrameUpdate frame 0x28 v 0x2c, 00ea5230
CLostTraderBanditSpawn::Main hoisted `lea ecx`, 00e95770 CM_UndeadHere::Main edx/eax in the AXE arm; NOTES.md has a next idea per row).
Lessons: frame size follows the callee's return-temporary type (respell virtuals to the PDB return type); whether a same-TU stand-in reads
or writes a parameter decides where VC homes locals; inline accessors on a member subobject leave a dead `add eax,off` (read the member word
directly); the shared CDefPointer.h Release() compiles to mov/dec/mov where retail does `dec [ecx+4]; jne` (header debt); a trivial
C3DVector copies with `rep movsd`, a user copy ctor gives paired copies, retail's interleaved order is still open. 9 NAME_FIXES + 4 review
rows queued (prefix c648): ApplyReverbSettings, GetAllThingsInRoughArea const, CWorldMap::GetLoadedRegion / GetRegion, CWorld::GetFrame,
CGameDefinitionManager::Get, CThingBuilding::IsPointInside const / GetReverbSettings, g_pSoundManager 0x013B8394; label 0x01436E3C is the
CEngineScreenEffectOutlineGlow global not g_pWaterRenderer (landed 0x00B33010; no PDB data name); header debt (CDefPointer.h Release shape,
CWorld.h / CAIGameCameraBase.h flatten, CRegion.h PDef at +0x2c vs retail +0x24, no CAtmosProcess / CRegionDef / CGlobalDef /
CGameDefinitionManager / CMainGameComponent / CThingBuilding header); CASoundManager slot 46 `SetGroupVolume` descriptive only.
Labels: ghidra_out/labels_chunk648_2026-10-27.tsv.

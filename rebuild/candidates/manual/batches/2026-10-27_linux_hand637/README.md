# Chunk 637 -- 9 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 637` (vland --land 9/9 RELOCATION_MATCH + PASS, then a full
recheck of the nine plus the respelled caller 0x008DFFD0: 10/10, multi-check 0). The lever that kept working: PDB inline helpers and
accessors (Ego_r + Ego_d name sets), which change register choice, immediate size and temporary packing.
- 0x009E4DA0 CDriveManager::CreateDirectoryW (/GS): private inline ConvertDriveLetterToNumber(wchar_t) -> imm8 `add edi,-40h`, convert
  before _getdrive(), `EDriveReturn result` declared before fullPath hoists `xor ebx,ebx`.
- 0x00A1DE30 CNavigatorAStar::CalcWayPointWidths: CSteeringAgent inline accessors; CalculateNavigableWidthOfLine by reference.
- 0x0071BCD0 CTCHeroMorph::CreateAlphaChannel: C2DExtentsI with user default + copy ctor packs the Lock temporaries (frame 0x20);
  CTexture::GetWidth/GetHeight inlines fix the multiply order.
- 0x00AEB060 NParticleEngine::CParticleEmitterList::Insert: size() as STLport distance() (its __distance copies the first iterator);
  `--it; insert(it, x)`. The old `#pragma pack` was removed, bytes unchanged.
- 0x00BE5E70 CLandscapeBackgroundPatch ctor (15 arguments): VC7.1 swaps consecutive parameter-copy stores in pairs; body order
  derived backwards from retail's store order.
- 0x0056EB48 CSliderAnimator::Update: the float fabs overload from <math.h>.
- 0x005223F0 CThingManager::LoadLoadableGlobalThingsFromFile (catalog label LoadGlobalThingBatch): named worldMap local, things
  vector in a nested block.
- 0x00F35ED0 CPerlinNoise::Initialise: inline GFFloatRandom with a named double fmod result (agent635's lever).
- 0x0095C0B0 CAIStateGroup_HoverHornetQueen::WaitWhileBabiesFight: the landed IsStillRunnable 0x0095A420 body as an inline member
  with a named CircleMarkers[0] reference and two early returns; old `#pragma pack` removed.
At landing the caller 0x008DFFD0 (CModeGuildmasterPowerUpHeal::FrameUpdate) was respelled from CountLivingHelperCreatures (QBE) to
the PDB CTCBoss::CountLivingHelpers (QAE) and rechecked. One row parked: 0x007471F0 CTCStealth::DrawPreparePrimitivesForRendering
(rtti slot 11; catalog vfunc_11) 308v308 -- frame 0x40 vs 0x44, the ToWideString argument temporary is homed in the dead `info`
argument slot where retail gives it its own frame word (`pending/2026-10-27_linux_hand637/differ/`).
11 NAME_FIXES + 5 review rows queued (prefix c637): DriveExists returns EDriveReturn, the NParticleEngine list element type for
0x00AEC6F0, GFIntToWideString 0x00405F00 (3 spellings), IsStopping 0x008FD010 protected const (12 spellings), CWideString operator
const unsigned short* 0x0099AF00 (6), CTCBase::PeekComponent 0x00686D60 (19), CThing::GetPos 0x004C73D0, CLevelLoader::LoadAllSectionName
0x013BAB08 (mangling unproven, descriptive kept); caller 0x00AEB6D0's CEmitterPointer stand-in to respell. Labels:
ghidra_out/labels_chunk637_2026-10-27.tsv.

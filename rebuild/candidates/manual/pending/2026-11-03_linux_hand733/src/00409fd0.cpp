// CUserProfileManager::SaveGameState(CAFile& file) -- persist the live options, key sets, score-board details, tutorial list and save-slot names as text, then write the "FablePro" header, the text and a CRC-32, padded to 16 KB, to the file
// Leaf: ?SaveGameState@CUserProfileManager@@IAEXAAVCAFile@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00409FD0, 1892 bytes, thiscall, `ret 4`.
// Callees: CPersistContext(CCharString&) 0x009BAE10, CWideString::ToCharString 0x0099B2C0, the CPersistContext::Transfer<T>
//   instances (bool 0x004045C0, float 0x00410620, unsigned long 0x004106F0, CCharString 0x004109A0, CWideString 0x00410AC0, long
//   0x00410BE0), TransferVector<CActionInputControl> 0x00412DB0 / <ETutorialCategory> 0x00412E80, GFIntToCharString 0x0099F830,
//   operator+(const char*, const CCharString&) 0x0099F690, CMemoryDataOutputStream / CDataOutputStream WriteString 0x009959F0,
//   WriteULONG, ReserveSize 0x00996BC0, Write, WriteRepeatedByte 0x00994230, CAFile::Write (vtable slot 4).
// Shape: two-argument Transfer + GetDefaultVal<T>() (inline; the string defaults construct, copy and destroy); the header and
//   the text CRC through the seeded header-inline CCRC::Calc(crc, data, length) (seed 0, then the running CRC), `if (length == 0)
//   return crc;` form; "FablePro" and L"" by implicit conversion (the temporary's address is re-taken with lea).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CAFILE
#include "fable_string.h"
#include "fable_user_profile.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CAFile.h"
#include "engine/CCRC.h"

CCharString FABLE_FASTCALL GFIntToCharString(long value);           // 0x0099F830 ?GFIntToCharString@@YI?AVCCharString@@J@Z

// The inline two-argument Transfer and GetDefaultVal<T>() (declared in engine/CPersistContext.h; FableWin.pdb
// ??$Transfer@_N@CPersistContext@@QAEXPBDAA_N@Z / ??$GetDefaultVal@VCCharString@@@CPersistContext@@KI?AVCCharString@@XZ, no retail copy).
template<class T> inline void CPersistContext::Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
template<class T> inline T FABLE_FASTCALL CPersistContext::GetDefaultVal() { T value = T(); return value; }

void CUserProfileManager::SaveGameState(CAFile& file)
{
    CCharString text;
    CPersistContext context(text);
    CCharString profileName = ProfileName.ToCharString();
    context.Transfer("ProfileName", profileName);
    context.Transfer("SubtitlingEnabled", SubtitlingEnabled);
    context.Transfer("VibrationEnabled", VibrationEnabled);
    context.Transfer("CameraXYInverted", CameraXYInverted);
    context.Transfer("CameraYZInverted", CameraYZInverted);
    context.Transfer("CameraFPBowXYInverted", CameraFPBowXYInverted);
    context.Transfer("CameraFPBowYZInverted", CameraFPBowYZInverted);
    context.Transfer("CameraPCBowXYInverted", CameraPCBowXYInverted);
    context.Transfer("CameraPCBowYZInverted", CameraPCBowYZInverted);
    context.Transfer("CameraPCViewHeroXYInverted", CameraPCViewHeroXYInverted);
    context.Transfer("CameraPCViewHeroYZInverted", CameraPCViewHeroYZInverted);
    context.Transfer("TutorialSystemEnabled", TutorialSystemEnabled);
    context.Transfer("GuildMasterEnabled", GuildMasterEnabled);
    context.Transfer("HUDTooltipEnabled", HUDTooltipEnabled);
    context.Transfer("Gamma", Gamma);
    context.Transfer("HUDAlpha", HUDAlpha);
    context.Transfer("SoundVolume", SoundVolume);
    context.Transfer("MusicVolume", MusicVolume);
    context.Transfer("DialogueVolume", DialogueVolume);
    context.Transfer("UsingHeadPhones", UsingHeadPhones);
    context.Transfer("ShowingBuddyNames", ShowingBuddyNames);
    context.Transfer("CameraSensitivity", CameraSensitivity);
    context.Transfer("CameraResetting", CameraResetting);
    context.Transfer("Antialiasing", Antialiasing);
    context.Transfer("TextureDetail", TextureDetail);
    context.Transfer("ShadowDetail", ShadowDetail);
    context.Transfer("EffectsDetail", EffectsDetail);
    context.Transfer("MeshDetail", MeshDetail);
    context.Transfer("Resolution", Resolution);
    context.Transfer("RefreshRate", RefreshRate);
    context.Transfer("ScreenAspectRatio", ScreenAspectRatio);
    context.Transfer("VerticalSyncEnabled", VerticalSyncEnabled);
    context.TransferVector("AssignedInputsPassive", AssignedInputsPassive);
    context.TransferVector("AssignedInputsAggressive", AssignedInputsAggressive);
    context.Transfer("ShowTargetingStatus", ShowTargetingStatus);
    context.Transfer("ScoreboardUserName", ScoreBoardUserName);
    context.Transfer("ScoreboardPassword", ScoreBoardPassword);
    context.Transfer("ScoreboardProxyIp", ScoreBoardProxyIp);
    context.Transfer("ScoreboardProxyPort", ScoreBoardProxyPort);
    context.Transfer("ScoreboardServerPort", ScoreBoardServerPort);
    context.TransferVector("GivenTutorialCategories", GivenTutorialCategories);
    for (long i = 0; i < 50; ++i) {
        CCharString saveName;
        saveName = SaveGameNames[i].ToCharString();
        context.Transfer("SaveGameNames" + GFIntToCharString(i + 1), saveName);
    }

    CMemoryDataOutputStream header;
    header.WriteString("FablePro");
    header.WriteULONG(0);
    header.WriteULONG(text.GetLength() + 0x10);
    CMemoryDataOutputStream stream;
    stream.ReserveSize(0x4000);
    stream.Write(header.PeekData(), header.GetLength());
    stream.WriteString(text);
    unsigned long crc = CCRC::Calc(0, header.PeekData(), header.GetLength());
    crc = CCRC::Calc(crc, text.GetAsCharArray(), text.GetLength());
    stream.WriteULONG(crc);
    unsigned long position = stream.GetPosition();
    if (position < 0x4000)
        stream.WriteRepeatedByte(' ', 0x4000 - position);
    file.Write(stream.PeekData(), stream.GetLength(), 0);
}

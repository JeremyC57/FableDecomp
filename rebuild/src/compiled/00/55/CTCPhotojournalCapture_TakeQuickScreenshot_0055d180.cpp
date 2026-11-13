// CTCPhotojournalCapture::TakeQuickScreenshot() -- if the screenshot message is showing, hide it and set the capture to wait 4 frames;
// otherwise name a shot from the local date and time ("\" and "/" to "-", ":" to "."), build <photojournal dir><profile>\<name>.jpg,
// and when there is room for 10 MB create the file, save the back buffer to it as a JPEG, show the screenshot message and play
// the snapshot sound.
// Leaf: ?TakeQuickScreenshot@CTCPhotojournalCapture@@QAEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D180, 772 bytes; /GS esp frame (cookie 0x0139C8A8, check 0x00BFE9F9), no EH frame, plain `ret`.
// Ego_r.pdb / FableWin.pdb ?TakeQuickScreenshot@CTCPhotojournalCapture@@QAEXXZ.
// Callees: CPlayerGui::IsShowingScreenshotMessage 0x00437A00 / HideScreenshotMessage 0x004379F0 / ShowScreenshotMessage 0x004379E0
//   (CPlayerGui singleton 0x013B8790, re-read per call), GetLocalTime [0x0143FE18], CDateAndTime(const _SYSTEMTIME&) 0x009E9A30,
//   CDateAndTime::ToFileTime 0x009E9D40, CXBConfig::FormatDateTime 0x00A0D980, CWideString(const wchar_t*) 0x0099B6B0,
//   ~CWideString 0x0099B510, CWideString::operator= 0x0099B7D0, operator+(W, const wchar_t*) 0x0099BF30, operator+(W, W) 0x0099BE70,
//   CWideString::Replace 0x0099C1E0, CUserProfileManager::Get 0x0040D2A0, GetValidProfileName 0x00406350,
//   NResourceDirectoryNames::GetPhotojournalDir 0x0041A600, CAFile::IsRoomForFile 0x00999AB0, CDiskFileWin32 ctor 0x0099AD80 /
//   Close 0x0099A920 (inline dtor: vtable 0x0122D06C, Close, ~CWideString, ~CBaseClassNonCopyable 0x0099A300),
//   operator const wchar_t* 0x0099AF00, D3DXSaveSurfaceToFileW 0x00BFEFB6, NUISystem::CManager::GetInstance 0x0041E5F2,
//   CManager::PlaySample (vtable +0xA4), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0.
// Shape: the "no room" case is an early `return` (the file and the sound name are function-scope locals with their own frame
// slots: the sound name at +0x18 does not share the path-chain temporaries' slots, which a block-scoped local would), the profile
// manager is fetched into a reference first (Get is called before the literals are pushed), and the back-buffer surface is read
// through the inline accessors GetBackBuffer().GetD3DSurface().
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_ISROOMFORFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CDATEANDTIME_SYSTEMTIME
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PROFILENAME_MEMBER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_SCREENSHOTMESSAGE
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_GETD3DSURFACE
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_FRONTEND
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#include "fable_string.h"
#include "engine/CDiskFileWin32.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CDateAndTime.h"
#include "engine/CUserProfileManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CSurface.h"
#include "engine/CDisplayManager.h"

typedef struct _SYSTEMTIME { unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds; } SYSTEMTIME;
typedef struct _FILETIME { unsigned long dwLowDateTime, dwHighDateTime; } FILETIME;
extern "C" __declspec(dllimport) void __stdcall GetLocalTime(_SYSTEMTIME* systemTime);   // [0x0143FE18] kernel32
enum D3DXIMAGE_FILEFORMAT { D3DXIFF_BMP = 0, D3DXIFF_JPG = 1, D3DXIFF_FORCE_DWORD = 0x7fffffff };
struct tagPALETTEENTRY; struct tagRECT;
extern "C" long __stdcall D3DXSaveSurfaceToFileW(const wchar_t* destFile, D3DXIMAGE_FILEFORMAT destFormat, IDirect3DSurface9* srcSurface,
                                                 const tagPALETTEENTRY* srcPalette, const tagRECT* srcRect);   // 0x00BFEFB6 (d3dx9 import thunk)

class CXBConfig { public:
    // Ego_r.pdb / FableWin.pdb ?FormatDateTime@CXBConfig@@SIXABU_FILETIME@@PA_W1H@Z (static __fastcall; retail 0x00A0D980)
    static void __fastcall FormatDateTime(const _FILETIME& time, wchar_t* dateText, wchar_t* timeText, int flags);
};
CWideString __fastcall GetValidProfileName(const CWideString& name);   // 0x00406350 ?GetValidProfileName@@YI?AVCWideString@@ABV1@@Z
namespace NResourceDirectoryNames {
CWideString __fastcall GetPhotojournalDir();                            // 0x0041A600 ?GetPhotojournalDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
}
extern CDisplayManager* GPDisplayManager;                               // 0x013B8390 ?GPDisplayManager@@3PAVCDisplayManager@@A

class CThing;
class CTCPhotojournalCapture { public:             // 0x44 bytes in retail (PDB 0x48: the STLport vector is 12 bytes); no engine header
    void*          __vftable;               // +0x00 (CTCBase)
    CThing*        ParentThing;             // +0x04
    bool           ToBeRemoved;             // +0x08
    bool           IsInGlobalUpdateTCs;     // +0x09
    CWideString    Description;             // +0x0c
    bool           DiscardScreenShot;       // +0x10
    long           FrameCounter;            // +0x14
    bool           FirstRenderFrame;        // +0x18
    bool           ScreenOpen;              // +0x19
    double         LastRenderTime;          // +0x20
    CWideString    FileName;                // +0x28
    void*          Components[3];           // +0x2c vector<NUISystem::CComponent*>
    CThing*        PMannequin;              // +0x38
    bool           WasPlayerGuiOpen;        // +0x3c
    long           TakeScreenshotAfterFrames; // +0x40
    void TakeQuickScreenshot();             // 0x0055D180
};

void CTCPhotojournalCapture::TakeQuickScreenshot()
{
    if (CPlayerGui::Get().IsShowingScreenshotMessage()) {
        CPlayerGui::Get().HideScreenshotMessage();
        TakeScreenshotAfterFrames = 4;
        return;
    }
    _SYSTEMTIME now;
    GetLocalTime(&now);
    CDateAndTime dateTime(now);
    wchar_t dateText[128];
    wchar_t timeText[128];
    CXBConfig::FormatDateTime(dateTime.ToFileTime(), dateText, timeText, 0);
    CWideString name(dateText);
    CWideString clock(timeText);
    name = name + L" " + clock;
    name.Replace(L"\\", L"-");
    name.Replace(L"/", L"-");
    name.Replace(L":", L".");
    CUserProfileManager& profiles = CUserProfileManager::Get();
    CWideString path = NResourceDirectoryNames::GetPhotojournalDir() + GetValidProfileName(profiles.ProfileName) + L"\\" + name + L".jpg";
    if (!CAFile::IsRoomForFile(10000000, path))
        return;
    CDiskFileWin32 file(path, FILE_ACCESS_CREATE, 2);
    file.Close();
    D3DXSaveSurfaceToFileW(path, D3DXIFF_JPG, GPDisplayManager->GetBackBuffer().GetD3DSurface(), 0, 0);
    CPlayerGui::Get().ShowScreenshotMessage();
    CCharString sound("SND_MM_SNAPSHOT");
    NUISystem::CManager::GetInstance()->PlaySample(sound, 1);
}
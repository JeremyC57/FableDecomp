// Grade: DIFFER(772v772, /GS) (sec1b 2026-10-31) -- path decoded: GetPhotojournalDir() + GetValidProfileName(profile->ProfileName) + L"\\" + name + L".jpg" with the singleton fetched into a local first; CDiskFileWin32 modelled as the PDB 0x1c-byte class over CBaseClassNonCopyable with the inline dtor (Close, ~Pathname, base dtor) (137->48 difflines, length now exact); left: retail gives the PlaySound CCharString temporary its own slot [esp+18h] where ours shares a wide-string temp slot, so every frame offset above it is 4 lower (tried a named sound local)
// CTCPhotojournalCapture::TakeQuickScreenshot() -- dismiss a showing screenshot message (state 4); otherwise name a screenshot from the local date and time ("\" and "/" to "-", ":" to "."), and when the save directory has room write the back buffer there as a .jpg, show the screenshot message and play the snapshot sound
// Leaf: TakeQuickScreenshot
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D180, 772 bytes, plain `ret`.
#include <string.h>
struct _SYSTEMTIME { unsigned short w[8]; };
struct _FILETIME { unsigned long Low, High; };
extern "C" __declspec(dllimport) void __stdcall GetLocalTime(_SYSTEMTIME* time);   // [0x0143FE18]
extern "C" long __stdcall D3DXSaveSurfaceToFileW(const wchar_t* file, long format, void* surface, const void* palette, const void* rect);   // 0x00BFEFB6
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                     // 0x0099B6B0
    ~CWideString();                                       // 0x0099B510
    CWideString& operator=(const CWideString& other);     // 0x0099B7D0
    void Replace(const CWideString& find, const CWideString& with);   // 0x0099C1E0
    const wchar_t* GetString() const;                     // 0x0099AF00
};
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);      // 0x0099BF30
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);  // 0x0099BE70
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};
struct CDateAndTime {
    unsigned char _data[0x1c];
    CDateAndTime(const _SYSTEMTIME& time);                // 0x009E9A30
    _FILETIME ToFileTime() const;                         // 0x009E9D40
};
void __fastcall FormatDateTime(const _FILETIME& time, wchar_t* date, wchar_t* clock, int flags);   // 0x00A0D980
bool __fastcall IsRoomForFile(long bytes, const CWideString& path);   // 0x00999AB0
struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();                     // 0x0099A300
};
struct CDiskFileWin32 : CBaseClassNonCopyable {           // Ego_r.pdb 0x1c bytes, vtable 0x0122D06C
    long          AccessType;                             // +0x04 FileAccessType
    long          Position;                               // +0x08
    long          Length;                                 // +0x0c
    void*         WinFileHandle;                          // +0x10
    bool          Opened;                                 // +0x14
    CWideString   Pathname;                               // +0x18
    CDiskFileWin32(const CWideString& path, long access, unsigned long flags);   // 0x0099AD80
    virtual void  Close();                                // 0x0099A920 ?Close@CDiskFileWin32@@UAEXXZ
    virtual ~CDiskFileWin32() { Close(); }                // inline: vtable store, Close, ~Pathname, ~CBaseClassNonCopyable
};
CWideString __fastcall GetValidProfileName(const CWideString& name);   // 0x00406350
namespace NResourceDirectoryNames {
CWideString __fastcall GetPhotojournalDir();                           // 0x0041A600
}
struct CProfileManager {
    unsigned char _pad00[8];
    CWideString   ProfileName;                            // +0x08 (the current profile's name)
    static CProfileManager* __fastcall Get();             // 0x0040D2A0
};
struct CPlayerGui {
    bool IsScreenshotMessageShowing();                    // 0x00437A00
    void HideScreenshotMessage();                         // 0x004379F0
    void ShowScreenshotMessage();                         // 0x004379E0
};
extern CPlayerGui* g_pPlayerGui;                          // 0x013B8790
struct CDisplay { unsigned char _pad[0x30]; void* BackBuffer; };
extern CDisplay* g_pDisplay;                              // 0x013B8390
struct CFrontEnd {
    static CFrontEnd* __fastcall GetInstance();           // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void PlaySound(const CCharString& name, bool now);   // slot 41 (+0xa4)
};

struct CTCPhotojournalCapture {
    virtual ~CTCPhotojournalCapture();
    unsigned char _pad04[0x40 - 0x04];
    long          State;                                  // +0x40
    void TakeQuickScreenshot();
};

void CTCPhotojournalCapture::TakeQuickScreenshot()
{
    if (g_pPlayerGui->IsScreenshotMessageShowing()) {
        g_pPlayerGui->HideScreenshotMessage();
        State = 4;
        return;
    }
    _SYSTEMTIME now;
    GetLocalTime(&now);
    CDateAndTime dateTime(now);
    wchar_t dateText[128];
    wchar_t timeText[128];
    FormatDateTime(dateTime.ToFileTime(), dateText, timeText, 0);
    CWideString name(dateText);
    CWideString clock(timeText);
    name = name + L" " + clock;
    name.Replace(L"\\", L"-");
    name.Replace(L"/", L"-");
    name.Replace(L":", L".");
    CProfileManager* profileManager = CProfileManager::Get();
    CWideString path = NResourceDirectoryNames::GetPhotojournalDir() + GetValidProfileName(profileManager->ProfileName) + L"\\" + name + L".jpg";
    if (IsRoomForFile(10000000, path)) {
        CDiskFileWin32 file(path, 4, 2);
        file.Close();
        D3DXSaveSurfaceToFileW(path.GetString(), 1, g_pDisplay->BackBuffer, 0, 0);
        g_pPlayerGui->ShowScreenshotMessage();
        CFrontEnd::GetInstance()->PlaySound("SND_MM_SNAPSHOT", true);
    }
}

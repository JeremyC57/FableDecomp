// CTCInventory::HasScreenshots() -- whether the current profile's save directory holds any "*.jpg" screenshot
// Grade: DIFFER(194v170) -- frame shape only: retail builds the biased /GS frame `push ebp ; lea ebp,[esp-1ACh] ; sub esp,22Ch` (8-bit displacements, cookie at [ebp+1A8h]); the lane emits `mov ebp,esp` with 32-bit displacements (y-off+s-on, y-off alone, /O1, /Os tried); the instruction stream otherwise lines up
// Leaf: HasScreenshots
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581B33, 170 bytes; /GS frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)` with /GS: the biased `lea ebp,[esp-1ACh]` frame and the cookie
// check @ 0x00BFE9F9 against 0x0139C8A8); a static member (no `this` use, plain `ret`).
// Callees: CTBaseSingleton<CUserProfileManager>::Get() @ 0x0040D2A0 (__fastcall), GetValidProfileName(const
//          CWideString&) @ 0x00406350 and GetSaveDir() @ 0x0041A600 (__fastcall, hidden result in ecx), operator+(const
//          CWideString&, const CWideString&) @ 0x0099BE70, operator+(const CWideString&, const wchar_t*) @ 0x0099BF30
//          (x2), ~CWideString @ 0x0099B510 (x5), the wide find-first wrapper @ 0x00999760 (__fastcall pattern / handle,
//          the file-info block on the stack; returns bool; manifest "CAFile_FindFirstFile").
// Shape: `GetSaveDir() + GetValidProfileName(profile name) + L"\\" + L"*.jpg"` in one expression: the literal
// arguments of the outer concatenations are pushed first and the profile name is built before the save
// directory (arguments right to left); four temporaries die in reverse order once the pattern is built; the find
// result rides in bl across the pattern's destructor.
// Layout: CUserProfileManager's current profile name at +0x08.
#pragma optimize("y", off)
#pragma optimize("s", on)

typedef unsigned short wchar_t16;

struct CWideString {
    void* PStringData;
    ~CWideString();                                  // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const wchar_t16* right);     // 0x0099BF30

CWideString __fastcall GetValidProfileName(const CWideString& name);   // 0x00406350
CWideString __fastcall GetSaveDir();                                    // 0x0041A600

struct CUserProfileManager {
    unsigned char _pad_0x00[0x08];
    CWideString   ProfileName;                       // +0x08
};
template <class T> struct CTBaseSingleton {
    static T& __fastcall Get();                      // 0x0040D2A0
};

struct SFindHandle {                                 // the wrapper's search state (handle at +4)
    long Flags;
    long Handle;
};
struct SFileInfo {                                   // the file-info block the wrapper fills
    unsigned char Data[0x20C];
};
bool __fastcall FindFirstFileW_00999760(const CWideString& pattern, SFindHandle* handle, SFileInfo* info);   // 0x00999760

static const wchar_t16 s_backslash[] = { '\\', 0 };  // 0x0122DE78
static const wchar_t16 s_jpgMask[] = { '*', '.', 'j', 'p', 'g', 0 };   // 0x0124FED0

struct CTCInventory {
    static bool HasScreenshots();
};

bool CTCInventory::HasScreenshots()
{
    CWideString pattern = GetSaveDir() + GetValidProfileName(CTBaseSingleton<CUserProfileManager>::Get().ProfileName)
                          + s_backslash + s_jpgMask;
    SFindHandle handle;
    SFileInfo info;
    bool found = FindFirstFileW_00999760(pattern, &handle, &info);
    return found;
}

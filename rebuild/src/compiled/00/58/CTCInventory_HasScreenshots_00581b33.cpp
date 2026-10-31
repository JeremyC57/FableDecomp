// CTCInventory::HasScreenshots() -- whether the current profile's save directory holds any "*.jpg" screenshot
// Grade: RELOCATION_MATCH (170v170) -- 4 grades: `optimize("s", on)` alone gives the biased /GS frame (`y` off forced `mov ebp,esp`), and the profile-manager singleton fetched into a reference local first (retail calls Get() before pushing the two literals, lea edx,[eax+8])
// Leaf: ?HasScreenshots@CTCInventory@@QAE_NXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581B33, 170 bytes; /GS frame-pointer size TU
// (`optimize("s", on)` with /GS: the biased `lea ebp,[esp-1ACh]` frame and the cookie check @ 0x00BFE9F9 against
// 0x0139C8A8); Ego_r.pdb ?HasScreenshots@CTCInventory@@QAE_NXZ, a thiscall member that never reads `this` (plain `ret`).
// Callees: CTBaseSingleton<CUserProfileManager>::Get() @ 0x0040D2A0 (__fastcall), GetValidProfileName(const
//          CWideString&) @ 0x00406350 and NResourceDirectoryNames::GetSaveDir() @ 0x0041A600 (__fastcall, hidden result in ecx), operator+(const
//          CWideString&, const CWideString&) @ 0x0099BE70, operator+(const CWideString&, const wchar_t*) @ 0x0099BF30
//          (x2), ~CWideString @ 0x0099B510 (x5), CAFile::StartFileScan @ 0x00999760 (static __fastcall: pattern in ecx,
//          the CFileScanInfo in edx, the 0x20C-byte name buffer on the stack; returns bool; reloc_bindings + Ego_r.pdb).
// Shape: the profile-manager singleton into a reference local first, then `GetSaveDir() + GetValidProfileName(name) + L"\\" + L"*.jpg"`: the literal
// arguments of the outer concatenations are pushed first and the profile name is built before the save
// directory (arguments right to left); four temporaries die in reverse order once the pattern is built; the find
// result rides in bl across the pattern's destructor.
// Layout: CUserProfileManager's current profile name at +0x08.
#pragma optimize("s", on)                           // alone: `y` off as well loses the biased frame

typedef unsigned short wchar_t16;

class CWideString { public:
    void* PStringData;
    ~CWideString();                                  // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const wchar_t16* right);     // 0x0099BF30

CWideString __fastcall GetValidProfileName(const CWideString& name);   // 0x00406350 ?GetValidProfileName@@YI?AVCWideString@@ABV1@@Z
namespace NResourceDirectoryNames {
CWideString __fastcall GetSaveDir();                                    // 0x0041A600 ?GetSaveDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
}

class CUserProfileManager { public:
    unsigned char _pad_0x00[0x08];
    CWideString   ProfileName;                       // +0x08
};
template <class T> struct CTBaseSingleton {
    static T& __fastcall Get();                      // 0x0040D2A0
};

class CAFile {
public:
    class CFileScanInfo { public:                           // Ego_r.pdb CAFile::CFileScanInfo, 8 bytes
        bool Stopped;                                // +0x00
        long SearchHandle;                           // +0x04
    };
    // 0x00999760 ?StartFileScan@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PAG@Z (Ego_r.pdb: the wchar_t* overload;
    // wchar_t is unsigned short in this toolchain, so PA_W spells PAG)
    static bool __fastcall StartFileScan(const CWideString& pattern, CFileScanInfo& info, wchar_t16* fileName);
};

static const wchar_t16 s_backslash[] = { '\\', 0 };  // 0x0122DE78
static const wchar_t16 s_jpgMask[] = { '*', '.', 'j', 'p', 'g', 0 };   // 0x0124FED0

class CTCInventory {
public:
    bool HasScreenshots();                           // ?HasScreenshots@CTCInventory@@QAE_NXZ (`this` unused)
};

bool CTCInventory::HasScreenshots()
{
    CUserProfileManager& profiles = CTBaseSingleton<CUserProfileManager>::Get();
    CWideString pattern = NResourceDirectoryNames::GetSaveDir() + GetValidProfileName(profiles.ProfileName) + s_backslash + s_jpgMask;
    CAFile::CFileScanInfo scanInfo;
    wchar_t16 fileName[0x20C / 2];
    bool found = CAFile::StartFileScan(pattern, scanInfo, fileName);
    return found;
}
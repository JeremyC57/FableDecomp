// Behaviour test for 0x00581765: the reconstructed body (pragmas stripped) driven through recording stubs.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
// CTattooDef::IsInvalidCustomTattoo() const -- a custom tattoo (BankIndex -1) is invalid when its file is missing from the profile's save directory, when the base file is missing from the custom-tattoo directory, or when the two files compare equal; banked tattoos are never invalid
// Leaf: IsInvalidCustomTattoo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581765, length 428 bytes (`ret`). Mangled (Ego_r.pdb): ?IsInvalidCustomTattoo@CTattooDef@@QBE_NXZ.
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0; GetValidProfileName 0x00406350, the save directory 0x0041A650 and
//   the custom-tattoo directory 0x0041A6A0 (all __fastcall, hidden CWideString result in ecx); operator+(CWideString, CWideString)
//   0x0099BE70 / (CWideString, const wchar_t*) 0x0099BF30 (__fastcall); CCharString::ToWideString 0x0099E480; PathExists 0x00999230;
//   CDiskFileWin32 ctor 0x0099AD80 and its out-of-line dtor copy 0x00401630 (manifest "~CAIStateGroup_CreatureUseCoverBase",
//   COMDAT-folded); the two-file compare 0x00998F60 (__fastcall; manifest "IsInvalidCustomTattoo _global" is propagated).
// Data: L"\\" 0x0122DE78.
// Layout: CTattooDef BankIndex +0x34, CustomFileName +0x4c, CustomFileNameBase +0x50 (shared header CTattooDef.h offsets; the header
//   flattens the strings to PStringData pointers and declares no member functions, so `this` is a local projection).
// Shape: the singleton pointer is a named local (Get() runs before the L"\\" push, `lea edx,[eax+8]`); the two existence tests
//   nest (the first result parks in a frame byte across the temporaries' destructors, the second in bl); every path ends in one
//   `return true`, except the files-differ exit.
// Opt: odd catalog address, frame pointer: `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
#include <stddef.h>                                         // wchar_t
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                         // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);        // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);            // 0x0099BF30
bool __fastcall PathExists(const CWideString& path);        // 0x00999230
CWideString __fastcall GetValidProfileName(const CWideString& name);   // 0x00406350
namespace NResourceDirectoryNames {
CWideString __fastcall GetSaveDir();                        // 0x0041A650
CWideString __fastcall GetCustomTattooDir();                // 0x0041A6A0
}
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CWideString ToWideString() const;                       // 0x0099E480
};
struct CUserProfileManager {
    unsigned char _pad00[8];
    CWideString ProfileName;                                // +0x08
    static CUserProfileManager* Get();                      // 0x0040D2A0
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CDiskFileWin32 {                                     // 0x1c bytes
    void* __vftable;
    unsigned char _pad04[0x18];
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32();                                      // 0x00401630
};
bool __fastcall FilesAreIdentical(CDiskFileWin32& a, CDiskFileWin32& b);   // 0x00998F60 (name inferred from use)
struct CTattooDef {
    void* __vftable;
    unsigned char _pad04[0x30];
    long BankIndex;                                         // +0x34
    unsigned char _pad38[0x14];
    CCharString CustomFileName;                             // +0x4c
    CCharString CustomFileNameBase;                         // +0x50
    bool IsInvalidCustomTattoo() const;
};

bool CTattooDef::IsInvalidCustomTattoo() const
{
    if (BankIndex != -1)
        return false;
    CUserProfileManager* profiles = CUserProfileManager::Get();
    CWideString profileDir = NResourceDirectoryNames::GetSaveDir() + GetValidProfileName(profiles->ProfileName) + L"\\";
    if (PathExists(profileDir + CustomFileName.ToWideString())) {
        CWideString baseDir = NResourceDirectoryNames::GetCustomTattooDir() + L"\\";
        CDiskFileWin32 file(profileDir + CustomFileName.ToWideString(), FILE_ACCESS_READ, 2);
        if (PathExists(baseDir + CustomFileNameBase.ToWideString())) {
            CDiskFileWin32 baseFile(baseDir + CustomFileNameBase.ToWideString(), FILE_ACCESS_READ, 2);
            if (!FilesAreIdentical(file, baseFile))
                return false;
        }
    }
    return true;
}

struct CWideStringData { int tag; };
static CWideStringData g_tags[8];
static int g_dtors = 0, g_pathCalls = 0, g_fileCtors = 0, g_fileDtors = 0;
static bool g_exists[2], g_identical;
CWideString::~CWideString() { ++g_dtors; }
static CWideString Make(int tag) { CWideString w; w.PStringData = &g_tags[tag]; return w; }
CWideString __fastcall operator+(const CWideString& a, const CWideString& b) { return Make(5); }
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b) { return Make(b[0] == L'\\' ? 6 : 7); }
bool __fastcall PathExists(const CWideString& path) { return g_exists[g_pathCalls++]; }
CWideString __fastcall GetValidProfileName(const CWideString& name) { return Make(1); }
CWideString __fastcall NResourceDirectoryNames::GetSaveDir() { return Make(2); }
CWideString __fastcall NResourceDirectoryNames::GetCustomTattooDir() { return Make(3); }
CWideString CCharString::ToWideString() const { return Make(4); }
static CUserProfileManager g_profiles;
CUserProfileManager* CUserProfileManager::Get() { return &g_profiles; }
CDiskFileWin32::CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags) { ++g_fileCtors; }
CDiskFileWin32::~CDiskFileWin32() { ++g_fileDtors; }
bool __fastcall FilesAreIdentical(CDiskFileWin32& a, CDiskFileWin32& b) { return g_identical; }

static int g_fail = 0;
static void Check(bool c, const char* what) { if (!c) { printf("FAIL %s\n", what); g_fail = 1; } }
static bool Run(long bank, bool e0, bool e1, bool same)
{
    CTattooDef def;
    memset(&def, 0, sizeof(def));
    def.BankIndex = bank;
    g_exists[0] = e0; g_exists[1] = e1; g_identical = same;
    g_pathCalls = 0; g_fileCtors = 0; g_fileDtors = 0;
    return def.IsInvalidCustomTattoo();
}
int main()
{
    Check(Run(3, true, true, true) == false, "banked tattoo is valid");
    Check(g_pathCalls == 0, "banked tattoo touches no file");
    Check(Run(-1, false, true, false) == true, "missing custom file is invalid");
    Check(g_pathCalls == 1 && g_fileCtors == 0, "stops after the first test");
    Check(Run(-1, true, false, false) == true, "missing base file is invalid");
    Check(g_pathCalls == 2 && g_fileCtors == 1 && g_fileDtors == 1, "one file opened and closed");
    Check(Run(-1, true, true, true) == true, "identical files are invalid");
    Check(g_fileCtors == 2 && g_fileDtors == 2, "both files closed");
    Check(Run(-1, true, true, false) == false, "different files are valid");
    Check(g_fileCtors == 2 && g_fileDtors == 2, "both files closed on the valid path");
    if (!g_fail) printf("00581765_TEST PASS\n");
    return g_fail;
}
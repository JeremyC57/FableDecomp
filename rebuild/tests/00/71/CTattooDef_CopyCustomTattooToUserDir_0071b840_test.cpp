// Behaviour test for 0x0071B840: the reconstructed body (pragmas stripped) driven through recording stubs.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
// CTattooDef::CopyCustomTattooToUserDir() const -- copy a custom tattoo's base file from the custom-tattoo directory into the profile's save directory unless it is already there; false when the base file is missing or the disk has no room
// Leaf: CopyCustomTattooToUserDir
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071B840, length 439 bytes (`ret`). Mangled (Ego_r.pdb):
//   ?CopyCustomTattooToUserDir@CTattooDef@@QBE_NXZ.
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0; GetValidProfileName 0x00406350, the save directory 0x0041A650, the
//   custom-tattoo directory 0x0041A6A0 (__fastcall, hidden CWideString result); operator+ 0x0099BE70 / 0x0099BF30 (__fastcall);
//   CCharString::ToWideString 0x0099E480; PathExists 0x00999230; IsRoomForFile 0x00999AB0 (__fastcall, the source length by value);
//   CDiskFileWin32 ctor 0x0099AD80 and its inlined dtor (vtable 0x0122D06C, Close 0x0099A920, ~CWideString 0x0099B510, ~CBase
//   0x0099A300; landed SaveBinary 0x006F92A0 model); CopyFile(W) 0x00998E70 (__fastcall).
// Data: L"\\" 0x0122DE78.
// Layout: CTattooDef CustomFileName +0x4c, CustomFileNameBase +0x50 (shared header CTattooDef.h offsets; local projection because the
//   generated header flattens the strings and declares no member functions).
// Shape: the singleton pointer is a named local; the two path strings share the outer scope (their frame words stay apart from the
//   first expression's five temporaries); the copy is the one nested block, with `return true` inside it and `return false` after.
#include <stddef.h>
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    ~CWideString();                                         // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);        // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);            // 0x0099BF30
bool __fastcall PathExists(const CWideString& path);        // 0x00999230
bool __fastcall IsRoomForFile(long size, const CWideString& path);   // 0x00999AB0
CWideString __fastcall GetValidProfileName(const CWideString& name);   // 0x00406350
namespace NResourceDirectoryNames {
CWideString __fastcall GetSaveDir_0041a650();                        // 0x0041A650
CWideString __fastcall GetCustomTattooDir();                // 0x0041A6A0
}
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CWideString ToWideString() const;                       // 0x0099E480
};
class CUserProfileManager { public:
    unsigned char _pad00[8];
    CWideString ProfileName;                                // +0x08
    static CUserProfileManager* Get();                      // 0x0040D2A0
};
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                               // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CDiskFileWin32 : public CBaseClassNonCopyable { public:                             // vtable 0x0122D06C, 0x1c bytes
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                                   // 0x0099A920 (slot 2)
    long        AccessType;                                 // +0x04
    long        Position;                                   // +0x08
    long        Length;                                     // +0x0c
    void*       WinFileHandle;                              // +0x10
    bool        Opened;                                     // +0x14
    CWideString Pathname;                                   // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
};
void __fastcall CopyFileW(CDiskFileWin32& from, CDiskFileWin32& to);   // 0x00998E70
class CTattooDef { public:
    void* __vftable;
    unsigned char _pad04[0x30];
    long BankIndex;                                         // +0x34
    unsigned char _pad38[0x14];
    CCharString CustomFileName;                             // +0x4c
    CCharString CustomFileNameBase;                         // +0x50
    bool CopyCustomTattooToUserDir() const;
};

bool CTattooDef::CopyCustomTattooToUserDir() const
{
    CUserProfileManager* profiles = CUserProfileManager::Get();
    CWideString destination = NResourceDirectoryNames::GetSaveDir_0041a650() + GetValidProfileName(profiles->ProfileName) + L"\\" + CustomFileName.ToWideString();
    if (PathExists(destination))
        return true;
    CWideString source = NResourceDirectoryNames::GetCustomTattooDir() + CustomFileNameBase.ToWideString();
    if (!PathExists(source))
        return false;
    CDiskFileWin32 from(source, FILE_ACCESS_READ, 2);
    if (IsRoomForFile(from.Length, destination)) {
        CDiskFileWin32 to(destination, FILE_ACCESS_WRITE, 2);
        CopyFileW(from, to);
        return true;
    }
    return false;
}

class CWideStringData { public: int tag; };
static CWideStringData g_tags[8];
static int g_pathCalls = 0, g_opens = 0, g_closes = 0, g_copies = 0, g_lastAccess = 0;
static bool g_exists[2], g_room;
static long g_roomSize = 0;
CWideString::~CWideString() {}
static CWideString Make(int tag) { CWideString w; w.PStringData = &g_tags[tag]; return w; }
CWideString __fastcall operator+(const CWideString& a, const CWideString& b) { return Make(5); }
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b) { return Make(6); }
bool __fastcall PathExists(const CWideString& path) { return g_exists[g_pathCalls++]; }
bool __fastcall IsRoomForFile(long size, const CWideString& path) { g_roomSize = size; return g_room; }
CWideString __fastcall GetValidProfileName(const CWideString& name) { return Make(1); }
CWideString __fastcall NResourceDirectoryNames::GetSaveDir_0041a650() { return Make(2); }
CWideString __fastcall NResourceDirectoryNames::GetCustomTattooDir() { return Make(3); }
CWideString CCharString::ToWideString() const { return Make(4); }
static CUserProfileManager g_profiles;
CUserProfileManager* CUserProfileManager::Get() { return &g_profiles; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() { ++g_closes; }
CDiskFileWin32::CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags)
{ ++g_opens; g_lastAccess = access; AccessType = access; Length = 1234; Pathname.PStringData = 0; }
void __fastcall CopyFileW(CDiskFileWin32& from, CDiskFileWin32& to) { if (from.AccessType == FILE_ACCESS_READ && to.AccessType == FILE_ACCESS_WRITE) ++g_copies; }

static int g_fail = 0;
static void Check(bool c, const char* what) { if (!c) { printf("FAIL %s\n", what); g_fail = 1; } }
static bool Run(bool e0, bool e1, bool room)
{
    CTattooDef def;
    memset(&def, 0, sizeof(def));
    g_exists[0] = e0; g_exists[1] = e1; g_room = room;
    g_pathCalls = 0; g_opens = 0; g_closes = 0; g_copies = 0; g_lastAccess = 0;
    return def.CopyCustomTattooToUserDir();
}
int main()
{
    Check(Run(true, false, false) == true, "already copied");
    Check(g_pathCalls == 1 && g_opens == 0, "no file opened when present");
    Check(Run(false, false, true) == false, "missing base file fails");
    Check(g_pathCalls == 2 && g_opens == 0, "no file opened when the base is missing");
    Check(Run(false, true, false) == false, "no room fails");
    Check(g_opens == 1 && g_closes == 1 && g_roomSize == 1234, "source opened, sized and closed");
    Check(Run(false, true, true) == true, "copy succeeds");
    Check(g_opens == 2 && g_closes == 2 && g_copies == 1 && g_lastAccess == FILE_ACCESS_WRITE, "copied once into a write file");
    if (!g_fail) printf("0071b840_TEST PASS\n");
    return g_fail;
}
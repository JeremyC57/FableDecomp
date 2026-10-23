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
struct CWideStringData;
struct CWideString {
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
namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                               // 0x0099A300
}; } using N_0099a300::CBase;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CDiskFileWin32 : CBase {                             // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
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
struct CTattooDef {
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
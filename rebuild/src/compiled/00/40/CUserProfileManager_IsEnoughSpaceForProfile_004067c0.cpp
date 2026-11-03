// CUserProfileManager::IsEnoughSpaceForProfile() const -- is there room for a profile (0x53C000 bytes) in the user's data folder
// Leaf: ?IsEnoughSpaceForProfile@CUserProfileManager@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004067C0, 39 bytes (+ int3 padding), `ret`.
// Identity: Ego_r.pdb 0x00405420 / FableWin.pdb 0x0187FA40 ?IsEnoughSpaceForProfile@CUserProfileManager@@QBE_NXZ (the
// catalog name); `this` is not read.
// Callees (naming wave 34; the earlier lane landed a CTemp_ctor / CSpaceChecker stand-in that modelled the size immediate as a
// global): NResourceDirectoryNames::GetUserDataDir @ 0x0041A4C0 (__fastcall, CWideString returned through the hidden pointer),
// CAFile::IsRoomForFile(long, const CWideString&) @ 0x00999AB0 (static __fastcall: size in ecx, path in edx; Ego_r / FableWin
// ?IsRoomForFile@CAFile@@SI_NJABVCWideString@@@Z), CWideString::~CWideString @ 0x0099B510.
// Retail bytes:  push ecx ; push ebx ; lea ecx,[esp+4] ; call GetUserDataDir ; mov edx,eax ; mov ecx,0x53C000 ;
//   call IsRoomForFile ; lea ecx,[esp+4] ; mov bl,al ; call ~CWideString ; mov al,bl ; pop ebx ; pop ecx ; ret
#include "fable_string.h"

namespace NResourceDirectoryNames {
    CWideString __fastcall GetUserDataDir();          // 0x0041A4C0
}
class CAFile { public:
    static bool __fastcall IsRoomForFile(long size, const CWideString& path);   // 0x00999AB0
};
class CUserProfileManager { public:
    bool IsEnoughSpaceForProfile() const;
};

bool CUserProfileManager::IsEnoughSpaceForProfile() const
{
    return CAFile::IsRoomForFile(0x53C000, NResourceDirectoryNames::GetUserDataDir());
}

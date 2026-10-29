// CDriveManager::GetDriveFromPath(const wchar_t* path) -- split the path with _wsplitpath and return the first character of its drive component
// Leaf: ?GetDriveFromPath@CDriveManager@@AAEGPBG@Z
// Flags: /GS (no /Zc:wchar_t: retail's wchar_t is the <stddef.h> unsigned short typedef, owner 2026-10-29)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4C70, 53 bytes, thiscall `ret 4`, /GS cookie frame.
// Identity: Ego_r.pdb ?GetDriveFromPath@CDriveManager@@AAE_WPB_W@Z (0xB36710, between IsPathDirectory and GetItemFromPath);
//   the catalog label already names it. No direct callers (the private helper's users are inlined or ICF-folded).
// Callees: _wsplitpath @ 0x00BFED4C (CRT, bound __wsplitpath), __security_check_cookie @ 0x00BFE9F9; the cookie
//   ___security_cookie @ 0x0139C8A8.
// Shape: a 3-character drive buffer below the cookie (`sub esp,0Ch`); dir/fname/ext are null. The PDB `_W` lands in the
//   G form (wchar_t is the unsigned short typedef).
#include <stddef.h>
extern "C" void __cdecl _wsplitpath(const wchar_t* path, wchar_t* drive, wchar_t* dir, wchar_t* fname, wchar_t* ext);   // 0x00BFED4C
class CSystemManager;
class CDriveManager { public:
    void*           Vtbl;                          // +0x00
    CSystemManager* SystemManager;                 // +0x04
private:
    wchar_t GetDriveFromPath(const wchar_t* path);
};

wchar_t CDriveManager::GetDriveFromPath(const wchar_t* path)
{
    wchar_t drive[3];
    _wsplitpath(path, drive, 0, 0, 0);
    return drive[0];
}
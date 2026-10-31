// CDriveManager::CreateDirectoryW(const CWideString& path) -- create every missing directory along the full path: check and select the path's drive (restored at the end), then change into each path item in turn, making it when the change fails; 0 when a directory cannot be made or entered, else the drive check's result
// Leaf: ?CreateDirectoryW@CDriveManager@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4DA0, length 294 bytes (`ret 4`; /GS cookie @ 0x0139C8A8).
// Identity: Ego_r.pdb ?CreateDirectoryW@CDriveManager@@QAE?AW4EDriveReturn@@ABVCWideString@@@Z (0xB36830, after the private inlines
//   ConvertDriveNumberToLetter / ConvertDriveLetterToNumber 0xB36800/0xB36810 and DriveExists). The PDB `_W` lands in the G form
//   (wchar_t is the unsigned short typedef, as the landed siblings).
// Callees: CAFile::ConvertPathToFullPath @ 0x00999110 (static `__fastcall`, hidden CWideString result in ecx, the path in edx),
//   CWideString::operator const wchar_t*() const @ 0x0099AF00, _wsplitpath @ 0x00BFED4C, CDriveManager::DriveExists(wchar_t) @ 0x009E4AE0
//   (landed), _getdrive @ 0x00BFED70, _chdrive @ 0x00BFED6A, CDriveManager::GetItemFromPath(const wchar_t*, unsigned long, wchar_t*)
//   @ 0x009E4CB0 (private), _wchdir @ 0x00BFEAFE, _wmkdir @ 0x00BFED64, CWideString::~CWideString @ 0x0099B510,
//   __security_check_cookie @ 0x00BFE9F9.
// Layout: engine/CDriveManager.h declares the two words but no methods (and packs the class), so the class is modelled locally with
//   the same layout, as the landed siblings DriveExists 0x009E4AE0 / DeleteDirectory 0x009E4B70 / GetDriveFromPath 0x009E4C70.
// Shape (what matched): the PDB's private helpers. GetDriveFromPath (landed out of line at 0x009E4C70) is inlined into the caller:
//   its 3-character drive buffer sits in the caller frame. ConvertDriveLetterToNumber returns a long, so the
//   letter-to-number step is int arithmetic (`add edi,-40h`, imm8) narrowed back into the wchar_t; a plain 16-bit `- '@'` folds to
//   `add esi,0FFC0h` and swaps the this/letter registers. The conversion statement sits before `_getdrive()` (retail schedules the
//   add ahead of that call), and `result` is declared before the full path (`xor ebx,ebx` ahead of the first call). The item loop
//   is a counted `for` whose `== true` test is rotated to the bottom.
typedef unsigned short wchar16;

extern "C" void __cdecl _wsplitpath(const wchar16* path, wchar16* drive, wchar16* dir, wchar16* fname, wchar16* ext);   // 0x00BFED4C
extern "C" int __cdecl _getdrive();                   // 0x00BFED70
extern "C" int __cdecl _chdrive(int drive);           // 0x00BFED6A
extern "C" int __cdecl _wchdir(const wchar16* path);  // 0x00BFEAFE
extern "C" int __cdecl _wmkdir(const wchar16* path);  // 0x00BFED64

class CWideString { public:
    wchar16* m_pchData;
    operator const wchar16*() const;                  // 0x0099AF00
    ~CWideString();                                   // 0x0099B510
};
class CAFile {
public:
    static CWideString __fastcall ConvertPathToFullPath(const CWideString&);   // 0x00999110
};

enum EDriveReturn { };                                // enumerator names not in the PDB name sets; 0 / 1 / 3 used by the bodies

class CSystemManager;

class CDriveManager { public:
    void*           __vftable;                        // +0x00
    CSystemManager* SystemManager;                    // +0x04
    EDriveReturn DriveExists(wchar16 driveLetter);    // 0x009E4AE0
    EDriveReturn CreateDirectoryW(const CWideString& path);
private:
    long ConvertDriveLetterToNumber(wchar16 driveLetter) { return driveLetter - L'@'; }
    wchar16 GetDriveFromPath(const wchar16* path)     // 0x009E4C70 (landed out of line; inlined here)
    {
        wchar16 drive[3];
        _wsplitpath(path, drive, 0, 0, 0);
        return drive[0];
    }
    bool GetItemFromPath(const wchar16* path, unsigned long index, wchar16* item);   // 0x009E4CB0
};

EDriveReturn CDriveManager::CreateDirectoryW(const CWideString& path)
{
    EDriveReturn result = (EDriveReturn)0;
    CWideString fullPath = CAFile::ConvertPathToFullPath(path);
    wchar16 driveNumber = GetDriveFromPath(fullPath);
    long oldDrive = -1;
    if (driveNumber) {
        result = DriveExists(driveNumber);
        driveNumber = (wchar16)ConvertDriveLetterToNumber(driveNumber);
        oldDrive = _getdrive();
        _chdrive(driveNumber);
    }
    if (!driveNumber || result == 1) {
        wchar16 item[262];
        for (unsigned long index = 0; GetItemFromPath(fullPath, index, item) == true; ++index) {
            if (_wchdir(item) != 0) {
                if (_wmkdir(item) == -1 || _wchdir(item) != 0) {
                    result = (EDriveReturn)0;
                    break;
                }
            }
        }
    }
    if (oldDrive != -1)
        _chdrive(oldDrive);
    return result;
}
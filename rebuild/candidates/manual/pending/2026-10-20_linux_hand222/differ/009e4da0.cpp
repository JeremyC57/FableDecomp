// Grade: DIFFER(294v294) -- (1) this/driveNumber colouring esi<->edi swapped; (2) the drive-letter-to-number step: retail `add edi,-40h` (imm8, int arithmetic) where the lane folds the 16-bit result into `add esi,0FFC0h` (imm32), the 3 bytes coming back as loop-head padding (retail `jmp` over `lea esp,[esp]`). Fixed by this form: the 0x21c frame (item buffer 262 wide chars) and the c_str conversion before the _wsplitpath pushes. Tried: self-subtraction, a separate `letter + -0x40`, a 32-bit variable with 16-bit views.
// CDriveManager::CreateDirectoryW(const CWideString& path) -- create every missing directory along the full path: check and select the path's drive (restored at the end), then change into each path item in turn, making it when the change fails; 0 when a directory cannot be made or entered, else the drive check's result
// Leaf: CreateDirectoryW
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4DA0, length 291 bytes (`ret 4`; /GS cookie @ 0x0139C8A8).
// Callees: ConvertPathToFullPath @ 0x00999110 (`__fastcall`, hidden CWideString result in ecx, path in edx), CWideString::operator
// const wchar_t* @ 0x0099AF00, _wsplitpath @ 0x00BFED4C, CDriveManager::DriveExists(wchar_t) @ 0x009E4AE0 (landed), _getdrive @ 0x00BFED70,
// _chdrive @ 0x00BFED6A, CDriveManager::GetItemFromPath(const wchar_t*, unsigned long, wchar_t*) @ 0x009E4CB0, _wchdir @ 0x00BFEAFE,
// _wmkdir @ 0x00BFED64, CWideString::~CWideString @ 0x0099B510, __security_check_cookie @ 0x00BFE9F9.
// Layout: as the landed CDriveManager siblings (DeleteDirectory 0x009E4B70, DriveExists 0x009E4AE0): engine/CDriveManager.h has no
// methods, so the two-word class is modelled locally; wchar_t is `unsigned short` under the era defaults.
// Frame: the full path (+0), the drive buffer (+4, _MAX_DRIVE), the item buffer (+0xc, MAX_PATH wide characters), the cookie.
// Shape: the result doubles as the zero for the _wsplitpath NULLs; the drive letter becomes the drive number in place (`add edi,-40h`)
// and is re-tested with the drive result; the item loop is a counted `for` whose `== true` test is rotated to the bottom.
typedef unsigned short wchar16;

extern "C" void __cdecl _wsplitpath(const wchar16* path, wchar16* drive, wchar16* dir, wchar16* fname, wchar16* ext);   // 0x00BFED4C
extern "C" int __cdecl _getdrive();               // 0x00BFED70
extern "C" int __cdecl _chdrive(int drive);       // 0x00BFED6A
extern "C" int __cdecl _wchdir(const wchar16* path);   // 0x00BFEAFE
extern "C" int __cdecl _wmkdir(const wchar16* path);   // 0x00BFED64

struct CWideString {
    wchar16* m_pchData;
    operator const wchar16*();                     // 0x0099AF00
    ~CWideString();                                // 0x0099B510
};
CWideString __fastcall ConvertPathToFullPath(const CWideString& path);   // 0x00999110

struct CSystemManager;

struct CDriveManager {
    void*           __vftable;                     // +0x00
    CSystemManager* SystemManager;                 // +0x04
    long DriveExists(wchar16 driveLetter);         // 0x009E4AE0
    bool GetItemFromPath(const wchar16* path, unsigned long index, wchar16* item);   // 0x009E4CB0
    long CreateDirectoryW(const CWideString& path);
};

long CDriveManager::CreateDirectoryW(const CWideString& path)
{
    CWideString fullPath = ConvertPathToFullPath(path);
    wchar16 drive[3];
    long result = 0;
    const wchar16* fullPathText = fullPath;
    _wsplitpath(fullPathText, drive, 0, 0, 0);
    int oldDrive = -1;
    wchar16 driveNumber = drive[0];
    if (driveNumber) {
        result = DriveExists(driveNumber);
        oldDrive = _getdrive();
        driveNumber = driveNumber - '@';
        _chdrive(driveNumber);
    }
    if (!driveNumber || result == 1) {
        wchar16 item[262];
        for (unsigned long index = 0; GetItemFromPath(fullPath, index, item) == true; ++index) {
            if (_wchdir(item) != 0) {
                if (_wmkdir(item) == -1 || _wchdir(item) != 0) {
                    result = 0;
                    break;
                }
            }
        }
    }
    if (oldDrive != -1)
        _chdrive(oldDrive);
    return result;
}

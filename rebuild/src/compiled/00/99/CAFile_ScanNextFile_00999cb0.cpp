// CAFile::ScanNextFile(const CWideString& pattern, CFileScanInfo& info, CWideString& nextFile) -- CWideString overload: fetch the next wildcard match into a stack buffer and copy it out
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999CB0, 93 bytes. FableWin.pdb
// ?ScanNextFile@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@AAV2@@Z: a static `__fastcall` (pattern in ecx,
// info in edx, nextFile on the stack, `ret 4`).
// Built /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9) -- the 0x20C-byte buffer is a /GS array.
// Callees: CAFile::ScanNextFile(const CWideString&, CFileScanInfo&, wchar_t*) @ 0x00999850 (landed as
// "CAFile::FindNextFileSequential"; ecx/edx passed through untouched), CWideString::operator=(const wchar_t*) @ 0x0099B800.
// Shape: the twin of StartFileScan(CWideString&) 0x00999C50; a 262-character (_MAX_PATH + 2) buffer; each return carries its own cookie check.
// Leaf: ?ScanNextFile@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@AAV2@@Z
// Flags: /GS
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

struct CWideStringData;
class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    const CWideString& operator=(const wchar16* text);  // 0x0099B800
};

class CAFile
{
public:
    class CFileScanInfo {
    public:
        bool Stopped;                                   // +0x00
        long SearchHandle;                              // +0x04
    };
    static bool __fastcall ScanNextFile(const CWideString& pattern, CFileScanInfo& info, wchar16* nextFile);         // 0x00999850
    static bool __fastcall ScanNextFile(const CWideString& pattern, CFileScanInfo& info, CWideString& nextFile);
};

bool __fastcall CAFile::ScanNextFile(const CWideString& pattern, CFileScanInfo& info, CWideString& nextFile)
{
    wchar16 fileName[262];
    if (ScanNextFile(pattern, info, fileName)) {
        nextFile = fileName;
        return true;
    }
    return false;
}
// CAFile::StartFileScan(const CWideString& pattern, CFileScanInfo& info, CWideString& firstFile) -- CWideString overload: start a wildcard scan into a stack buffer and copy the first match out
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999C50, 93 bytes. FableWin.pdb
// ?StartFileScan@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@AAV2@@Z: a static `__fastcall` (pattern in ecx,
// info in edx, firstFile on the stack, `ret 4`). Manifest label "ScanNextFile" is the sibling 0x00999CB0's name.
// Built /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9) -- the 0x20C-byte buffer is a /GS array.
// Callees: CAFile::StartFileScan(const CWideString&, CFileScanInfo&, wchar_t*) @ 0x00999760 (ecx/edx passed through
// untouched), CWideString::operator=(const wchar_t*) @ 0x0099B800.
// Shape: a 262-character (_MAX_PATH + 2) buffer; each return carries its own cookie check.
// Leaf: ?StartFileScan@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@AAV2@@Z
// Flags: /GS
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

class CWideStringData;
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
    static bool __fastcall StartFileScan(const CWideString& pattern, CFileScanInfo& info, wchar16* firstFile);       // 0x00999760
    static bool __fastcall StartFileScan(const CWideString& pattern, CFileScanInfo& info, CWideString& firstFile);
};

bool __fastcall CAFile::StartFileScan(const CWideString& pattern, CFileScanInfo& info, CWideString& firstFile)
{
    wchar16 fileName[262];
    if (StartFileScan(pattern, info, fileName)) {
        firstFile = fileName;
        return true;
    }
    return false;
}
// CAFile::StartFileScan(const CWideString& pattern, CFileScanInfo& info, wchar_t* firstFile) -- open a wildcard search on the expanded pattern and write drive + directory + first match into firstFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999760, 237 bytes. FableWin.pdb
// ?StartFileScan@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PA_W@Z: a static `__fastcall` (pattern in ecx, info
// in edx, firstFile on the stack, `ret 4`). Manifest label "CAFile_FindFirstFile" is descriptive.
// Built /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Callees: CAFile::GetCurrentPath() @ 0x00997F80, CAFile::ExpandPath @ 0x00998C90 (static `__fastcall`, hidden result
// in ecx), ~CWideString @ 0x0099B510, the two CWideString text accessors 0x0099AF10 (for _wfindfirst) and 0x0099AF00
// (for _wsplitpath) -- byte-identical const members, modelled as GetAsCharArray() and operator const wchar_t*() so
// each call keeps its own relocation target -- and the CRT imports _wfindfirst 0x00BFED7C, _wsplitpath 0x00BFED4C,
// wcscpy 0x00BFED76, wcscat 0x00BFED5E.
// Shape: the sibling of the landed ScanNextFile (0x00999850, "FindNextFileSequential"): function-scope buffers
// (_MAX_DRIVE 3, _MAX_DIR 256 wide characters, then the _wfinddata_t), the handle stored before the -1 test, the
// success arm written inside a positive `!= -1` if so the failure arm is placed last, each return with its own destructor and cookie check.
// Layout: CFileScanInfo {bool Stopped +0, long SearchHandle +4} (Ego_r.pdb); _wfinddata_t (32-bit time_t) name at +0x14.
// Leaf: ?StartFileScan@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PAG@Z
// Flags: /GS
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

struct _wfinddata_t {
    unsigned attrib;
    long time_create;
    long time_access;
    long time_write;
    unsigned long size;
    wchar16 name[260];
};
extern "C" long __cdecl _wfindfirst(const wchar16* filespec, _wfinddata_t* data);   // 0x00BFED7C
extern "C" void __cdecl _wsplitpath(const wchar16* path, wchar16* drive, wchar16* dir, wchar16* fname, wchar16* ext);   // 0x00BFED4C
extern "C" wchar16* __cdecl wcscpy(wchar16* dest, const wchar16* src);   // 0x00BFED76
extern "C" wchar16* __cdecl wcscat(wchar16* dest, const wchar16* src);   // 0x00BFED5E

class CWideStringData;
class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    ~CWideString();                                     // 0x0099B510
    operator const wchar16*() const;                    // 0x0099AF00
    const wchar16* GetAsCharArray() const;              // 0x0099AF10
};

class CAFile
{
public:
    class CFileScanInfo {
    public:
        bool Stopped;                                   // +0x00
        long SearchHandle;                              // +0x04
    };
    static CWideString __fastcall GetCurrentPath();                                             // 0x00997F80
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base);  // 0x00998C90
    static bool __fastcall StartFileScan(const CWideString& pattern, CFileScanInfo& info, wchar16* firstFile);
};

bool __fastcall CAFile::StartFileScan(const CWideString& pattern, CFileScanInfo& info, wchar16* firstFile)
{
    wchar16 drive[3];
    wchar16 dir[256];
    _wfinddata_t data;
    CWideString path = ExpandPath(pattern, GetCurrentPath());
    info.SearchHandle = _wfindfirst(path.GetAsCharArray(), &data);
    if (info.SearchHandle != -1) {
        _wsplitpath(path, drive, dir, 0, 0);
        wcscpy(firstFile, drive);
        wcscat(firstFile, dir);
        wcscat(firstFile, data.name);
        info.Stopped = false;
        return true;
    }
    info.Stopped = true;
    return false;
}
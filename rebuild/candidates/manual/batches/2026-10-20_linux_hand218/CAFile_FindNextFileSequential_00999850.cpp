// CAFile::FindNextFileSequential(const CWideString& pattern, SFindFileState& state, wchar_t* outPath) -- next match of a wildcard search: the full path (drive + directory of the expanded pattern + file name) into outPath, or close the search when there is none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999850, 243 bytes, `__fastcall` (pattern in ecx, state in edx, outPath on the stack, `ret 4`).
// Flags: /GS (security cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Callees: GetCurrentWorkingDirectory 0x00997F80 (`__fastcall`, CWideString result through ecx; manifest
// "CInputProcessMain_GetCameraPath" is propagated, landed caller 0x009E4AE0), ExpandPath(const CWideString&, const
// CWideString&) 0x00998C90 (`__fastcall`, hidden result in ecx), ~CWideString 0x0099B510, CWideString::operator const
// wchar_t* 0x0099AF00 (manifest "CCharString::operator char const*" is a propagated byte twin), the CRT imports
// _wfindnext 0x00BFED88, _wsplitpath 0x00BFED4C, wcscpy 0x00BFED76, wcscat 0x00BFED5E, _findclose 0x00BFED82.
// Shape: the expanded pattern (the working-directory temporary dies right after ExpandPath) is held across both
// paths; an unfinished search asks _wfindnext; on success the pattern's drive and directory are split out
// (_MAX_DRIVE 3, _MAX_DIR 256 wide characters) and drive + dir + file name are concatenated into outPath (one
// `add esp,2Ch` for all five CRT calls) and true is returned; on failure the search is marked finished (stored
// before the close call) and closed. The buffers are function-scope locals: block-scoped ones let the working-
// directory temporary share their frame words (a 0x42C frame instead of retail's 0x430).
// Layout: the search state is {bool Finished +0, intptr_t Handle +4}; _wfinddata_t (32-bit time_t) name at +0x14.
// Leaf: FindNextFileSequential
// Flags: /GS
typedef unsigned short wchar16;

struct _wfinddata_t {
    unsigned attrib;
    long time_create;
    long time_access;
    long time_write;
    unsigned long size;
    wchar16 name[260];
};
extern "C" int __cdecl _wfindnext(long handle, _wfinddata_t* data);   // 0x00BFED88
extern "C" int __cdecl _findclose(long handle);                       // 0x00BFED82
extern "C" void __cdecl _wsplitpath(const wchar16* path, wchar16* drive, wchar16* dir, wchar16* fname, wchar16* ext);   // 0x00BFED4C
extern "C" wchar16* __cdecl wcscpy(wchar16* dest, const wchar16* src);   // 0x00BFED76
extern "C" wchar16* __cdecl wcscat(wchar16* dest, const wchar16* src);   // 0x00BFED5E

struct CWideString {
    wchar16* m_pchData;
    operator const wchar16*() const;             // 0x0099AF00
    ~CWideString();                              // 0x0099B510
};
CWideString __fastcall GetCurrentWorkingDirectory();                                    // 0x00997F80
CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base);   // 0x00998C90

struct SFindFileState {
    bool Finished;                               // +0
    long Handle;                                 // +4
};

struct CAFile {
    static bool __fastcall FindNextFileSequential(const CWideString& pattern, SFindFileState& state, wchar16* outPath);
};

bool __fastcall CAFile::FindNextFileSequential(const CWideString& pattern, SFindFileState& state, wchar16* outPath)
{
    wchar16 drive[3];
    wchar16 dir[256];
    _wfinddata_t data;
    CWideString path = ExpandPath(pattern, GetCurrentWorkingDirectory());
    if (!state.Finished) {
        if (_wfindnext(state.Handle, &data) == 0) {
            _wsplitpath(path, drive, dir, 0, 0);
            wcscpy(outPath, drive);
            wcscat(outPath, dir);
            wcscat(outPath, data.name);
            return true;
        }
        state.Finished = true;
        _findclose(state.Handle);
    }
    return false;
}

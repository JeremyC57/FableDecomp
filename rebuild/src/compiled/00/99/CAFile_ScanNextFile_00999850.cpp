// CAFile::ScanNextFile(const CWideString& pattern, CFileScanInfo& info, wchar_t* nextFile) -- next match of a wildcard search: the full path (drive + directory of the expanded pattern + file name) into the buffer, or stop the search when there is none
// Leaf: ?ScanNextFile@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PAG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00999850, 243 bytes, `__fastcall` (pattern in ecx, info in edx, the buffer on the stack, `ret 4`).
// Name: Ego_r.pdb + FableWin.pdb ?ScanNextFile@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PA_W@Z; VC7.1 without /Zc:wchar_t spells the
//   buffer unsigned short (...PAG@Z), the spelling the landed callers (StartFileScan's users, e.g. 0x005955AB, 0x004959E0) already bind.
// /GS: security cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9.
// Callees: CAFile::GetCurrentPath 0x00997F80 and CAFile::ExpandPath(const CWideString&, const CWideString&) 0x00998C90 (Ego_r.pdb
//   static __fastcall, hidden result in ecx), ~CWideString 0x0099B510, CWideString::operator const wchar_t* 0x0099AF00, the CRT imports
//   _wfindnext 0x00BFED88, _wsplitpath 0x00BFED4C, wcscpy 0x00BFED76, wcscat 0x00BFED5E, _findclose 0x00BFED82.
// Shape: the expanded pattern (the working-directory temporary dies right after ExpandPath) is held across both paths; an unfinished
//   search asks _wfindnext; on success drive + dir + file name are concatenated into the buffer and true is returned; on failure the
//   search is marked stopped (stored before the close call) and closed.
// Layout: engine/CAFile.h CFileScanInfo (Ego_r.pdb: Stopped +0, SearchHandle +4); _wfinddata_t (32-bit time_t) name at +0x14.
// NF41 (2026-11-16): was a local CAFile / CWideString / SFindFileState model with the made-up FindNextFileSequential and free
//   GetCurrentWorkingDirectory / ExpandPath callees; now the shared engine/CAFile.h (FABLE_RETAIL_CAFILE + _SCANNEXTFILE + the NF41
//   opt-in FABLE_RETAIL_CAFILE_EXPANDPATH) and fable_string.h, as the landed caller 0x004959E0 includes them.
// Flags: /GS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SCANNEXTFILE
#define FABLE_RETAIL_CAFILE_EXPANDPATH
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CAFile.h"

struct _wfinddata_t {
    unsigned attrib;
    long time_create;
    long time_access;
    long time_write;
    unsigned long size;
    unsigned short name[260];
};
extern "C" int __cdecl _wfindnext(long handle, _wfinddata_t* data);   // 0x00BFED88
extern "C" int __cdecl _findclose(long handle);                       // 0x00BFED82
extern "C" void __cdecl _wsplitpath(const unsigned short* path, unsigned short* drive, unsigned short* dir, unsigned short* fname, unsigned short* ext);   // 0x00BFED4C
extern "C" unsigned short* __cdecl wcscpy(unsigned short* dest, const unsigned short* src);   // 0x00BFED76
extern "C" unsigned short* __cdecl wcscat(unsigned short* dest, const unsigned short* src);   // 0x00BFED5E

bool __fastcall CAFile::ScanNextFile(const CWideString& pattern, CFileScanInfo& info, unsigned short* nextFile)
{
    unsigned short drive[3];
    unsigned short dir[256];
    _wfinddata_t data;
    CWideString path = ExpandPath(pattern, GetCurrentPath());
    if (!info.Stopped) {
        if (_wfindnext(info.SearchHandle, &data) == 0) {
            _wsplitpath(path, drive, dir, 0, 0);
            wcscpy(nextFile, drive);
            wcscat(nextFile, dir);
            wcscat(nextFile, data.name);
            return true;
        }
        info.Stopped = true;
        _findclose(info.SearchHandle);
    }
    return false;
}

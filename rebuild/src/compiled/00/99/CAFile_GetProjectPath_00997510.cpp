// CAFile::GetProjectPath() -- the directory holding the running executable: GetModuleFileNameW, split off drive + directory, glue them back with "%s%s"
// Leaf: ?GetProjectPath@CAFile@@SI?AVCWideString@@XZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00997510, length 146 bytes, /GS frame (0x42C bytes of locals:
// module path[260], drive[10], directory[262]), static fastcall with the hidden result pointer in ecx.
// Callees: GetModuleFileNameW (IAT 0x0143FDC0), _wsplitpath @ 0x00BFED4C, _snwprintf @ 0x00BFED46 (format L"%s%s"
// @ 0x0129A7A4), CWideString::CWideString(const wchar_t*) @ 0x0099B6B0.
// Reference: also landed as rebuild/src/compiled/00/99/CAFile_GetProjectPath_00997510.cpp (shared-header form).
#include <stddef.h>

extern "C" __declspec(dllimport) unsigned long __stdcall GetModuleFileNameW(void* module, wchar_t* fileName,
                                                                             unsigned long size);
extern "C" void __cdecl _wsplitpath(const wchar_t* path, wchar_t* drive, wchar_t* dir, wchar_t* fname,
                                    wchar_t* ext);   // 0x00BFED4C
extern "C" int __cdecl _snwprintf(wchar_t* buffer, size_t count, const wchar_t* format, ...);  // 0x00BFED46

class CWideString
{
public:
    CWideString(const wchar_t* text);   // 0x0099B6B0
    ~CWideString();

private:
    void* Data;
};

class CAFile
{
public:
    static CWideString __fastcall GetProjectPath();
};

CWideString __fastcall CAFile::GetProjectPath()
{
    wchar_t path[260];
    wchar_t drive[10];
    wchar_t dir[262];
    GetModuleFileNameW(0, path, 260);
    _wsplitpath(path, drive, dir, 0, 0);
    _snwprintf(path, 260, L"%s%s", drive, dir);
    path[259] = 0;
    return CWideString(path);
}
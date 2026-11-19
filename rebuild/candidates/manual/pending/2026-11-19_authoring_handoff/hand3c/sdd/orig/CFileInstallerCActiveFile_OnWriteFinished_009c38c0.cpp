// CFileInstaller::CActiveFile::OnWriteFinished() -- once the copy is written: reopen the destination, trim it to the output size, copy the
//   source's write time onto it, close both files, and when the file wants a marker create "<name>_<ext>.mkf" beside it (a failure
//   raises the dirty-disc error)
// Leaf: ?OnWriteFinished@CActiveFile@CFileInstaller@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C38C0, 409 bytes, no frame pointer, no SEH, `ret`. Callees: CloseHandle / CreateFileW /
//   SetFilePointer / SetEndOfFile / GetFileSize / GetFileTime / SetFileTime (IAT), CWideString copy 0x0099B720, operator const
//   wchar_t* 0x0099AF00, ~CWideString 0x0099B510, GetMarkerFileName 0x009C33A0 (file static, below), CTBaseSingleton<CFileInstaller>::Get
//   0x00404440, CCriticalSectionLock 0x00A62920 / 0x00A629A0, GFGetTime 0x009E1BC0 (result discarded: `fstp st(0)`).
// Name: FableWin.pdb ?OnWriteFinished@CActiveFile@CFileInstaller@@QAEXXZ (0x0304B490): the nested CFileInstaller::CActiveFile of the
//   shared engine/CFileInstaller.h, not the free CActiveFile the parked drafts used.
// Shape: FableWin.pdb 0x0304B490 statement for statement (its ASSERTs and the timing debug print compiled out; the print's
//   GFGetTime() call stays); Ego_d locals distance_to_move, src_filetime, time, size_mb, pathname, flags, file_size, dst_filetime,
//   marker_name, mrk_handle. SetDirtyDiscError (0x009C3530) is inline here with the FableWin.pdb 0x0304C9B0 text
//   `Get().ExitThread = Get().ExitThread || on;`: the landed `if (... || error) error = true;` spelling gives DIFFER(409v409) here
//   (eax/ecx/edx rotated by one from 0x009C392B), while this text matches here and at 0x009C3530 and 0x009C4440 alike (measured).
//   ExitThread is volatile in the shared header (retail's dead `mov al,[eax+7Ah]`).
// GetMarkerFileName: FableWin.pdb static `GetMarkerFileName` 0x0304C700 (no decoration: file static; Ego_d locals dst_name,
//   marker_file_drive / _dir / _name / _ext, marker_path). Declared __fastcall (FableWin's default for free functions, @@YI) and
//   noinline: VC7.1 then gives the static its own convention, the result pointer in esi and the path on the stack with `ret 4`, as
//   retail (0x009C39BE `lea esi,[esp+14h] ; call`). A plain static is caller-cleaned (412v409). Its text grades RELOCATION_MATCH at
//   0x009C33A0 from this source.
// Headers: engine/CFileInstaller.h (full PDB model), CAFile.h + FABLE_RETAIL_CAFILE_SPLITPATH_WIDE, CCriticalSectionLock.h,
//   CTBaseSingleton.h + FABLE_RETAIL_CTBASESINGLETON_GET_DECL (retail calls Get), GFGetTime.h. No header change.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CCRITICALSECTIONLOCK
#define FABLE_RETAIL_CTBASESINGLETON_GET_DECL
#include "engine/CFileInstaller.h"
#include "engine/CAFile.h"
#include "engine/CCriticalSectionLock.h"
#include "engine/GFGetTime.h"

struct _FILETIME { unsigned long dwLowDateTime, dwHighDateTime; };
extern "C" __declspec(dllimport) void* __stdcall CreateFileW(const wchar_t* name, unsigned long access, unsigned long share, void* security, unsigned long disposition, unsigned long flags, void* templateFile);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* h);
extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void* h, long distance, long* distanceHigh, unsigned long method);
extern "C" __declspec(dllimport) int __stdcall SetEndOfFile(void* h);
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileSize(void* h, unsigned long* sizeHigh);
extern "C" __declspec(dllimport) int __stdcall GetFileTime(void* h, _FILETIME* creation, _FILETIME* access, _FILETIME* write);
extern "C" __declspec(dllimport) int __stdcall SetFileTime(void* h, const _FILETIME* creation, const _FILETIME* access, const _FILETIME* write);

inline void FABLE_FASTCALL CFileInstaller::SetDirtyDiscError(bool error)
{
    CCriticalSectionLock lock(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
    DirtyDisc = error;
    CTBaseSingleton<CFileInstaller>::Get().ExitThread = CTBaseSingleton<CFileInstaller>::Get().ExitThread || error;
}

static __declspec(noinline) CWideString FABLE_FASTCALL GetMarkerFileName(const CWideString& dst_name)
{
    CWideString marker_file_drive;
    CWideString marker_file_dir;
    CWideString marker_file_name;
    CWideString marker_file_ext;
    CWideString marker_path;
    CAFile::SplitPath(dst_name, &marker_file_drive, &marker_file_dir, &marker_file_name, &marker_file_ext);
    marker_file_ext.Replace(L".", L"_");
    marker_path = marker_file_drive + marker_file_dir + marker_file_name + marker_file_ext + L".mkf";
    return marker_path;
}

void CFileInstaller::CActiveFile::OnWriteFinished()
{
    if (DstFileHandle != (void*)-1)
    {
        CloseHandle(DstFileHandle);
        DstFileHandle = (void*)-1;
        unsigned long flags = 0x08000000;
        CWideString pathname(PFileInfo->DstPath);
        DstFileHandle = CreateFileW(pathname, 0x40000000, 0, 0, 4, flags, 0);
        unsigned long distance_to_move = OutputSize;
        SetFilePointer(DstFileHandle, distance_to_move, 0, 0);
        SetEndOfFile(DstFileHandle);
        long file_size = GetFileSize(DstFileHandle, 0);
        SrcFileHandle = CreateFileW(PFileInfo->SrcPath, 0x80000000, 1, 0, 3, flags, 0);
        _FILETIME src_filetime;
        GetFileTime(SrcFileHandle, 0, 0, &src_filetime);
        SetFileTime(DstFileHandle, 0, 0, &src_filetime);
        _FILETIME dst_filetime;
        GetFileTime(DstFileHandle, 0, 0, &dst_filetime);
        CloseHandle(SrcFileHandle);
        SrcFileHandle = (void*)-1;
        CloseHandle(DstFileHandle);
        DstFileHandle = (void*)-1;
        if (PFileInfo->CreateMarkerFile)
        {
            CWideString marker_name = GetMarkerFileName(PFileInfo->DstPath);
            void* mrk_handle = CreateFileW(marker_name, 0x40000000, 1, 0, 2, flags, 0);
            if (mrk_handle == (void*)-1)
            {
                SetDirtyDiscError(true);
                return;
            }
            CloseHandle(mrk_handle);
            mrk_handle = (void*)-1;
        }
        float size_mb = (float)(OutputSize / 1048576.0);
        float time = (float)(GFGetTime() - StartTime);
    }
}
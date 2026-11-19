// GetMarkerFileName(const CWideString& dst_name) -- the install marker path for a destination file: "<drive><dir><name><ext>.mkf"
//   with every '.' of the extension turned into '_' (C:\d\t.big -> C:\d\t_big.mkf)
// Leaf: ?GetMarkerFileName@@YI?AVCWideString@@ABV1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C33A0, 315 bytes, no frame pointer, no SEH, `ret 4` (VC7.1's own convention for a noinline
//   file static: dst_name on the stack, the result pointer in esi). Extent: entered only from 0x009C38C0 (call at 0x009C39C2); int3
//   padding before it (after the `ret 4` of the function at 0x009C3380) and after its `ret 4` at 0x009C34D8, next function 0x009C34E0.
// Identity: Ego_r.pdb S_LPROC32 `GetMarkerFileName` (file static, CWideString (const CWideString&)); FableWin.pdb
//   ?GetMarkerFileName@@YI?AVCWideString@@ABV1@@Z 0x0304C700 (its __ehhandler / __unwindfunclet labels carry that decoration).
//   Ego_d.pdb locals dst_name, marker_file_drive, marker_file_dir, marker_file_name, marker_file_ext, marker_path.
// Callees: CWideString() 0x0099AED0, CAFile::SplitPath (wide) 0x00997620, CWideString(const wchar_t*) 0x0099B6B0,
//   CWideString::Replace 0x0099C1E0, operator+(const CWideString&, const CWideString&) 0x0099BE70, operator+(const CWideString&,
//   const wchar_t*) 0x0099BF30, CWideString::operator= 0x0099B7D0, CWideString copy 0x0099B720, ~CWideString 0x0099B510.
// Shape: FableWin.pdb 0x0304C700 statement for statement. The TU is the CActiveFile::OnWriteFinished 0x009C38C0 row's (a file
//   static is emitted only beside its caller, as the word-wrap statics 0x00A10240 / 0x00A10170); declared __fastcall (FableWin's
//   @@YI) and noinline, which gives retail's esi-result / `ret 4` convention.
// SetDirtyDiscError text: FableWin.pdb 0x0304C9B0 `Get().ExitThread = Get().ExitThread || on;` (Ego_d.pdb names on, cs), the one
//   spelling every CFileInstaller row shares (0x009C3530 and the rows that inline it).
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

inline void FABLE_FASTCALL CFileInstaller::SetDirtyDiscError(bool on)
{
    CCriticalSectionLock cs(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
    DirtyDisc = on;
    CTBaseSingleton<CFileInstaller>::Get().ExitThread = CTBaseSingleton<CFileInstaller>::Get().ExitThread || on;
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
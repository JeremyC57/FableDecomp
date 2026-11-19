// CFileInstaller::CActiveFile::OnWriteFinished()
// Leaf: ?OnWriteFinished@CActiveFile@CFileInstaller@@QAEXXZ
// Flags: none
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
    CTBaseSingleton<CFileInstaller>::Get().ExitThread = (CTBaseSingleton<CFileInstaller>::Get().ExitThread || error) ? true : false;
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

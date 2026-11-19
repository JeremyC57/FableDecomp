// CFileInstaller::IsFileCopyNecessary(CCountedPointer<const CFileInfo>& file) const -- decide whether an install file must be
// (re)copied: true when either path is missing, when the destination cannot be opened, when a non-compressed file's sizes differ
// (or the destination size cannot be read), or when the modification stamps differ by more than 3 s (30,000,000 x 100 ns);
// an unreadable source raises the dirty-disc error (SetDirtyDiscError(true)) and returns false; IgnoreTimeStampWhenCopying skips
// the stamp test.
// Retail: Fable.exe 0x009C4440, 609 bytes, __fastcall `ret 4`, no frame pointer, no SEH.
// Identity: FableWin.pdb + Ego_r.pdb ?IsFileCopyNecessary@CFileInstaller@@IBE_NAAV?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@@Z.
// Callees: CAFile::PathExists 0x00999230, CWideString::operator const wchar_t* 0x0099AF00, CreateFileW/CloseHandle/GetFileSizeEx
//   (IAT 0x0143FE2C/0x0143FE30/0x0143FD5C), CFileInstaller::SetDirtyDiscError 0x009C3530 (same TU, earlier: its real body sits here
//   `inline`; c2 expands it at the hot first site and calls it at the cold GetFileSizeEx-failure site), CTBaseSingleton<CFileInstaller>::Get
//   0x00404440, CCriticalSectionLock ctor/dtor 0x00A62920/0x00A629A0, CWideString() 0x0099AED0, CAFile::SplitPath (wide) 0x00997620,
//   CWideString::ToCharString 0x0099B2C0, CCharString::GetAsLowerCase 0x0099ED10 (Ego_r 0x00AFA690 / FableWin 0x02F61580), _Rb_tree<CCharString,...>::_M_find<CCharString> 0x009AB5D0
//   (out of line; the tree's const find body is visible), ~CCharString 0x0099EAE0, ~CWideString 0x0099B510,
//   CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber 0x00999320.  Global: CFileInstaller::DirtyDisc 0x013CA814.
// Shape: the paths are plain `file->SrcPath` / `file->DstPath` references (retail spills source then dest to [esp+0x20]/[esp+0x24]
//   at 0x009C4454/0x009C4458). `found` is its own const_iterator statement compared against CompressedFileTypes.end(), with no
//   map-reference local: retail forms &CompressedFileTypes only after ToCharString/GetAsLowerCase (`mov edi,[esp+0x10]; add edi,0x94` at
//   0x009C455A/0x009C455E); a `const map& types = CompressedFileTypes;` local hoists those two above the ToCharString call
//   (to 0x009C4540), and folding find into the `if` compare spills the find result differently (615 v 609 bytes).
//   The size test is two ifs (GetFileSizeEx failure, then the 8-byte memcmp = `repe cmpsd`), which is what gives the stamp
//   test's temporaries ecx:edi; the volatile ExitThread read is retail's dead `mov cl,[eax+7Ah]` in the inlined setter.
// Flags: /d2inlT120 (inline threshold: >=110 expands the setter at the first site only; the default expands neither)
// Leaf: ?IsFileCopyNecessary@CFileInstaller@@IBE_NAAV?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CAFILE_MODIFIEDTIME_WACKY
#define FABLE_RETAIL_CCRITICALSECTIONLOCK
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#include "engine/CFileInstaller.h"
#include "engine/CAFile.h"
#include "engine/CCriticalSectionLock.h"
#include <string.h>
#pragma intrinsic(memcmp)
typedef union _LARGE_INTEGER { struct { unsigned long LowPart; long HighPart; } u; __int64 QuadPart; } LARGE_INTEGER;
extern "C" __declspec(dllimport) void* __stdcall CreateFileW(const wchar_t* name, unsigned long access, unsigned long share, void* security, unsigned long disposition, unsigned long flags, void* templateFile);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* h);
extern "C" __declspec(dllimport) int __stdcall GetFileSizeEx(void* h, LARGE_INTEGER* size);

inline void FABLE_FASTCALL CFileInstaller::SetDirtyDiscError(bool error)
{
    CCriticalSectionLock lock(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
    DirtyDisc = error;
    if (CTBaseSingleton<CFileInstaller>::Get().ExitThread || error)
        error = true;
    CTBaseSingleton<CFileInstaller>::Get().ExitThread = error;
}

bool CFileInstaller::IsFileCopyNecessary(CCountedPointer<CFileInfo const>& file) const
{
    const CWideString& source = file->SrcPath;
    const CWideString& dest = file->DstPath;
    if (!CAFile::PathExists(source) || !CAFile::PathExists(dest))
        return true;
    void* sourceFile = CreateFileW(source, 0x80000000, 1, 0, 3, 0x8000001, 0);
    void* destFile = CreateFileW(dest, 0x80000000, 1, 0, 3, 0x8000001, 0);
    if (sourceFile == (void*)-1) {
        SetDirtyDiscError(true);
        if (destFile != (void*)-1)
            CloseHandle(destFile);
        return false;
    }
    if (destFile == (void*)-1) {
        CloseHandle(sourceFile);
        return true;
    }
    CWideString ext;
    CAFile::SplitPath(source, 0, 0, 0, &ext);
    std::map<CCharString,CCharString>::const_iterator found = CompressedFileTypes.find(ext.ToCharString().GetAsLowerCase());
    if (found == CompressedFileTypes.end()) {
        LARGE_INTEGER sourceSize;
        if (!GetFileSizeEx(sourceFile, &sourceSize)) {
            CloseHandle(sourceFile);
            CloseHandle(destFile);
            SetDirtyDiscError(true);
            return false;
        }
        LARGE_INTEGER destSize;
        if (!GetFileSizeEx(destFile, &destSize)) {
            CloseHandle(sourceFile);
            CloseHandle(destFile);
            return true;
        }
        if (memcmp(&sourceSize, &destSize, sizeof(LARGE_INTEGER)) != 0) {
            CloseHandle(sourceFile);
            CloseHandle(destFile);
            return true;
        }
    }
    CloseHandle(sourceFile);
    CloseHandle(destFile);
    if (IgnoreTimeStampWhenCopying)
        return false;
    unsigned __int64 sourceTime = CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(source);
    unsigned __int64 destTime = CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(dest);
    if (destTime >= sourceTime && destTime - sourceTime > 30000000)
        return true;
    if (sourceTime > destTime && sourceTime - destTime > 30000000)
        return true;
    return false;
}
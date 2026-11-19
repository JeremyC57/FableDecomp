// CFileInstaller::OnFileWriteCompletion(unsigned long err, unsigned long no_bytes, _OVERLAPPED* pover) -- WriteFileEx completion routine: on an I/O error flag a dirty disc (inline SetDirtyDiscError), then under the installer's lock add the bytes written to the active file, retire the write, signal the installer's event and free the write record
// Leaf: ?OnFileWriteCompletion@CFileInstaller@@KGXKKPAU_OVERLAPPED@@@Z
// SetDirtyDiscError text: FableWin.pdb 0x0304C9B0 `Get().ExitThread = Get().ExitThread || on;` (Ego_d.pdb names on, cs), the one
//   spelling every CFileInstaller row shares (0x009C3530 and the rows that inline it).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3820, 151 bytes, `ret 0Ch` (protected static __stdcall).
// Identity: Ego_d.pdb lib_file_installer.obj lists OnFileWriteCompletion (params err / no_bytes / pover, locals cs / pwrite)
//   right after OnFileReadCompletion's twin PostFileReadCompletion; Ego_r.pdb ?OnFileWriteCompletion@CFileInstaller@@KGXKKPAU_OVERLAPPED@@@Z
//   sits after OnFileReadCompletion (donor 0x00B11C60 / 0x00B11CF0 = retail 0x009C37A0 / 0x009C3820); FableWin.pdb same symbol.
// Retail-volatile: 0x009C3853 `mov cl,byte ptr [eax+7Ah]` -- the discarded read of ExitThread in the inline
//   SetDirtyDiscError(true) (shared header: ExitThread is volatile in Ego_d / Ego_r).
// Callees: CTBaseSingleton<CFileInstaller>::Get 0x00404440, CCriticalSectionLock ctor 0x00A62920 / dtor 0x00A629A0,
//   KERNEL32 SetEvent (IAT 0x0143FDE0), operator delete 0x00BFE9BC (CActiveWrite has no destructor); SetDirtyDiscError
//   (out-of-line copy 0x009C3530) expanded inline.
#define FABLE_RETAIL_CCRITICALSECTIONLOCK
#define FABLE_RETAIL_CTBASESINGLETON_GET_DECL
#include "engine/CFileInstaller.h"
#include "engine/CCriticalSectionLock.h"

extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);     // IAT 0x0143FDE0

void FABLE_FASTCALL CFileInstaller::SetDirtyDiscError(bool on)
{
    CCriticalSectionLock cs(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
    DirtyDisc = on;
    CTBaseSingleton<CFileInstaller>::Get().ExitThread = CTBaseSingleton<CFileInstaller>::Get().ExitThread || on;
}

void FABLE_STDCALL CFileInstaller::OnFileWriteCompletion(unsigned long err, unsigned long no_bytes, _OVERLAPPED* pover)
{
    CActiveWrite* pwrite = (CActiveWrite*)pover->hEvent;
    if (err)
        SetDirtyDiscError(true);
    CCriticalSectionLock cs(pwrite->PFileInstaller->CriticalSection, false);
    pwrite->PActiveFile->NoBytesWritten += no_bytes;
    --pwrite->PFileInstaller->NoActiveWrites;
    SetEvent(pwrite->PFileInstaller->Event);
    delete pwrite;
}
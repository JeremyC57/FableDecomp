// CFileInstaller::OnFileReadCompletion(unsigned long err, unsigned long no_bytes, _OVERLAPPED* pover) -- ReadFileEx completion routine: on an I/O error flag a dirty disc (inline SetDirtyDiscError), then under the installer's lock post the finished read and signal the installer's event
// Leaf: ?OnFileReadCompletion@CFileInstaller@@KGXKKPAU_OVERLAPPED@@@Z
// SetDirtyDiscError text: FableWin.pdb 0x0304C9B0 `Get().ExitThread = Get().ExitThread || on;` (Ego_d.pdb names on, cs), the one
//   spelling every CFileInstaller row shares (0x009C3530 and the rows that inline it).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C37A0, 128 bytes, `ret 0Ch` (protected static __stdcall).
// Identity: Ego_d.pdb lib_file_installer.obj lists OnFileReadCompletion (params err / no_bytes / pover, locals cs / pread);
//   Ego_r.pdb ?OnFileReadCompletion@CFileInstaller@@KGXKKPAU_OVERLAPPED@@@Z sits between StartWritingFile and
//   OnFileWriteCompletion (donor 0x00B11B20 / 0x00B11C60 / 0x00B11CF0 = retail 0x009C3660 / 0x009C37A0 / 0x009C3820);
//   FableWin.pdb has the same symbol. Caller: ProcessActiveFileForRead 0x009C3AE0 passes it to ReadFileEx.
// Retail-volatile: 0x009C37D3 `mov cl,byte ptr [eax+7Ah]` -- the discarded read of ExitThread in the inline
//   SetDirtyDiscError(true) (shared header: ExitThread is volatile in Ego_d / Ego_r).
// Callees: CTBaseSingleton<CFileInstaller>::Get 0x00404440, CCriticalSectionLock ctor 0x00A62920 / dtor 0x00A629A0,
//   KERNEL32 SetEvent (IAT 0x0143FDE0); SetDirtyDiscError (out-of-line copy 0x009C3530) expanded inline.
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

void FABLE_STDCALL CFileInstaller::OnFileReadCompletion(unsigned long err, unsigned long no_bytes, _OVERLAPPED* pover)
{
    CActiveRead* pread = (CActiveRead*)pover->hEvent;
    if (err)
        SetDirtyDiscError(true);
    CCriticalSectionLock cs(pread->PFileInstaller->CriticalSection, false);
    pread->PFileInstaller->PCompletedRead = pread;
    SetEvent(pread->PFileInstaller->Event);
}
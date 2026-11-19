// CFileInstaller::SetDirtyDiscError(bool on) -- record a dirty-disc read error and make the installer thread exit
// Leaf: ?SetDirtyDiscError@CFileInstaller@@KIX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3530, 72 bytes, `ret` (protected static __fastcall, on in cl).
// Identity: FableWin.pdb ?SetDirtyDiscError@CFileInstaller@@KIX_N@Z; it is inlined into OnFileReadCompletion 0x009C37A0
//   and OnFileWriteCompletion 0x009C3820 (`SetDirtyDiscError(true)`, the same lock / store / Get pair).
// Callees: CTBaseSingleton<CFileInstaller>::Get 0x00404440 (out of line), CCriticalSectionLock ctor 0x00A62920 /
//   dtor 0x00A629A0.  Global: CFileInstaller::DirtyDisc 0x013CA814 (Ego_r.pdb static member).
// Layout: retail CFileInstaller (Ego_r.pdb minus the 16->12 CArray and 12->4 list shrink): CriticalSection +0x04, ExitThread +0x7A.
// Shape: FableWin.pdb 0x0304C9B0 statement for statement: `Get().ExitThread = Get().ExitThread || on;` (Ego_d.pdb names: on, cs).
//   The same text is inlined by OnFileReadCompletion 0x009C37A0, OnFileWriteCompletion 0x009C3820, StartReadingFile 0x009C3590,
//   StartWritingFile 0x009C3660, CActiveFile::OnWriteFinished 0x009C38C0 and IsFileCopyNecessary 0x009C4440.
// Retail-volatile: 0x009C3555 `mov cl,byte ptr [eax+7Ah]` -- ExitThread is read through the volatile member of the shared header
//   (written by the main thread, polled by the installer threads; see engine/CFileInstaller.h).
#define FABLE_RETAIL_CCRITICALSECTIONLOCK
#define FABLE_RETAIL_CTBASESINGLETON_GET_DECL
#include "engine/CFileInstaller.h"
#include "engine/CCriticalSectionLock.h"

void FABLE_FASTCALL CFileInstaller::SetDirtyDiscError(bool on)
{
    CCriticalSectionLock cs(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
    DirtyDisc = on;
    CTBaseSingleton<CFileInstaller>::Get().ExitThread = CTBaseSingleton<CFileInstaller>::Get().ExitThread || on;
}

// CAInputProcess::RemoveFromInterface()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D2D0, oracle length 33 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je R ; mov eax,[esi] ; call [eax+0Ch] ; mov ecx,[esi+4] ; push esi ; call CInputProcessManager::RemoveInputProcess ; mov dword ptr [esi+4],0 ; R: pop esi ; ret
// engine/CAInputProcess.h (PDB): +0x04 PPlayerInterface (the owning CInputProcessManager).
// If attached: notify self through virtual slot 3 (OnRemovedFromInterface), unregister from the
// manager (0x00A0D780, __fastcall on the manager, argument = this process), then clear the link.
#include "engine/CAInputProcess.h"

struct CInputProcessManagerCalls {
    void RemoveInputProcess(CAInputProcess* process);    // 0x00A0D780
};

struct CAInputProcessVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void OnRemovedFromInterface();                // slot 3 (+0x0C)
};

struct CAInputProcess_Methods : CAInputProcess {
    void RemoveFromInterface();
};

void CAInputProcess_Methods::RemoveFromInterface()
{
    if (this->PPlayerInterface) {
        reinterpret_cast<CAInputProcessVirtuals*>(this)->OnRemovedFromInterface();
        reinterpret_cast<CInputProcessManagerCalls*>(this->PPlayerInterface)->RemoveInputProcess(this);
        this->PPlayerInterface = 0;
    }
}
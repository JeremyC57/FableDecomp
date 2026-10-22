#include "engine/CASuspendableProcess.h"  // retyped onto the PDB layout; byte parity re-verified

struct CASuspendableProcessVTable {
    void (__fastcall *Slot0)(CASuspendableProcess* process);
    void (__fastcall *Pump)(CASuspendableProcess* process);
};

void __fastcall TerminateProcess(CASuspendableProcess* process)
{
    process->StopRunning = true;
    while (process->Running) {
        CASuspendableProcessVTable* vtable =
            (CASuspendableProcessVTable*)process->__vftable;
        vtable->Pump(process);
    }
}

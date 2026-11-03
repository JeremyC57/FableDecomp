#ifndef FABLE_ENGINE_CSUSPENDABLEPROCESSMICROTHREAD_H
#define FABLE_ENGINE_CSUSPENDABLEPROCESSMICROTHREAD_H
// Retail layout (hand-written from Ego_r.pdb: sizeof 0x14, PMicrothread +0x10). RTTI CSuspendableProcessMicrothread :
// CASuspendableProcess : CBaseClassNonCopyable : CBaseClass; vtable 0x0129D440: slot 0 scalar deleting dtor 0x00A44A40,
// 1 RunSuspendableProcess 0x00A44880, 2 SuspendSuspendableProcess 0x00A44840, 3 Restart 0x00A447D0, 4 SuspendableProcess
// (inherited 0x00A44600). Ctor 0x00A44740 (protected), dtor 0x00A44620 (protected virtual). Ego_r.pdb / FableWin.pdb names.
// Only the FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in of engine/CASuspendableProcess.h / CSpawnedFuncBase.h uses it.
#include <stddef.h>
#include "rebuild_abi.h"
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CASuspendableProcess.h"

#if defined(FABLE_RETAIL_OAKVALE) || defined(FABLE_RETAIL_HANGING_TREE)
namespace MicroThread { struct MicroThreadInfo; }
#else
struct MicroThreadInfo;
#endif

class CSuspendableProcessMicrothread : public CASuspendableProcess {
public:
    virtual void RunSuspendableProcess();                                // slot 1  0x00A44880
    virtual bool SuspendSuspendableProcess();                            // slot 2  0x00A44840
    virtual void Restart();                                              // slot 3  0x00A447D0
protected:
    CSuspendableProcessMicrothread(bool, long, float);                   // 0x00A44740
    virtual ~CSuspendableProcessMicrothread();                           // slot 0  0x00A44620
    static void FABLE_FASTCALL InternalSuspend();
    static void FABLE_FASTCALL InternalSwitchToProcess(CSuspendableProcessMicrothread&);
private:
    static void ThreadEntry(void*);
#if defined(FABLE_RETAIL_OAKVALE) || defined(FABLE_RETAIL_HANGING_TREE)
private:
    MicroThread::MicroThreadInfo* PMicrothread;
#else
public:
    MicroThreadInfo* PMicrothread;                                       // +0x10
#endif
};
FABLE_STATIC_ASSERT(sizeof(CSuspendableProcessMicrothread) == 0x14);

#endif // FABLE_ENGINE_CSUSPENDABLEPROCESSMICROTHREAD_H

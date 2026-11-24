#ifndef FABLE_ENGINE_CINPUTPROCESSMANAGER_H
#define FABLE_ENGINE_CINPUTPROCESSMANAGER_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CLinkedList.h"
#include "engine/CProcessedInput.h"
class CGameEvent;class CAInputProcess;
// Layout: complete original Ego19e82/FW1a872,size1784; ten original queued events.
class CInputProcessManager:public CBaseClassNonCopyable{
protected:
 CLinkedList<CAInputProcess> InputProcessList;
 CProcessedInput QueuedProcessedInputs[10];
 long NoQueuedProcessedInputs;
 void AddInputProcess(CAInputProcess & p0);
 void RemoveInputProcess(CAInputProcess & p0);
#ifdef FABLE_RETAIL_CINPUTPROCESSMANAGER_FRIEND_CAINPUTPROCESS // NF325 opt-in, declaration only: retail CAInputProcess::AddToInterface 0x00A0D2B0 calls the protected
 friend class CAInputProcess; // AddInputProcess 0x00A0D4F0 (both PDBs ?AddInputProcess@CInputProcessManager@@IAEXAAVCAInputProcess@@@Z, protected), so the real class is a friend
#endif
public:
 CInputProcessManager(const CInputProcessManager & p0);
 CInputProcessManager();
 virtual ~CInputProcessManager();
 void RemoveAllInputProcesses();
 void StopAllScansAndRemoveAllInputProcesses();
 void AddQueuedProcessedInput(const CProcessedInput & p0);
 void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo & p0) const;
 CInputProcessManager & operator=(const CInputProcessManager & p0);
};
FABLE_STATIC_ASSERT_AT(25, sizeof(CInputProcessManager)==1784);
#endif

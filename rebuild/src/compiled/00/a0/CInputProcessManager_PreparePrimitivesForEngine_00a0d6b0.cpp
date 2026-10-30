// CInputProcessManager::PreparePrimitivesForEngine -- scan active input processes with mutation-safe scan registration
// Retail00A0D6B0 length197. Genuine CAInputProcess virtual slot5; no direct callees.
// No manager/list header exists. PDB CLinkedListScanInfo28 and list16 model retained.
#include "rebuild_abi.h"
#define CAInputProcess DonorCAInputProcess
#include "engine/CAInputProcess.h"
#undef CAInputProcess
#define CInputProcessGameBase DonorCInputProcessGameBase
#include "engine/CInputProcessGameBase.h"
#undef CInputProcessGameBase
#include "engine/CProcessedInput.h"
class CInputEvent; class CEnginePrimitivePrepareInfo;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CBaseClassNonCopyable:CBaseClass {virtual ~CBaseClassNonCopyable();};
class CLinkedListPosition { public:CBaseClass* Data;CLinkedListPosition* Next;CLinkedListPosition* Previous;};
class CInputProcessManager;
class CAInputProcess:public CBaseClassNonCopyable { public:
 CInputProcessManager* PPlayerInterface;
 CLinkedListPosition InputProcessListPos;
 virtual ~CAInputProcess();
 virtual bool AddToInterface(CInputProcessManager&);
 virtual void Init();
 virtual void Cleanup();
 virtual void ProcessInput(const CInputEvent&,CProcessedInput&)=0;
 virtual void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&)const;
 virtual void Update(CProcessedInput&);
 virtual bool ProcessWhilePaused()const;
 virtual bool IsInputEventToBeSentToThisInputProcess(const CInputEvent&)const;
};

class CLinkedListScanInfo;
struct CInputProcessList {
 CLinkedListPosition* Head; CLinkedListPosition* Tail; int NoEntries; mutable CLinkedListScanInfo* FirstScanInfo;
 CAInputProcess* StartScanAtHead(CLinkedListScanInfo*)const;
 CAInputProcess* ScanForward(CLinkedListScanInfo*)const;
 void RemoveScanInfo(CLinkedListScanInfo*)const;
};
class CLinkedListScanInfo { public:
 CLinkedListPosition* Current; CLinkedListPosition* Next; CLinkedListPosition* Previous;
 bool Started,Forwards;
 CLinkedListScanInfo* NextScan; CLinkedListScanInfo* PreviousScan;
 const CInputProcessList* List;
 CLinkedListScanInfo(){}
};
inline CAInputProcess* CInputProcessList::StartScanAtHead(CLinkedListScanInfo* s)const {
 s->NextScan=FirstScanInfo;if(FirstScanInfo)FirstScanInfo->PreviousScan=s;
 FirstScanInfo=s;s->List=this;s->PreviousScan=0;s->Started=true;
 CLinkedListPosition* p=Head;
 if(!p)return 0;
 s->Current=p;s->Next=p->Next;s->Previous=0;
 return static_cast<CAInputProcess*>(p->Data);
}
inline CAInputProcess* CInputProcessList::ScanForward(CLinkedListScanInfo* s)const {
 CLinkedListPosition* p=s->Next;s->Current=p;
 if(!p)return 0;
 s->Next=p->Next;s->Previous=p->Previous;
 return static_cast<CAInputProcess*>(p->Data);
}
inline void CInputProcessList::RemoveScanInfo(CLinkedListScanInfo* s)const {
 if(!s->PreviousScan){
  CLinkedListScanInfo* next=s->NextScan;FirstScanInfo=next;
  if(next)next->PreviousScan=0;
  return;
 }
 s->PreviousScan->NextScan=s->NextScan;
 if(s->NextScan)s->NextScan->PreviousScan=s->PreviousScan;
}
class CInputProcessManager:public CBaseClassNonCopyable { public:
 CInputProcessList InputProcessList;
 CProcessedInput QueuedProcessedInputs[10]; int NoQueuedProcessedInputs;
 void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&)const;
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CLinkedListScanInfo)==28);
FABLE_STATIC_ASSERT(sizeof(CInputProcessList)==16);
FABLE_STATIC_ASSERT(sizeof(CInputProcessManager)==1784);
#endif
void CInputProcessManager::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info)const {
 CLinkedListScanInfo scan;
 for(CAInputProcess* p=InputProcessList.StartScanAtHead(&scan);p;p=InputProcessList.ScanForward(&scan))
  p->PreparePrimitivesForEngine(info);
 InputProcessList.RemoveScanInfo(&scan);
}
// CGamePlayerInterface::GetInputProcessEventFromUpdate(CProcessedInput&) const.
// Actual player/process headers retained; retail list4, player-info6FC, flag894.
#define CGamePlayerInterface CGamePlayerInterfaceDonor
#include "engine/CGamePlayerInterface.h"
#undef CGamePlayerInterface
#include "engine/CAInputProcess.h"
#include <string.h>
#define CProcessedInput CProcessedInputStorage
#include "engine/CProcessedInput.h"
#undef CProcessedInput
// Actual storage matches native B0; field-free adapter restores missing methods.
struct CProcessedInput:CProcessedInputStorage {void Clear();void SetPlayer(unsigned long);}; //A0D300/A0D3A0
#define CPlayerManager CPlayerManagerDonor
#include "engine/CPlayerManager.h"
#undef CPlayerManager
// The donor Players vector is 16 bytes versus retail 12, moving MainPlayer
// from donor20 to native1C. No storage is accessed by this field-free adapter.
struct CPlayerManager:CPlayerManagerDonor {long GetMainPlayerNumber()const;}; //449700
struct CInputNode {CAInputProcess* Data;CInputNode* Next;CInputNode* Previous;};
struct CInputScan;
struct CInputList {CInputNode* Head;CInputNode* Tail;int Count;CInputScan* FirstScan;};
struct CInputScan {
 CInputNode* Current;CInputNode* Next;CInputNode* Previous;bool Started,Forwards;CInputScan* NextScan;CInputScan* PreviousScan;CInputList* List;
 CInputScan():Started(false),NextScan(0),PreviousScan(0),List(0){}
 CAInputProcess* Begin(CInputList& list){NextScan=list.FirstScan;if(list.FirstScan)list.FirstScan->PreviousScan=this;list.FirstScan=this;List=&list;PreviousScan=0;Started=true;if(!list.Head)return 0;Current=list.Head;Next=Current->Next;Previous=0;return Current->Data;}
 CAInputProcess* Advance(){Current=Next;if(!Current)return 0;Next=Current->Next;Previous=Current->Previous;return Current->Data;}
 void End(CInputList& list){if(!PreviousScan){list.FirstScan=NextScan;if(NextScan)NextScan->PreviousScan=0;}else{PreviousScan->NextScan=NextScan;if(NextScan)NextScan->PreviousScan=PreviousScan;}}
};
struct CGamePlayerInterface {
 void* Vtable;mutable CInputList InputProcesses;unsigned char BeforePlayer[0x6e8];CPlayerManager* PlayerManager;unsigned char BeforeProcessed[0x194];mutable bool AlreadyProcessed;
 bool GetInputProcessEventFromUpdate(CProcessedInput&)const;
};
typedef void(CAInputProcess::*CInputUpdateCall)(CProcessedInput&);
#ifdef _MSC_VER
typedef char Input176[sizeof(CProcessedInput)==176?1:-1];
typedef char Scan28[sizeof(CInputScan)==28?1:-1];
typedef char Player6FC[offsetof(CGamePlayerInterface,PlayerManager)==0x6fc?1:-1];
typedef char Flag894[offsetof(CGamePlayerInterface,AlreadyProcessed)==0x894?1:-1];
typedef char PMF4[sizeof(CInputUpdateCall)==4?1:-1];
#endif
bool CGamePlayerInterface::GetInputProcessEventFromUpdate(CProcessedInput& input)const{
 if(!AlreadyProcessed){AlreadyProcessed=true;CInputScan scan;
 input.Clear();input.SetPlayer(PlayerManager->GetMainPlayerNumber());
 for(CAInputProcess* p=scan.Begin(InputProcesses);p;p=scan.Advance()){
 CInputUpdateCall update=0;memcpy(&update,static_cast<const CInputUpdateCall*>(p->__vftable)+6,sizeof(update));(p->*update)(input);
 }
 scan.End(InputProcesses);if(input.NoGameEvents)return true;}return false;
}
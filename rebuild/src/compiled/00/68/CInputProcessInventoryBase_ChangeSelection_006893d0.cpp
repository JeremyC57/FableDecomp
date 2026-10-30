// CInputProcessInventoryBase::ChangeSelection -- reset movement or wait for its category/item repeat interval
// Retail006893D0 length230. CurrentServerFrame416225; FPS global1375550;2^32 pool122DCB4.
// Actual quarantine CTCInventoryBase retained. Its native virtual entries42..45 are
// projected with genuine const float PMFs, native4/full host width, no invented virtual padding.
#define CInputProcessInventoryBase DonorCInputProcessInventoryBase
#include "engine/CInputProcessInventoryBase.h"
#undef CInputProcessInventoryBase
#include "engine/_quarantine/CTCInventoryBase.h"
#include "rebuild_abi.h"
#define CAInputProcess DonorCAInputProcess
#include "engine/CAInputProcess.h"
#undef CAInputProcess
#define CInputProcessGameBase DonorCInputProcessGameBase
#include "engine/CInputProcessGameBase.h"
#undef CInputProcessGameBase
#include "engine/CProcessedInput.h"
class CInputEvent; class CEnginePrimitivePrepareInfo;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CBaseClassNonCopyable:CBaseClass {protected:CBaseClassNonCopyable();public:virtual ~CBaseClassNonCopyable();};
class CLinkedListPosition { public:CBaseClass* Data;CLinkedListPosition* Next;CLinkedListPosition* Previous;};
class CInputProcessManager;
class CAInputProcess:public CBaseClassNonCopyable { public:
 CAInputProcess(); //PDBegorB74B80 genuine default
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

class CGame;class CMainGameComponentInit;
class CMainGameComponent { public: CMainGameComponent(CGame&,const CMainGameComponentInit&); long GetCurrentServerFrame()const; }; //00416225
namespace NInventory {
class CTCInventoryBase;
#pragma pointers_to_members(full_generality,single_inheritance)
union InventorySlot {
 void* Address;
 float(CTCInventoryBase::*RepeatDelay)()const;
};
class CTCInventoryBase : public ::CTCInventoryBase { public: CTCInventoryBase(CThing&); };
}
class CInputProcessGameBaseInit { public:
 CWorld* World;CPlayer* MyPlayer;CGameDefinitionManager* DefinitionManager;CMainGameComponent* Component;
 CInputProcessGameBaseInit(); //PDBfablewin1AC7CF0; no available header
};
struct CInputProcessGameBase:CAInputProcess {
 CInputProcessGameBase(const CInputProcessGameBaseInit&); //PDBegor6B2B40
 CWorld* World;CPlayer* MyPlayer;CGameDefinitionManager* DefinitionManager;
 CMainGameComponent* Component;CGamePlayerInterface* PGamePlayerInterface;
};
struct CInputProcessInventoryBase:CInputProcessGameBase {
 CInputProcessInventoryBase(const CInputProcessGameBaseInit&); //PDBfablewin1AC84D0
 enum ECurrentMovement {MOVEMENT_NONE,MOVEMENT_UP,MOVEMENT_DOWN,MOVEMENT_LEFT,MOVEMENT_RIGHT};
 ECurrentMovement CurrentMovement;unsigned int FrameSinceLastMovement;bool StartedRepeatingMovement;
 virtual NInventory::CTCInventoryBase* GetPInventory()const=0; //slot9
 bool ChangeSelection(ECurrentMovement);
};
extern int g_GameFramesPerSecond; //01375550 signed32
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(NInventory::InventorySlot)==4);
FABLE_STATIC_ASSERT(sizeof(CInputProcessInventoryBase)==sizeof(DonorCInputProcessInventoryBase));
FABLE_STATIC_ASSERT(offsetof(CInputProcessInventoryBase,FrameSinceLastMovement)==0x2c);
#endif
bool CInputProcessInventoryBase::ChangeSelection(ECurrentMovement movement){
 if(CurrentMovement!=movement){
  StartedRepeatingMovement=false;unsigned int frame=Component->GetCurrentServerFrame();CurrentMovement=movement;FrameSinceLastMovement=frame;return true;
 }
 float delay;
 if(movement==MOVEMENT_RIGHT||movement==MOVEMENT_LEFT){
  if(StartedRepeatingMovement){NInventory::CTCInventoryBase* p=GetPInventory();delay=(p->*static_cast<const NInventory::InventorySlot*>(p->__vftable)[45].RepeatDelay)();}
  else{NInventory::CTCInventoryBase* p=GetPInventory();delay=(p->*static_cast<const NInventory::InventorySlot*>(p->__vftable)[44].RepeatDelay)();}
 }else{
  if(StartedRepeatingMovement){NInventory::CTCInventoryBase* p=GetPInventory();delay=(p->*static_cast<const NInventory::InventorySlot*>(p->__vftable)[43].RepeatDelay)();}
  else{NInventory::CTCInventoryBase* p=GetPInventory();delay=(p->*static_cast<const NInventory::InventorySlot*>(p->__vftable)[42].RepeatDelay)();}
 }
 int fps=g_GameFramesPerSecond;
 unsigned int elapsed=(unsigned int)Component->GetCurrentServerFrame()-FrameSinceLastMovement;
 if(elapsed>=fps*delay){StartedRepeatingMovement=true;FrameSinceLastMovement=Component->GetCurrentServerFrame();return true;}
 return false;
}
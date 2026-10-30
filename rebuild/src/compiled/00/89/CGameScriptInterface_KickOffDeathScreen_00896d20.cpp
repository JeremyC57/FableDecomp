// CGameScriptInterface::KickOffDeathScreen() -- activate death completion UI and yield while it remains active
// Retail00896D20 196bytes; uses native TC252 at CThing.Components+44 (donor+48).
//449970 GetMainPlayer,487DC0 GetPControlledCreature,40F020 LowerBound,99B6B0/99B510 wide ctor/dtor,
//99EBF0/99EAE0 narrow ctor/dtor,5E4740 CTCQuestCompletionUI::Activate.
// Interface slots394(88E980 IsHeroControlledByPlayer) and7(6E7410 NewScriptFrame) are audited real slots.
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include <stddef.h>
#include "fable_string.h"
#include "engine/_quarantine/CTCQuestCompletionUI.h"
struct CTCQuestCompletionUIMethods:CTCQuestCompletionUI {void Activate(long,const CCharString&,bool,const CWideString&,bool);};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const ETCInterfaceType& key);};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap;
struct CThingNative {void* VTable;unsigned char Base[0x1c];unsigned long UsingComponents[9];ComponentMap Components;
 __forceinline bool GetQuestCompletionUI(CTCQuestCompletionUIMethods** out){if(UsingComponents[7]&0x10000000){ETCInterfaceType key = (ETCInterfaceType)(252);ComponentMap* map=&Components;ComponentEntry* it=map->LowerBound(key);ComponentEntry* end=map->Finish;*out=static_cast<CTCQuestCompletionUIMethods*>(((void*)(it==end||it->first>252?end:it)->second));return true;}return false;}
};
class CThingPlayerCreature : public CThingNative { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; }; //00487DC0
struct CGameScriptInterfaceMethods:CGameScriptInterface {void KickOffDeathScreen()const;};
union InterfaceSlot {void* Native;bool(CGameScriptInterfaceMethods::*IsHeroControlledByPlayer)()const;void(CGameScriptInterfaceMethods::*NewScriptFrame)()const;};
FABLE_STATIC_ASSERT(sizeof(InterfaceSlot)==4);
FABLE_STATIC_ASSERT(offsetof(CThingNative,Components)==0x44);
FABLE_STATIC_ASSERT(sizeof(ComponentMap)==8);
FABLE_STATIC_ASSERT(offsetof(CTCQuestCompletionUIMethods,Active)==0x0c);
void CGameScriptInterfaceMethods::KickOffDeathScreen()const{
 CTCQuestCompletionUIMethods* ui;CThingNative* hero=PlayerManager->GetMainPlayer().GetPControlledCreature();
 if(hero->UsingComponents[7]&0x10000000){ComponentMap* map=&hero->Components;ComponentEntry* it=map->LowerBound((ETCInterfaceType)252);ComponentEntry* end=map->Finish;ui=static_cast<CTCQuestCompletionUIMethods*>(((void*)(it==end||it->first>252?end:it)->second));}
 {CWideString summary(L"");CCharString name("",-1);ui->Activate(3,name,true,summary,true);}
 if((this->*reinterpret_cast<InterfaceSlot*>(__vftable)[394].IsHeroControlledByPlayer)())while(ui->Active)(this->*reinterpret_cast<InterfaceSlot*>(__vftable)[7].NewScriptFrame)();
}
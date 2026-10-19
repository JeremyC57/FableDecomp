// CGameScriptInterface::KickOffDeathScreen() -- activate death completion UI and yield while it remains active
// Retail00896D20 196bytes; uses native TC252 at CThing.Components+44 (donor+48).
//449970 GetMainPlayer,487DC0 GetHeroThing,40F020 LowerBound,99B6B0/99B510 wide ctor/dtor,
//99EBF0/99EAE0 narrow ctor/dtor,5E4740 CTCQuestCompletionUI::Activate.
// Interface slots394(88E980 IsHeroControlledByPlayer) and7(6E7410 NewScriptFrame) are audited real slots.
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include <stddef.h>
#include "fable_string.h"
#include "engine/_quarantine/CTCQuestCompletionUI.h"
struct CTCQuestCompletionUIMethods:CTCQuestCompletionUI {void Activate(long,const CCharString&,bool,const CWideString&,bool);};
struct ComponentEntry {long Type;void* Component;};
struct ComponentMap {ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const long&);};
struct CThingNative {void* VTable;unsigned char Base[0x1c];unsigned long UsingComponents[9];ComponentMap Components;
 __forceinline bool GetQuestCompletionUI(CTCQuestCompletionUIMethods** out){if(UsingComponents[7]&0x10000000){long key=252;ComponentMap* map=&Components;ComponentEntry* it=map->LowerBound(key);ComponentEntry* end=map->Finish;*out=static_cast<CTCQuestCompletionUIMethods*>((it==end||it->Type>252?end:it)->Component);return true;}return false;}
};
struct CPlayerMethods:CPlayer {CThingNative* GetHeroThing();}; //00487DC0
struct CPlayerManagerMethods:CPlayerManager {CPlayerMethods* GetMainPlayer();}; //00449970
struct CGameScriptInterfaceMethods:CGameScriptInterface {void KickOffDeathScreen()const;};
union InterfaceSlot {void* Native;bool(CGameScriptInterfaceMethods::*IsHeroControlledByPlayer)()const;void(CGameScriptInterfaceMethods::*NewScriptFrame)()const;};
FABLE_STATIC_ASSERT(sizeof(InterfaceSlot)==4);
FABLE_STATIC_ASSERT(offsetof(CThingNative,Components)==0x44);
FABLE_STATIC_ASSERT(sizeof(ComponentMap)==8);
FABLE_STATIC_ASSERT(offsetof(CTCQuestCompletionUIMethods,Active)==0x0c);
void CGameScriptInterfaceMethods::KickOffDeathScreen()const{
 CTCQuestCompletionUIMethods* ui;CThingNative* hero=static_cast<CPlayerManagerMethods*>(PlayerManager)->GetMainPlayer()->GetHeroThing();
 if(hero->UsingComponents[7]&0x10000000){ComponentMap* map=&hero->Components;ComponentEntry* it=map->LowerBound(252);ComponentEntry* end=map->Finish;ui=static_cast<CTCQuestCompletionUIMethods*>((it==end||it->Type>252?end:it)->Component);}
 {CWideString summary(L"");CCharString name("",-1);ui->Activate(3,name,true,summary,true);}
 if((this->*reinterpret_cast<InterfaceSlot*>(__vftable)[394].IsHeroControlledByPlayer)())while(ui->Active)(this->*reinterpret_cast<InterfaceSlot*>(__vftable)[7].NewScriptFrame)();
}
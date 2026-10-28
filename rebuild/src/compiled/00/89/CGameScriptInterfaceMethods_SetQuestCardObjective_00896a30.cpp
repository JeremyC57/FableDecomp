// CGameScriptInterface::SetQuestCardObjective -- update objective and play the hero log-entry cue
// Retail00896A30,294 bytes.
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "engine/CTCHeroStats.h"
#include "fable_string.h"
#include "engine/_quarantine/CTCInventoryBase.h"
struct ComponentEntry{long Type;void* Component;};struct ComponentMap{ComponentEntry *Start,*Finish,*Capacity;ComponentEntry* LowerBound(const long&);}; //0040F020
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Rest[0x41];unsigned char DeletedFlags;};
class CThingPlayerCreature : public CThingNative { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; }; //00487DC0
struct PlayerManagerMethods:CPlayerManager{CPlayer* GetMainPlayer();}; //00449970

#include "engine/CQuestManager.h"
#include "engine/CTCQuestCard.h"
#include "engine/CTCBase.h"
struct QuestManagerMethods:CQuestManager{bool SetQuestCardObjective(const CCharString&,const CCharString&,const CCharString&,const CCharString&);bool IsQuestCardActive(const CCharString&)const;CThingNative* GetActiveQuestCardFromScriptName(const CCharString&)const;}; //004B0D30,004AF610,004B0C80
extern QuestManagerMethods* g_QuestManager; //013B89FC
struct QuestCardMethods:CTCQuestCard{bool IsVignette()const;}; //007022E0 reads native Def+59; donor+5D differs due vector stride
struct SoundPlayerMethods:CTCBase{unsigned long PlaySoundEvent(const CCharString&,bool,bool,bool,long);}; //0066BD00 returns sound handle, forwards to internal0066B120
__forceinline void PlayLogCue(SoundPlayerMethods* sound){CCharString cue("LOG_ENTRY",-1);sound->PlaySoundEvent(cue,false,true,false,-1);}
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBD0 IS_THING_ALIVE<CThingPlayerCreature*>
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}
struct CGameScriptInterfaceMethods:CGameScriptInterface{void SetQuestCardObjective(const CCharString&,const CCharString&,const CCharString&,const CCharString&)const;};
void CGameScriptInterfaceMethods::SetQuestCardObjective(const CCharString& quest,const CCharString& objective,const CCharString& region,const CCharString& extra)const{
 if(g_QuestManager->SetQuestCardObjective(quest,objective,region,extra)&&g_QuestManager->IsQuestCardActive(quest)){
 CThingNative* card=g_QuestManager->GetActiveQuestCardFromScriptName(quest);if(card&&!(card->DeletedFlags&1)&&(card->UsingComponents[3]&0x1000)){
 ComponentMap* map=&card->Components;ComponentEntry* it=map->LowerBound(108);ComponentEntry* end=map->Finish;QuestCardMethods* tc=static_cast<QuestCardMethods*>((it==end||it->Type>108?end:it)->Component);if(!tc->IsVignette()){
 CThingPlayerCreature* hero=static_cast<PlayerManagerMethods*>(PlayerManager)->GetMainPlayer()->GetPControlledCreature();if(IS_THING_ALIVE(hero)&&(hero->UsingComponents[0]&0x10000)){
 ComponentMap* soundMap=&hero->Components;ComponentEntry* soundIt=soundMap->LowerBound(16);ComponentEntry* soundEnd=soundMap->Finish;SoundPlayerMethods* sound=static_cast<SoundPlayerMethods*>((soundIt==soundEnd||soundIt->Type>16?soundEnd:soundIt)->Component);PlayLogCue(sound);
 }}}}
}
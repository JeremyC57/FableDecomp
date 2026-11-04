// CGameScriptInterface::ResumeAbilitiesForCutscene() const -- Dispatch to the live main-player character component.
// Leaf: ?ResumeAbilitiesForCutscene@CGameScriptInterface@@UBEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: own virtual const PDB API, component167 and CTCSpecialAbilities::ResumeAbilitiesForCutscene callee0072bec0. Player accessor00487dc0 retains established GetPPlayerCharacter binding, distinct from duplicate GetPControlledCreature00487dd0.
#define CGameScriptInterface CGameScriptInterface_HeaderLayout
#include "engine/CGameScriptInterface.h"
#undef CGameScriptInterface
#define CScriptThing CScriptThing_HeaderLayout
#include "engine/CScriptThing.h"
#undef CScriptThing
#include "engine/CTCBase.h"
#include "engine/CTCTargeted.h"
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
class CCPPointerInfo;
enum ETCInterfaceType {TC_TARGETED=9,TC_ENEMY=73};
#include "engine/CArray.h"
template<class K,class V>class CKeyPairCompareLess {public:bool operator()(const std::pair<K,V>& entry,const K& key)const{return entry.first<key;}};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap {public:typedef std::pair<K,V> value_type;typedef value_type* iterator;CArray<value_type> Data;Cmp Compare;bool Dirty;
__declspec(noinline) iterator LowerBound(const K& key){iterator first=Data.begin();int count=Data.end()-first;while(count>0){int half=count/2;iterator middle=first+half;if(Compare(*middle,key)){first=middle+1;count-=half+1;}else count=half;}return first;}
__forceinline V Get(K key){iterator it=LowerBound(key);iterator last=Data.end();return (it==last || key<it->first ? last:it)->second;}
};
// Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.
class CThing {public:unsigned char Prefix[0x20];unsigned int UsingComponents[9];CVectorMap<ETCInterfaceType,CTCBase*> Components;unsigned char RestToFlags[0x3d];unsigned char Dead:1;unsigned char OtherFlags:7;template<class T>__forceinline bool GetTC(ETCInterfaceType kind,T** out){if(!HasTC(kind))return false;*out=(T*)Components.Get(kind);return true;} __forceinline bool HasTC(ETCInterfaceType kind)const{return (UsingComponents[(unsigned int)kind>>5]&(1u<<((unsigned int)kind&31)))!=0;}};
// Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
class CTCEnemy:public CTCBase {public:bool FriendsWithEverythingFlag,EnableFollowersEnemyProxy;};
#define CPlayer CPlayer_HeaderLayout
#include "engine/CPlayer.h"
#undef CPlayer
#define CPlayerManager CPlayerManager_HeaderLayout
#include "engine/CPlayerManager.h"
#undef CPlayerManager
#define CTCOracleMinigame CTCOracleMinigame_HeaderLayout
#include "engine/CTCOracleMinigame.h"
#undef CTCOracleMinigame
// Layout: shared player/player-manager/oracle headers supply external receiver layout only. No instance fields are read on them; genuine methods restore original symbols.
class CThingPlayerCreature:public CThing {};
class CPlayer:public CPlayer_HeaderLayout {public:CThingPlayerCreature* GetPPlayerCharacter()const;};
class CPlayerManager:public CPlayerManager_HeaderLayout {public:CPlayer& GetMainPlayer();};
class CTCOracleMinigame:public CTCOracleMinigame_HeaderLayout {public:void PlayGame();};
// Layout: no active CTCSpecialAbilities header; external method receiver only, no instance fields accessed.
class CTCSpecialAbilities:public CTCBase {public:void CancelAbilitiesForCutscene();void ResumeAbilitiesForCutscene();};
// Layout: genuine script vptr+0, PlayerManager+0x14 from shared header; no other owner fields read. CThing Dead bit0 at+0x91 is retail, donor+0xa1.
class CGameScriptInterface {public:virtual void ResumeAbilitiesForCutscene()const;unsigned char Prefix[0x10];CPlayerManager* PlayerManager;unsigned char RemainingOwnerFields[0x38];};
void CGameScriptInterface::ResumeAbilitiesForCutscene()const{CThingPlayerCreature* thing=PlayerManager->GetMainPlayer().GetPPlayerCharacter();if(thing && !thing->Dead){CTCSpecialAbilities* component;if(thing->GetTC((ETCInterfaceType)167,&component))component->ResumeAbilitiesForCutscene();}}
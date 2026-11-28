// Retail event fingerprints cover 2,400 input combinations in 24 batches.
// The x86 test runs Draw on its shared headers. Other hosts verify the portable expectations.
#include <cassert>
#include <cstdio>
#include <cstring>
namespace {
struct Inputs {
    unsigned int value[29];
    void Fill(unsigned int i,unsigned int& state) {
        for(unsigned j=0;j<29;++j) {state=state*1664525u+1013904223u;value[j]=state>>31;}
        state=state*1664525u+1013904223u;
        value[0]=i%4;value[1]=state>>14;value[2]=(i%29!=0);value[3]=(i%13==0);value[4]=(i%11==0);
        value[5]=(i%7!=0);value[9]=((i%5)<3?0:(i%5)-2);value[28]=i%3;
    }
    unsigned int operator[](unsigned int n) const {return value[n];}
} scenario;
struct Trace {
    unsigned int hash;
    void Word(unsigned int value) {hash=(hash^value)*16777619u;}
    void Event(unsigned int kind,unsigned int slot,unsigned int value) {Word(kind);Word(slot);Word(value);}
} trace;
void ExpectedEvents(const Inputs& q) {
    trace.Event(0,0x28,0);trace.Event(0,0x44,0);
    const bool hasTarget=q[5] && (q[3]||!q[4]);
    for(unsigned int i=0;q[2]&&i<q[0];++i) {
        const unsigned int bindings=(q[1]|(q[1]>>9))>>(3*i);
        if(i==0 || (bindings&1)) {
            const unsigned int interact=i==0?1:i;
            trace.Event(1,interact,0);
            if(hasTarget&&!q[3]) {
                unsigned int graphic=0;
                if(q[7]&&!q[8]) {
                    if(q[9]) graphic=444;
                    else if(q[10]&&q[11]&&q[12]) graphic=555;
                    else if(!q[13]) graphic=333;
                } else if(q[6]&&(!q[14]||q[15])&&(q[16]?q[17]:!q[18]||q[19])) graphic=444;
                if(graphic)trace.Event(1,interact,graphic);
            }
        }
        if(bindings&4u) {
            const float alpha=((hasTarget&&(q[20]||q[21]||q[22]))||q[23]||!q[24])?1.0f:0.5f;
            unsigned int bits;std::memcpy(&bits,&alpha,sizeof(bits));trace.Event(2,i,bits);
        }
        if(bindings&2u) {
            const float alpha=(q[25]||q[26]||q[27])?1.0f:0.5f;
            unsigned int bits;std::memcpy(&bits,&alpha,sizeof(bits));trace.Event(2,i,bits);
        }
    }
}
} // namespace

#if defined(_MSC_VER) && defined(_M_IX86)
// 0064B8B3 NPlayerGui::CDrawTargetInfo::Draw
// Leaf: ?Draw@CDrawTargetInfo@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /O1 /Oy-
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_FORCEINLINE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_SPEED_OWNER_PASS27
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYER_ISTARGETLOCKED
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_COMPONENT_QUERY_PASS27
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#define FABLE_RETAIL_TARGET_INFO_PASS30
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD_MAP_TABLE_QUERY
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ENGINEGRAPHIC
#define FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPENING_PASS26
#define FABLE_RETAIL_CTCTROPHY_ISMINIGAMEINPROGRESS
#define FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_ISUSABLE
#define FABLE_RETAIL_NTARGETING_MELEE
#define FABLE_RETAIL_CPLAYERGUI_GETINPUTINTERFACE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CTCHERORANGEDATTACKS
#define FABLE_RETAIL_CTCTROPHY
#define FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHING_HASTC_SPEED_OWNER_PASS27
#define FABLE_RETAIL_STLP_THING_BITSET_SPEED_OWNER_PASS26
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#ifdef _MSC_VER
#pragma optimize("s", off)
#pragma optimize("ty", on)
#endif
#include "stlport/fable_thing_bitset.h"
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
#include "engine/CDefString.h"
#include "engine/EGameAction.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCEnemy.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCTrophy.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCActionUseBase.h"
#include "engine/CTCActionUseScriptedHook.h"
#include "engine/NTargeting.h"
#include "engine/CWorld.h"
#include "engine/NPlayerGui_CDrawTargetInfo.h"

void NPlayerGui::CDrawTargetInfo::Draw(const CEnginePrimitivePrepareInfo& info)
{
    AttackAlphaFade.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);
    BlockAlphaFade.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);
    const CThingPlayerCreature* phero = GetPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(phero)) {
        SetAsDrawing(true);
        for (long index = 0; index < CPlayerGui::PeekGuiDef().AbilityButtons.size(); ++index) {
            const CGamePlayerInterface& game_player_interface = PlayerGui.GetInputInterface();
            long attack_button = -1;
            long block_button = -1;
            long interact_button = -1;
            if (game_player_interface.IsButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].ControllerButton, GAME_ACTION_ATTACK) ||
                game_player_interface.IsMouseButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].MouseButton, GAME_ACTION_ATTACK))
                attack_button = index;
            if (game_player_interface.IsButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].ControllerButton, GAME_ACTION_BLOCK) ||
                game_player_interface.IsMouseButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].MouseButton, GAME_ACTION_BLOCK))
                block_button = index;
            if (game_player_interface.IsButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].ControllerButton, GAME_ACTION_INTERACT) ||
                game_player_interface.IsMouseButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].MouseButton, GAME_ACTION_INTERACT))
                interact_button = index;
            if (index == 0) interact_button = 1;
            if (interact_button > -1) PlayerGui.SetAbilityButton(interact_button, 0);
            CThing* ptarget = 0;
            if (PlayerGui.IsDrawingTargetingCentre()) {
                const CTCTargetingBase* ptc_targeting;
                phero->PeekTC(&ptc_targeting);
                ptarget = ptc_targeting->GetPTargetedThing();
            } else if (!phero->HasTC(TCI_BERSERK)) {
                const CTCTargetingBase* ptc_targeting;
                phero->PeekTC(&ptc_targeting);
                ptarget = ptc_targeting->GetPTargetedThing();
                if (interact_button > -1 && IS_THING_ALIVE(ptarget)) {
                    if (ptc_targeting->IsTargetedThingInRange(2) && !NTargeting::IsThingVulnerableToMeleeStrikesFromThing(*ptarget,*phero)) {
                        if (ptarget->GetDefName() == "CREATURE_DEMON_DOOR_FACE_01")
                            PlayerGui.SetAbilityButton(interact_button, CPlayerGui::PeekGuiDef().AbilityButtonActivateGraphic);
                        else if (ptarget->GetDefName() == "CREATURE_NW_DEMON_DOOR_FACE_01")
                            PlayerGui.SetAbilityButton(interact_button, CPlayerGui::PeekGuiDef().AbilityButtonActivateGraphic);
                        else {
                            bool carrying_trophy = false;
                            const CTCHeroRangedAttacks* ptc_ranged;
                            phero->PeekTC(&ptc_ranged);
                            bool in_process_of_firing = false;
                            if (ptc_ranged->IsInProcessOfFiringRangedWeapon()) in_process_of_firing = true;
                            const CTCCarrying* ptc_carrying;
                            phero->PeekTC(&ptc_carrying);
                            const CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
                            if (IS_THING_ALIVE(pcarried_thing)) {
                                const CTCTrophy* ptc_trophy;
                                if (pcarried_thing->PeekTC(&ptc_trophy) && ptc_trophy->IsMiniGameInProgress()) {
                                    const CTCInventoryItem* ptc_inventory_item;
                                    pcarried_thing->PeekTC(&ptc_inventory_item);
                                    PlayerGui.SetAbilityButton(interact_button, ptc_inventory_item->GetEngineGraphic().BankIndex);
                                    carrying_trophy = true;
                                }
                            }
                            if (!carrying_trophy && !in_process_of_firing)
                                PlayerGui.SetAbilityButton(interact_button, CPlayerGui::PeekGuiDef().AbilityButtonTalkGraphic);
                        }
                    } else if (ptc_targeting->IsTargetedThingInRange(1)) {
                        bool targetable = true;
                        if (ptarget->HasTC(TCI_HERO_GUIDE) && !World.IsMapTableToShowQuestCardsWhenUsed()) targetable = false;
                        if (targetable) {
                            const CTCActionUseBase* ptc_action_use;
                            if (ptarget->PeekTC(&ptc_action_use)) {
                                if (!ptc_action_use->IsUsable()) targetable = false;
                            } else {
                                const CTCActionUseScriptedHook* ptc_action_use_hook = 0;
                                if (ptarget->PeekTC(&ptc_action_use_hook) && !ptc_action_use_hook->IsUsable()) targetable = false;
                            }
                        }
                        if (targetable) PlayerGui.SetAbilityButton(interact_button, CPlayerGui::PeekGuiDef().AbilityButtonActivateGraphic);
                    }
                }
            }
            if (attack_button > -1) {
                if (IS_THING_ALIVE(ptarget) && (CTCEnemy::AreThingsEnemies(*phero, *ptarget) || GetPlayer().IsTargetLocked()))
                    PlayerGui.SetButtonAlpha(attack_button, true, 1.0f);
                else if (IS_THING_ALIVE(ptarget) && ptarget->HasTC(TCI_SMASHABLE))
                    PlayerGui.SetButtonAlpha(attack_button, true, 1.0f);
                else if (AttackAlphaFade.IsOpening()) PlayerGui.SetButtonAlpha(attack_button, true, 1.0f);
                else if (!GetPlayer().IsAggressiveMode()) PlayerGui.SetButtonAlpha(attack_button, true, 1.0f);
                else PlayerGui.SetButtonAlpha(attack_button, true, 0.5f);
            }
            if (block_button > -1) {
                if (phero->IsBlocking()) PlayerGui.SetButtonAlpha(block_button, true, 1.0f);
                else if (phero->IsInMelee()) PlayerGui.SetButtonAlpha(block_button, true, 1.0f);
                else if (BlockAlphaFade.IsOpening()) PlayerGui.SetButtonAlpha(block_button, true, 1.0f);
                else PlayerGui.SetButtonAlpha(block_button, true, 0.5f);
            }
        }
    }
}

// The actual optimized row is checked separately. This x86 fixture also runs the
// included C++ source at /Od against its shipping declarations and layouts.
#include "engine/CPlayerManager.h"
#include <cstring>
namespace {
template<class T> struct TestStorage {
    union { double alignment; unsigned char data[sizeof(T)]; };
    T& Ref() {return *reinterpret_cast<T*>(data);}
    void Set(unsigned int offset,const void* value) { std::memcpy(data+offset,&value,sizeof(value)); }
    void Byte(unsigned int offset,bool flag) {data[offset]=flag?1:0;}
    void Long(unsigned int offset,long value) {std::memcpy(data+offset,&value,4);}
};
TestStorage<NPlayerGui::CDrawTargetInfo> panel;
TestStorage<CPlayerGui> gui;
TestStorage<CPlayerManager> manager;
TestStorage<CPlayer> actor;
TestStorage<CThingPlayerCreature> hero;
TestStorage<CThing> target, carried;
TestStorage<CPlayerGuiDef> defs;
TestStorage<CWorld> world;
TestStorage<CGamePlayerInterface> input;
TestStorage<CEnginePrimitivePrepareInfo> prepare;
union ButtonStorage { double alignment; unsigned char bytes[3*sizeof(CAbilityButtonGuiDef)]; } buttons;
TestStorage<CTCTargetingBase> targeting;
TestStorage<CTCHeroRangedAttacks> ranged;
TestStorage<CTCCarrying> carrying;
TestStorage<CTCTrophy> trophy;
TestStorage<CTCInventoryItem> item;
TestStorage<CTCActionUseBase> action;
TestStorage<CTCActionUseScriptedHook> hook;
TestStorage<CEngineGraphic> icon;
struct TestVtable {void* entries[39];} dispatch;
struct VirtualCallbacks {
    CThing* GetTarget() const {return scenario[5]?&target.Ref():0;}
    bool InRange(unsigned long kind) const {assert(kind==1||kind==2);return scenario[kind==1?6:7]!=0;}
};
struct ComponentTable {
    struct Element {int key;void* component;} slots[16];
    int used;
    template<class T> void Reset(TestStorage<T>& thing) {
        used=0; std::memset(thing.data+0x20,0,36);thing.Set(0x44,slots);thing.Set(0x48,slots);thing.Set(0x4c,slots+16);
    }
    template<class T> void Add(TestStorage<T>& thing,int key,void* component,bool enable) {
        if(!enable)return;
        thing.data[0x20+(key>>3)]|=(1u<<(key&7));int i=used++;
        while(i>0&&slots[i-1].key>key){slots[i]=slots[i-1];--i;}
        slots[i].key=key;slots[i].component=component;thing.Set(0x48,slots+used);
    }
} ht,tt,ct;
template<class Method> void Slot(unsigned int i,Method method) {
    assert(sizeof(method)==sizeof(dispatch.entries[i]));std::memcpy(&dispatch.entries[i],&method,sizeof(dispatch.entries[i]));
}
void ActualEvents(const Inputs& q) {
    panel.Set(4,&gui.Ref());panel.Set(8,&manager.Ref());panel.Set(12,&world.Ref());panel.Byte(0x18,false);
    panel.Byte(0x35,q[23]!=0);panel.Byte(0x51,q[27]!=0);actor.Byte(0x217,q[24]!=0);world.Byte(0xda,q[15]!=0);
    gui.Set(12,&input.Ref());defs.Set(0x7fc,buttons.bytes);defs.Set(0x800,buttons.bytes+q[0]*sizeof(CAbilityButtonGuiDef));defs.Set(0x804,buttons.bytes+3*sizeof(CAbilityButtonGuiDef));
    defs.Long(0x75c,333);defs.Long(0x760,444);icon.Long(0,555);
    for(unsigned i=0;i<q[0];++i){unsigned int controller=32+i,mouse=48+i; unsigned char* entry=buttons.bytes+i*sizeof(CAbilityButtonGuiDef); std::memcpy(entry+0x40,&controller,4);std::memcpy(entry+0x44,&mouse,4);}
    float elapsed=.25f;std::memcpy(prepare.data,&elapsed,sizeof(elapsed));
    targeting.Set(0,&dispatch);Slot(33,&VirtualCallbacks::GetTarget);Slot(38,&VirtualCallbacks::InRange);
    ranged.Byte(0x14,q[13]!=0);trophy.Byte(0x14,q[12]!=0);action.Byte(0x10,q[17]!=0);hook.Byte(0xc,q[19]!=0);
    ht.Reset(hero);ht.Add(hero,TCI_TARGETING,&targeting.Ref(),true);ht.Add(hero,TCI_BERSERK,0,q[4]!=0);
    ht.Add(hero,TCI_CARRYING,&carrying.Ref(),true);ht.Add(hero,TCI_HERO_RANGED_ATTACKS,&ranged.Ref(),true);
    tt.Reset(target);tt.Add(target,TCI_HERO_GUIDE,0,q[14]!=0);tt.Add(target,TCI_SMASHABLE,0,q[22]!=0);
    tt.Add(target,CTCActionUseBase::StatGetInterfaceType(),&action.Ref(),q[16]!=0);
    tt.Add(target,CTCActionUseScriptedHook::StatGetInterfaceType(),&hook.Ref(),q[18]!=0);
    ct.Reset(carried);ct.Add(carried,CTCTrophy::StatGetInterfaceType(),&trophy.Ref(),q[11]!=0);
    ct.Add(carried,CTCInventoryItem::StatGetInterfaceType(),&item.Ref(),true);
    panel.Ref().Draw(prepare.Ref());assert((panel.data[0x18]!=0)==(q[2]!=0));
}
}
void CFadeInFadeOutBase::Update(float d){assert(d==.25f);trace.Event(0,(unsigned int)(reinterpret_cast<const unsigned char*>(this)-panel.data),0);}
CPlayer& CPlayerManager::GetMainPlayer(){return actor.Ref();}
CThingPlayerCreature* CPlayer::GetPControlledCreature() const {return scenario[2]?&hero.Ref():0;}
const CPlayerGuiDef& FABLE_FASTCALL CPlayerGui::PeekGuiDef(){return defs.Ref();}
bool CGamePlayerInterface::IsButtonForGameEvent(EXboxControllerButton button,EGameAction ev) const {
    return (scenario[1] & (1u<<(3*(int(button)-32)+(ev==GAME_ACTION_ATTACK?2:ev==GAME_ACTION_BLOCK?1:0))))!=0;
}
bool CGamePlayerInterface::IsMouseButtonForGameEvent(EMouseButtonControl button,EGameAction ev) const {
    return (scenario[1] & (1u<<(9+3*(int(button)-48)+(ev==GAME_ACTION_ATTACK?2:ev==GAME_ACTION_BLOCK?1:0))))!=0;
}
void CPlayerGui::SetAbilityButton(long n,long value){trace.Event(1,n,value);}
void CPlayerGui::SetButtonAlpha(long n,bool normal,float alpha){assert(normal);unsigned int v;std::memcpy(&v,&alpha,4);trace.Event(2,n,v);}
bool CPlayerGui::IsDrawingTargetingCentre() const{return scenario[3]!=0;}
bool FABLE_FASTCALL NTargeting::IsThingVulnerableToMeleeStrikesFromThing(const CThing&,const CThing&){return scenario[8]!=0;}
CDefString CThing::GetDefName() const {return CDefString();}
bool CDefString::operator==(const char* s) const {
    if(!std::strcmp(s,"CREATURE_DEMON_DOOR_FACE_01")) return scenario[9]==1;
    assert(!std::strcmp(s,"CREATURE_NW_DEMON_DOOR_FACE_01"));return scenario[9]==2;
}
CThing* CTCCarrying::GetThingInPrimarySlot()const{return scenario[10]?&carried.Ref():0;}
const CEngineGraphic& CTCInventoryItem::GetEngineGraphic()const{return icon.Ref();}
bool FABLE_FASTCALL CTCEnemy::AreThingsEnemies(const CThing&,const CThing&){return scenario[20]!=0;}
bool CPlayer::IsTargetLocked()const{return scenario[21]!=0;}
bool CThingCreatureBase::IsBlocking()const{return scenario[25]!=0;}
bool CThingCreatureBase::IsInMelee()const{return scenario[26]!=0;}

#endif
int main() {
    static const unsigned int expected[] = {1360742883u, 176882159u, 846871410u, 4200344752u, 1479703674u, 754170460u, 3180642974u, 4292636177u, 559185353u, 2958719805u, 3320321829u, 1437573057u, 3208805169u, 328169875u, 1183521192u, 246121581u, 3280420987u, 2177398384u, 1767989247u, 2431165512u, 3282579007u, 3813856593u, 999780492u, 2412869702u};
    unsigned int state=0x645b8b3u;
    for(unsigned int group=0;group<sizeof(expected)/sizeof(expected[0]);++group) {
        unsigned int predicted=2166136261u;
#if defined(_MSC_VER) && defined(_M_IX86)
        unsigned int actual=2166136261u;
#endif
        for(unsigned int k=0;k<100;++k) {
            scenario.Fill(group*100+k,state);
            trace.hash=predicted;ExpectedEvents(scenario);trace.Word(255);predicted=trace.hash;
#if defined(_MSC_VER) && defined(_M_IX86)
            trace.hash=actual;ActualEvents(scenario);trace.Word(255);actual=trace.hash;
#endif
        }
        assert(predicted==expected[group]);
#if defined(_MSC_VER) && defined(_M_IX86)
        assert(actual==expected[group]);
#endif
    }
    std::puts("0064b8b3_TEST PASS"); return 0;
}
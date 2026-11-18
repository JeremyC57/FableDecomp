#include <stdio.h>
#include <string>
#include <vector>

typedef std::string CCharString;
struct CThing;
enum EGameAction { GAME_ACTION_INTERACT=6 };
enum { TCI_CARRIED_ON_ACTION_USE=14,TCI_HERO=0x29,TCI_EXPRESSION=0x8f,TCI_HERO_GUIDE=0x6b,TCI_DOOR=0x26 };
enum { TARGETING_USABLE=1,TARGETING_TALKABLE=2 };
struct CTCCarrying {
    CThing* Carried;
    CTCCarrying():Carried(0) {}
    CThing* GetThingInPrimarySlot() const { return Carried; }
};
struct CTCTargetingBase {
    CThing* Target;
    int Type;
    bool InRange;
    CTCTargetingBase():Target(0),Type(1),InRange(true) {}
    CThing* GetPTargetedThing() const { return Target; }
    bool IsTargetedThingOfType(int type) const { return type==Type; }
    bool IsTargetedThingInRange(int type) const { return InRange&&type==Type; }
};
struct CTCActionUseBase {
    bool Enabled,Readable,Tutorial;
    unsigned long ButtonMask;
    CTCActionUseBase():Enabled(true),Readable(false),Tutorial(false),ButtonMask(1) {}
    bool IsUsable() const { return Enabled; }
    unsigned long GetWhenToActivate() const { return ButtonMask; }
    unsigned long GetTutorialCategory() const { return Tutorial?17:0; }
    CCharString GetClassTypeName() const { return Readable?"CTCActionUseReadable":"CTCActionUse"; }
};
struct CTCActionUseReadable : CTCActionUseBase {
    bool InfoDisplay;
    CTCActionUseReadable():InfoDisplay(false) {}
    bool UsesInfoDisplay() const { return InfoDisplay; }
};
struct CTCActionUseScriptedHook {
    bool Usable;
    bool IsUsable() const { return Usable; }
};
struct CTCHero {
    bool CanAdvise;
    unsigned Advice;
    CTCHero():CanAdvise(false),Advice(0) {}
    bool GiveTutorialForCategory(unsigned long category,bool force)
    { ++Advice;return category==17&&!force&&CanAdvise; }
};
struct CTCSoundPlayer {
    unsigned Fails;
    CTCSoundPlayer():Fails(0) {}
    void PlayAttachedCriteria(const CCharString& name,bool a,bool b,bool c,int d)
    { if(name=="DOOR_OPEN_FAIL"&&!a&&!b&&!c&&d==-1) ++Fails; }
};
struct CThing {
    unsigned long UID,m_flags91;
    bool Enemy;
    void* Components[256];
    explicit CThing(unsigned long uid):UID(uid),m_flags91(0),Enemy(false)
    { for(unsigned i=0;i!=256;++i) Components[i]=0; }
    unsigned long GetUniqueID() const { return UID; }
    bool IsDead() const { return (m_flags91&1)!=0; }
    bool HasTC(unsigned id) const { return Components[id]!=0; }
    bool PeekTC(const CTCCarrying** out) const { *out=static_cast<CTCCarrying*>(Components[0x46]);return *out!=0; }
    bool PeekTC(const CTCTargetingBase** out) const { *out=static_cast<CTCTargetingBase*>(Components[8]);return *out!=0; }
    bool PeekTC(const CTCActionUseBase** out) const { *out=static_cast<CTCActionUseBase*>(Components[0xd]);return *out!=0; }
    bool PeekTC(const CTCActionUseScriptedHook** out) const
    { *out=static_cast<CTCActionUseScriptedHook*>(Components[0xc2]);return *out!=0; }
    bool GetTC(CTCActionUseBase** out) { *out=static_cast<CTCActionUseBase*>(Components[0xd]);return *out!=0; }
    bool GetTC(unsigned id,CTCHero** out) { *out=static_cast<CTCHero*>(Components[id]);return *out!=0; }
    bool GetTC(CTCSoundPlayer** out) { *out=static_cast<CTCSoundPlayer*>(Components[0x10]);return *out!=0; }
};
bool IS_THING_ALIVE(const CThing* thing) { return thing&&!thing->IsDead(); }
typedef CThing CThingPlayerCreature;
namespace NTargeting {
bool IsThingVulnerableToMeleeStrikesFromThing(const CThing& enemy,const CThing&) { return enemy.Enemy; }
}
struct CPlayer {
    CThingPlayerCreature* Character;
    bool SpellMode;
    explicit CPlayer(CThing& hero):Character(&hero),SpellMode(false) {}
    bool IsSpellMode() const { return SpellMode; }
    CThingPlayerCreature* GetPPlayerCharacter() const { return Character; }
    CThingPlayerCreature* GetPControlledCreature() const { return Character; }
};
struct WorldStub {
    CPlayer* Player;
    bool ShowCards;
    explicit WorldStub(CPlayer& p):Player(&p),ShowCards(false) {}
    const CPlayer* PeekMyPlayer() const { return Player; }
    bool IsMapTableToShowQuestCardsWhenUsed() const { return ShowCards; }
};
struct CInputEvent { int Action;unsigned Buttons; };
bool IsEventButtonPressed(const CInputEvent& e) { return (e.Buttons&1)!=0; }
bool IsEventButtonReleased(const CInputEvent& e) { return (e.Buttons&2)!=0; }
bool IsEventButtonHeld(const CInputEvent& e) { return (e.Buttons&4)!=0; }
struct CGameEvent {
    int Type;
    std::vector<unsigned long> Values;
    explicit CGameEvent(int type):Type(type) {}
    void AddUHUGE(unsigned long id) { Values.push_back(id); }
    void AddUBYTE(unsigned char value) { Values.push_back(value); }
};
struct CProcessedInput {
    std::vector<CGameEvent> Events;
    bool Handled;
    CProcessedInput():Handled(false) {}
    void AddGameEvent(const CGameEvent& e) { Events.push_back(e); }
    void SetAsPerformedEvent() { Handled=true; }
};
struct CInputProcessUseEnvironment {
    CPlayer* MyPlayer;
    WorldStub* World;
    CInputProcessUseEnvironment(CPlayer& player,WorldStub& world):MyPlayer(&player),World(&world) {}
    bool IsEventGameAction(long action,const CInputEvent& event) { return action==event.Action; }
    void ProcessInput(const CInputEvent&,CProcessedInput&);
    CProcessedInput Use(unsigned buttons,int action=6)
    { CInputEvent event={action,buttons};CProcessedInput out;ProcessInput(event,out);return out; }
};

void CInputProcessUseEnvironment::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input)
{
    if (MyPlayer->IsSpellMode())
        return;
    CThingPlayerCreature* pplayer_creature = MyPlayer->GetPPlayerCharacter();
    if (!IsEventGameAction(GAME_ACTION_INTERACT, event))
        return;
    unsigned long activation_flags = IsEventButtonPressed(event) ? 1 : 0;
    activation_flags |= IsEventButtonReleased(event) ? 2 : 0;
    activation_flags |= IsEventButtonHeld(event) ? 4 : 0;
    if (!activation_flags)
        return;

    const CTCCarrying* ptc_carrying;
    pplayer_creature->PeekTC(&ptc_carrying);
    const CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
    if (pcarried_thing && !pcarried_thing->IsDead() && pcarried_thing->HasTC(TCI_CARRIED_ON_ACTION_USE)) {
        const CTCActionUseBase* ptc_use;
        if (!pcarried_thing->PeekTC(&ptc_use) || !(activation_flags & ptc_use->GetWhenToActivate()) || !ptc_use->IsUsable())
            return;
        CGameEvent action(0x11);
        action.AddUHUGE(pplayer_creature->GetUniqueID());
        action.AddUHUGE(pcarried_thing->GetUniqueID());
        processed_input.AddGameEvent(action);
        return;
    }
    const CTCTargetingBase* ptc_targeting;
    pplayer_creature->PeekTC(&ptc_targeting);
    CThing* ptarget = ptc_targeting->GetPTargetedThing();
    if (ptarget && !ptarget->IsDead() && NTargeting::IsThingVulnerableToMeleeStrikesFromThing(*ptarget, *pplayer_creature))
        return;
    const CThing* ptalk_thing;
    if (ptc_targeting->IsTargetedThingOfType(TARGETING_TALKABLE) && ptc_targeting->IsTargetedThingInRange(TARGETING_TALKABLE) &&
        (ptalk_thing = ptc_targeting->GetPTargetedThing()) != 0) {
        if (!(activation_flags & 1))
            return;
        if (ptalk_thing->HasTC(TCI_EXPRESSION)) {
            CGameEvent action(0x13);
            processed_input.AddGameEvent(action);
            return;
        }
        CGameEvent action(0x23);
        action.AddUBYTE(0);
        processed_input.AddGameEvent(action);
        return;
    }
    if (!ptc_targeting->IsTargetedThingOfType(TARGETING_USABLE) || !ptc_targeting->IsTargetedThingInRange(TARGETING_USABLE))
        return;
    CThing* puse_thing = ptc_targeting->GetPTargetedThing();
    if (!puse_thing || puse_thing->IsDead())
        return;
    bool use = false;
    const CTCActionUseBase* ptc_use;
    if (puse_thing->PeekTC(&ptc_use) && ptc_use->IsUsable()) {
        if (ptc_use->GetTutorialCategory()) {
            CThingPlayerCreature* phero = World->PeekMyPlayer()->GetPControlledCreature();
            if (IS_THING_ALIVE(phero)) {
                CTCHero* ptc_hero;
                if (phero->GetTC(TCI_HERO, &ptc_hero) && ptc_hero && ptc_hero->GiveTutorialForCategory(ptc_use->GetTutorialCategory(), false)) {
                    processed_input.SetAsPerformedEvent();
                    return;
                }
            }
        }
        use = (activation_flags & ptc_use->GetWhenToActivate()) != 0;
    }
    const CTCActionUseScriptedHook* ptc_scripted_hook;
    if (((activation_flags & 1) && puse_thing->PeekTC(&ptc_scripted_hook) && ptc_scripted_hook->IsUsable()) || use) {
        CTCActionUseBase* action_use;
        if (puse_thing->GetTC(&action_use) && action_use->GetClassTypeName() == CCharString("CTCActionUseReadable")) {
            CTCActionUseReadable* ptc_readable = static_cast<CTCActionUseReadable*>(action_use);
            if (ptc_readable->UsesInfoDisplay())
                goto locked;
        }
        if (!(puse_thing->HasTC(TCI_HERO_GUIDE) && !World->IsMapTableToShowQuestCardsWhenUsed())) {
            CGameEvent action(0x11);
            action.AddUHUGE(pplayer_creature->GetUniqueID());
            action.AddUHUGE(puse_thing->GetUniqueID());
            processed_input.AddGameEvent(action);
            return;
        }
    }
locked:
    CTCSoundPlayer* ptc_sound_player;
    if (puse_thing->HasTC(TCI_DOOR) && (activation_flags & 1) && puse_thing->GetTC(&ptc_sound_player)) {
        ptc_sound_player->PlayAttachedCriteria("DOOR_OPEN_FAIL", false, false, false, -1);
        processed_input.SetAsPerformedEvent();
    }
}

int main()
{
    CThing hero(10),item(20),target(30);
    CPlayer player(hero);
    WorldStub world(player);
    CInputProcessUseEnvironment run(player,world);
    CTCCarrying carrying;CTCTargetingBase targeting;CTCActionUseReadable action;CTCHero tutorial;CTCSoundPlayer sound;
    CTCActionUseScriptedHook flag={true};
    hero.Components[0x46]=&carrying;hero.Components[8]=&targeting;hero.Components[0x29]=&tutorial;
    carrying.Carried=&item;targeting.Target=&target;
    item.Components[0xe]=&action;item.Components[0xd]=&action;
    action.ButtonMask=4;
    if(!run.Use(0).Events.empty()||!run.Use(1).Events.empty()||!run.Use(4,7).Events.empty())
        return 1;
    CProcessedInput used=run.Use(4);
    if(used.Events.size()!=1||used.Events[0].Type!=0x11||used.Events[0].Values.size()!=2||
        used.Events[0].Values[0]!=10||used.Events[0].Values[1]!=20)
        return 2;
    player.SpellMode=true;
    if(!run.Use(4).Events.empty()) return 3;
    player.SpellMode=false;
    carrying.Carried=0;
    targeting.Type=2;target.Enemy=true;
    if(!run.Use(1).Events.empty()) return 4;
    target.Enemy=false;
    used=run.Use(1);
    if(used.Events.size()!=1||used.Events[0].Type!=0x23||used.Events[0].Values.size()!=1||used.Events[0].Values[0])
        return 5;
    target.Components[0x8f]=&flag;
    used=run.Use(1);
    if(used.Events.size()!=1||used.Events[0].Type!=0x13||!run.Use(4).Events.empty())
        return 6;
    targeting.Type=1;target.Components[0xd]=&action;action.ButtonMask=1;action.Tutorial=true;tutorial.CanAdvise=true;
    used=run.Use(1);
    if(!used.Handled||!used.Events.empty()||tutorial.Advice!=1)
        return 7;
    action.Tutorial=false;
    target.Components[0x6b]=&flag;target.Components[0x26]=&flag;target.Components[0x10]=&sound;
    used=run.Use(1);
    if(sound.Fails!=1||!used.Handled||!used.Events.empty())
        return 8;
    world.ShowCards=true;action.Readable=true;action.InfoDisplay=true;
    used=run.Use(1);
    if(sound.Fails!=2||!used.Handled||!used.Events.empty())
        return 9;
    target.Components[0xd]=0;target.Components[0xc2]=&flag;
    used=run.Use(1);
    if(used.Events.size()!=1||used.Events[0].Type!=0x11||used.Events[0].Values[1]!=30)
        return 10;
    puts("0068dd00_TEST PASS");
    return 0;
}

#include <cassert>
#include <cstdio>
#include <list>
#include <string>

typedef std::string CCharString;
enum ETCInterfaceType { TCI_THING_OWNER, TCI_TAVERN_TABLE, TCI_TAVERN_GAME, TCI_ENTITY_MODE_MANAGER,
                       TCI_AI_SCRATCHPAD, TCI_TAVERN_TANKARD };
enum EAIActionReturn { AI_ACTION_STOPPED=0, AI_ACTION_ACTIVE=4 };
enum EMovementType { AI_MOVEMENT_WALKING=3 };
struct C3DVector { float X,Y,Z; };
struct CRightHandedSet { C3DVector up, forward; };
struct CThing {
    bool dead;
    void* components[6];
    CThing() : dead(false)
    {
        for (unsigned i=0;i<6;++i)
            components[i]=0;
    }
    bool IsDead() const { return dead; }
    bool HasTC(ETCInterfaceType type) const { return components[type]!=0; }
    template<class T> bool GetTC(ETCInterfaceType type,T** component)
    {
        *component=static_cast<T*>(components[type]);
        return *component!=0;
    }
};
struct CThingBuilding : CThing { };
struct CThingObject : CThing { };
struct CCreatureActionBase { };
struct CThingAICreature : CThing {
    bool action_allowed;
    CThingAICreature() : action_allowed(true) { }
    bool IsFreeToPerformAction(const CCreatureActionBase&) const { return action_allowed; }
};
template<class T> struct CIntelligentPointer {
    T* data;
    CIntelligentPointer(T* p=0) : data(p) { }
    CIntelligentPointer& operator=(T* p) { data=p; return *this; }
    T* Get() const { return data; }
    T* operator->() const { return data; }
    bool operator!=(int) const { return data!=0; }
};
template<class T> bool IS_THING_ALIVE(const T& p) { return p!=0 && !p->IsDead(); }
struct CTCThingOwner {
    std::list<CIntelligentPointer<CThing> > objects;
    std::list<CIntelligentPointer<CThing> >& GetObjectList() { return objects; }
};
struct CTCTavernTable {
    bool can_offer_seat;
    unsigned inquiries, freed;
    CThingObject* tankard;
    explicit CTCTavernTable(CThingObject* drink=0) : can_offer_seat(false),inquiries(0),freed(0),tankard(drink) { }
    bool GetSeat(CThing&,C3DVector&,CRightHandedSet&,long& point,long& seat,const CCharString*)
    {
        ++inquiries;
        if (can_offer_seat) { point=8; seat=2; }
        return can_offer_seat;
    }
    CThingObject* GetTankard(long seat) { assert(seat==2); return tankard; }
    void SetSeatState(long seat,bool occupied)
    {
        assert(seat==2 && !occupied);
        ++freed;
    }
};
struct CTCTavernGame { bool IsBeingPlayed; };
struct CTCCreatureModeManager {
    unsigned start, stop_hands, stop_sit;
    CTCCreatureModeManager() : start(0), stop_hands(0), stop_sit(0) { }
    bool AddMode(unsigned long id) { assert(id==0x2d); ++start; return true; }
    void RemoveMode(unsigned long id)
    {
        if (id==0x2d)
            ++stop_hands;
        else {
            assert(id==0xb);
            ++stop_sit;
        }
    }
};
struct CTCAIScratchpad {
    CCharString table, seat;
    CCharString GetSeatName() const { return seat; }
    CCharString GetTableName() const { return table; }
};
struct CTCTavernTankard { float Fullness; };
struct CBrainOutput {
    bool action_set;
    CBrainOutput() : action_set(false) { }
    void SetAsPerformAction(const CCreatureActionBase&) { action_set=true; }
};
struct CCreatureAction_GetUpFromSitting : CCreatureActionBase {
    CCreatureAction_GetUpFromSitting(CThingAICreature&, const CCharString& name, long n, bool f)
    {
        assert(name=="SIT_AT_TABLE_OUTOF" && n==0 && !f);
    }
};
struct CAIBrain {
    CThingAICreature& creature;
    unsigned outputs;
    explicit CAIBrain(CThingAICreature& thing) : creature(thing),outputs(0) { }
    CThingAICreature& GetCreature() const { return creature; }
    void SetOutput(const CBrainOutput& output) { assert(output.action_set); ++outputs; }
};
struct CMainGameComponent { static long GetConstantFPS() { return 10; } };
class CWorld {
public:
    static long frame;
    static long GetFrame() { return frame; }
};
long CWorld::frame = 1;
unsigned long GFRandom(unsigned long range,unsigned long* seed)
{
    assert(range!=0);
    ++*seed;
    return range==100 ? 0 : range-1;
}
class CAIStateGroup_GoToPub {
public:
    CIntelligentPointer<CThingBuilding> PPub;
    CIntelligentPointer<CThingObject> PTable;
    CIntelligentPointer<CThing> PTankard;
    CAIBrain* PBrain;
    C3DVector Pos;
    CRightHandedSet RHSet;
    long ActionPointIndex, SeatIndex, FrameLeavePub, NextFramePub;
    bool Sitting, HandsOffMode, UnderScriptControl;
    bool allow_sit, stopping, drink_success, staffed;
    unsigned terminations, sit_calls, silent, choices, drinks, frees, paid, wandered;
    unsigned long seed;
    explicit CAIStateGroup_GoToPub(CAIBrain& brain) : PBrain(&brain),ActionPointIndex(42),SeatIndex(0),
        FrameLeavePub(0),NextFramePub(0),Sitting(false),HandsOffMode(false),UnderScriptControl(false),
        allow_sit(true),stopping(false),drink_success(true),staffed(true),
        terminations(0),sit_calls(0),silent(0),choices(0),drinks(0),frees(0),paid(0),wandered(0),seed(0) { }
    void Update();
    unsigned long* GetPWorldSeed() { return &seed; }
    void Terminate() { ++terminations; }
    bool WalktoSeatAndSit(bool during_time) { assert(during_time); ++sit_calls; Sitting=allow_sit; return Sitting; }
    bool ChooseAction(bool during_time)
    {
        assert(during_time);
        return ++choices<=1;
    }
    EAIActionReturn AIAction_DoNothing()
    {
        CTCTavernGame* game;
        assert(PTable->GetTC(TCI_TAVERN_GAME,&game));
        game->IsBeingPlayed=false;
        ++silent;
        return AI_ACTION_ACTIVE;
    }
    bool IsStaffed() const { return staffed; }
    bool IsStopping() const { return stopping; }
    bool Drink()
    {
        ++drinks;
        CTCTavernTankard* thing;
        assert(PTankard->GetTC(TCI_TAVERN_TANKARD,&thing));
        thing->Fullness-=1;
        return drink_success;
    }
    void FreeSeat() { ++frees; }
    bool PayAtBar() { ++paid; return true; }
    void AIAction_MinionWander(EMovementType mode,CThingBuilding* pub)
    {
        assert(mode==AI_MOVEMENT_WALKING && !pub);
        ++wandered;
    }
};

// Selected Update body.

// The selected implementation is reproduced verbatim for this standalone fixture.
void CAIStateGroup_GoToPub::Update()
{
    {
        std::list<CThing*> tables;
        CTCThingOwner* owner;
        if (PPub.Get() && !PPub.Get()->IsDead() && ((CThing*)PPub.Get())->GetTC(TCI_THING_OWNER, &owner)) {
            for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin(); it != owner->GetObjectList().end(); ++it) {
                if ((*it).Get() && !(*it).Get()->IsDead() && (*it).Get()->HasTC(TCI_TAVERN_TABLE))
                    tables.push_back((*it).Get());
            }
            if (!tables.empty()) {
                PTable = 0;
                while (!tables.empty()) {
                    unsigned long index = GFRandom(tables.size(), GetPWorldSeed());
                    std::list<CThing*>::iterator it = tables.begin();
                    for (; it != tables.end(); ++it) {
                        if (index == 0) {
                            PTable = (CThingObject*)*it;
                            break;
                        }
                        --index;
                    }
                    CTCTavernTable* tc_tavern_table;
                    if (PTable.Get()->GetTC(TCI_TAVERN_TABLE, &tc_tavern_table) &&
                        tc_tavern_table->GetSeat(PBrain->GetCreature(), Pos, RHSet, ActionPointIndex, SeatIndex, 0)) {
                        PTankard = (CThing*)tc_tavern_table->GetTankard(SeatIndex);
                        goto found;
                    }
                    tables.erase(it);
                }
                ActionPointIndex = -1;
            }
        }
    }
    Terminate();
    return;
found:
    if (!WalktoSeatAndSit(true)) {
        Terminate();
        return;
    }
    {
        long fps = CMainGameComponent::GetConstantFPS();
        unsigned long extra = GFRandom(100, GetPWorldSeed());
        FrameLeavePub = (long)(CWorld::GetFrame() + ((float)extra + 100.0f) * fps);
    }
    bool to_continue;
    do {
        CTCTavernGame* tc_tavern_game;
        if (((CThing*)PTable.Get())->GetTC(TCI_TAVERN_GAME, &tc_tavern_game) && tc_tavern_game && tc_tavern_game->IsBeingPlayed) {
            if (!HandsOffMode) {
                HandsOffMode = true;
                CTCCreatureModeManager* ptc_mode_manager;
                PBrain->GetCreature().GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_mode_manager);
                ptc_mode_manager->AddMode(0x2d);
            }
            EAIActionReturn ret = AIAction_DoNothing();
            to_continue = ret == (EAIActionReturn)4;
        } else {
            if (HandsOffMode) {
                HandsOffMode = false;
                CTCCreatureModeManager* ptc_mode_manager;
                PBrain->GetCreature().GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_mode_manager);
                ptc_mode_manager->RemoveMode(0x2d);
            }
            to_continue = ChooseAction(true);
        }
        CTCAIScratchpad* tc_ai_scratchpad;
        PBrain->GetCreature().GetTC(TCI_AI_SCRATCHPAD, &tc_ai_scratchpad);
        UnderScriptControl = tc_ai_scratchpad->GetSeatName() != "" && tc_ai_scratchpad->GetTableName() != "";
    } while (IsStaffed() && to_continue && (FrameLeavePub > CWorld::GetFrame() || UnderScriptControl));
    if (!IsStopping() && !HandsOffMode && IS_THING_ALIVE(PTankard)) {
        CTCTavernTankard* tc_tavern_tankard;
        PTankard.Get()->GetTC(TCI_TAVERN_TANKARD, &tc_tavern_tankard);
        while (tc_tavern_tankard->Fullness != 0.0f) {
            if (!Drink()) {
                Terminate();
                return;
            }
        }
    }
    if (Sitting) {
        Sitting = false;
        if (PTable.Get() && !PTable.Get()->IsDead()) {
            CTCTavernTable* tc_tavern_table;
            ((CThing*)PTable.Get())->GetTC(TCI_TAVERN_TABLE, &tc_tavern_table);
            tc_tavern_table->SetSeatState(SeatIndex, false);
        }
        CBrainOutput output;
        CCreatureAction_GetUpFromSitting action(PBrain->GetCreature(), CCharString("SIT_AT_TABLE_OUTOF"), 0, false);
        if (PBrain->GetCreature().IsFreeToPerformAction(action)) {
            output.SetAsPerformAction(action);
            PBrain->SetOutput(output);
        }
        FreeSeat();
        CTCCreatureModeManager* ptc_mode_manager;
        PBrain->GetCreature().GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_mode_manager);
        ptc_mode_manager->RemoveMode(0xb);
    }
    {
        long fps = CMainGameComponent::GetConstantFPS();
        NextFramePub = (long)(CWorld::GetFrame() + fps * 60.0f);
    }
    PayAtBar();
    AIAction_MinionWander(AI_MOVEMENT_WALKING, 0);
    Terminate();
}


int main()
{
    CThingBuilding pub;
    CThingObject table_a, table_b, tankard;
    CThingObject dead_table;
    dead_table.dead=true;
    CThingAICreature creature;
    CAIBrain brain(creature);
    CTCCreatureModeManager modes;
    CTCAIScratchpad scratch;
    CTCTavernTankard drink={2};
    CTCTavernTable seat_a(&tankard), seat_b(&tankard);
    CTCTavernGame game={true};
    CTCThingOwner owner;
    owner.objects.push_back(CIntelligentPointer<CThing>(&table_a));
    owner.objects.push_back(CIntelligentPointer<CThing>(&table_b));
    owner.objects.push_back(CIntelligentPointer<CThing>(&dead_table));
    pub.components[TCI_THING_OWNER]=&owner;
    table_a.components[TCI_TAVERN_TABLE]=&seat_a;
    table_a.components[TCI_TAVERN_GAME]=&game;
    table_b.components[TCI_TAVERN_TABLE]=&seat_b;
    dead_table.components[TCI_TAVERN_TABLE]=&seat_b;
    tankard.components[TCI_TAVERN_TANKARD]=&drink;
    creature.components[TCI_ENTITY_MODE_MANAGER]=&modes;
    creature.components[TCI_AI_SCRATCHPAD]=&scratch;
    {
        CAIStateGroup_GoToPub no_pub(brain);
        no_pub.Update();
        assert(no_pub.terminations==1 && !no_pub.sit_calls);
    }
    {
        CAIStateGroup_GoToPub none_seat(brain);
        none_seat.PPub=&pub;
        none_seat.Update();
        assert(none_seat.terminations==1 && none_seat.ActionPointIndex==-1);
        assert(seat_a.inquiries==1 && seat_b.inquiries==1);
    }
    seat_a.can_offer_seat=true;
    {
        CAIStateGroup_GoToPub failed_to_sit(brain);
        failed_to_sit.PPub=&pub;
        failed_to_sit.allow_sit=false;
        failed_to_sit.Update();
        assert(failed_to_sit.terminations==1 && failed_to_sit.sit_calls==1 && !failed_to_sit.paid);
    }
    {
        CAIStateGroup_GoToPub visit(brain);
        visit.PPub=&pub;
        scratch.seat="Seat";
        scratch.table="Table";
        visit.Update();
        assert(visit.terminations==1 && visit.sit_calls==1 && visit.choices==2 && visit.silent==1);
        assert(visit.UnderScriptControl && !visit.HandsOffMode && !visit.Sitting);
        assert(visit.drinks==2 && drink.Fullness==0 && visit.frees==1);
        assert(visit.paid==1 && visit.wandered==1 && brain.outputs==1);
        assert(modes.start==1 && modes.stop_hands==1 && modes.stop_sit==1 && seat_a.freed==1);
        assert(visit.FrameLeavePub==1001 && visit.NextFramePub==601);
    }
    {
        CAIStateGroup_GoToPub drink_failed(brain);
        drink_failed.PPub=&pub;
        drink_failed.drink_success=false;
        scratch.seat="";
        drink.Fullness=1;
        drink_failed.Update();
        assert(drink_failed.terminations==1 && !drink_failed.UnderScriptControl);
        assert(drink_failed.drinks==1 && !drink_failed.paid && !drink_failed.frees);
    }
    {
        CAIStateGroup_GoToPub stopping(brain);
        stopping.PPub=&pub;
        stopping.stopping=true;
        stopping.staffed=false;
        creature.action_allowed=false;
        drink.Fullness=1;
        stopping.Update();
        assert(stopping.choices==1 && !stopping.drinks && stopping.frees==1 && stopping.paid==1);
        assert(brain.outputs==1);
    }
    puts("0094ab00_TEST PASS");
    return 0;
}

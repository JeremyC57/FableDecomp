// CAIStateGroup_GoToPub::Update() -- the pub visit: pick a random free tavern table of the pub (dropping tables with no free seat), take its seat and tankard, walk to the seat and sit; then idle there (hands-off while the table's tavern game is being played, else choose an action) until the pub is unstaffed, the visit time is up and no script holds the creature; drink the tankard dry, get up and free the seat, schedule the next visit, pay at the bar and wander off
// Grade: DIFFER(2018v2067) -- 246 difflines (743 for chunk 800's parked source): with row-local __forceinline helpers at the two sites retail expands (ListPushBackInline for the tables push_back, CThingObject::GetTCLowerBoundInline for the TAVERN_TABLE lookup; 0x008DDAD0 push_back_inline precedent) the call sequence follows retail, but the frame is 0x188 v 0x190: retail gives each CIntelligentPointer temporary its own slot (0x1c / 0x30 / 0x24) where ours share [esp+10h]; the tables-node spill is missing and the GFRandom result sits in edx (retail eax)
// Next idea: the three CIntelligentPointer temporaries: retail's separate slots suggest implicit conversion temporaries for reference arguments (chunk 825) -- try the PTable assignments / GetSeat arguments as converting calls rather than named or explicit temporaries; then the tables-node spill
// Leaf: ?Update@CAIStateGroup_GoToPub@@MAEXXZ
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CVectorMap.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"

class CThingBuilding : public CThing { };
class CThingObject : public CThing { public:
    template<class T> __forceinline bool GetTCLowerBoundInline(ETCInterfaceType id, T** ptc)
{
    if (HasTC(id)) {
        ETCInterfaceType type = id;
        CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >* map = &Components;
        CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator first = map->begin();
        long count = map->end() - first;
        while (count > 0) {
            long half = count / 2;
            CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator middle = first + half;
            if (middle->first < type) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        *ptc = (T*)((first == map->end() || type < first->first) ? map->end() : first)->second;
        return true;
    }
    return false;
}
};
class CThingAICreature : public CThing { };
class CBaseObject;
class CRightHandedSet { public: float Up[3], Forward[3]; };

template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>& thing);   // 0x004CC340 (IS_THING_ALIVE<CIntelligentPointer<CThing> >)

long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870
class CMainGameComponent { public: static long ConstantFPS; };   // 0x01375550

class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
};
class CSoundPair { public: long First, Second; };
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _pad04[0x114 - 0x04];
    ~CCombatActionBase();                       // 0x0062E900
};
class CThingCreatureBase : public CThing { public:
    bool IsFreeToPerformAction(const CCreatureActionBase& action) const;   // 0x00662210
};
class CCreatureAction_GetUpFromSitting : public CCombatActionBase { public:
    long SitModeType;                           // +0x114
    bool Flag;                                  // +0x118
    CCreatureAction_GetUpFromSitting(CThingCreatureBase& creature, const CCharString& anim, long sit_mode_type, bool flag);   // 0x00860150
};
class CBrainOutput { public:
    unsigned char _pad[0x48];
    CBrainOutput();                             // 0x00835620
    ~CBrainOutput();                            // 0x006D60C0
    void SetAsPerformAction(const CCreatureActionBase& action);   // 0x00860130
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
    void SetOutput(const CBrainOutput& output); // 0x0088B8F0
};

class CTCCreatureModeManager { public:          // TCI_ENTITY_MODE_MANAGER
    bool AddMode(unsigned long type);           // 0x0073AE10
    void RemoveMode(unsigned long type);        // 0x0073ABA0
};
class CTCThingOwner { public:                   // TCI_THING_OWNER
    unsigned char _pad[0xc];
    std::list<CIntelligentPointer<CThing> > OwnedThings;   // +0x0c
};
class CTCTavernTable { public:                  // TCI_TAVERN_TABLE
    bool GetSeat(CThing& creature, C3DVector& pos, CRightHandedSet& rhset, long& action_point, long& seat, const CCharString* name);   // 0x007B8840
    CThingObject* GetTankard(long seat);        // 0x007B8590
    void SetSeatState(long seat, bool state);   // 0x007B8530
};
class CTCTavernGame { public:                   // TCI_TAVERN_GAME
    unsigned char _pad[0x20];
    bool IsBeingPlayed;                         // +0x20
};
class CTCTavernTankard { public:                // TCI_TAVERN_TANKARD
    unsigned char _pad[0xc];
    float Amount;                               // +0x0c
};
class CTCAIScratchpad { public:                 // TCI_AI_SCRATCHPAD
    unsigned char _pad[0x70];
    CCharString ScriptName0;                    // +0x70
    CCharString ScriptName1;                    // +0x74
};

enum EAIActionReturn { AI_ACTION_RETURN_4 = 4 };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
protected:
    bool IsStopping() const;                    // 0x008FD010
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
    EAIActionReturn AIAction_MinionWander(long repeat, long flags);   // 0x0097F870
public:
    void Terminate();                           // 0x008FCF40
};
class CAIStateGroup_GoToPub : public CAIStateGroupBase {
protected:
    virtual void Update();
    bool ChooseAction(bool);                    // 0x0094A3B0
    bool Drink();                               // 0x00948880
    void FreeSeat();                            // 0x009486D0
    bool IsStaffed();                           // 0x00947FB0
    bool PayAtBar();                            // 0x0094AAC0
    bool WalktoSeatAndSit(bool);                // 0x00949580
    bool GetSeat(CThing* table, const CCharString* name)
    {
        CTCTavernTable* tavern_table;
        if (table->GetTC(TCI_TAVERN_TABLE, &tavern_table))
            return tavern_table->GetSeat(*PBrain->PCreature, Pos, RHSet, ActionPointIndex, SeatIndex, name);
        return false;
    }
public:
    CIntelligentPointer<CThingBuilding> PPub;   // +0x14
    CIntelligentPointer<CThingObject> PBar;     // +0x1c
    CIntelligentPointer<CThingObject> PTable;   // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;   // +0x2c
    CIntelligentPointer<CThing> PTankard;       // +0x34
    long SeatIndex;                             // +0x3c
    C3DVector Pos;                              // +0x40
    CRightHandedSet RHSet;                      // +0x4c
    long ActionPointIndex;                      // +0x64
    long NextFramePub;                          // +0x68
    long FrameLeavePub;                         // +0x6c
    bool Sitting;                               // +0x70
    long BarActionPointIndex;                   // +0x74
    C3DVector BarPos;                           // +0x78
    CRightHandedSet BarRHSet;                   // +0x84
    long BarIndex;                              // +0x9c
    bool HandsOffMode;                          // +0xa0
    bool UnderScriptControl;                    // +0xa1
    long TimeBeforeAskingForBeer;               // +0xa4
};


// Per-site expansion (chunk 801 / 0x008DDAD0 precedent): retail expands the tables push_back at its one site and the CVectorMap
// LowerBound of the tavern-table lookup inside the table-choice loop (0x0094ACE9..0x0094AD23) where every other lookup calls 0x0040F020.
template<class T> __forceinline void ListPushBackInline(std::list<T>& l, const T& x)
{
    std::_List_node<T>* tmp = l._M_node.allocate(1);
    new((void*)&tmp->_M_data) T(x);
    std::_List_node_base* n = l._M_node._M_data;
    std::_List_node_base* p = n->_M_prev;
    tmp->_M_next = n; tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp;
}

void CAIStateGroup_GoToPub::Update()
{
    {
        std::list<CThing*> tables;
        CTCThingOwner* owner;
        if (PPub.Get() && !PPub.Get()->IsDead() && ((CThing*)PPub.Get())->GetTC(TCI_THING_OWNER, &owner)) {
            for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->OwnedThings.begin(); it != owner->OwnedThings.end(); ++it) {
                if ((*it).Get() && !(*it).Get()->IsDead() && (*it).Get()->HasTC(TCI_TAVERN_TABLE))
                    ListPushBackInline<CThing*>(tables, (*it).Get());
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
                    CTCTavernTable* tavern_table;
                    if (PTable.Get()->GetTCLowerBoundInline(TCI_TAVERN_TABLE, &tavern_table) &&
                        tavern_table->GetSeat(*PBrain->PCreature, Pos, RHSet, ActionPointIndex, SeatIndex, 0)) {
                        PTankard = (CThing*)tavern_table->GetTankard(SeatIndex);
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
        long fps = CMainGameComponent::ConstantFPS;
        unsigned long extra = GFRandom(100, GetPWorldSeed());
        FrameLeavePub = (long)(CWorld::GetFrame() + ((float)extra + 100.0f) * fps);
    }
    bool still_here;
    do {
        CTCTavernGame* game;
        if (((CThing*)PTable.Get())->GetTC(TCI_TAVERN_GAME, &game) && game && game->IsBeingPlayed) {
            if (!HandsOffMode) {
                HandsOffMode = true;
                CTCCreatureModeManager* modes;
                PBrain->PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
                modes->AddMode(0x2d);
            }
            still_here = AIAction_DoNothing() == AI_ACTION_RETURN_4;
        } else {
            if (HandsOffMode) {
                HandsOffMode = false;
                CTCCreatureModeManager* modes;
                PBrain->PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
                modes->RemoveMode(0x2d);
            }
            still_here = ChooseAction(true);
        }
        CTCAIScratchpad* scratchpad;
        PBrain->PCreature->GetTC(TCI_AI_SCRATCHPAD, &scratchpad);
        UnderScriptControl = CCharString(scratchpad->ScriptName1) != "" && CCharString(scratchpad->ScriptName0) != "";
    } while (IsStaffed() && still_here && (FrameLeavePub > CWorld::GetFrame() || UnderScriptControl));
    if (!IsStopping() && !HandsOffMode && IS_THING_ALIVE(PTankard)) {
        CTCTavernTankard* tankard;
        PTankard.Get()->GetTC(TCI_TAVERN_TANKARD, &tankard);
        while (tankard->Amount != 0.0f) {
            if (!Drink()) {
                Terminate();
                return;
            }
        }
    }
    if (Sitting) {
        Sitting = false;
        if (PTable.Get() && !PTable.Get()->IsDead()) {
            CTCTavernTable* tavern_table;
            ((CThing*)PTable.Get())->GetTC(TCI_TAVERN_TABLE, &tavern_table);
            tavern_table->SetSeatState(SeatIndex, false);
        }
        CBrainOutput output;
        CCreatureAction_GetUpFromSitting action(*PBrain->PCreature, CCharString("SIT_AT_TABLE_OUTOF"), 0, false);
        if (PBrain->PCreature->IsFreeToPerformAction(action)) {
            output.SetAsPerformAction(action);
            PBrain->SetOutput(output);
        }
        FreeSeat();
        CTCCreatureModeManager* modes;
        PBrain->PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        modes->RemoveMode(0xb);
    }
    {
        long fps = CMainGameComponent::ConstantFPS;
        NextFramePub = (long)(CWorld::GetFrame() + fps * 60.0f);
    }
    PayAtBar();
    AIAction_MinionWander(3, 0);
    Terminate();
}

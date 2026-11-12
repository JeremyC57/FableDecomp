// CAIStateGroup_UsesSeat::GetUpFromSeat() -- free the seat's action point and perform the get-up action (bench or seat animation); when it does not complete, drop the sitting movement mode and report failure
// Leaf: ?GetUpFromSeat@CAIStateGroup_UsesSeat@@IAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00938A10, length 363 bytes, plain `ret`. Ego_r.pdb name (protected, bool).
// Callees: CCharString() 0x0099E4B0 / operator=(const CCharString&) 0x0099EFB0 / ~CCharString 0x0099EAE0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (PSeat +0x14), CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0x2D =
//   CTCUserActionPointBase, bit 13 of +0x24, and 0x31 = CTCCreatureModeManager, bit 17; unchecked: a miss reads the key
//   slot), CTCUserActionPointBase::SetActionPointAsFree 0x007FEF40 (pushes the index onto FreeActionPointIndexes +0xC), CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&)
//   0x00858030 / SetCombatAnimation 0x008574D0 / ~CCombatActionBase 0x0062E900 (CCreatureAction_GetUpFromSitting's
//   inline ctor and dtor, vtable 0x01276C2C), CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70,
//   CTCCreatureModeManager::RemoveMode 0x0073ABA0.
// Data: the get-up animation names GET_UP_FROM_CHAIR_ANIM_NAME @ 0x013BB6E0 ("ST_STAND_UP_FROM_SEAT") and GET_UP_FROM_BENCH_ANIM_NAME @ 0x013BB6E8
//   ("ST_STAND_UP_FROM_BENCH") -- descriptive names of the landed sibling OnReplace 0x009387F0.
// Layout: Ego_r.pdb CAIStateGroup_UsesSeat (PSeat +0x14, GotSeatPos +0x1C, Sitting +0x1D, SeatPosIndex +0x20,
//   SittingOnBench +0x30; no engine header carries the methods); CCreatureAction_GetUpFromSitting SittingMode +0x114 /
//   RemoveOnPerformAction +0x118 (PDB +0x12C / +0x130 over the PDB's 0x18-larger CCombatActionBase).
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BB6E8 GET_UP_FROM_BENCH_ANIM_NAME (anonymous namespace; initializer 0x01221B00), was g_GetUpNameAlternate.
//   0x013BB6E0 GET_UP_FROM_CHAIR_ANIM_NAME (anonymous namespace; initializer 0x01221AC0), was g_GetUpName.
class CCharStringData;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
namespace { extern CCharString GET_UP_FROM_CHAIR_ANIM_NAME; }                         // 0x013BB6E0 (descriptive)
namespace { extern CCharString GET_UP_FROM_BENCH_ANIM_NAME; }                // 0x013BB6E8 (descriptive)

class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public: };
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThingObject : public CThing { public: };
class CTCUserActionPointBase { public:                  // interface 0x2D
    void SetActionPointAsFree(long index);              // 0x007FEF40
};
class CTCCreatureModeManager { public:                  // interface 0x31
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) {}
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail 0x114 bytes
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase& creature, long priority, const CSoundPair& sound);   // 0x00858030
    virtual ~CCombatActionBase();                       // 0x0062E900
    void SetCombatAnimation(const CCharString& animation);      // 0x008574D0
};
class CCreatureAction_GetUpFromSitting : public CCombatActionBase { public:   // vtable 0x01276C2C
    long SittingMode;                                   // +0x114
    bool RemoveOnPerformAction;                         // +0x118
    CCreatureAction_GetUpFromSitting(CThingCreatureBase& creature, const CCharString& animation, long sitting_mode,
                                     bool remove_on_perform_action)
        : CCombatActionBase(creature, 50, CSoundPair()), SittingMode(sitting_mode),
          RemoveOnPerformAction(remove_on_perform_action)
    {
        SetCombatAnimation(animation);
    }
    virtual CCreatureActionBase* Clone() const;
};
enum EAIActionReturn { };
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                      // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                   // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
protected:
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool interruptable);   // 0x0097DB70
};
class CAIStateGroup_UsesSeat : public CAIStateGroupBase { public:
    CIntelligentPointer<CThingObject> PSeat;            // +0x14
    bool GotSeatPos;                                    // +0x1C
    bool Sitting;                                       // +0x1D
    long SeatPosIndex;                                  // +0x20
    float SeatPos[3];                                   // +0x24 C3DVector
    bool SittingOnBench;                                // +0x30
protected:
    bool GetUpFromSeat();
};

bool CAIStateGroup_UsesSeat::GetUpFromSeat()
{
    CCharString animation;
    long sitting_mode;
    if (SittingOnBench) {
        animation = GET_UP_FROM_BENCH_ANIM_NAME;
        sitting_mode = 9;
    } else {
        animation = GET_UP_FROM_CHAIR_ANIM_NAME;
        sitting_mode = 8;
    }
    Sitting = false;
    CTCUserActionPointBase* action_point;
    PSeat.Get()->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &action_point);
    action_point->SetActionPointAsFree(SeatPosIndex);
    GotSeatPos = false;
    SeatPosIndex = -1;
    if (AIAction_PerformActionAndWait(CCreatureAction_GetUpFromSitting(*GetCreature(), animation, sitting_mode, false), false) != 2) {
        CTCCreatureModeManager* modes;
        GetCreature()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        modes->RemoveMode(sitting_mode);
        return false;
    }
    return true;
}
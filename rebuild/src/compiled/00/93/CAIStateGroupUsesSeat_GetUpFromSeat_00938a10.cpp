// CAIStateGroup_UsesSeat::GetUpFromSeat() -- free the seat's action point and perform the get-up action (bench or seat animation); when it does not complete, drop the sitting movement mode and report failure
// Leaf: ?GetUpFromSeat@CAIStateGroup_UsesSeat@@IAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00938A10, length 363 bytes, plain `ret`. Ego_r.pdb name (protected, bool).
// Callees: CCharString() 0x0099E4B0 / operator=(const CCharString&) 0x0099EFB0 / ~CCharString 0x0099EAE0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (PSeat +0x14), CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0x2D =
//   CTCUserActionPointBase, bit 13 of +0x24, and 0x31 = CTCCreatureModeManager, bit 17; unchecked: a miss reads the key
//   slot), CTCUserActionPointBase::SetActionPointAsFree 0x007FEF40 (pushes the index onto FreeActionPointIndexes +0xC), CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&)
//   0x00858030 / SetCombatAnimation 0x008574D0 / ~CCombatActionBase 0x0062E900 (CCreatureAction_GetUpFromSitting's
//   inline ctor and dtor, vtable 0x01276C2C), CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70,
//   CTCCreatureModeManager::RemoveMode 0x0073ABA0.
// Data: the get-up animation names g_GetUpName @ 0x013BB6E0 ("ST_STAND_UP_FROM_SEAT") and g_GetUpNameAlternate @ 0x013BB6E8
//   ("ST_STAND_UP_FROM_BENCH") -- descriptive names of the landed sibling OnReplace 0x009387F0.
// Layout: Ego_r.pdb CAIStateGroup_UsesSeat (PSeat +0x14, GotSeatPos +0x1C, Sitting +0x1D, SeatPosIndex +0x20,
//   SittingOnBench +0x30; no engine header carries the methods); CCreatureAction_GetUpFromSitting SittingMode +0x114 /
//   RemoveOnPerformAction +0x118 (PDB +0x12C / +0x130 over the PDB's 0x18-larger CCombatActionBase).
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
extern CCharString g_GetUpName;                         // 0x013BB6E0 (descriptive)
extern CCharString g_GetUpNameAlternate;                // 0x013BB6E8 (descriptive)

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
        }
    }
};
class CThingCreatureBase : public CThing { public: };
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer { public:                 // vtable 0x0129C95C
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
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
        animation = g_GetUpNameAlternate;
        sitting_mode = 9;
    } else {
        animation = g_GetUpName;
        sitting_mode = 8;
    }
    Sitting = false;
    CTCUserActionPointBase* action_point;
    PSeat.GetPItem()->GetInterfaceV(&action_point, 0x2d);
    action_point->SetActionPointAsFree(SeatPosIndex);
    GotSeatPos = false;
    SeatPosIndex = -1;
    if (AIAction_PerformActionAndWait(CCreatureAction_GetUpFromSitting(*GetCreature(), animation, sitting_mode, false), false) != 2) {
        CTCCreatureModeManager* modes;
        GetCreature()->GetInterfaceV(&modes, 0x31);
        modes->RemoveMode(sitting_mode);
        return false;
    }
    return true;
}
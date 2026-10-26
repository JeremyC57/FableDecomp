// CTCAssassinRush::CTCAssassinRush(CThing* parent) -- set up the assassin-rush component: members cleared (the behind-target position to FLT_MAX, UseCollision on), its special-ability def fetched, a player's spell-cast state switched to rush mode, the spell level read from the inventory abilities, then Reset
// Leaf: ??0CTCAssassinRush
// Retail 0x007A9E40, 416 bytes (`ret 4`). Catalog label "CTCBattleCharge::CTCBattleCharge" is propagated: the vtable stored is
// 0x0126CF2C (CTCAssassinRush, RTTI) and the def name is "SPECIAL_ABILITIES_ASSASSIN_RUSH_DEF".
// Callees: CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer() @ 0x00A01B10 (CIntelligentPointer<CThing> vtable 0x01238C6C),
// CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CGameDefinitionManager::Get @ 0x0044C6B0,
// CDefinitionManager::GetGlobalIndexFromClassIndex @ 0x009AD390, GetDef<CSpecialAbilitiesAssassinRushDef> @ 0x007AB530,
// CVectorMap LowerBound @ 0x0040F020 / const @ 0x004365B0 (inlined GetTC 0xA7 / 0x6F), CThing::GetPlayer @ 0x004C7A30,
// the player's forwarder @ 0x00487170 (calls 0x006345C0 with 0x12, 0), CTCInventoryAbilities::GetAbilityLevel @ 0x005EDE93,
// CTCAssassinRush::Reset @ 0x007A9B70.
// Layout: Ego_r.pdb CTCAssassinRush (packed: PositionStartedFrom +0x2d, CollidedThing +0x39, BehindTargetPos +0x55) up to
// SlowTimeLength +0x44 / BehindTargetPos; retail ItemAlphaList is a 12-byte STLport vector (+0x64), so SpellLevel +0x8c,
// FinalAction +0x90, RadialBlurFadeKey +0x98, UseCollision +0x9c, ScheduleForCancel +0x9e (as the landed CancelMove
// 0x007AA480), FakeCollideWithSomething +0x9f, RushDelayFrameCount +0xa0, CompletedExitModes +0xa4 sit 4 below the PDB.
// Shape: the sibling CTCBattleCharge constructor 0x007A71B0 (same TU family); FLT_MAX is 0x7F7FFFFF.
class CThing;
class CBaseObject;
class CTCBase;
class CPlayer;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSpecialAbilitiesAssassinRushDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
};

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
};

class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x007AB530 for this def
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                                     // 0x0044C6B0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() {}
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

class CTCInventoryAbilities {
public:
    long GetAbilityLevel(long ability);             // 0x005EDE93 (EHeroAbility)
};

class CPlayer {
public:
    void SetModeState(long mode, long state);       // 0x006345C0
    void StopChargeMode();                          // 0x00487170: SetModeState(0x12, 0)
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                        // +0x10 (2 = creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    void FinishCurrentAction();                     // 0x00662650 (CThingCreatureBase)
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
public:
    CPlayer& GetPlayer();   // 0x004C7A30
};

struct CTCSpellCastState {                          // interface type 0xA7 (class not pinned)
    unsigned char _pad00[0x0c];
    bool Active;                                    // +0x0c
    unsigned char _pad0d[3];
    long Mode;                                      // +0x10
};

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];                      // ToBeRemoved, IsInGlobalUpdateTCs
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

template<class T> struct CVector {                  // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

struct SCountBlock;
template<class T> class CCountedPointer { public:
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
};
class CCreatureActionBase;

#pragma pack(push, 1)
class CTCAssassinRush : public CTCBase {            // vtable 0x0126CF2C
public:
    CIntelligentPointer<CThing> PTarget;            // +0x0c
    CDefPointer<const CSpecialAbilitiesAssassinRushDef> PDef;   // +0x14
    CIntelligentPointer<CThing> AssassinTrail;      // +0x18
    CIntelligentPointer<CThing> AssassinDisplacementTrail;   // +0x20
    long TrailDelay;                                // +0x28
    bool StartTrailDelay;                           // +0x2c
    C3DVector PositionStartedFrom;                  // +0x2d
    CIntelligentPointer<CThing> CollidedThing;      // +0x39
    bool StartedMoving;                             // +0x41
    bool JustReleased;                              // +0x42
    unsigned char _pad43;
    long SlowTimeLength;                            // +0x44
    bool DirectionOverride;                         // +0x48
    C3DVector DirectionOverrideVector;              // +0x49
    C3DVector BehindTargetPos;                      // +0x55
    unsigned char _pad61[3];
    CVector<CIntelligentPointer<CThing> > ItemAlphaList;   // +0x64
    float DashDistance;                             // +0x70
    C3DVector LocalRelativeVectorForCamera;         // +0x74
    C3DVector RelativeVectorForCamera;              // +0x80
    long SpellLevel;                                // +0x8c
    CCountedPointer<CCreatureActionBase> FinalAction;   // +0x90
    void* RadialBlurFadeKey;                        // +0x98
    bool UseCollision;                              // +0x9c
    bool IsZTargetting;                             // +0x9d
    bool ScheduleForCancel;                         // +0x9e
    bool FakeCollideWithSomething;                  // +0x9f
    long RushDelayFrameCount;                       // +0xa0
    bool CompletedExitModes;                        // +0xa4
    CTCAssassinRush(CThing* parent);
    virtual void v00();
    void Reset();                                   // 0x007A9B70
};
#pragma pack(pop)

CTCAssassinRush::CTCAssassinRush(CThing* parent)
    : CTCBase(parent), SlowTimeLength(0), BehindTargetPos(3.402823466e+38f, 3.402823466e+38f, 3.402823466e+38f),
      SpellLevel(0), RadialBlurFadeKey(0), UseCollision(true), ScheduleForCancel(false), FakeCollideWithSomething(false),
      RushDelayFrameCount(0), CompletedExitModes(false)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_ASSASSIN_RUSH_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    if (ParentThing->ThingType == 2) {
        if (ParentThing->HasTC(0xa7)) {
            CTCSpellCastState* state = (CTCSpellCastState*)ParentThing->GetTC(0xa7);
            state->Active = true;
            state->Mode = 0xd;
        }
        ParentThing->GetPlayer().StopChargeMode();
    }
    const CThing* parentThing = ParentThing;
    if (parentThing->HasTC(0x6f))
        SpellLevel = ((CTCInventoryAbilities*)parentThing->GetTC(0x6f))->GetAbilityLevel(0xd);
    Reset();
}
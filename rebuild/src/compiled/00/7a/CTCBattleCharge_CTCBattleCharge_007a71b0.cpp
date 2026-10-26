// CTCBattleCharge::CTCBattleCharge(CThing* parent) -- set up the battle-charge component: members cleared, its special-ability def fetched, a player's spell-cast state switched to charge mode, the spell level read from the inventory abilities, then Reset and the creature's current action finished
// Leaf: ??0CTCBattleCharge
// Retail 0x007A71B0, 448 bytes (`ret 4`). Vtable 0x0126CBFC (CTCBattleCharge, RTTI). Callees: CTCBase(CThing*) @ 0x00686800,
// CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90 (CIntelligentPointer<CThing> vtable 0x01238C6C),
// CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CGameDefinitionManager::Get @ 0x0044C6B0,
// CDefinitionManager::GetGlobalIndexFromClassIndex @ 0x009AD390, GetDef<CSpecialAbilitiesBattleChargeDef> @ 0x007A90F0,
// CVectorMap LowerBound @ 0x0040F020 / const @ 0x004365B0 (inlined GetTC 0xA7 / 0x6F), CThing::GetPlayer @ 0x004C7A30,
// the player's 10-byte forwarder @ 0x00487170 (calls 0x006345C0 with 0x12, 0), CTCInventoryAbilities::GetAbilityLevel
// @ 0x005EDE93, CTCBattleCharge::Reset @ 0x007A6F00, CThingCreatureBase::FinishCurrentAction @ 0x00662650.
// Layout: Ego_r.pdb CTCBattleCharge up to RightFeetEffect +0x60 / PoseDelay +0x68 (PTarget +0xc, PDef +0x14, CollidedThing
// +0x24, DashDistance +0x3c, SpellLevel +0x40, PItemRemoved +0x44, RadiusOfEffect +0x4c, ShieldEffect +0x50); retail
// PeopleHit is a 12-byte STLport vector (+0x70), so DisplacementVector +0x7c, FakeCollideWithSomething +0x89,
// ChargeDelayFrameCount +0x8c, CompletedExitModes +0x90 sit 4 below the PDB. The thing's type byte +0x10 (2 = creature).
// Shape: the landed CTCWillEnflame/CTCForcePushPower constructor (0x007A43A0 / 0x007A54A0): default-constructed intelligent
// pointers store only their vtable, the effect pointers are constructed from 0 (SetPItem); the def name is block-scoped.
class CThing;
class CBaseObject;
class CTCBase;
class CPlayer;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSpecialAbilitiesBattleChargeDef {
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
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x007A90F0 for this def
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
    CPlayer* GetPlayer() const;                     // 0x004C7A30
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

class CTCBattleCharge : public CTCBase {            // vtable 0x0126CBFC
public:
    CIntelligentPointer<CThing> PTarget;            // +0x0c
    CDefPointer<const CSpecialAbilitiesBattleChargeDef> PDef;   // +0x14
    C3DVector PositionStartedFrom;                  // +0x18
    CIntelligentPointer<CThing> CollidedThing;      // +0x24
    bool StartedMoving;                             // +0x2c
    bool JustReleased;                              // +0x2d
    bool DirectionOverride;                         // +0x2e
    unsigned char _pad2f;
    C3DVector DirectionOverrideVector;              // +0x30
    float DashDistance;                             // +0x3c
    long SpellLevel;                                // +0x40
    CIntelligentPointer<CThing> PItemRemoved;       // +0x44
    float RadiusOfEffect;                           // +0x4c
    CIntelligentPointer<CThing> ShieldEffect;       // +0x50
    CIntelligentPointer<CThing> LeftFeetEffect;     // +0x58
    CIntelligentPointer<CThing> RightFeetEffect;    // +0x60
    long PoseDelay;                                 // +0x68
    bool StartPoseDelay;                            // +0x6c
    bool HitSomething;                              // +0x6d
    CVector<CIntelligentPointer<CThing> > PeopleHit;   // +0x70
    C3DVector DisplacementVector;                   // +0x7c
    bool IsZTargetting;                             // +0x88
    bool FakeCollideWithSomething;                  // +0x89
    long ChargeDelayFrameCount;                     // +0x8c
    bool CompletedExitModes;                        // +0x90
    CTCBattleCharge(CThing* parent);
    virtual void v00();
    void Reset();                                   // 0x007A6F00
};

CTCBattleCharge::CTCBattleCharge(CThing* parent)
    : CTCBase(parent), DashDistance(0.0f), SpellLevel(0), RadiusOfEffect(0.0f), ShieldEffect(0), LeftFeetEffect(0),
      RightFeetEffect(0), PoseDelay(0), StartPoseDelay(false), HitSomething(false), DisplacementVector(0.0f, 0.0f, 0.0f),
      FakeCollideWithSomething(false), ChargeDelayFrameCount(0), CompletedExitModes(false)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_BATTLE_CHARGE_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    if (ParentThing->ThingType == 2) {
        if (ParentThing->HasTC(0xa7)) {
            CTCSpellCastState* state = (CTCSpellCastState*)ParentThing->GetTC(0xa7);
            state->Active = true;
            state->Mode = 0xc;
        }
        ParentThing->GetPlayer()->StopChargeMode();
    }
    const CThing* parentThing = ParentThing;
    if (parentThing->HasTC(0x6f))
        SpellLevel = ((CTCInventoryAbilities*)parentThing->GetTC(0x6f))->GetAbilityLevel(0xc);
    Reset();
    ParentThing->FinishCurrentAction();
}
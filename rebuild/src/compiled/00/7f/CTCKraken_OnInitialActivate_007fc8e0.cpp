// CTCKraken::OnInitialActivate() -- link the boss component and the kraken def, find the centre marker, reset the explosion list and the attack state, enter the boss's current phase, then make the kraken untargetable and clear its leave-experience-orbs override
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FC8E0, 230 bytes (plain `ret`). Vtable 0x01241B94 (CTCKraken, rtti-vtables.tsv) slot 6.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (twice: CTCBoss 0xAD, CTCTargeted 9),
// CDefClassBase::GetSubDef<CKrakenDef>(CDefPointer*) @ 0x007FCD50 (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>",
// propagated), CTCKraken::GetCentreMarker @ 0x007FBC90 (landed), the explosion list's clear @ 0x004B8050, CTCBoss::GetCurrentPhase
// @ 0x007AED10, CTCKraken::SetPhase @ 0x007FC820; the thing def's virtual slot 1 on the last release.
// Layout (retail, as SetPhase 0x007FC820): PKrakenDef +0x0C, ExplosionList +0x30, PTCBoss +0x40, Tentacles +0x44,
// AttackingTentacleIterator +0x48, TentacleReadyToStrike +0x50, KillBeamCounter +0x58, ReturnHeadToWater +0x5C. CThing:
// interface bitset +0x20, interface map +0x44, def pointer +0x70; CThingCreatureBase bit field byte +0x15C (PDB 0x184), bit 5 =
// LeaveExperienceOrbsOverride. CTCTargeted (Ego_r.pdb) Targetable +0x10.
// Shape: the parent thing is a block-scoped local for the first two statements (ebx, later the zero register); the boss lookup
// is the out-parameter GetInterface writing PTCBoss; the thing def is an inline by-value getter whose
// null tests share the zero register with the later clears; the targeted lookup is the unchecked `T* tc; GetInterface(&tc, 9);`
// (the absent path reads the never-written local, as the landed 0x00898410).
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
};

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                              // slot 1
    long m_refCount;                                     // +0x04
};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object && --Object->m_refCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};

struct CKrakenDef;
struct CDefClassBase : CDefPointee {
    bool GetSubDef(CDefPointer<CKrakenDef>* out);        // 0x007FCD50
};

class CThing {
public:
    unsigned char              _pad00[0x20];
    unsigned long              m_presentBits[9];         // +0x20
    CInterfaceMap              m_interfaces;             // +0x44
    unsigned char              _pad4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;                     // +0x70
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc;
        GetInterface(&tc, interfaceType);
        return tc;
    }
};

class CThingCreatureBase : public CThing {
public:
    unsigned char _pad74[0x15c - 0x74];
    unsigned char CurrentlyFrameUpdating : 1;            // +0x15c bit 0
    unsigned char Blinking : 1;
    unsigned char MeleeAttacker : 1;
    unsigned char MeleeAttacked : 1;
    unsigned char LeaveDeadBodyOverride : 1;
    unsigned char LeaveExperienceOrbsOverride : 1;       // bit 5
    unsigned char FollowHeroThroughTeleporters : 1;
    unsigned char HeadPosCached : 1;
};

struct CTCTargeted {
    unsigned char _pad00[0x10];
    bool          Targetable;                            // +0x10
};

struct CTCBoss {
    long GetCurrentPhase();                              // 0x007AED10
};

struct SListNode { SListNode* next; SListNode* prev; };

struct CThingPointerList {                               // VC6-style std::list<CIntelligentPointer<CThing> >: the head node only
    SListNode* m_head;
    void clear();                                        // 0x004B8050
};

typedef long EKrakenBossPhase;

class CTCKraken {
public:
    void*                     __vfptr;                   // +0x00
    CThingCreatureBase*       ParentThing;               // +0x04 (CTCBase)
    unsigned char             _pad08[4];
    CDefPointer<CKrakenDef>   PKrakenDef;                // +0x0c
    unsigned char             _pad10[0x30 - 0x10];
    CThingPointerList         ExplosionList;             // +0x30
    unsigned char             _pad34[0x40 - 0x34];
    CTCBoss*                  PTCBoss;                   // +0x40
    CThingPointerList         Tentacles;                 // +0x44
    SListNode*                AttackingTentacleIterator; // +0x48
    long                      TentacleStrikeCounter;     // +0x4c
    bool                      TentacleReadyToStrike;     // +0x50
    bool                      HeadTrackTarget;           // +0x51
    bool                      HeadHasBeenHit;            // +0x52
    long                      LastStrikeTick;            // +0x54
    long                      KillBeamCounter;           // +0x58
    bool                      ReturnHeadToWater;         // +0x5c
    void GetCentreMarker();                              // 0x007FBC90
    void SetPhase(EKrakenBossPhase phase);               // 0x007FC820
    void OnInitialActivate();
};

void CTCKraken::OnInitialActivate()
{
    {
        CThingCreatureBase* thing = ParentThing;
        thing->GetInterface(&PTCBoss, 0xad);
        thing->GetDef()->GetSubDef(&PKrakenDef);
    }
    GetCentreMarker();
    ExplosionList.clear();
    AttackingTentacleIterator = Tentacles.m_head;
    KillBeamCounter = 0;
    TentacleReadyToStrike = false;
    ReturnHeadToWater = false;
    SetPhase(PTCBoss->GetCurrentPhase());
    ParentThing->GetTC<CTCTargeted>(9)->Targetable = false;
    ParentThing->LeaveExperienceOrbsOverride = false;
}
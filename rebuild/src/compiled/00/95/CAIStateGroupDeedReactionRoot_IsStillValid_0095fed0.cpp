// CAIStateGroup_DeedReactionRoot::IsStillValid() const -- the reaction stays valid while its reactee is alive (and, for hero-only reactions, still the hero) unless the creature has a pending deed of another type that the reactee ranks above the one being reacted to
// Leaf: IsStillValid@CAIStateGroup_DeedReactionRoot
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095FED0; SW2 sweep (advisor/0095fed0/probe/a2.cpp).
// Layout: the base follows engine/CAIStateGroupBase.h (PBrain +4), modelled locally; Reactee CIntelligentPointer<CThing> +0x14,
//   DeedType +0x1c, OnlyReactToHero +0x24.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool TestTC(long interfaceType) const { return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0; }
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
    const CThing* GetPItemConst() const { return GetPItem(); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    CThing* item = p.GetPItem();
    return item != 0 && !(item->m_flags91 & 1);
}
struct CPlayer {
    unsigned char _pad[0x714];
    CBaseIntelligentPointer CharacterThing;     // +0x714
    const CBaseIntelligentPointer& GetCharacterThing() const { return CharacterThing; }
};
struct CWorld {
    unsigned char _pad[0x74];
    CPlayer* PMainPlayer;                       // +0x74
};
struct CIVCountedObject {
    virtual void Destroy(int flags);
    virtual void Delete();                      // slot 1 (+4)
    long IVRefCount;                            // +4
    long Release() { return --IVRefCount; }
};
template<class T> struct CDefPointer {
    T* m_pObject;
    ~CDefPointer() { if (m_pObject) { T* p = m_pObject; if (p->Release() == 0) p->Delete(); m_pObject = 0; } }
    T* operator->() const { return m_pObject; }
};
struct COpinionDeedEffectsDef : CIVCountedObject {
    unsigned char _08[0x48];
    long Priority;                              // +0x50
};
struct CTCHeroOpinionDeedLog {                  // interface 0x51
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630 (landed spelling; label COpinionDeedDefTable::GetDeedDef)
};
struct COpinionDeed { long Type; };
struct CTCCreatureOpinionOfHero {               // interface 0x4f
    COpinionDeed* GetHeadPendingDeed();         // 0x00683770
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0xc];
    CWorld* GetPThingWorld() const;             // 0x008FCFA0
};
struct CAIStateGroup_DeedReactionRoot : CAIStateGroupBase {
    CBaseIntelligentPointer Reactee;            // +0x14
    long DeedType;                              // +0x1c
    long _20;
    bool OnlyReactToHero;                       // +0x24
    virtual bool IsStillValid() const;
};

bool CAIStateGroup_DeedReactionRoot::IsStillValid() const
{
    CThing* hero = GetPThingWorld()->PMainPlayer->GetCharacterThing().GetPItem();
    if (IS_THING_ALIVE(Reactee) && (!OnlyReactToHero || hero == Reactee.GetPItem() || hero == 0)) {
        CTCHeroOpinionDeedLog* reactions;
        if (Reactee.GetPItemConst()->GetInterface(&reactions, 0x51)) {
            CTCCreatureOpinionOfHero* opinion;
            PBrain->Creature->GetInterface(&opinion, 0x4f);
            COpinionDeed* deed = opinion->GetHeadPendingDeed();
            if (deed && deed->Type != DeedType && reactions->GetDeedEffectsDef(deed->Type)->Priority < reactions->GetDeedEffectsDef(DeedType)->Priority)
                return false;
        }
        return true;
    }
    return false;
}
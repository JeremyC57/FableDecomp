// CTCCreatureOpinionOfHero::InformOfDeed(long deedHandle, bool flag) -- look the hero's logged deed up by handle and react to it (its type, the handle, the deed type's effects def)
// Leaf: ?InformOfDeed@CTCCreatureOpinionOfHero@@QAEXJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00683590, 179 bytes (`ret 8`).
// Callees: CTCBase::GetPlayerManager() @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature
// @ 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCHeroOpinionDeedLog::GetDeedFromHandle(long)
// @ 0x006C58F0, the deed log's effects-def getter @ 0x006C5630 (CDefPointer by hidden pointer; manifest "CThingBuilding::
// GetFloorMaterial" is propagated, named descriptively), CTCCreatureOpinionOfHero::InformOfDeed(EOpinionDeedType, long,
// const COpinionDeedEffectsDef*, bool) @ 0x00682810, the def's virtual slot 1 on the last release.
// Layout: CThing bitset +0x20 (deed log = interface 0x51, bit 17 of the +0x28 word), map +0x44; COpinionDeed type +0.
// Shape: the deed log is the unchecked out-parameter GetInterface (the absent arm reads the never-written local's home, the
// `deedHandle` slot, into edi); the effects def is a CDefPointer temporary returned into the dead handle slot (the handle lives
// on in ebx), its object read through the returned pointer, and released after the call by a null-tested dtor calling the def's inline ReleaseTemporary() over its ref-count base at +4.
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};
struct CPlayer {
    CThing* GetHeroCreature();                          // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct COpinionDeedEffectsDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
struct COpinionDeed {
    long Type;                                          // +0x00 EOpinionDeedType
};
struct CTCHeroOpinionDeedLog {
    COpinionDeed* GetDeedFromHandle(long handle);       // 0x006C58F0
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};
struct CTCCreatureOpinionOfHero {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CPlayerManager* GetPlayerManager();                 // 0x00686D40 (CTCBase)
    void InformOfDeed(long deedType, long deedHandle, const COpinionDeedEffectsDef* effects, bool flag);   // 0x00682810
    void InformOfDeed(long deedHandle, bool flag);
};

void CTCCreatureOpinionOfHero::InformOfDeed(long deedHandle, bool flag)
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        COpinionDeed* deed = log->GetDeedFromHandle(deedHandle);
        if (deed)
            InformOfDeed(deed->Type, deedHandle, log->GetDeedEffectsDef(deed->Type).Object, flag);
    }
}

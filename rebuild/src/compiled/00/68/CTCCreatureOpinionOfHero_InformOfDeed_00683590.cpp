// CTCCreatureOpinionOfHero::InformOfDeed(long deedHandle, bool flag) -- look the hero's logged deed up by handle and react to it (its type, the handle, the deed type's effects def)
// Leaf: ?InformOfDeed@CTCCreatureOpinionOfHero@@QAEXJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00683590, 179 bytes (`ret 8`).
// Callees: CTCBase::GetPlayerManager() @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature
// @ 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCHeroOpinionDeedLog::GetDeedFromHandle(long)
// @ 0x006C58F0, the deed log's effects-def getter @ 0x006C5630 (CDefPointer by hidden pointer; manifest "CThingBuilding::
// GetFloorMaterial" is propagated, named descriptively), CTCCreatureOpinionOfHero::InformOfDeed(EOpinionDeedType, long,
// const COpinionDeedEffectsDef*, bool) @ 0x00682810, the def's virtual slot 1 on the last release.
// Layout: CThing bitset +0x20 (deed log = interface 0x51, bit 17 of the +0x28 word), map +0x44; COpinionDeed type +0.
// Shape: the deed log is the unchecked out-parameter GetInterface (the absent arm reads the never-written local's home, the
// `deedHandle` slot, into edi); the effects def is a CDefPointer temporary returned into the dead handle slot (the handle lives
// on in ebx), its object read through the returned pointer, and released after the call by a null-tested dtor calling the def's inline ReleaseTemporary() over its ref-count base at +4.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
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
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                          // 0x00487DC0
};
class CPlayerManager { public:
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
class COpinionDeedEffectsDef : public CDefVirtual, public CRefCounted { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
class COpinionDeed { public:
    long Type;                                          // +0x00 EOpinionDeedType
};
class CTCHeroOpinionDeedLog { public:
    COpinionDeed* GetDeedFromHandle(long handle);       // 0x006C58F0
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCCreatureOpinionOfHero : public CTCBase { public:
    CThing* ParentThing;                                // +0x04 (CTCBase)
    void InformOfDeed(long deedType, long deedHandle, const COpinionDeedEffectsDef* effects, bool flag);   // 0x00682810
    void InformOfDeed(long deedHandle, bool flag);
};

void CTCCreatureOpinionOfHero::InformOfDeed(long deedHandle, bool flag)
{
    CThing* hero = GetPlayerManager().GetMainPlayer()->GetPControlledCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        COpinionDeed* deed = log->GetDeedFromHandle(deedHandle);
        if (deed)
            InformOfDeed(deed->Type, deedHandle, log->GetDeedEffectsDef(deed->Type).Object, flag);
    }
}
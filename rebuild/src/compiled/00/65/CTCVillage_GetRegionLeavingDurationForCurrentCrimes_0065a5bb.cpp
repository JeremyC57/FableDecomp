// CTCVillage::GetRegionLeavingDurationForCurrentCrimes() -- the longest knowledge life, in frames, of the hero's deeds recorded as crimes against this village (0 when there is no hero)
// Leaf: GetRegionLeavingDurationForCurrentCrimes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A5BB, 203 bytes (plain `ret`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (`and [ebp-4],0`, `push 51h ; pop edi`).
// Callees: CWorld::GetCurrentPlayer() const 0x0049D9D0 (on g_pMainGameComponent 0x013B86A0 +0x24); CPlayer::GetPControlledCreature()
//          0x00487DC0; std::bitset<>::test(size_t) const 0x00410DE0 (interface 0x51, the deed log); CVectorMap<
//          ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetTC); CTCHeroOpinionDeedLog::GetDeedFromHandle(
//          long) 0x006C58F0; the deed log's effects-def getter 0x006C5630 (CDefPointer by hidden pointer; landed spelling
//          GetDeedEffectsDef, Ego_r.pdb PeekPDeedEffectsDef(EOpinionDeedType) const); the STLport tree increment
//          Successor 0x004292C0 (`__cdecl`); g_framesPerSecond 0x01375550; the def's slot 1 on the last release.
// Layout: retail CTCVillage PlayersCrimes (set<long>, header node pointer first) +0x64 (as the landed IsHeroMurderer
//         0x0065A1FD); set node value +0x10; COpinionDeed DeedType +0x00; retail COpinionDeedEffectsDef
//         KnowledgeLifeInSeconds +0x3c (PDB +0x40, 0x3c-byte def base).
// Shape: the deed log is assigned only when present (`T* log; if (HasTC) log = GetTC;`, kept in a frame word); the set walk
//        keeps end() in ebx; the effects def is a temporary whose null-tested destructor releases it and clears its
//        pointer (the store survives because the compare follows; `__forceinline`, this size TU otherwise calls it out
//        of line); its frame word is the lookup key's.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
    }
};
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CWorld { public:
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern long g_framesPerSecond;                  // 0x01375550

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class COpinionDeedEffectsDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad_0x08[0x34];
    long KnowledgeLifeInSeconds;                // +0x3c
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
class COpinionDeed { public:
    long DeedType;                              // +0x00 EOpinionDeedType
};
class CTCHeroOpinionDeedLog { public:
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};

struct SNode {                                  // STLport _Rb_tree_node<long>
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   _M_value_field;                      // +0x10
};
SNode* __cdecl Successor(SNode* node);          // 0x004292C0

class CTCVillage { public:
    unsigned char _pad_0x00[0x64];
    SNode*        PlayersCrimes;                // +0x64 set<long> header node
    long GetRegionLeavingDurationForCurrentCrimes();
};

long CTCVillage::GetRegionLeavingDurationForCurrentCrimes()
{
    long longest = 0;
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetPControlledCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        if (hero->HasTC(0x51))
            log = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
        SNode* end = PlayersCrimes;
        for (SNode* it = end->_M_left; it != end; it = Successor(it)) {
            COpinionDeed* deed = log->GetDeedFromHandle(it->_M_value_field);
            if (deed) {
                long duration = log->GetDeedEffectsDef(deed->DeedType).Object->KnowledgeLifeInSeconds * g_framesPerSecond;
                if (duration > longest)
                    longest = duration;
            }
        }
    }
    return longest;
}
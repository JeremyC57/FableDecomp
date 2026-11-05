// CTCVillage::GetRegionLeavingDurationForCurrentCrimes() -- the longest knowledge life, in frames, of the hero's deeds recorded as crimes against this village (0 when there is no hero)
// Leaf: GetRegionLeavingDurationForCurrentCrimes
// Plan phase 5 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members first/last are the header CArray base's begin() / end() (STLport vector start +0 / finish +4); the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad24[0x20] removed.
// Naming wave 18: the interface-presence stand-in CBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A5BB, 203 bytes (plain `ret`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (`and [ebp-4],0`, `push 51h ; pop edi`).
// Callees: CWorld::PeekMyPlayer() const 0x0049D9D0 (on CMainGameComponent::Get() 0x013B86A0 +0x24); CPlayer::GetPControlledCreature()
//          0x00487DC0; std::bitset<>::test(size_t) const 0x00410DE0 (interface 0x51, the deed log); CVectorMap<
//          ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetTC); CTCHeroOpinionDeedLog::GetDeedFromHandle(
//          long) 0x006C58F0; the deed log's effects-def getter 0x006C5630 (CDefPointer by hidden pointer; landed spelling
//          GetDeedEffectsDef, Ego_r.pdb PeekPDeedEffectsDef(EOpinionDeedType) const); the STLport tree increment
//          Successor 0x004292C0 (`__cdecl`); CMainGameComponent::GetConstantFPS() 0x01375550; the def's slot 1 on the last release.
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

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    std::bitset<274>       m_present;                    // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
};
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CWorld {
public:
    CPlayer const* PeekMyPlayer() const;   // 0x0049D9D0 (Ego_r.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ)
private:           // 0x0049D9D0
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
private:
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};

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
    CThing* hero = ((CPlayer*)((CWorld*)(CMainGameComponent::Get()->World))->PeekMyPlayer())->GetPControlledCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        if (hero->HasTC(0x51))
            log = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
        SNode* end = PlayersCrimes;
        for (SNode* it = end->_M_left; it != end; it = Successor(it)) {
            COpinionDeed* deed = log->GetDeedFromHandle(it->_M_value_field);
            if (deed) {
                long duration = log->GetDeedEffectsDef(deed->DeedType).Object->KnowledgeLifeInSeconds * CMainGameComponent::GetConstantFPS();
                if (duration > longest)
                    longest = duration;
            }
        }
    }
    return longest;
}
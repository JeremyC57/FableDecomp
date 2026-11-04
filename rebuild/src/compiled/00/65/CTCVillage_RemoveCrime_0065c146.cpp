// CTCVillage::RemoveCrime(long deedHandle) -- forgets one of the hero's crimes: the deed log drops the deed and every witness deed chained to it, the village drops the handle, and with no crimes left the village is no longer the hero's enemy because of crime
// Leaf: ?RemoveCrime@CTCVillage@@QAEXJ@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad24[0x20] removed.
// Naming wave 18: the interface-presence stand-in CBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065C146, 182 bytes (`ret 4`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Ego_r.pdb `?RemoveCrime@CTCVillage@@QAEXJ@Z`.
// Callees: CWorld::PeekMyPlayer() const 0x0049D9D0 (CMainGameComponent::Get() 0x013B86A0 +0x24); CPlayer::GetPControlledCreature()
//          0x00487DC0; std::bitset<>::test(size_t) const 0x00410DE0 (interface 0x51); CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound 0x0040F020; CTCHeroOpinionDeedLog::GetDeedFromHandle(long) 0x006C58F0,
//          ::RemoveDeedStillSearchingForWitnesses(long) 0x006C5920, ::RemoveDeed(long) 0x006C59E0 (landed spellings),
//          ::FindWitnessDeed(long) 0x006C5710 (Ego_r.pdb `?FindWitnessDeed@CTCHeroOpinionDeedLog@@QAEJJ@Z`);
//          set<long>::erase(const long&) 0x00639590.
// Layout: retail CTCVillage PlayersCrimes (set<long>: header node, node count) +0x64/+0x68, IsEnemyBecauseOfCrime +0x7c.
// Shape: the deed log comes from the unchecked out-parameter GetInterface (the absent arm reads the never-written log's
//        home, the handle's argument slot -- the handle itself lives on in ebx); the witness chain is a `for (;;)` that
//        removes each deed found for the ORIGINAL handle; `this` is spilled to [ebp-4]; the set erase takes the handle
//        parameter by address.
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
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CWorld {
public:
    CPlayer const* PeekMyPlayer() const;   // 0x0049D9D0 (Ego_r.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ)
private:           // 0x0049D9D0
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};


class CTCHeroOpinionDeedLog { public:
    class COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
    long FindWitnessDeed(long handle);          // 0x006C5710
    void RemoveDeedStillSearchingForWitnesses(long handle);   // 0x006C5920
    void RemoveDeed(long handle);               // 0x006C59E0
};

struct SNode;
struct CCrimeSet {                              // retail STLport set<long>, 12 bytes
    SNode*        _M_header;                    // +0x00
    unsigned long _M_node_count;                // +0x04
    unsigned long _M_key_compare;               // +0x08 (empty less<long>, padded)
    unsigned long size() const { return _M_node_count; }
    unsigned long erase(const long& key);       // 0x00639590
};

class CTCVillage { public:
    unsigned char _pad_0x00[0x64];
    CCrimeSet     PlayersCrimes;                // +0x64
    unsigned char _pad_0x70[0xc];
    bool          IsEnemyBecauseOfCrime;        // +0x7c
    void RemoveCrime(long deedHandle);
};

// R&D I-3 sweep: a same-TU out-of-line set<long>::erase(const long&) (the key is only read) lets the crime handle stay
// in its argument slot as in retail.
#pragma auto_inline(off)
unsigned long CCrimeSet::erase(const long&)
{
    unsigned long erased = _M_node_count != 0;
    _M_node_count -= erased;
    return erased;
}
#pragma auto_inline(on)
void CTCVillage::RemoveCrime(long deedHandle)
{
    CThing* hero = ((CPlayer*)((CWorld*)(CMainGameComponent::Get()->World))->PeekMyPlayer())->GetPControlledCreature();
    if (hero) {
        long handle = deedHandle;
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        if (log->GetDeedFromHandle(handle)) {
            log->RemoveDeedStillSearchingForWitnesses(handle);
            long deed = handle;
            for (;;) {
                log->RemoveDeed(deed);
                long witness = log->FindWitnessDeed(handle);
                if (witness == -1)
                    break;
                log->RemoveDeedStillSearchingForWitnesses(witness);
                deed = witness;
            }
        }
    }
    PlayersCrimes.erase(deedHandle);
    if (PlayersCrimes.size() == 0)
        IsEnemyBecauseOfCrime = false;
}
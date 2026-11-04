// CTCVillage::RemoveTimedoutCrimes() -- drops every crime handle the hero's deed log no longer knows (the deed timed out); with no crimes left the village is no longer the hero's enemy because of crime
// Leaf: RemoveTimedoutCrimes
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad24[0x20] removed.
// Naming wave 18: the interface-presence stand-in CBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A7A9, 169 bytes (plain `ret`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Ego_r.pdb `?RemoveTimedoutCrimes@CTCVillage@@QAEXXZ`.
// Callees: CWorld::PeekMyPlayer() const 0x0049D9D0 (CMainGameComponent::Get() 0x013B86A0 +0x24); CPlayer::GetPControlledCreature()
//          0x00487DC0; std::bitset<>::test(size_t) const 0x00410DE0 (interface 0x51); CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound 0x0040F020; the STLport tree increment Successor 0x004292C0 (`__cdecl`);
//          CTCHeroOpinionDeedLog::GetDeedFromHandle(long) 0x006C58F0; set<long>::erase(iterator) 0x00638D70 (the iterator
//          by value, built in its argument slot).
// Layout: retail CTCVillage PlayersCrimes (set<long>: header node, node count) +0x64/+0x68, IsEnemyBecauseOfCrime +0x7c
//         (PDB +0x108).
// Shape: the deed log comes from the unchecked out-parameter GetInterface (its address taken, so it lives in a frame word); the set is reached through a named pointer local (ebx, so `this` is
//        spilled to [ebp-8]); end() is cached (edi); the walk is the landed RemoveApologiseableCrimes 0x0065CE56 shape: a
//        separate `next` copy (kept in the lookup key's frame word) advanced before the lookup, the current node erased
//        when its deed is gone, then `it = next` (the `crime = it; ++it` and `it++` forms keep `next` in a register and
//        shrink the frame); the iterator has a user copy
//        constructor so erase's by-value argument is built in place (`push ecx ; mov eax,esp ; mov [eax],esi`).
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


class COpinionDeed { public:
    long DeedType;                              // +0x00 EOpinionDeedType
};
class CTCHeroOpinionDeedLog { public: COpinionDeed const* GetDeedFromHandle(long a0) const; private:   // 0x006C58F0
};

struct SNode {                                  // STLport _Rb_tree_node<long>
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   _M_value_field;                      // +0x10
};
SNode* __cdecl Successor(SNode* node);          // 0x004292C0

struct SCrimeIterator {                         // STLport set<long>::iterator
    SNode* _M_node;
    SCrimeIterator(SNode* node) : _M_node(node) { }
    SCrimeIterator(const SCrimeIterator& other) : _M_node(other._M_node) { }
    long operator*() const { return _M_node->_M_value_field; }
    SCrimeIterator& operator++() { _M_node = Successor(_M_node); return *this; }
    bool operator!=(const SCrimeIterator& other) const { return _M_node != other._M_node; }
};
struct CCrimeSet {                              // retail STLport set<long>, 12 bytes
    SNode*        _M_header;                    // +0x00
    unsigned long _M_node_count;                // +0x04
    unsigned long _M_key_compare;               // +0x08 (empty less<long>, padded)
    SCrimeIterator begin() const { return SCrimeIterator(_M_header->_M_left); }
    SCrimeIterator end() const { return SCrimeIterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
    void erase(SCrimeIterator position);        // 0x00638D70
};

class CTCVillage { public:
    unsigned char _pad_0x00[0x64];
    CCrimeSet     PlayersCrimes;                // +0x64
    unsigned char _pad_0x70[0xc];
    bool          IsEnemyBecauseOfCrime;        // +0x7c
    void RemoveTimedoutCrimes();
};

void CTCVillage::RemoveTimedoutCrimes()
{
    CThing* hero = ((CPlayer*)((CWorld*)(CMainGameComponent::Get()->World))->PeekMyPlayer())->GetPControlledCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        CCrimeSet* crimes = &PlayersCrimes;
        SCrimeIterator end = crimes->end();
        SCrimeIterator it = crimes->begin();
        SCrimeIterator next = it;
        while (it != end) {
            ++next;
            if (!((COpinionDeed*)log->GetDeedFromHandle(*it)))
                crimes->erase(it);
            it = next;
        }
    }
    if (PlayersCrimes.size() == 0)
        IsEnemyBecauseOfCrime = false;
}
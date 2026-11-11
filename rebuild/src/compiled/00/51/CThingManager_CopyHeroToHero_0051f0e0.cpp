// CThingManager::CopyHeroToHero(CThing& from, CThing& to, vector<CThing*>& things) -- carries the hero state over from one hero thing to another: the components the source has (interface types 3, 4, 0x5E, 0x5C, 0x68) are copied into the target through CThing::AddAndCopyTC (type 3 after resetting the target's), types 0x6F and 0x14 through the target component's virtual copy
// Leaf: ?CopyHeroToHero@CThingManager@@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051F0E0, 595 bytes (0x253 + int3 padding to 608), thiscall `ret 0Ch` (`this` and
//   `things` unused). Callers: CThingManager 0x00521E42 (the morph loader at 0x00521CF0) and a function-pointer slot at
//   0x0047FB0C.
// Identity: Ego_r.pdb ?CopyHeroToHero@CThingManager@@AAEXAAVCThing@@0AAV?$vector@PAVCThing@@...@std@@@Z; the catalog's
//   "CTCHeroMorph_ApplyEffects_WithLookup" is descriptive.
// Callees: CInterfaceMap::LowerBound 0x0040F020 (non-const: the type-3 lookups and the target lookups) and its const
//   overload 0x004365B0 (the source lookups through the const CThing reference), the type-3
//   component's reset 0x0071C260, CThing::AddAndCopyTC(const CTCBase&, bool) 0x004C9F60, the component's virtual copy (slot 12,
//   +0x30).
// Layout: retail CThing: the interface presence bitset<274> +0x20 (nine words), the interface map +0x44 (as the landed
//   GetInterface models).
// Shape: the interface lookup is the out-parameter form with the UNCHECKED presence test (`bitset::operator[] const`,
//   inline mask tests instead of the out-of-line checked test 0x00410DE0); the lookup key temporary reuses the dead
//   `from` argument slot (from lives in ebx), the target's uninitialised out locals share the `to` slot (the miss path
//   calls through whatever it holds, as YesCloseInventory 0x0056FBED does).
//   The six source lookups after the type-3 block go through a const CThing reference, i.e. the const GetInterface
//   over the const LowerBound overload (retail's 0x004365B0); the type-3 and target lookups use the non-const one
//   (0x0040F020): the two byte-identical copies are the two overloads, one symbol per retail address.
#include "fable_compiler.h"
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void CopyStateFrom(CTCBase* source);     // slot 12 (+0x30)
};
class CTCHeroMorph { public:
    void KillParticles();   // 0x0071C260 (Ego_r.pdb ?KillParticles@CTCHeroMorph@@QAEXXZ)
};
struct CTCHeroType3 : CTCBase {                      // interface type 3
};

struct CPresenceBits {                               // std::bitset<274> (nine words)
    unsigned long _M_w[9];
    bool operator[](unsigned int pos) const { return (_M_w[pos / 32] & (1UL << (pos % 32))) != 0; }
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    void AddAndCopyTC(const CTCBase& other, bool flag);   // 0x004C9F60 (Ego_r.pdb ?AddAndCopyTC@CThing@@QAEXABVCTCBase@@_N@Z)
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present[wanted]) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* found = map->LowerBound(type);
            const CInterfaceMap::value_type* it;
            if (found != map->end() && !(wanted < found->first))
                it = found;
            else
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted) const
    {
        if (m_present[wanted]) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* found = map->LowerBound(type);
            const CInterfaceMap::value_type* it;
            if (found != map->end() && !(wanted < found->first))
                it = found;
            else
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};


class CThingManager {
public:
    void CopyHeroToHero(CThing& from, CThing& to, std::vector<CThing*>& things);
};

void CThingManager::CopyHeroToHero(CThing& from, CThing& to, std::vector<CThing*>&)
{
    CTCHeroType3* sourceType3;
    if (from.GetInterface(&sourceType3, 3)) {
        CTCHeroType3* targetType3;
        to.GetInterface(&targetType3, 3);
        ((CTCHeroMorph*)targetType3)->KillParticles();
        to.AddAndCopyTC(*sourceType3, true);
    }
    const CThing& sourceThing = from;
    CTCBase* source;
    if (sourceThing.GetInterface(&source, 4))
        to.AddAndCopyTC(*source, true);
    if (sourceThing.GetInterface(&source, 0x5e))
        to.AddAndCopyTC(*source, true);
    if (sourceThing.GetInterface(&source, 0x5c))
        to.AddAndCopyTC(*source, true);
    if (sourceThing.GetInterface(&source, 0x68))
        to.AddAndCopyTC(*source, true);
    if (sourceThing.GetInterface(&source, 0x6f)) {
        CTCBase* target;
        to.GetInterface(&target, 0x6f);
        target->CopyStateFrom(source);
    }
    if (sourceThing.GetInterface(&source, 0x14)) {
        CTCBase* target;
        to.GetInterface(&target, 0x14);
        target->CopyStateFrom(source);
    }
}
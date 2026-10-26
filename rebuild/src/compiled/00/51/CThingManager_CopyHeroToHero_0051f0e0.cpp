// CThingManager::CopyHeroToHero(CThing& from, CThing& to, vector<CThing*>& things) -- carries the hero state over from one hero thing to another: the components the source has (interface types 3, 4, 0x5E, 0x5C, 0x68) are copied into the target through CThing::CopyTCState (type 3 after resetting the target's), types 0x6F and 0x14 through the target component's virtual copy
// Leaf: ?CopyHeroToHero@CThingManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051F0E0, 595 bytes (0x253 + int3 padding to 608), thiscall `ret 0Ch` (`this` and
//   `things` unused). Callers: CThingManager 0x00521E42 (the morph loader at 0x00521CF0) and a function-pointer slot at
//   0x0047FB0C.
// Identity: Ego_r.pdb ?CopyHeroToHero@CThingManager@@AAEXAAVCThing@@0AAV?$vector@PAVCThing@@...@std@@@Z; the catalog's
//   "CTCHeroMorph_ApplyEffects_WithLookup" is descriptive.
// Callees: CInterfaceMap::LowerBound 0x0040F020 (non-const: the type-3 lookups and the target lookups) and its const
//   overload 0x004365B0 (the source lookups through the const CThing reference), the type-3
//   component's reset 0x0071C260, CThing::CopyTCState(CTCBase*, bool) 0x004C9F60, the component's virtual copy (slot 12,
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
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void CopyStateFrom(CTCBase* source);     // slot 12 (+0x30)
};
struct CTCHeroType3 : CTCBase {                      // interface type 3
    void ResetState();                               // 0x0071C260
};

struct CPresenceBits {                               // std::bitset<274> (nine words)
    unsigned long _M_w[9];
    bool operator[](unsigned int pos) const { return (_M_w[pos / 32] & (1UL << (pos % 32))) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
    SEntry* LowerBound(const long* type) const;      // 0x004365B0 (the const overload: a byte-identical copy)
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    void CopyTCState(CTCBase* source, bool flag);    // 0x004C9F60
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present[wanted]) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(wanted < found->type))
                it = found;
            else
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present[wanted]) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(wanted < found->type))
                it = found;
            else
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}

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
        targetType3->ResetState();
        to.CopyTCState(sourceType3, true);
    }
    const CThing& sourceThing = from;
    CTCBase* source;
    if (sourceThing.GetInterface(&source, 4))
        to.CopyTCState(source, true);
    if (sourceThing.GetInterface(&source, 0x5e))
        to.CopyTCState(source, true);
    if (sourceThing.GetInterface(&source, 0x5c))
        to.CopyTCState(source, true);
    if (sourceThing.GetInterface(&source, 0x68))
        to.CopyTCState(source, true);
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
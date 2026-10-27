// CTCInventoryExperience::GetTCInventoryAbilities() -- the owner's CTCInventoryAbilities component (interface 0x6F), not checked for presence
// Leaf: ?GetTCInventoryAbilities@CTCInventoryExperience@@AAEPAVCTCInventoryAbilities@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056FFC5, 68 bytes, thiscall, plain `ret`, ebp frame.
// Identity (proven): Ego_r.pdb ?GetTCInventoryAbilities@CTCInventoryExperience@@AAEPAVCTCInventoryAbilities@@XZ (private); interface type 0x6F is CTCInventoryAbilities
//   (as the landed CTCInventoryExperience / inventory rows name it); callers are CTCInventoryExperience's ability / stat / level-up
//   code (0x00570332 .. 0x005724CD).
// Callees: std::bitset::test 0x00410DE0 (the presence bit), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020.
// Shape (family M140, representative 0x0056FF3D): `CTCInventoryAbilities* tc; ParentThing->GetInterface(&tc, 0x6F); return tc;` -- the
//   landed F88 GetInterface (0x0056FBED model, `optimize("y", off)` + `optimize("s", on)`): the out local, the lookup key and
//   the miss path's returned value share one frame word.
// Header debt: CThing / CTCInventoryExperience modelled locally (presence bits +0x20, interface map +0x44, ParentThing +4).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CPresenceBits {
    bool test(long bit);                                    // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; void* component; };
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    __declspec(noinline) SEntry* LowerBound(const long* type)   // 0x0040F020 (same-TU read-only body, as landed)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                                // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
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
};
class CTCInventoryAbilities;
class CTCInventoryExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
private:
    CTCInventoryAbilities* GetTCInventoryAbilities();
};
CTCInventoryAbilities* CTCInventoryExperience::GetTCInventoryAbilities()
{
    CTCInventoryAbilities* tc;
    ParentThing->GetInterface(&tc, 0x6F);
    return tc;
}
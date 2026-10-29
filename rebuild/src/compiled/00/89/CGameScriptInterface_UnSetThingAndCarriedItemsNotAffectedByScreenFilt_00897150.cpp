// CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing& thing) const -- script call: clears the screen-filter bypass flag of the thing's carrying component (interface 0x46)
// Leaf: ?UnSetThingAndCarriedItemsNotAffectedByScreenFilter@CGameScriptInterface@@UBEXAAVCScriptThing@@@Z
// Retail: Fable.exe @ 0x00897150, 64 bytes, thiscall `ret 4`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 678 holds this body; slot 678 is
//   ?UnSetThingAndCarriedItemsNotAffectedByScreenFilter@CGameScriptInterface@@UBEXAAVCScriptThing@@@Z (a non-const
//   reference). Interface 0x46 = CTCCarrying (vtable 0x012433F4 slot 21 = landed 0x004E74C5, returns 0x46).
// Body: t = thing.GetThing() (slot 11) with no null or dead test; if t holds interface 0x46 (UsingComponents word 2 bit 6):
//   GetTC (non-const LowerBound 0x0040F020) and clear the byte at +0x1c. The inline that clears it is not named in the
//   PDB sets (FableWin.pdb has CTCCarrying::UnSetAllCarriedThingsAsBypassFilter() but its body is not proven to be this
//   store): descriptive member and inline.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const ETCInterfaceType& type);          // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;    // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (const T*)it->second;
        return true;
    }
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
};
class CTCCarrying {                                     // interface 0x46
public:
    enum { INTERFACE_TYPE = 0x46 };
    unsigned char _pad00[0x1c];
    bool BypassScreenFilter;                             // +0x1c (descriptive)
    void ClearBypassScreenFilter() { BypassScreenFilter = false; }
};
class CGameScriptInterface {
public:
    virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing& thing) const;
};
void CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing& thing) const
{
    CThing* t = thing.GetThing();
    CTCCarrying* tc;
    if (t->GetTC(&tc))
        tc->ClearBypassScreenFilter();
}
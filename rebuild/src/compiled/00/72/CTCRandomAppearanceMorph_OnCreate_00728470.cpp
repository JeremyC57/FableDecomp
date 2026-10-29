// CTCRandomAppearanceMorph::OnCreate() -- fetch the parent thing's skeletal-morph component (TC kind 120) into PTCSkeletalMorph when the thing has one; then rebuild the morph set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728470, length 71 bytes (slot 4 of the CTCRandomAppearanceMorph vtable 0x0126653C,
// rtti-vtables.tsv).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the thing's component map at +0x44),
// CTCRandomAppearanceMorph::UpdateMorphSet() @ 0x007280C0.
// Shape: the thing's inline OUT-PARAMETER GetInterface(type, &out) (the landed 0x007504E0 shape) into a LOCAL out pointer,
// assigned to the member only when the lookup succeeds (`if (GetInterface(120, &morph)) PTCSkeletalMorph = morph;`): the
// presence bitset word tested raw (kind 120 = word 3, bit 24: `test dword ptr [eax+2Ch],1000000h`), the key in the `push ecx`
// frame slot, the map pointer a named local (esi, then overwritten by `last`), and the component loaded into eax for the
// store (the member's own address as the out pointer loads it into ecx).
// Layout = Ego_r.pdb CTCRandomAppearanceMorph with the 12-byte retail vector: CTCBase ParentThing +0x04, PMeshMorphSet +0x10,
// Seed +0x18, PrimHandles +0x1c, PTCSkeletalMorph +0x28.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline bool GetInterface(long type, CTCBase** out)   // inline: the landed GetInterface family shape
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = it->second;
            return true;
        }
        return false;
    }
};

class CTCRandomAppearanceMorph { public:
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x20];                             // +0x08 .. +0x27: flags, PMeshMorphSet, Seed, PrimHandles
    CTCBase*      PTCSkeletalMorph;                         // +0x28 CTCSkeletalMorph*
    void UpdateMorphSet();                                  // 0x007280C0
    void OnCreate();
};

void CTCRandomAppearanceMorph::OnCreate()
{
    CTCBase* morph;
    if (ParentThing->GetInterface(120, &morph))
        PTCSkeletalMorph = morph;
    UpdateMorphSet();
}
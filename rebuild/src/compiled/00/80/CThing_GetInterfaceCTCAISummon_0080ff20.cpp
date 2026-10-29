// CThing::GetInterface<CTCAISummon>(CTCAISummon** out) -- if the thing has the AI summon component (interface 0xfa), hand it back and return true
// Leaf: ??$GetInterface@VCTCAISummon@@@CThing@@QAE_NPAPAVCTCAISummon@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080FF20, 73 bytes, `ret 4`; no callers (an unreferenced COMDAT kept by the link).
// Identity (descriptive): the body is the out-parameter interface lookup that the CTCNymph TU (0x0080F5E0 StartFlyingAround,
//   0x0080FD30 GetShouldSummon) inlines for interface 0xFA = CTCAISummon (bit 26 of the bitset word at +0x3c); it sits between
//   the nymph TU's GetSubDef<CAICreatureWillPowerIndicatorDef> 0x0080FE60 and the explosive-trail TU. Ego_r.pdb has no
//   out-of-line instantiation of it, so the template's real name is not known.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (non-const).
class CTCBase;
class CTCAISummon;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

template <class T> struct STCInterfaceType;
template <> struct STCInterfaceType<CTCAISummon> { enum { Value = 0xfa }; };

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> bool GetInterface(T** ppOut);
};

template <class T> bool CThing::GetInterface(T** ppOut)
{
    const long interfaceType = STCInterfaceType<T>::Value;
    if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        *ppOut = (T*)it->second;
        return true;
    }
    return false;
}

template bool CThing::GetInterface<CTCAISummon>(CTCAISummon** ppOut);
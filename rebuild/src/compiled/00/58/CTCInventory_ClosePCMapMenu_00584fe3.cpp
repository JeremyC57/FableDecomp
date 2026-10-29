#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCInventory::ClosePCMapMenu() -- when the owner has a map-menu component (interface type 0x1B), closes it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584FE3, length 74 bytes; size-optimised TU with a frame pointer
// (`optimize("y",off)` + `("s",on)`, `leave`).
// Callees: std::bitset::test 0x00410DE0 on the thing's interface-presence bits (+0x20), CVectorMap::LowerBound
//          0x0040F020 on its sorted {type, component} map (+0x44), then the component's close routine 0x005FAD30
//          (non-virtual: clears its words +0x214 and +0x188; manifest label "CWADFile::Close" is propagated;
//          this is its only caller).
// Ego_r PDB: ?ClosePCMapMenu@CTCInventory@@QAEXXZ.
// The lookup is the thing's inlined GetInterface template (the landed out-of-line copies, e.g. 0x007BEA08, have
// the same bitset-then-lower-bound shape with a by-address key in the frame slot).
// Retail layout: CTCBase ParentThing at +0x04 (the PDB header layout of CTCInventory is wider), so `this` is
// modelled locally; the map-menu class and its close routine are named by use.
// Shape: an absent interface skips to the epilogue; a present one is null-tested (`test eax,eax`) before the
// call. A size TU only inlines the lookup when it is `__forceinline`.
class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(long wanted, T** out)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CTCMapMenu {
    void Close();                                    // 0x005FAD30
};

class CTCInventory { public:
    virtual void v00();
    CThing* ParentThing;                             // +0x04
    void ClosePCMapMenu();
};

void CTCInventory::ClosePCMapMenu()
{
    CTCMapMenu* mapMenu;
    if (ParentThing->GetInterface(0x1b, &mapMenu) && mapMenu)
        mapMenu->Close();
}
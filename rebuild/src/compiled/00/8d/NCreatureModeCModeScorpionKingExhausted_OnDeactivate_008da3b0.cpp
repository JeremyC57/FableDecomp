// NCreatureMode::CModeScorpionKingExhausted::OnDeactivate() -- switch the type-9 component on, wake the scorpion's physics, set creature flag 1 and mark the mode inactive
// Leaf: ?OnDeactivate@CModeScorpionKingExhausted
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA3B0, 94 bytes (+ int3 padding), `ret`; vtable 0x012858A0 slot 6.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined HasTC + GetTC, type 9 = bit 9 of the +0x20 dword), the physics
//   body's (+0x60) virtual slot 44 (+0xb0, wake; as the landed CModeOutOfActionBase::OnDeactivate 0x008D8F30).
// Name: FableWin.pdb `?OnDeactivate@CModeScorpionKingExhausted@NCreatureMode@@UAEXXZ` (slot 6).
// Layout: CModeBase Active +0x18; thing physics +0x60, CreatureFlags +0xbc; the type-9 component's flag +0x10.
// Shape: `T* p = 0; if (HasTC(k)) p = GetTC<T>(k);` gives the `xor ecx,ecx` preload with the dword presence test
//   (`mov edx,[eax+20h] ; test dh,2`), and the flag store goes through p unconditionally; ParentThing is re-read for the
//   physics call and the flag; the inline CModeBase::OnDeactivate (`Active = false`) comes last.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                     // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);  // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CPhysicsObject {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void Wake();   // slot 44 (+0xb0)
};

class CThing { public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];      // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;          // +0x44
    unsigned char   _pad4c[0x14];
    CPhysicsObject* PPhysics;              // +0x60
    unsigned char   _pad64[0xbc - 0x64];
    unsigned char   CreatureFlags;         // +0xbc (creature-derived member)
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->first) ? map->last : it;
        return (T*)sel->second;
    }
};

struct CTCType9 {                          // the type-9 component (as the landed 0x008E7B90)
    unsigned char _pad00[0x10];
    bool          m_active;                // +0x10
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x10];             // WeaponBasedFilter, FramesBeforeRemovingSelf
    bool          Active;                  // +0x18
    void OnDeactivate() { Active = false; }
};

namespace NCreatureMode {
struct CModeScorpionKingExhausted : CModeBase {   // vtable 0x012858A0
    void OnDeactivate();
};
}

void NCreatureMode::CModeScorpionKingExhausted::OnDeactivate()
{
    CTCType9* tc = 0;
    if (ParentThing->HasTC(9))
        tc = ParentThing->GetTC<CTCType9>(9);
    tc->m_active = true;
    ParentThing->PPhysics->Wake();
    ParentThing->CreatureFlags |= 1;
    CModeBase::OnDeactivate();
}
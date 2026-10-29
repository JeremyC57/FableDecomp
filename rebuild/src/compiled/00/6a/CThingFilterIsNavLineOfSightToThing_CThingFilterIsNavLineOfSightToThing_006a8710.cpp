// CThingFilter_IsNavLineOfSightToThing::CThingFilter_IsNavLineOfSightToThing(const CThing& thing) -- keep the thing's position and, when it has one, its creature-navigation component
// Leaf: ??0CThingFilter_IsNavLineOfSightToThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8710, length 74 bytes, `ret 4`.
// Callees: CThing::GetPos() const @ 0x004C73D0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0.
// Identity: Ego_r.pdb `??0CThingFilter_IsNavLineOfSightToThing@@QAE@ABVCThing@@@Z` and its layout {C3DVector* Pos;
//   CTCCreatureNavigation* PTCNav;} (8 bytes) -- exactly the two stores; its operator() is the landed 0x006A7640.
//   The catalog's "CVertexBufferWin32_GetSizeAndAux" (from the propagated 0x004C73D0 label "DoSizeof") is wrong.
//   Callers: CAIStateGroup_MinionGossip::IsRunnable / Update.
// Layout: interface type 7 (creature navigation) = bit 7 of the presence byte +0x20 (`mov al ; test al,al ; jns`),
//   the {kind, component} vector map at +0x44 (local CThing model of the CTCCarrying rows).
// Shape: member initialiser Pos(&thing.GetPos()); the body's PeekTC writes PTCNav only when the component
//   exists (the out-parameter is the member itself), the conversion-temporary key sits in the dead thing slot.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
enum ETCInterfaceType { TC_CREATURE_NAVIGATION = 0x07 };

namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
    __forceinline const SEntry* Find(long type) const
    {
        const SEntry* it = LowerBound((ETCInterfaceType)type);
        return (it == last || type < it->first) ? last : it;
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap Components;                       // +0x44
    const C3DVector& GetPos() const;           // 0x004C73D0
    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool PeekTC(T** pp) const
    {
        if (HasTC(T::StatGetInterfaceType())) {
            *pp = (T*)Components.Find(T::StatGetInterfaceType())->second;
            return true;
        }
        return false;
    }
};

class CTCCreatureNavigation;

class CThingFilter_IsNavLineOfSightToThing {
public:
    C3DVector*             Pos;                     // +0x00
    CTCCreatureNavigation* PTCNav;                  // +0x04
    CThingFilter_IsNavLineOfSightToThing(const CThing& thing);
};

class CTCCreatureNavigation {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_CREATURE_NAVIGATION; }
};

CThingFilter_IsNavLineOfSightToThing::CThingFilter_IsNavLineOfSightToThing(const CThing& thing)
    : Pos((C3DVector*)&thing.GetPos())
{
    thing.PeekTC(&PTCNav);
}
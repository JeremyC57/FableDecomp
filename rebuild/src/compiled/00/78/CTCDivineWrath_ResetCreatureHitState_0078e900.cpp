// CTCDivineWrath::ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase> creature) -- for a creature still alive: drop its creature mode 0x5f and re-arm its interface-0x8b component (flag set, +0x1c reloaded from +0x10)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078E900, 192 bytes (`ret 8`: the 8-byte pointer by value, destroyed here; `this`
// is not used).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (four times), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (two inlined lookups), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0, ~CBaseIntelligentPointer
// @ 0x00A01C10.
// Layout: CThing bitset +0x20 (mode manager 0x31 = bit 17 of the +0x24 word; 0x8b = bit 11 of the +0x30 word, `test ch,8`),
// map +0x44, dead bit +0x91; the 0x8b component's reset fields +0x10 / +0x18 / +0x1c (named descriptively).
// Shape: an inline IS_THING_ALIVE over the by-value pointer (GetPItem twice), then each component through the out-parameter
// GetInterface on a fresh GetPItem (component left in eax); the frame word is the lookup key.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                              // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
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
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer { };   // CIntelligentPointer<CThingCreatureBase>
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long modeType);            // 0x0073ABA0
};
struct CTCHitReaction {                                 // the interface-0x8b component
    unsigned char _pad00[0x10];
    long ResetValue;                                    // +0x10
    unsigned char _pad14[0x18 - 0x14];
    bool Armed;                                         // +0x18
    long CurrentValue;                                  // +0x1c
};
class CTCDivineWrath { public:
    virtual void v00();
    void ResetCreatureHitState(CIntelligentPointerCreature creature);
};

void CTCDivineWrath::ResetCreatureHitState(CIntelligentPointerCreature creature)
{
    if (IS_THING_ALIVE(creature)) {
        CTCCreatureModeManager* modes;
        if (creature.GetPItem()->GetInterface(&modes, 0x31))
            modes->RemoveMode(0x5f);
        CTCHitReaction* hit;
        if (creature.GetPItem()->GetInterface(&hit, 0x8b)) {
            hit->Armed = true;
            hit->CurrentValue = hit->ResetValue;
        }
    }
}
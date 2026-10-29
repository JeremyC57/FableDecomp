// NCreatureAI::IsThingRunningAwayFromCombat(const CThing&) -- true when the thing has a stealth component whose running-away-from-combat timer is set (>= 0)
// Leaf: ?IsThingRunningAwayFromCombat@
// Retail 0x00864CB0, 0x43 bytes (+ int3 padding), __fastcall (thing in ecx). PDB (Ego_r):
// ?IsThingRunningAwayFromCombat@NCreatureAI@@YI_NABVCThing@@@Z. Callers 0x00663B7D, 0x009057AE.
// Callee: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the thing's interface map at +0x44).
// Interface type 0x36 = CTCStealth (GetInterfaceType, vtable slot 21); RunningAwayFromCombatTimer is CTCStealth +0x30
// (Ego_r layout). Present-interface bitset at CThing +0x20: type 0x36 -> word 1 (+0x24), bit 22 (`test [ecx+24h],400000h`).
// Shape: the __forceinline GetInterface(type, &out) template -- bitset test, key in the `push ecx` slot, map pointer in
// esi, `it == last || type < it->type` -> last, `*out = it->component` -- then `timer >= 0`. The out-pointer form keeps the
// component in eax and loads the timer into ecx (`mov eax,[eax+4] ; mov ecx,[eax+30h]`); a PeekTC returning the
// component (or the entry) swaps those two registers.
// CThing is modelled locally: rebuild/include/engine has no CThing.h and CThingCreatureBase.h pads the CThing bytes.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    const SEntry* LowerBound(const ETCInterfaceType& type) const;      // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCStealth
{
public:
    unsigned char _pad0[0x30];
    long          RunningAwayFromCombatTimer;        // +0x30
};

class CThing
{
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 bitset of present interface types
    CInterfaceMap m_interfaces;                      // +0x44

    template <class T> __forceinline bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

namespace NCreatureAI
{
    bool __fastcall IsThingRunningAwayFromCombat(const CThing& thing);
}

bool __fastcall NCreatureAI::IsThingRunningAwayFromCombat(const CThing& thing)
{
    const CTCStealth* stealth;
    if (thing.GetInterface(0x36, &stealth) && stealth->RunningAwayFromCombatTimer >= 0)
        return true;
    return false;
}
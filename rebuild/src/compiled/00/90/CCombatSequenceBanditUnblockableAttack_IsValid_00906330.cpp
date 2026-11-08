// CCombatSequence_BanditUnblockableAttack::IsValid() -- valid unless the target has a combat component that is not vulnerable to unblockable attacks
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00906330, length 70 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (the inlined interface lookup for type 0x30 = bit 16 of
// bitset dword +0x24), CTCCombat::IsVulnerableToUnblockableAttack @ 0x007331C0.
// The CThing component-table model is the landed GetInterface shape. The target is the base's +0x20
// (retail layout, see 0x008B0AB0). The found path returns the callee's bool through `? true : false`
// (the false branch re-uses al, the true branch shares the `mov al,1` tail); an `||` form widens to eax.
#include "fable_compiler.h"
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44 sorted {type, component} vector map
    unsigned char _pad9[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    FABLE_FORCEINLINE bool HasInterface(long interfaceType) const   // the bitset test alone
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)   // forced: two lookups in one body exceed the inliner budget
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
struct CCombatSequenceBase_Retail {        // vtable slots 0..18, see 0x008B0AB0; only the data is needed here
    virtual ~CCombatSequenceBase_Retail();
    bool          Running;                     // +0x04
    long          NoAvailableActions;          // +0x08
    long          CurrentActionIndex;          // +0x0c
    unsigned char CombatActions[12];           // +0x10 std::vector<CCombatAction>
    CThing*       Creature;                    // +0x1c
    CThing*       Target;                      // +0x20
    bool          CreateCalled;                // +0x24
};

struct CTCCombat_Calls {                       // engine/CTCCombat.h exists; only the one call is needed, so no layout is modelled
    bool IsVulnerableToUnblockableAttack();    // 0x007331C0
};

struct CCombatSequence_BanditUnblockableAttack_Retail : CCombatSequenceBase_Retail {
    virtual bool IsValid();
};

bool CCombatSequence_BanditUnblockableAttack_Retail::IsValid()
{
    CTCCombat_Calls* pCombat;
    if (Target->GetInterface(&pCombat, 0x30) && !pCombat->IsVulnerableToUnblockableAttack())
        return false;
    return true;
}
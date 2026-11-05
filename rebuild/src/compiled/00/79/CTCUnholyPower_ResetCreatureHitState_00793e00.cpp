// CTCUnholyPower::ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase> creature) -- for a live creature drop its mode 0x5F and re-arm the hit state of its interface-0x8B component
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00793E00, length 190 bytes (+ int3 padding). __fastcall, `ret 8`
//         (the 8-byte intelligent pointer by value; `this` is never read and the callee destroys the parameter).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (inlined IS_THING_ALIVE, then one fetch per block),
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0x31 = CTCCreatureModeManager,
//          word 1 bit 17; interface 0x8B, word 4 bit 11), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0,
//          ~CBaseIntelligentPointer @ 0x00A01C10.
// Label: the catalog says CTCDivineWrath; all six callers sit in the CTCUnholyPower TU (0x00794DF8 .. 0x00796F8F) and the
//        CTCDivineWrath copy of this body is 0x0078E900.
// Shape: the out-parameter GetInterface template (component left in eax, `mov ecx,eax` after the push); both lookup keys
//        share the `push ecx` frame word.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCCreatureModeManager { public:                     // interface 0x31
    void RemoveMode(unsigned long mode);            // 0x0073ABA0
};

struct CTCHitState_8b {                             // interface 0x8B component (class not identified)
    unsigned char _pad00[0x10];
    long CurrentHitFrame;                           // +0x10
    unsigned char _pad14[4];
    bool Reset;                                     // +0x18
    unsigned char _pad19[3];
    long ResetHitFrame;                             // +0x1C
};

class CTCUnholyPower { public:
    virtual void v00();
    void ResetCreatureHitState(CIntelligentPointerCreature creature);
};

void CTCUnholyPower::ResetCreatureHitState(CIntelligentPointerCreature creature)
{
    if (IS_THING_ALIVE(creature)) {
        CTCCreatureModeManager* modes;
        if (((CThing*)creature.GetPItem())->GetInterface(&modes, 0x31))
            modes->RemoveMode(0x5f);
        CTCHitState_8b* hitState;
        if (((CThing*)creature.GetPItem())->GetInterface(&hitState, 0x8b)) {
            hitState->Reset = true;
            hitState->ResetHitFrame = hitState->CurrentHitFrame;
        }
    }
}
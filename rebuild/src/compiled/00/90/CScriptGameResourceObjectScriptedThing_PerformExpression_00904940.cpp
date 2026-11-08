// CScriptGameResourceObjectScriptedThing::PerformExpression(const CScriptThing& target, const CCharString& expression) -- queue a perform-expression action aimed at the target for the live scripted thing
// Leaf: PerformExpression
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904940, length 173 bytes (`ret 8`), vtable 0x0128D86C slot 17.
// PDB: ?PerformExpression@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@ABVCCharString@@@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f = CTCScriptedControl),
// CScriptThing::GetPThing (slot 11), operator new @ 0x00BFEA1A (0x28 bytes),
// NTCScriptedControl::CActionPerformExpression::CActionPerformExpression(CThing*, const CCharString&, long, long)
// @ 0x00905360 (vtable 0x0128D8EC), CTCScriptedControl::AddAction @ 0x007137D0.
// Shape: the family guard without a target life test; the unchecked GetInterface keys in the `push ecx`
// frame word and its miss arm reads the (still live) `target` slot, which VC7.1 picks as the home of the
// never-written component; the target thing is fetched after the lookup and both extra data words are -1.
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing reached through a local view).
#include "fable_compiler.h"
#include <stddef.h>
#include "engine/CScriptGameResourceObjectScriptedThing.h"

void* __cdecl operator new(size_t size);           // 0x00BFEA1A

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad54[0x18];
    unsigned long  ThingTypeFlags;                 // +0x6c
    unsigned char  _pad70[0x21];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
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
struct CIntelligentPointer_Retail {                // CIntelligentPointer<CThing>
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
}

class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

class CCharString;

namespace NTCScriptedControl {
struct CActionPerformExpression : CActionBase {                // vtable 0x0128D8EC, 0x28 bytes
    unsigned char _body[0x24];
    CActionPerformExpression(CThing_Retail* target, const CCharString& expression, long extraData1, long extraData2);   // 0x00905360
};
}

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void PerformExpression(const CScriptThing& target, const CCharString& expression);
};

void CScriptGameResourceObjectScriptedThing_Methods::PerformExpression(const CScriptThing& target, const CCharString& expression)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            CThing_Retail* targetThing = ((CThing_Retail*)target.GetPThing());
            control->AddAction(new NTCScriptedControl::CActionPerformExpression(targetThing, expression, -1, -1));
        }
    }
}
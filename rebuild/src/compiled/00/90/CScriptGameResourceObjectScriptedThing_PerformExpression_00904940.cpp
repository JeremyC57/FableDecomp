// CScriptGameResourceObjectScriptedThing::PerformExpression(const CScriptThing& target, const CCharString& expression) -- queue a perform-expression action aimed at the target for the live scripted thing
// Leaf: PerformExpression
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
#include <stddef.h>
#include "engine/CScriptGameResourceObjectScriptedThing.h"

void* __cdecl operator new(size_t size);           // 0x00BFEA1A

struct CTCBase;

struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};

struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x20];
    unsigned long  ThingTypeFlags;                 // +0x6c
    unsigned char  _pad70[0x21];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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

class CScriptThing {                               // vtable 0x01238C8C
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CThing_Retail* GetPThing() const;      // slot 11 (+0x2c) 0x004AAA60
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                  // slot 75 (+0x12c) 0x004AB130
};

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
}

struct CTCScriptedControl {                        // interface 0x1f component
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
            CThing_Retail* targetThing = target.GetPThing();
            control->AddAction(new NTCScriptedControl::CActionPerformExpression(targetThing, expression, -1, -1));
        }
    }
}
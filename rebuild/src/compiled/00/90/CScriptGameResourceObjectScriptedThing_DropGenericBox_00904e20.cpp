// CScriptGameResourceObjectScriptedThing::DropGenericBox() -- queue a drop-generic-box action for the live scripted thing
// Leaf: DropGenericBox
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904E20, length 180 bytes, vtable 0x0128D86C slot 24.
// PDB: ?DropGenericBox@CScriptGameResourceObjectScriptedThing@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f = CTCScriptedControl),
// operator new @ 0x00BFEA1A (0x14 bytes), CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (the action
// root; vtable 0x0129A7C4), CTCScriptedControl::AddAction @ 0x007137D0. Vtable 0x0128D7BC = CActionDropGenericBox.
// Shape: the family guard and the unchecked GetInterface (key and miss in the `push ecx` word), then the
// action's constructor inlined into the new-expression: the abstract CActionBase (novtable) zeroes PTCScript,
// Activated and Terminated before the derived vtable store, ActionID = -1 and ActionPluggedIn = false after it
// (Ego_r.pdb CActionBase 0xc / CActionDropGenericBox 0x14). The zero register is hoisted above the null test.
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


struct CTCScriptedControl;

class CBaseClassNonCopyable {                      // vtable 0x0129A7C4
public:
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};

namespace NTCScriptedControl {
struct __declspec(novtable) CActionBase : CBaseClassNonCopyable {   // vtable 0x01265504 (abstract: store dead)
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
    CActionBase() : PTCScript(0), Activated(false), Terminated(false) { }
};
struct CActionDropGenericBox : CActionBase {       // vtable 0x0128D7BC, 0x14 bytes
    long ActionID;                                 // +0x0c
    bool ActionPluggedIn;                          // +0x10
    virtual void v00();
    CActionDropGenericBox() : ActionID(-1), ActionPluggedIn(false) { }
};
}

struct CTCScriptedControl {                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void DropGenericBox();
};

void CScriptGameResourceObjectScriptedThing_Methods::DropGenericBox()
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            control->AddAction(new NTCScriptedControl::CActionDropGenericBox());
        }
    }
}
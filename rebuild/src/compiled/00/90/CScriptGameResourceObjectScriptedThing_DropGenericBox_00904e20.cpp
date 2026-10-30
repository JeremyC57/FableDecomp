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

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing_Retail { public:
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"


class CTCScriptedControl;

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

class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void DropGenericBox();
};

void CScriptGameResourceObjectScriptedThing_Methods::DropGenericBox()
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            control->AddAction(new NTCScriptedControl::CActionDropGenericBox());
        }
    }
}
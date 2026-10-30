// CScriptGameResourceObjectScriptedThing::~CScriptGameResourceObjectScriptedThing() -- when the scripted thing is still alive and its scripted control is locked, queue a stop-scripting action, unlock the control and clean up its actions; then destroy PThing, the PImp counted pointer and the base chain
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903AC0, length 256 bytes (called by the deleting destructor 0x009054D0).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (the inlined unchecked GetInterface, interface 0x1f = CTCScriptedControl), operator new @ 0x00BFEA1A (0xc bytes),
// CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (the inline NTCScriptedControl::CActionStopScripting ctor, vtable 0x0128D788), CTCScriptedControl::
// AddAction @ 0x007137D0, ClearLocked @ 0x00712C80, CleanupAllActions @ 0x00713630, CBaseIntelligentPointer::
// ~CBaseIntelligentPointer @ 0x00A01C10 (PThing), operator delete (import thunk 0x00BFE9BC), CBaseObject::~CBaseObject @ 0x0099A430.
// Vtables: 0x0128D86C (CScriptGameResourceObjectScriptedThing) at entry, 0x0126008C (CScriptGameResourceObjectBase) before
// the root destructor.
// Layout: engine/CScriptGameResourceObjectScriptedThing.h offsets (PImp +8/+0xc, ScriptInterface +0x10, PThing +0x14) with the
// class chain modelled locally as in the landed ctor 0x009039D0 (the generated header is a flat record without bases).
// Shape: the family guard (item present, dead bit +0x91 clear) and the unchecked GetInterface whose miss arm reads the
// never-written control from the `push ecx` frame word; the control's Locked byte is +0x18. The intermediate
// CScriptGameResourceObjectScriptedThingBase has a compiler-generated destructor (no vtable store), its PImp counted pointer
// member is released inline and both words cleared; CScriptGameResourceObjectBase's user `virtual ~() {}` keeps its store.
// Leaf: ??1CScriptGameResourceObjectScriptedThing
void* __cdecl operator new(unsigned int size);       // 0x00BFEA1A

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                               // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                     // +0x44
    unsigned char  _pad4c[0x45];
    unsigned char  m_dead : 1;                       // +0x91 bit 0
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

class CBaseClassNonCopyable {
protected:                                        // CBaseClassNonCopyable root
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                                         // 0x0099A2F0
};

namespace NTCScriptedControl {
struct CActionBase : CBaseClassNonCopyable {                         // vtable 0x01265504
    long m_actionState;                              // +0x04
    bool m_started;                                  // +0x08
    bool m_finished;                                 // +0x09
    __forceinline CActionBase() : m_actionState(0), m_started(false), m_finished(false) { }
};
struct CActionStopScripting : CActionBase {          // vtable 0x0128D788, 0xc bytes
    virtual ~CActionStopScripting();
    __forceinline CActionStopScripting() { }
};
}

class CTCScriptedControl { public:                          // interface 0x1f component
    unsigned char _pad00[0x18];
    bool          Locked;                            // +0x18
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
    void ClearLocked();                              // 0x00712C80
    void CleanupAllActions();                        // 0x00713630
};

struct SCountBlock {
    long m_references;                               // +0
    void (__fastcall* m_pfnDestroy)(void*);          // +4
    void* m_pObject;                                 // +8
};
template<class T> class CCountedPointer { public:
    T*           Data;                               // +0
    SCountBlock* Info;                               // +4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    CThing_Retail* GetPItem();                       // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
};

class CGameScriptInterface;
class CScriptGameResourceObjectScriptedThing : public CScriptGameResourceObjectScriptedThingBase { public:   // vtable 0x0128D86C
    CGameScriptInterface* ScriptInterface;           // +0x10
    CIntelligentPointer   PThing;                    // +0x14
    long                  ScriptID;                  // +0x1c
    bool                  NullResource;              // +0x20
    long                  ActionPriority;            // +0x24
    virtual ~CScriptGameResourceObjectScriptedThing();
};

CScriptGameResourceObjectScriptedThing::~CScriptGameResourceObjectScriptedThing()
{
    if (PThing.GetPItem()) {
        if (!PThing.GetPItem()->m_dead) {
            CTCScriptedControl* control;
            PThing.GetPItem()->GetInterface(&control, 0x1f);
            if (control->Locked) {
                control->AddAction(new NTCScriptedControl::CActionStopScripting());
                control->ClearLocked();
                control->CleanupAllActions();
            }
        }
    }
}
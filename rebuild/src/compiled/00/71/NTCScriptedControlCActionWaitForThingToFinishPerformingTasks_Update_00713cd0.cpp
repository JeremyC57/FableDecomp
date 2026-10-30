// NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::Update() -- finish once the target is dead or its own scripted control is neither locked nor holding queued actions
// Leaf: ?Update@CActionWaitForThingToFinishPerformingTasks@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00713CD0, 123 bytes (+5 int3 padding); vtable 0x0128D7F0 slot 1.
// FableWin.pdb: ?Update@CActionWaitForThingToFinishPerformingTasks@NTCScriptedControl@@UAEXXZ.
// Callees: CScriptThing virtual slots 77 IsValid (+0x134), 71 Refresh (+0x11c), 75 IsAlive (+0x12c), 11 GetThing (+0x2c)
//          (names as the landed CActionFollowCreature::Update @ 0x00713EE0 and CQHangingTreeGoodScript::RemoveDregs);
//          the const CVectorMap::LowerBound @ 0x004365B0 (inlined const GetInterface, interface 0x1F = CTCScriptedControl:
//          the sign bit of the bitset dword +0x20, so `jns`).
// Layout: CActionBase {vptr, PTCScript +4, Activated +8, Terminated +9}; PTargetThing (CScriptThing, 12 bytes) +0x0C.
//          Retail CTCScriptedControl PActionList (STLport list sentinel pointer) +0x14, Locked +0x18 (as the landed
//          CTCScriptedControl::InterruptControl @ 0x00713850). engine/CScriptThing.h carries no virtuals, so the
//          script-thing slots are modelled locally like the other landed NTCScriptedControl rows.
// Shape: dead target, or no scripted control, branch to the single Terminated store; Locked or a non-empty queue returns.
class CTCBase;
class CThing;
class CCPPointerInfo;
struct SActionNode { SActionNode* _M_next; SActionNode* _M_prev; void* Data; CCPPointerInfo* Info; };
struct CActionList {                               // STLport list<CCountedPointer<CActionBase>>
    SActionNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
class CTCScriptedControl { public:                        // interface 0x1F
    void*        __vftable;
    CThing*      ParentThing;                      // +0x04 (CTCBase)
    bool         ToBeRemoved;                      // +0x08
    bool         IsInGlobalUpdateTCs;              // +0x09
    void*        PScriptHandleImp[2];              // +0x0c CIntelligentPointer
    CActionList  PActionList;                      // +0x14
    bool         Locked;                           // +0x18
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    void*         __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];              // +0x20
    CInterfaceMap Components;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionWaitForThingToFinishPerformingTasks : public CActionBase {
public:
    virtual void Update();
    CScriptThing PTargetThing;                     // +0x0C
};

void CActionWaitForThingToFinishPerformingTasks::Update()
{
    if (!PTargetThing.IsNull())
        PTargetThing.UpdateThingAttachment();
    if (PTargetThing.IsAlive()) {
        CTCScriptedControl* control;
        if (PTargetThing.GetPThing()->GetInterface(&control, 0x1f)) {
            if (control->Locked || !control->PActionList.empty())
                return;
        }
    }
    Terminated = true;
}
}
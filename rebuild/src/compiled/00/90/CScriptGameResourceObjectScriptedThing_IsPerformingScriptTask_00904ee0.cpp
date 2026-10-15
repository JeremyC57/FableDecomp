// CScriptGameResourceObjectScriptedThing::IsPerformingScriptTask() -- true when the live scripted thing's scripted-control component (interface 0x1f) still has queued script actions
// Leaf: IsPerformingScriptTask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904EE0, length 128 bytes (125 + int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 three times (PThing at +0x14),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (this TU's copy of the inlined
// GetInterface lookup), the component's action-list empty test @ 0x007122C0 (labelled
// "CScriptTask_IsRunning"; it returns `head->next == head` on the list at +0x14, i.e. IsEmpty).
// Shape: same nested tests and inlined GetInterface as ClearCommands (0x00904310); the result is
// `!IsEmpty()`, which gives the widened `neg al ; sbb eax,eax ; inc eax` tail on both
// arms, and the dead/null exits share `xor al,al`.
#include "engine/CScriptGameResourceObjectScriptedThing.h"

struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x004365B0
};

struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x45];
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
struct CIntelligentPointer_Retail {
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50
};

struct CTCScriptedControl {
    bool IsActionListEmpty();                      // 0x007122C0
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    bool IsPerformingScriptTask();
};

bool CScriptGameResourceObjectScriptedThing_Methods::IsPerformingScriptTask()
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            return !control->IsActionListEmpty();
        }
    }
    return false;
}
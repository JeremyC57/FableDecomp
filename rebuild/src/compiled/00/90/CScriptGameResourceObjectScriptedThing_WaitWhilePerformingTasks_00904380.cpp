// CScriptGameResourceObjectScriptedThing::WaitWhilePerformingTasks() -- yield script frames while the live scripted thing still has queued scripted actions
// Grade: RELOCATION_MATCH -- the loop stays top-tested with a back `jmp` (no rotated GetPItem test at the bottom) when the liveness half of the condition is an inline IS_THING_ALIVE(p) helper: `while (IS_THING_ALIVE(thing) && !control->IsActionListEmpty())`; the open-coded `GetPItem() && !dead && ...` chain rotates its first term
// Leaf: WaitWhilePerformingTasks
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904380, length 163 bytes, vtable 0x0128D86C slot 11.
// PDB: ?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThing@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f), the control's action-list empty
// test @ 0x007122C0, the script runner's YieldFrame (slot 2) through g_pScriptRunner @ 0x013D2838 (stop byte +5).
// Shape: the family guard and one unchecked GetInterface (key and miss read in the `push ecx` frame word),
// then a loop that re-tests the thing (present, not dead) and the action list each frame; without a runner
// it spins, a stop request ends the wait.
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing reached through a local view).
#include "fable_compiler.h"
#include <stddef.h>
#include "engine/CScriptGameResourceObjectScriptedThing.h"


class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing_Retail { public:
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

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

class CTCScriptedControl;

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
}


struct CScriptRunner {
    virtual void v00();
    virtual void v01();
    virtual void YieldFrame();                    // slot 2 (+8)
    unsigned char m_pad4;                         // +0x04
    bool          m_bStopRequested;               // +0x05
};
extern CScriptRunner* g_pScriptRunner;            // 0x013D2838

class CTCScriptedControl { public:                      // interface 0x1f component
    bool IsActionListEmpty();                     // 0x007122C0
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void WaitWhilePerformingTasks();
};

inline bool IS_THING_ALIVE(CIntelligentPointer<CThing_Retail>* p)
{
    if (p->GetPItem()) {
        if (!p->GetPItem()->m_dead)
            return true;
    }
    return false;
}
void CScriptGameResourceObjectScriptedThing_Methods::WaitWhilePerformingTasks()
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            while (IS_THING_ALIVE(thing) && !control->IsActionListEmpty()) {
                if (g_pScriptRunner) {
                    if (g_pScriptRunner->m_bStopRequested)
                        break;
                    g_pScriptRunner->YieldFrame();
                }
            }
        }
    }
}
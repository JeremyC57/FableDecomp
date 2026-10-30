// CScriptGameResourceObjectScriptedThing::WaitWhilePerformingTasks() -- yield script frames while the live scripted thing still has queued scripted actions
// Grade: RELOCATION_MATCH -- the loop stays top-tested with a back `jmp` (no rotated GetPItem test at the bottom) when the liveness half of the condition is an inline IS_THING_ALIVE(p) helper: `while (IS_THING_ALIVE(thing) && !control->IsActionListEmpty())`; the open-coded `GetPItem() && !dead && ...` chain rotates its first term
// Leaf: WaitWhilePerformingTasks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904380, length 163 bytes, vtable 0x0128D86C slot 11.
// PDB: ?WaitWhilePerformingTasks@CScriptGameResourceObjectScriptedThing@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f), the control's action-list empty
// test @ 0x007122C0, the script runner's YieldFrame (slot 2) through g_pScriptRunner @ 0x013D2838 (stop byte +5).
// Shape: the family guard and one unchecked GetInterface (key and miss read in the `push ecx` frame word),
// then a loop that re-tests the thing (present, not dead) and the action list each frame; without a runner
// it spins, a stop request ends the wait.
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing reached through a local view).
#include <stddef.h>
#include "engine/CScriptGameResourceObjectScriptedThing.h"


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
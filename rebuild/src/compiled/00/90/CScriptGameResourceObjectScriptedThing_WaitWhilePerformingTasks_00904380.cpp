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

struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};

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
// behaviour test for 0x00904380 CScriptGameResourceObjectScriptedThing::WaitWhilePerformingTasks (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
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

struct CTCScriptedControl {                      // interface 0x1f component
    bool IsActionListEmpty();                     // 0x007122C0
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void WaitWhilePerformingTasks();
};

inline bool IS_THING_ALIVE(CIntelligentPointer_Retail* p)
{
    if (p->GetPItem()) {
        if (!p->GetPItem()->m_dead)
            return true;
    }
    return false;
}
void CScriptGameResourceObjectScriptedThing_Methods::WaitWhilePerformingTasks()
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
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


// ---- stubs ----
static long g_lbKey; static int g_lbOffset; static int g_lbCalls;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { g_lbKey = *type; g_lbCalls++; return first + g_lbOffset; }
CThing_Retail* CIntelligentPointer_Retail::GetPItem() { return (CThing_Retail*)m_pItem; }
static int g_yields;
void CScriptRunner::YieldFrame() { g_yields++; }
void CScriptRunner::v00() { }
void CScriptRunner::v01() { }
void CIntelligentPointer_Retail::v00() { }
CScriptRunner* g_pScriptRunner;
static CScriptRunner g_runner;
// action-list stub: busy for g_busyFrames calls, then empty; optional side effects at a given call
static int g_emptyCalls; static int g_busyFrames; static CTCScriptedControl* g_emptyThis;
static int g_killAt; static int g_dropAt; static int g_stopAt; static int g_runnerBackAt;
static CThing_Retail* g_thing; static CIntelligentPointer_Retail* g_ptr;
bool CTCScriptedControl::IsActionListEmpty()
{
    g_emptyCalls++; g_emptyThis = this;
    if (g_emptyCalls == g_killAt) g_thing->m_dead = 1;
    if (g_emptyCalls == g_dropAt) g_ptr->m_pItem = 0;
    if (g_emptyCalls == g_stopAt) g_runner.m_bStopRequested = true;
    if (g_emptyCalls == g_runnerBackAt) g_pScriptRunner = &g_runner;
    return g_emptyCalls > g_busyFrames;
}

static int fails;
static void check(int c, const char* w) { if (!c) { fails++; printf("FAIL %s\n", w); } }

static double g_objBuf[0x10], g_thingBuf[0x20];
static double g_ctlBuf[1], g_endCtlBuf[1];

static void Reset(int busy)
{
    g_emptyCalls = 0; g_busyFrames = busy; g_yields = 0; g_killAt = g_dropAt = g_stopAt = g_runnerBackAt = 0;
    g_runner.m_bStopRequested = false; g_pScriptRunner = &g_runner;
    g_thing->m_dead = 0; g_ptr->m_pItem = g_thing;
}

int main()
{
    CScriptGameResourceObjectScriptedThing_Methods* obj = (CScriptGameResourceObjectScriptedThing_Methods*)g_objBuf;
    CThing_Retail* thing = (CThing_Retail*)g_thingBuf; g_thing = thing;
    g_ptr = (CIntelligentPointer_Retail*)obj->PThing;
    CTCScriptedControl* ctl = (CTCScriptedControl*)g_ctlBuf;
    CTCScriptedControl* endCtl = (CTCScriptedControl*)g_endCtlBuf;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x1f; entries[0].component = (CTCBase*)ctl;
    entries[1].type = 0; entries[1].component = (CTCBase*)endCtl;
    thing->m_interfaces.first = entries; thing->m_interfaces.last = entries + 1;
    thing->m_presentBits[0] = 1UL << 31;

    // 1: no thing -> nothing
    Reset(0); g_ptr->m_pItem = 0;
    obj->WaitWhilePerformingTasks();
    check(g_emptyCalls == 0 && g_lbCalls == 0, "no thing");
    // 2: dead thing -> nothing
    Reset(0); thing->m_dead = 1;
    obj->WaitWhilePerformingTasks();
    check(g_emptyCalls == 0 && g_lbCalls == 0, "dead");
    // 3: already idle -> one test, no yield
    Reset(0);
    obj->WaitWhilePerformingTasks();
    check(g_emptyCalls == 1 && g_yields == 0 && g_lbKey == 0x1f && g_emptyThis == ctl, "idle");
    // 4: busy for 3 frames -> 3 yields
    Reset(3);
    obj->WaitWhilePerformingTasks();
    check(g_emptyCalls == 4 && g_yields == 3, "busy 3");
    // 5: stop requested -> break before yielding
    Reset(10); g_stopAt = 2;
    obj->WaitWhilePerformingTasks();
    check(g_emptyCalls == 2 && g_yields == 1, "stop");
    // 6: the thing dies mid-wait
    Reset(10); g_killAt = 3;
    obj->WaitWhilePerformingTasks();
    check(g_emptyCalls == 3 && g_yields == 3, "dies");
    // 7: the pointer clears mid-wait
    Reset(10); g_dropAt = 2;
    obj->WaitWhilePerformingTasks();
    check(g_emptyCalls == 2 && g_yields == 2, "dropped");
    // 8: no runner -> spins without yielding until a runner appears
    Reset(6); g_pScriptRunner = 0; g_runnerBackAt = 4;
    obj->WaitWhilePerformingTasks();
    check(g_emptyCalls == 7 && g_yields == 3, "no runner spin");
    // 9: lookup end / greater / hit
    Reset(0); g_lbOffset = 1; obj->WaitWhilePerformingTasks();
    check(g_emptyThis == endCtl, "end pick");
    Reset(0); g_lbOffset = 0; entries[0].type = 0x20; obj->WaitWhilePerformingTasks();
    check(g_emptyThis == endCtl, "greater-key pick");
    Reset(0); entries[0].type = 0x1f; obj->WaitWhilePerformingTasks();
    check(g_emptyThis == ctl, "hit pick");
    // 10: no scripted-control component -> no lookup (stale slot; the stub ignores it)
    Reset(2); thing->m_presentBits[0] = 0x7fffffff; int calls = g_lbCalls;
    obj->WaitWhilePerformingTasks();
    check(g_lbCalls == calls && g_yields == 2, "no component");
    if (!fails) printf("00904380_TEST PASS\n");
    return fails;
}
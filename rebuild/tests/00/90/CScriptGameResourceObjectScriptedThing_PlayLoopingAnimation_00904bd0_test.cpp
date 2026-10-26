// Behaviour test for 00904bd0: src/00904bd0.cpp copied verbatim; the scripted thing, its scripted-control component and
// the looping play-animation action stubbed so the live gate, the optional clear (plus FinishCurrentAction for creatures) and
// the queued action's arguments and increasing priority are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CScriptGameResourceObjectScriptedThing::PlayLoopingAnimation(const CCharString& animation, long numLoops, bool useMovement, bool clearActions, bool addAsQueuedAction, bool waitForAnimationToFinish, bool usePhysics, bool animMayNeedCameraPositionUpdated, bool allowLooking) -- queue a looping animation for the live scripted thing at the next action priority, optionally clearing its scripted actions and finishing a creature's current action first
// Retry 10b (chain-split sweep, FIXES B-16): the `GetPItem()` result is a named local `item` before it is dereferenced, which gives retail's register colouring/order (the chained call gives the parked difference).
// Leaf: PlayLoopingAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904BD0, length 253 bytes (`ret 24h`), vtable 0x0128D86C slot 20.
// PDB: ?PlayLoopingAnimation@CScriptGameResourceObjectScriptedThing@@UAEXABVCCharString@@J_N111111@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface, interface 0x1f), CTCScriptedControl::CleanupAllActions @ 0x00713630, CThingCreatureBase::
// FinishCurrentAction @ 0x00662650, operator new @ 0x00BFEA1A (0x28 bytes), NTCScriptedControl::CActionPlayAnimation::
// CActionPlayAnimation(const CCharString&, bool, bool, long, bool, long, bool x5) @ 0x009034F0, CTCScriptedControl::AddAction
// @ 0x007137D0.
// Shape: the landed PlayCombatAnimation 0x00904AF0 shape (unchecked GetInterface, the miss reading the `clearActions` slot,
// the creature test on ThingTypeFlags bit 3); the priority argument is `ActionPriority++` (old value pushed, the increment
// stored inside the new-expression's non-null arm); the new-expression's null result duplicates the AddAction tail.
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
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
};
}

class CCharString;

class CThingCreatureBase { public:
    void FinishCurrentAction();                    // 0x00662650
};

class CTCScriptedControl { public:                        // interface 0x1f component
    void CleanupAllActions();                      // 0x00713630
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

namespace NTCScriptedControl {
struct CActionPlayAnimation : CActionBase {        // vtable 0x0128D6F4, 0x28 bytes
    unsigned char _body[0x28];
    CActionPlayAnimation(const CCharString& animation, bool stayOnLastFrame, bool looping, long numLoops,
                         bool useMovement, long priority, bool addAsQueuedAction, bool waitForAnimationToFinish,
                         bool usePhysics, bool animMayNeedCameraPositionUpdated, bool allowLooking);   // 0x009034F0
};
}

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void PlayLoopingAnimation(const CCharString& animation, long numLoops, bool useMovement, bool clearActions, bool addAsQueuedAction,
                              bool waitForAnimationToFinish, bool usePhysics, bool animMayNeedCameraPositionUpdated, bool allowLooking);
};

void CScriptGameResourceObjectScriptedThing_Methods::PlayLoopingAnimation(const CCharString& animation, long numLoops, bool useMovement, bool clearActions, bool addAsQueuedAction,
                                                                         bool waitForAnimationToFinish, bool usePhysics, bool animMayNeedCameraPositionUpdated, bool allowLooking)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        CThing_Retail* item = thing->GetPItem();
        if (!item->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            if (clearActions) {
                control->CleanupAllActions();
                if (thing->GetPItem()->ThingTypeFlags & 8)
                    ((CThingCreatureBase*)thing->GetPItem())->FinishCurrentAction();
            }
            control->AddAction(new NTCScriptedControl::CActionPlayAnimation(animation, false, true, numLoops, useMovement, ActionPriority++,
                                                                            addAsQueuedAction, waitForAnimationToFinish, usePhysics,
                                                                            animMayNeedCameraPositionUpdated, allowLooking));
        }
    }
}

// ---- end source ----

CThing_Retail* CIntelligentPointer_Retail::GetPItem() { return (CThing_Retail*)m_pItem; }
static CTCScriptedControl g_control;
static CInterfaceMap::SEntry g_entries[1] = { { 0x1f, (CTCBase*)&g_control } };
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type <= 0x1f ? first : last; }
static int g_cleanups = 0, g_finishes = 0, g_adds = 0;
void CTCScriptedControl::CleanupAllActions() { ++g_cleanups; }
void CTCScriptedControl::AddAction(NTCScriptedControl::CActionBase*) { ++g_adds; }
void CThingCreatureBase::FinishCurrentAction() { ++g_finishes; }
static const CCharString* g_anim = 0; static long g_prio = -1, g_loops = 0; static bool g_looping = false, g_move = false; static bool g_stay = false, g_queued = false, g_wait = false, g_look = false;
NTCScriptedControl::CActionPlayAnimation::CActionPlayAnimation(const CCharString& a, bool stay, bool looping, long numLoops, bool useMovement, long priority,
    bool addAsQueued, bool wait, bool usePhysics, bool cam, bool look)
{ g_anim = &a; g_looping = looping; g_move = useMovement; g_stay = stay; g_loops = numLoops; g_prio = priority; g_queued = addAsQueued; g_wait = wait; g_look = look; }
int main()
{
    static CThing_Retail me; memset(&me, 0, sizeof(me)); me.m_interfaces.first = g_entries; me.m_interfaces.last = g_entries + 1;
    static unsigned char raw[sizeof(CScriptGameResourceObjectScriptedThing_Methods)]; memset(raw, 0, sizeof(raw));
    CScriptGameResourceObjectScriptedThing_Methods* s = (CScriptGameResourceObjectScriptedThing_Methods*)raw;
    ((CIntelligentPointer_Retail*)s->PThing)->m_pItem = &me; s->ActionPriority = 7;
    const CCharString& anim = *(const CCharString*)&g_prio;
    s->PlayLoopingAnimation(anim, 3, true, false, true, false, false, false, true);
    CHECK(g_adds == 1 && g_cleanups == 0 && g_anim == &anim && !g_stay && g_looping && g_move && g_loops == 3 && g_prio == 7 && g_queued && !g_wait && g_look && s->ActionPriority == 8);
    s->PlayLoopingAnimation(anim, 2, false, true, false, true, false, false, false);
    CHECK(g_adds == 2 && g_cleanups == 1 && g_finishes == 0 && g_prio == 8 && g_wait);
    me.ThingTypeFlags = 8;
    s->PlayLoopingAnimation(anim, 1, false, true, false, false, false, false, false);
    CHECK(g_cleanups == 2 && g_finishes == 1);
    me.m_dead = 1; s->PlayLoopingAnimation(anim, 1, false, true, false, false, false, false, false); CHECK(g_adds == 3);
    ((CIntelligentPointer_Retail*)s->PThing)->m_pItem = 0; s->PlayLoopingAnimation(anim, 1, false, false, false, false, false, false, false); CHECK(g_adds == 3);
    if (fails == 0) printf("00904bd0_TEST PASS\n"); else printf("00904bd0_TEST FAIL\n");
    return fails;
}
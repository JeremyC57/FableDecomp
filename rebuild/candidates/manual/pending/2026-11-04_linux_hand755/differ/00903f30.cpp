// CScriptGameResourceObjectScriptedThing::FollowPreCalculatedRoute(const CScriptThing& route, EScriptEntityMoveType moveType, bool wait, bool avoidDynamicObstacles) -- when the route thing is a pre-calculated route, queue a follow-route move action for the live scripted thing, then optionally wait while it performs its tasks
// Grade: DIFFER(254v254) -- callee-saved register swap only: retail keeps routeThing in ebx and the mapped movement type in ebp (push ebx before GetPThing, push ebp before the switch); ours swaps them
// Leaf: FollowPreCalculatedRoute
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903F30, length 254 bytes (`ret 10h`), vtable 0x0128D86C slot 6.
// PDB: ?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CScriptThing::IsAlive (slot 75) and GetPThing
// (slot 11), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f),
// operator new @ 0x00BFEA1A (0x50 bytes), NTCScriptedControl::CActionMoveTo::CActionMoveTo(CThing*, long, bool, bool)
// @ 0x00905290, CTCScriptedControl::AddAction @ 0x007137D0, this->WaitWhilePerformingTasks (slot 11, +0x2c).
// Shape: the MoveToThing 0x00903E20 twin: the route test is bit 6 of the route thing's +0x34 flags word, extracted as a byte
// and stored to the dead `route` slot through a const bool& temporary); the move type is mapped before the plain (`test ecx,ecx ; jns`) GetInterface;
// `this` is kept in the `push ecx` frame word for the final virtual call.
#include <stddef.h>

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
    unsigned long  m_presentBits[5];               // +0x20 std::bitset of interface types (+0x20..+0x33 used here)
    unsigned long  m_flag34_0 : 6;                 // +0x34
    unsigned long  m_isRoute : 1;                  // +0x34 bit 6 (descriptive: the thing is a pre-calculated route)
    unsigned long  m_flag34_7 : 25;
    unsigned long  m_presentBits_rest[3];
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x45];     // (+0x34 lies in _pad00: see m_thingFlags below)
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    unsigned char HasTC(long interfaceType) const { return (unsigned char)((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1); }
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

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

enum EScriptEntityMoveType { SCRIPT_MOVE_WALK = 0, SCRIPT_MOVE_RUN = 1 };
enum EMovementType { MOVEMENT_SNEAK = 1, MOVEMENT_WALK = 3, MOVEMENT_RUN = 5 };   // descriptive (3 walk / 5 run as the landed 0x00830000)
inline EMovementType __fastcall GFConvertScriptMoveTypeToAIMoveType(EScriptEntityMoveType moveType)   // Ego_r.pdb ?GFConvertScriptMoveTypeToAIMoveType@@YI?AW4EMovementType@@W4EScriptEntityMoveType@@@Z
{
    switch (moveType) {
    case SCRIPT_MOVE_WALK: return MOVEMENT_WALK;
    case SCRIPT_MOVE_RUN:  return MOVEMENT_RUN;
    }
    return MOVEMENT_SNEAK;
}
namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
struct CActionMoveTo : CActionBase {               // vtable 0x0127091C, 0x50 bytes
    unsigned char _body[0x4c];
    CActionMoveTo(CThing_Retail& target, EMovementType movementType, float proximity, bool faceMovement,
                  bool avoidDynamicObstacles, bool useActionPoint, bool useBackupProximity,
                  bool cancelAllActionsOnFailure, bool ignorePathPreferability);   // 0x009051F0
    CActionMoveTo(const CThing_Retail& routeThing, EMovementType movementType, bool faceMovement, bool avoidDynamicObstacles);   // 0x00905290
};
}

struct CTCScriptedControl {                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};



struct CGameScriptInterface;
struct CScriptGameResourceObjectScriptedThing {    // retail layout = engine/CScriptGameResourceObjectScriptedThing.h (vptr at +0)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void WaitWhilePerformingTasks();       // slot 11 (+0x2c) 0x00904380
    void*                      IntelligentPointer; // +0x04 (CBaseObject)
    void*                      PImp_Data;          // +0x08 (CScriptGameResourceObjectScriptedThingBase::PImp)
    void*                      PImp_Info;          // +0x0c
    CGameScriptInterface*      ScriptInterface;    // +0x10
    CIntelligentPointer_Retail PThing;             // +0x14 CIntelligentPointer<CThing>
    long                       ScriptID;           // +0x1c
    bool                       NullResource;       // +0x20
    long                       ActionPriority;     // +0x24
    void FollowPreCalculatedRoute(const CScriptThing& route, EScriptEntityMoveType moveType, bool wait, bool avoidDynamicObstacles);
};


void CScriptGameResourceObjectScriptedThing::FollowPreCalculatedRoute(const CScriptThing& route, EScriptEntityMoveType moveType, bool wait, bool avoidDynamicObstacles)
{
    CIntelligentPointer_Retail* thing = &PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (route.IsAlive()) {
                CThing_Retail* routeThing = ((CThing_Retail*)route.GetPThing());
                const bool& isRoute = routeThing->m_isRoute;
                if (isRoute) {
                    EMovementType movementType = GFConvertScriptMoveTypeToAIMoveType(moveType);
                    CTCScriptedControl* control;
                    thing->GetPItem()->GetInterface(&control, 0x1f);
                    control->AddAction(new NTCScriptedControl::CActionMoveTo(*routeThing, movementType, true, avoidDynamicObstacles));
                    if (wait)
                        WaitWhilePerformingTasks();
                }
            }
        }
    }
}

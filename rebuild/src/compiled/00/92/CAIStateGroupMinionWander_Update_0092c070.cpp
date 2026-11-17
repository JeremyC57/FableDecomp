// CAIStateGroup_MinionWander::Update() -- wander loop: wander and idle; when the creature has strayed out of its wander circle (the scratchpad centre and max distance for state 4, else its start position and 10) walk back, then idle; repeat while DoNothing keeps running
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C070, length 491 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xAA = CTCAIScratchpad, bit 10 of +0x34),
// CDefClassBase::GetSubDef<CCreatureNavigationDef> @ 0x006CD2F0 (on the creature's +0x70), CThing::GetPos @ 0x004C73D0
// (twice), CAIStateGroupBase::AIAction_MinionWander @ 0x0097F870, AIAction_MinionIdle @ 0x0097FA00 (twice),
// AIAction_MoveToPosProximity @ 0x0097E890, AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40 (twice).
// Shape: the presence bit is bound to a const reference (frame byte, later reused by the `reached` flag); the navigation
// def is copied out of its out slot into a holder (kept in edi, the slot reused for the radius) whose inline dtor releases
// it (no null test on the paths that already read AvoidDynamicObstacles); the state-4 centre comes from a by-value getter
// (temporary copy) chosen against the copied start position; `reached` is a function-scope bool assigned false inside the
// move arm (an initialised block local is stored after the address lea instead of after the first push).
// Layout: no engine header for CTCAIScratchpad or CCreatureNavigationDef; members named from Ego_r.pdb (WanderCentrePoint
// +0x14, WanderMaxDistance +0x24, ScriptingStateGroup +0x38; AvoidDynamicObstacles +0x34).
#include "fable_compiler.h"
class CTCBase;

#include "engine/C3DVector.h"

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CCreatureNavigationDef { public:
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x2c];
    bool AvoidDynamicObstacles;                 // +0x34
    void Release() { if (--RefCount == 0) DeleteThis(); }
};

struct CDefPointerNav {
    CCreatureNavigationDef* m_p;
    CDefPointerNav() : m_p(0) { }
};

struct CNavDefHolder {
    CCreatureNavigationDef* p;
    CNavDefHolder(CCreatureNavigationDef* d) : p(d) { }
    ~CNavDefHolder() { if (p) p->Release(); }
};

class CDefClassBase { public:
    bool GetSubDef(CDefPointerNav* out);        // 0x006CD2F0 (GetSubDef<CCreatureNavigationDef>)
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x1c];
    CDefClassBase* PhysicsTC;                   // +0x70
    const C3DVector& GetPos() const;       // 0x004C73D0
    unsigned char HasTC(long interfaceType) const { return (unsigned char)((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1); }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        const unsigned char& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
        }
        return present != 0;
    }
};

class CTCAIScratchpad { public:                        // interface 0xAA
    unsigned char _pad[0x14];
    C3DVector WanderCentrePoint;                // +0x14
    float WanderMinDistance;                    // +0x20
    float WanderMaxDistance;                    // +0x24
    unsigned char _pad28[0x10];
    long ScriptingStateGroup;                   // +0x38
    C3DVector GetWanderCentrePoint() const { return WanderCentrePoint; }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CThingBuilding;
// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model
// with long / bool* / CThing* parameters): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; header _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h
// (replace the local empty enum EAIActionReturn); AIAction_MinionWander 0x0097F870 is the header's protected
// ?AIAction_MinionWander@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EMovementType@@PAVCThingBuilding@@@Z (_MINIONWANDER) and
// AIAction_MinionIdle 0x0097FA00 is ?AIAction_MinionIdle@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ (both PDBs; overlay opt-in _MINIONIDLE).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MINIONIDLE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MINIONWANDER
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

#pragma auto_inline(off)
long TmplSink(const void* self, long key);      // stand-in: the callee's own work (outside the graded bytes)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointerNav* out)
{
    CCreatureNavigationDef* old = out->m_p;
    out->m_p = (CCreatureNavigationDef*)TmplSink(this, (long)old);
    return out->m_p != 0;
}
#pragma auto_inline(on)

struct CAIStateGroup_MinionWander : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_MinionWander::Update()
{
    CThing* creature = PBrain->PCreature;
    CTCAIScratchpad* scratchpad;
    creature->GetInterface(&scratchpad, 0xaa);
    CDefPointerNav navDef;
    creature->PhysicsTC->GetSubDef(&navDef);
    C3DVector start = PBrain->PCreature->GetPos();
    CNavDefHolder def(navDef.m_p);
    bool reached;
    do {
        if (AIAction_MinionWander(AI_MOVEMENT_WALKING, 0) != 2)
            return;
        if (AIAction_MinionIdle() != 2)
            return;
        C3DVector centre = scratchpad->ScriptingStateGroup == 4 ? scratchpad->GetWanderCentrePoint() : start;
        float radius = scratchpad->ScriptingStateGroup == 4 ? scratchpad->WanderMaxDistance : 10.0f;
        float distSq;
        {
            const C3DVector& pos = PBrain->PCreature->GetPos();
            float dx = pos.X - centre.X;
            float dy = pos.Y - centre.Y;
            distSq = dy * dy + dx * dx;
        }
        float radiusSq = radius * radius;
        if (!(distSq < radiusSq)) {
            reached = false;
            if (AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED, centre, radius, reached, AI_MOVEMENT_WALKING, true, false, def.p->AvoidDynamicObstacles, 0, true, 0.0f) != 2) {
                Terminate();
                return;
            }
            if (AIAction_MinionIdle() != 2) {
                Terminate();
                return;
            }
        }
    } while (AIAction_DoNothing() == 4);
}
// CCreatureActionBase::GetActionStartRunningTimeSeconds() -- the action's running time scaled by the
// "start running" animation event time, or -1.0f when the action has no anim or no such event
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006923A0, length 88 bytes (+ int3 padding).
// Callees: vtable slot 7 (+0x1c, long: the action's anim id), the anim-manager getter @ 0x006F90B0
// (returns the global @ 0x013BABB4; its "GFGetPreMainMemoryUsed" label is propagated),
// CManager::AnimHasEvent(ulong, const CCharString&) @ 0x006F9720, vtable slot 63 (+0xfc, float),
// CManager::GetEventTimeInSeconds(ulong, const CCharString&) @ 0x006F9790. The event name is the
// static CCharString @ 0x013BAC8C (passed by reference twice); the failure result is the float
// pool constant -1.0f @ 0x0122DEE0. The slot-63 result is parked in the one frame slot (`push ecx`)
// across the second manager call and multiplied by that call's result.
// engine/CCreatureActionBase.h (PDB) only supplies the vtable-less layout; the virtual slots are
// reached through a vtable-shaped view of `this` as in the landed UpdateActionRunningTime.
#include "engine/CCreatureActionBase.h"

class CCharString;
extern CCharString g_startRunningEventName_013bac8c;   // 0x013BAC8C

struct CAnimManager_Calls {                    // CManager (anim manager)
    bool  AnimHasEvent(unsigned long animID, const CCharString& eventName);              // 0x006F9720
    float GetEventTimeInSeconds(unsigned long animID, const CCharString& eventName);     // 0x006F9790
};

CAnimManager_Calls* __fastcall GetAnimManager_006f90b0();   // 0x006F90B0

struct CCreatureActionBaseVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18();
    virtual long GetAnimID();                  // slot 7 (+0x1c)
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0(); virtual void vb4(); virtual void vb8(); virtual void vbc();
    virtual void vc0(); virtual void vc4(); virtual void vc8(); virtual void vcc();
    virtual void vd0(); virtual void vd4(); virtual void vd8(); virtual void vdc();
    virtual void ve0(); virtual void ve4(); virtual void ve8(); virtual void vec();
    virtual void vf0(); virtual void vf4(); virtual void vf8();
    virtual float GetActionRunningTimeSeconds();   // slot 63 (+0xfc)
};

struct CCreatureActionBase_Methods : CCreatureActionBase {
    float GetActionStartRunningTimeSeconds();
};

float CCreatureActionBase_Methods::GetActionStartRunningTimeSeconds()
{
    CCreatureActionBaseVirtuals* self = reinterpret_cast<CCreatureActionBaseVirtuals*>(this);
    long animID = self->GetAnimID();
    if (animID > 0) {
        CAnimManager_Calls* manager = GetAnimManager_006f90b0();
        if (manager->AnimHasEvent(animID, g_startRunningEventName_013bac8c)) {
            float runningTime = self->GetActionRunningTimeSeconds();
            return manager->GetEventTimeInSeconds(animID, g_startRunningEventName_013bac8c) * runningTime;
        }
    }
    return -1.0f;
}
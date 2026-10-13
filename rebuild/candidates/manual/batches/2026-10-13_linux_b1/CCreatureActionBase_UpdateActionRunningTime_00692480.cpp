// CCreatureActionBase::UpdateActionRunningTime()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692480, oracle length 19 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; call [eax+0F8h] ; fadd [esi+40h] ; fstp [esi+40h] ; pop esi ; ret
// engine/CCreatureActionBase.h (PDB): +0x40 float RunningTimeSeconds. Virtual slot 62 (+0xF8)
// returns this frame's time step (float, st0) which is accumulated into RunningTimeSeconds.
// The slot is reached through a vtable-shaped view of `this` so the call site is a plain
// `call [eax+0F8h]`; the real slot name is not in the PDB export.
#include "engine/CCreatureActionBase.h"

struct CCreatureActionBaseVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
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
    virtual void vf0(); virtual void vf4();
    virtual float GetFrameTimeStep();   // slot 62 (+0xf8)
};

struct CCreatureActionBase_Methods : CCreatureActionBase {
    void UpdateActionRunningTime();
};

void CCreatureActionBase_Methods::UpdateActionRunningTime()
{
    CCreatureActionBaseVirtuals* self = reinterpret_cast<CCreatureActionBaseVirtuals*>(this);
    this->RunningTimeSeconds += self->GetFrameTimeStep();
}

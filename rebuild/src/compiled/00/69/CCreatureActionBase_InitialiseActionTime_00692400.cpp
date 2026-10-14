// CCreatureActionBase::InitialiseActionTime() -- when the action reports a positive duration, seed
// the running-time pair with it and compute the start frame from the frames-per-second global
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692400, length 70 bytes (+ int3 padding).
// Callees: vtable slot 64 (+0x100, float, called twice: once for the test, once for the value),
// __ftol2 @ 0x00BFEA70. Globals: the float 0.0f @ 0x0122DEDC, the frame-rate long @ 0x01375550.
// engine/CCreatureActionBase.h (PDB): RunningTimeSeconds +0x40, LastRunningTimeSeconds +0x44,
// StartFrame +0x48. The frame-rate global is read into a local BEFORE the second virtual call
// (it is spilled to the frame slot and `fild`ed later), the duration is stored to both members
// with `fst` and then multiplied by the converted rate for the truncating conversion.
#include "engine/CCreatureActionBase.h"

extern long g_FramesPerSecond_01375550;        // 0x01375550

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
    virtual void vf0(); virtual void vf4(); virtual void vf8(); virtual void vfc();
    virtual float GetActionTimeSeconds();      // slot 64 (+0x100)
};

struct CCreatureActionBase_Methods : CCreatureActionBase {
    void InitialiseActionTime();
};

void CCreatureActionBase_Methods::InitialiseActionTime()
{
    CCreatureActionBaseVirtuals* self = reinterpret_cast<CCreatureActionBaseVirtuals*>(this);
    if (self->GetActionTimeSeconds() > 0.0f) {
        long framesPerSecond = g_FramesPerSecond_01375550;
        float seconds = self->GetActionTimeSeconds();
        RunningTimeSeconds = seconds;
        LastRunningTimeSeconds = seconds;
        StartFrame = (long)(seconds * (float)framesPerSecond);
    }
}
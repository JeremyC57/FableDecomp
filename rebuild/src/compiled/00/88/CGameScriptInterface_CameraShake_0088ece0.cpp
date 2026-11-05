// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/C3DVector.h"

struct CCamObj {
    char body[4]; // C3DVector at +4
    void SetEarthquakeAt(C3DVector* pos, float magnitude, float duration, bool oneShot);
};

struct CCamMgr {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual bool GetObj(CCamObj** out); // slot 9 -> vtbl offset 0x24
};

class CWorld { public: char pad[0x38]; }; // CCamMgr* at +0x34
// outer at +0x4

void __fastcall CGameScriptInterface_CameraShake(const CGameScriptInterface* self, float magnitude, float duration)
{
    CCamObj* local;
    CWorld* world = self->World;
    CCamMgr* b = *(CCamMgr**)((char*)world + 0x34);
    if (b->GetObj(&local)) {
        local->SetEarthquakeAt((C3DVector*)((char*)local + 4), magnitude, duration, false);
    }
}
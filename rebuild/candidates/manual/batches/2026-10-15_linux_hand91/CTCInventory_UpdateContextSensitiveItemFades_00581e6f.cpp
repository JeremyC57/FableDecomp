#pragma optimize("s", on)
// CTCInventory::UpdateContextSensitiveItemFades(const CEnginePrimitivePrepareInfo& info) -- advances the fade of every context-sensitive slot by the frame's predicted render time
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581E6F, length 78 bytes; size-optimised TU ("s").
// Callees: CFadeInFadeOutBase::Update(float) 0x00434870.
// Ego_r PDB: ?UpdateContextSensitiveItemFades@CTCInventory@@QAEXABVCEnginePrimitivePrepareInfo@@@Z.
// Retail layout: engine/CTCInventory.h is the wider Ego_r.pdb layout (16-byte vectors); the shipped object keeps
// ContextSensitiveItems (vector<long>) at +0x154 and ContextSensitiveFadeTimes (vector<CFadeInFadeOutBase>,
// 0x1c-byte elements, engine/CFadeInFadeOutBase.h) at +0x160 as 12-byte {first,last,end} vectors, so `this`
// is modelled locally. The float read is info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame (+0).
// Shape: loop bound is the ITEM count (re-evaluated), the fade index strength-reduced to a byte offset
// (`add ebx,1Ch`) added to the reloaded fade base; the float is copied through x87 (`fld ; push ecx ; fstp`).
#include "engine/CFadeInFadeOutBase.h"

struct CFadeInFadeOutBase_Calls : CFadeInFadeOutBase {
    void Update(float elapsed);                      // 0x00434870
};

struct CInterpolationInfoSet {
    float GTPredictedTimeSinceLastRenderFrame;       // +0x00
    float WFInterpolate;                             // +0x04
};

struct CEnginePrimitivePrepareInfo {
    CInterpolationInfoSet Current;                   // +0x00 (Interpolate.Current)
};

struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
};

struct CFadeVector {
    CFadeInFadeOutBase_Calls* first;                 // +0x00
    CFadeInFadeOutBase_Calls* last;                  // +0x04
    CFadeInFadeOutBase_Calls* end_of_storage;        // +0x08
    CFadeInFadeOutBase_Calls& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventory {
    unsigned char _pad_0x000[0x154];
    CLongVector   ContextSensitiveItems;             // +0x154 (PDB +0x178)
    CFadeVector   ContextSensitiveFadeTimes;         // +0x160 (PDB +0x188)
    void UpdateContextSensitiveItemFades(const CEnginePrimitivePrepareInfo& info);
};

void CTCInventory::UpdateContextSensitiveItemFades(const CEnginePrimitivePrepareInfo& info)
{
    for (unsigned long i = 0; i < ContextSensitiveItems.size(); ++i)
        ContextSensitiveFadeTimes[i].Update(info.Current.GTPredictedTimeSinceLastRenderFrame);
}

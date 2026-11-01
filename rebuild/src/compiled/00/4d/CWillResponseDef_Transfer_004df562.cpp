// CWillResponseDef::Transfer(CPersistContext& context) -- persist the eight will-response flags (IsForcePushable .. IsEpicSpellable), one CPersistContext::Transfer<bool> each
// Leaf: ?Transfer@CWillResponseDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF562, 101 bytes, thiscall, `ret 4` (unaligned entry, def module).
// Identity (proven): slot 18 of the CWillResponseDef vtable 0x0123E324 (rebuild/manifest/rtti-vtables.tsv); Ego_r.pdb and
//   FableWin.pdb ?Transfer@CWillResponseDef@@UAEXAAVCPersistContext@@@Z; the eight transferred bytes are Ego_r.pdb's
//   CWillResponseDef bools at +0x25..+0x2c (retail = Ego_r: the def sits straight on the packed 0x25-byte CDefClassBase).
// Callees: CPersistContext::Transfer<bool>(bool&) 0x0043314A (x8; landed ??$Transfer@_N@CPersistContext@@QAEXAA_N@Z).
// Shape: the def-module family (CMultiStaticMeshEntryDef::Transfer 0x004EB8C3): one Transfer<T>(T&) per member in
//   declaration order, context in edi, size TU (every member address formed in eax, the last one by `add esi`).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CPersistContext {
public:
    template <class T> void Transfer(T& value);          // CPersistContext::Transfer<T>(T&) (bool: 0x0043314A)
};
class CWillResponseDef : public CDefClassBase {          // vtable 0x0123E324
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    virtual void Copy(const CDefClassBase*);             // slot 19 0x004E18C9
    virtual unsigned long GetSizeofClass();              // slot 20 0x004D649F
    bool IsForcePushable;                                // +0x25
    bool IsLifeDrainable;                                // +0x26
    bool IsLightningable;                                // +0x27
    bool IsNotBloodyWillable;                            // +0x28
    bool IsAffectedBySlowTime;                           // +0x29
    bool IsAbleToBeLifted;                               // +0x2a
    bool IsEpicPowerExplodable;                          // +0x2b
    bool IsEpicSpellable;                                // +0x2c
};
void CWillResponseDef::Transfer(CPersistContext& context)
{
    context.Transfer(IsForcePushable);
    context.Transfer(IsLifeDrainable);
    context.Transfer(IsLightningable);
    context.Transfer(IsNotBloodyWillable);
    context.Transfer(IsAffectedBySlowTime);
    context.Transfer(IsAbleToBeLifted);
    context.Transfer(IsEpicPowerExplodable);
    context.Transfer(IsEpicSpellable);
}
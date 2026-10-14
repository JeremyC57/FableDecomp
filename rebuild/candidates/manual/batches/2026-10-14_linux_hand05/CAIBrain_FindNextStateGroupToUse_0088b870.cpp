// CAIBrain::FindNextStateGroupToUse(long priority) -- first enabled, runnable state group whose priority is above `priority` (the list is sorted by descending priority)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088B870, length 70 bytes.
// Callees: CAIStateGroupBase::IsEnabledAndRunnable @ 0x008FCF10 (Enabled && IsRunnable(): the
// manifest's "Script_GetCurrentStateGroupPriority" label is a propagated name).
// engine/CAIBrain.h (PDB): +0x28 CVectorMap<long, CAIStateGroupBase*> StateGroupList, whose
// first two dwords are begin/end of its 8-byte {key, group} pairs (same idiom as the landed
// CAIBrain::SetAllStateGroupsEnabled @ 0x0088B8C0). CAIStateGroupBase::Priority is the byte at
// +0x11 (engine/CAIStateGroupBase.h), widened with movzx for the signed compare. The walk stops
// (returning null) at the first group whose priority is not above the request -- the map is
// kept in CKeyPairCompareGreater order -- and the end pointer is re-read every iteration.
#include "engine/CAIBrain.h"

struct CAIStateGroupBaseCalls {
    unsigned char _pad_0x00[0x11];
    unsigned char Priority;           // +0x11
    bool          Enabled;            // +0x12
    bool IsEnabledAndRunnable();      // 0x008FCF10
};

struct CStateGroupEntry {
    long                    Key;      // +0x00 state group id
    CAIStateGroupBaseCalls* Group;    // +0x04
};

struct CStateGroupVectorMap {
    CStateGroupEntry* Begin;          // +0x00
    CStateGroupEntry* End;            // +0x04
};

struct CAIBrain_Methods : CAIBrain {
    CAIStateGroupBaseCalls* FindNextStateGroupToUse(long priority);
};

CAIStateGroupBaseCalls* CAIBrain_Methods::FindNextStateGroupToUse(long priority)
{
    CStateGroupVectorMap* groups = reinterpret_cast<CStateGroupVectorMap*>(&this->StateGroupList[0]);
    for (CStateGroupEntry* it = groups->Begin; it != groups->End; ++it) {
        CAIStateGroupBaseCalls* group = it->Group;
        if ((long)group->Priority <= priority)
            return 0;
        if (group->IsEnabledAndRunnable())
            return group;
    }
    return 0;
}

// CCrateStackDef::Transfer(CPersistContext& context) -- persist StackType and Priority
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE9A1, length 49 bytes.
// Callees: the context's enum transfer helper @ 0x00431102 (one by-address argument, as in the
// landed CPersistable_0046a6dd::Transfer) for StackType, then CPersistContext::Transfer<long>
// (name, value, default) @ 0x00410BE0 for Priority with the literal "Priority" @ 0x01241928 and a
// zero default built in the frame slot (`and [ebp-4],0`).
// engine/CCrateStackDef.h (PDB): StackType +0x28, Priority +0x2c -- the layout is taken from it.
// Shape: a frame-pointer, size-optimised translation unit (`push ebp ; mov ebp,esp ; push ecx`,
// `leave`, the second member's address formed in place with `add esi,2Ch`), the retail persist
// hook shape: #pragma optimize("y",off) then ("s",on).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CCrateStackDef.h"

struct CPersistContext {
    void TransferEnum_00431102(long* value);                                          // 0x00431102
    void Transfer(const char* name, long& value, const long& defaultValue);           // Transfer<long> @ 0x00410BE0
};

struct CCrateStackDef_Methods : CCrateStackDef {
    void Transfer(CPersistContext& context);
};

void CCrateStackDef_Methods::Transfer(CPersistContext& context)
{
    context.TransferEnum_00431102(&StackType);
    long defaultPriority = 0;
    context.Transfer("Priority", Priority, defaultPriority);
}
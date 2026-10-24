// CThing::operator new(unsigned int size, bool flag) -- allocate a thing's storage through the global operator new once the thing manager accepts the allocation (null otherwise)
// Leaf: ??2CThing@@KIPAXI_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C7380, length 48 bytes (34 + int3 padding), __fastcall (ecx = size, edx = flag), plain `ret`.
// Identity: Ego_r.pdb `??2CThing@@KIPAXI_N@Z` (protected static __fastcall operator new(unsigned int, bool)); the 22 callers
// are thing constructions (0x0052xxxx, 0x0083xxxx, 0x006AC923). The catalog "operator_new" is right but unscoped.
// Callees: the thing manager's bool(bool) query @ 0x00A36F70 (thiscall on g_pThingManager 0x013B8A1C, landed name; its
// retail body is a folded `mov al,1 ; ret 4`, so the member's own name is not recoverable from the bytes -- named by
// effect), the global operator new @ 0x00BFEB22.
// Shape: size is kept in esi across the query; the flag is pushed straight from edx.
class CThingManager {
public:
    bool CanAllocateThing(bool flag);           // 0x00A36F70 (folded `return true` body)
};
extern CThingManager* g_pThingManager;          // 0x013B8A1C

void* __cdecl operator new[](unsigned int size);  // 0x00BFEB22, the MSVCR71 operator new[] import (retail calls new[], not the scalar thunk 0x00BFEA1A)

class CThing {
protected:
    static void* __fastcall operator new(unsigned int size, bool flag);
};

void* __fastcall CThing::operator new(unsigned int size, bool flag)
{
    if (g_pThingManager->CanAllocateThing(flag))
        return ::operator new[](size);
    return 0;
}
// CInstanceRegistry::DestroyInstance(CInstance* instance) -- destroys a registered instance (its destructor @ 0x00C37DA0 unregisters it, then operator delete); nothing for null
// Leaf: ?DestroyInstance@CInstanceRegistry@@QAEXPAUCInstance@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2BAF0, 29 bytes, __thiscall (`this` unused), `ret 4`.
// Naming (sweep1, 2026-10-25): was landed as CTCMapwho::OnDie (a propagated manifest label) built as a `__stdcall`
//   global; FINDINGS chunk 436: it is CInstanceRegistry::DestroyInstance, a thiscall member (the callers load ecx with
//   the registry: the landed CInstanceRegistry::DestroyOwnerInstances 0x00C2BC10 at 0x00C2BC34, 0x00C2BB5D, and the tail
//   jumps 0x00C37A07 / 0x00C37A37). Descriptive (no PDB covers libpk). The thiscall member and the __stdcall global
//   compile to the same bytes here because `this` is never read.
// Callees: the instance's destructor @ 0x00C37DA0 (landed as the address-named ~CTCActivationTrigger_00c37da0: drains
//   its predicates and drops itself from the owner's registries), MSVCR71 operator delete (thunk 0x00BFE9BC).
// Shape: the destructor call and operator delete are written out (as the landed sibling ReleaseEntry 0x00C2A5C0 does).
void __cdecl operator delete(void* block);       // MSVCR71 import thunk 0x00BFE9BC

struct CInstance {
    ~CInstance();                                // 0x00C37DA0
};

class CInstanceRegistry {
public:
    void DestroyInstance(CInstance* instance);
};

void CInstanceRegistry::DestroyInstance(CInstance* instance)
{
    if (instance) {
        instance->~CInstance();
        operator delete(instance);
    }
}

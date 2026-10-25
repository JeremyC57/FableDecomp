// CPKObject::operator new(unsigned int size) -- the libpk class allocator: hand the size to the allocation hook (0x0139C97C) the host installs through the API 0x00C332A0 and return its block
// Leaf: ??2CPKObject@@SAPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C360, 12 bytes (+ int3 padding), `__cdecl`: `mov eax,[esp+4] ; push eax ;
//   call [hook] ; ret` (the hook is `__stdcall`: no stack adjust after the call).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442 "CTCHeroStats::InformOfEvent 0xC3C360/0xC3C370 = libpk operator
//   new/delete"): agent442's landed CEaxContext rows declare 0x00C3C360 / 0x00C3C370 as the static operator new / delete
//   of CPKObject, the library root (vtable 0x012B5868; rebuild/oracles/reloc_bindings.tsv binds ??2CPKObject@@SAPAXI@Z to
//   0x00C3C360 from 0x00C609A0), and the deleting destructors call 0x00C3C370 (CAudioPlatform 0x00C3E380, the CPKObject
//   root 0x00C3C840). The two hooks are stored side by side by the `__stdcall` API 0x00C332A0 (`mov [0x0139C97C],alloc ;
//   mov [0x0139C980],free ; ret 8`). The CTCHeroStats::InformOfEvent / CCombatActionModule names are propagated game
//   labels; descriptive names (libpk: no PDB/RTTI).
typedef void* (__stdcall* TPKAllocHook)(unsigned int size);
typedef void  (__stdcall* TPKFreeHook)(void* p);
extern TPKAllocHook g_pfnPKAlloc;                   // 0x0139C97C
extern TPKFreeHook  g_pfnPKFree;                    // 0x0139C980

class CPKObject {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject();
    static void* __cdecl operator new(unsigned int size);
    static void __cdecl operator delete(void* p);
};

void* __cdecl CPKObject::operator new(unsigned int size)
{
    return g_pfnPKAlloc(size);
}

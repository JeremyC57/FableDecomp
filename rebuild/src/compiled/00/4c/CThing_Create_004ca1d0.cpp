// CThing::Create(long defIndex, long player, long map, bool levelPersistent) -- allocate and construct a bare CThing (class type 15) and initialise it from the parameters; null when the allocation is refused or the initialisation fails
// Leaf: ?Create@CThing@@SIPAV1@JJJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CA1D0, length 96 bytes (95 + int3 padding), __fastcall (ecx = defIndex, edx = player), `ret 8`.
// Identity: Ego_r.pdb `?Create@CThing@@SIPAV1@JJJ_N@Z` (static __fastcall); 16 callers (0x006E08FE .. 0x00870BCF).
// Callees: the thing manager's bool(bool) query @ 0x00A36F70 (g_pThingManager 0x013B8A1C; the body of
// CThing::operator new(size, bool) 0x004C7380, chunk 372, inlined here with flag false), the global operator new
// @ 0x00BFEB22 (0x98 bytes = sizeof retail CThing), CThing::CThing(unsigned long) @ 0x004C9030 (landed),
// CThing::ConstructFromParams(long, long, long, bool) @ 0x004CA010 (landed, protected).
// Shape: `new (false) CThing(15)` (the class operator new inlined, the null-checked construction), then
// `if (thing && thing->ConstructFromParams(...)) return thing; return 0;` -- the failure path does not delete the thing.
void* __cdecl operator new[](unsigned int size);  // 0x00BFEB22, the MSVCR71 operator new[] import (retail calls new[], not the scalar thunk 0x00BFEA1A)

class CThingManager {
public:
    bool CanAllocateThing(bool flag);           // 0x00A36F70 (folded `return true` body; named by effect)
};
extern CThingManager* g_pThingManager;          // 0x013B8A1C

class CThing {
public:
    static CThing* __fastcall Create(long defIndex, long player, long map, bool levelPersistent);
protected:
    CThing(unsigned long classType);            // 0x004C9030
    bool ConstructFromParams(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA010
    static void* __fastcall operator new(unsigned int size, bool flag)                      // 0x004C7380
    {
        if (g_pThingManager->CanAllocateThing(flag))
            return ::operator new[](size);
        return 0;
    }
    unsigned char _body[0x98];                  // retail CThing is 0x98 bytes (layout: landed 0x004C9030)
};

CThing* __fastcall CThing::Create(long defIndex, long player, long map, bool levelPersistent)
{
    CThing* thing = new (false) CThing(15);
    if (thing && thing->ConstructFromParams(defIndex, player, map, levelPersistent))
        return thing;
    return 0;
}
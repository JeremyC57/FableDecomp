// CSoundEventManager::`scalar deleting destructor'(unsigned int flags) -- run the destructor 0x00C13470, free the object when bit 0 of the flags is set, return it
// Leaf: ?CSoundEventManager_scalar_deleting_destructor@@YIPAXPAUCSoundEventManager@@HI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C09810, 30 bytes: `push esi ; mov esi,ecx ; call 0x00C13470 ; test byte [esp+8],1 ;
//   je ; push esi ; call operator delete 0x00BFE9BC ; add esp,4 ; mov eax,esi ; pop esi ; ret 4` (no xrefs of its own).
// Identity: renamed (sweep1, 2026-10-25) from C3DMesh2::`scalar deleting destructor' (the manifest's
//   CLandscapeBackgroundPatch::`vector deleting destructor' and the catalog's C3DMesh2 are propagated): the destructor it
//   calls is CSoundEventManager::~CSoundEventManager 0x00C13470 (vtable 0x012B2748, renamed this pass; FINDINGS chunk 433).
// Shape: kept as the landed `__fastcall` stand-in with the dummy edx (this in ecx, the flags the one stack argument);
//   a genuine ??_G needs the manager's virtual destructor in a vtable slot, which retail's 0x012B2748 does not show.
struct CSoundEventManager {
    void* vftable;
};

void __fastcall CSoundEventManager_dtor(CSoundEventManager* manager);   // 0x00C13470
void operator delete(void*);

void* __fastcall CSoundEventManager_scalar_deleting_destructor(CSoundEventManager* manager, int edx_unused, unsigned int flags)
{
    CSoundEventManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}

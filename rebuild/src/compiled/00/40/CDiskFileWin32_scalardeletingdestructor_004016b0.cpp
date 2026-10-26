// CDiskFileWin32::`vector deleting destructor'(unsigned int flags) -- destroy the node and free it when bit 0 of `flags` is set
// Retail: Fable.exe (TLC, VC7.1) @ 0x004016B0, 51 bytes. Callees: Member599270, Member18Dtor, BaseDtor, operator delete.
//
// The compiler-generated deleting destructor that sits in the vtable at 0x0122D06C. It re-points the
// vtable at this class, runs the same teardown as the plain destructor at 0x00401630 (helper over the
// object, destroy the member at +0x18, base destructor) and then, when the caller passed the "delete"
// bit, returns the storage to the engine heap. `delete p` on a base pointer is what reaches this with
// flags = 1; destroying an array element reaches it with flags = 0. Returns `this` so the array form can
// step to the next element.
//
// Matching: `flags` is tested with `test [esp+..],1` after the base destructor; `edx` is the unused
// __fastcall second register so `flags` lands on the stack; the free is a plain `__cdecl` call.
class CDiskFileWin32 { public:
    void* vptr;                 // vtable pointer, reset to this class's table on entry
    char pad[0x18 - 4];         // base-class state (untouched here)
    int member18;               // owned sub-object destroyed by Member18Dtor
};

// This class's vtable (0x0122D06C).
extern const void* CDiskFileWin32_vftable;
// Helper run over the whole object before members are destroyed.
extern void __fastcall Member599270(void* win32);
// Destructor of the member sub-object at +0x18.
extern void __fastcall Member18Dtor(void* p);
// Base-class destructor.
extern void __fastcall BaseDtor_0099a300(void* win32);
// Engine heap release used by every deleting destructor.
void operator delete(void*);

void* __fastcall CDiskFileWin32_scalar_deleting_destructor(CDiskFileWin32* win32, int edx, unsigned int flags)
{
    (void)edx;
    // Virtual calls during teardown must land on this class, not a derived one.
    win32->vptr = (void*)&CDiskFileWin32_vftable;
    Member599270(win32);
    Member18Dtor((char*)win32 + 0x18);
    BaseDtor_0099a300(win32);
    // Bit 0: the caller used `delete`, so release the storage as well.
    if (flags & 1) {
        operator delete(win32);
    }
    return win32;
}

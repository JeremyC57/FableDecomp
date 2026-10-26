#pragma optimize("s",on)
// Leaf: Alloc
// Reconstruction of Alloc @ 0x00471c79
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // MSVCR71 import thunk 0x00BFEA0E

class CScriptDataBase;

// Subobject at +8 whose constructor takes the long arg (native thiscall, value-returning).
namespace N_0057a3a2 { struct SubObj {
    SubObj(long a);   // extern ctor
    int pad[6];
}; } using N_0057a3a2::SubObj;

CScriptDataBase* __stdcall Alloc(long a) {
    void* base = malloc(0x20);
    SubObj* p = (SubObj*)((char*)base + 8);
    if (p) {
        new (p) SubObj(a);   // placement construct -> native thiscall, arg on stack
    }
    return (CScriptDataBase*)base;
}
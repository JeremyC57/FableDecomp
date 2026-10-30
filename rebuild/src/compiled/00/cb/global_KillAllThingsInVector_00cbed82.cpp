// KillAllThingsInVector(CGameScriptInterfaceBase& game, const vector<CScriptThing>& things, bool fade) -- remove every living script thing in the vector through the game script interface
#pragma optimize("s", on)
// Leaf: ?KillAllThingsInVector@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBED82, 56 bytes, `__fastcall` global (interface in ecx, vector in
//   edx, the bool on the stack: `ret 4`) (PDB ?KillAllThingsInVector@@YIXAAVCGameScriptInterfaceBase@@ABV?$vector@
//   VCScriptThing@@...@@_N@Z). No frame although the bool sits on the stack: this body is built without the module's
//   `optimize("y",off)` (with it VC7.1 builds an ebp frame for the stack argument, 59 v 56); `("s",on)` alone.
// Callees: CScriptThing (vtable 0x01238C8C) slot 75 IsAlive (+0x12c); CGameScriptInterface slot 108 RemoveThing
//   (+0x1b0, 0x008910D0, ?RemoveThing@CGameScriptInterface@@UBEXABVCScriptThing@@_N1@Z).
// Shape: begin read once into esi, end re-read every pass; the interface is kept in ebx, the bool argument re-read
//   from its stack slot for every call (`push [esp+14h]`), the second RemoveThing flag is the constant true.
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
}
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"

void __fastcall KillAllThingsInVector(CGameScriptInterfaceBase& game, const std::vector<CScriptThing>& things, bool fade)
{
    for (const CScriptThing* it = things.begin(); it != things.end(); ++it)
        if (it->IsAlive())
            game.RemoveThing(*it, fade, true);
}
// NUISystem::CIME::Callback_malloc(unsigned int size) -- ImeUi allocation hook: forward the request to the CRT malloc
// Leaf: ?Callback_malloc@CIME@NUISystem@@SIPAXI@Z
// Retail: Fable.exe @ 0x00404A60, 10 bytes, static __fastcall (size in ecx): `push ecx ; call malloc ; add esp,4 ; ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?Callback_malloc@CIME@NUISystem@@SIPAXI@Z (public static __fastcall). The only
// reference is CIME::Initialise @ 0x00405650, which stores this address into the ImeUi malloc hook 0x0143E514 next to
// Callback_free 0x00404A70, Callback_DrawRect 0x00404D50, Callback_DrawFans 0x00404E10, Callback_OnChar 0x00404F00.
// Callee: malloc @ 0x00BFEA0E (MSVCR71 import thunk). `add esp,4` (not `pop ecx`) because the result is returned.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

namespace NUISystem {
class CIME {
public:
    static void* __fastcall Callback_malloc(unsigned int size);
};

void* __fastcall CIME::Callback_malloc(unsigned int size)
{
    return malloc(size);
}
}
struct CTCMapwho { int dummy; };
// Leaf: OnDie

extern "C" void __fastcall Mapwho_PreDie(CTCMapwho* self);
void __cdecl operator delete(void* block);   // MSVCR71 import thunk 0x00BFE9BC

void __fastcall OnDie(CTCMapwho* self)
{
    if (self) {
        Mapwho_PreDie(self);
        operator delete(self);
    }
}
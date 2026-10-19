struct CTCMapwho;

// thiscall method on self (ecx=this, no other args)
void __fastcall CTCMapwho_inner(CTCMapwho *self);
// cdecl free function taking self on the stack
void __cdecl operator delete(void* block);   // MSVCR71 import thunk 0x00BFE9BC

void __fastcall CTCMapwho_OnDie(CTCMapwho *self)
{
    if (self)
    {
        CTCMapwho_inner(self);
        operator delete(self);
    }
}
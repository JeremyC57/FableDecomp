struct CTCMapwho;
extern "C" void __fastcall CTCMapwho_Cleanup(CTCMapwho* self);
void __cdecl operator delete(void* block);   // MSVCR71 import thunk 0x00BFE9BC

void __fastcall CTCMapwho_OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_Cleanup(self);
        operator delete(self);
    }
}
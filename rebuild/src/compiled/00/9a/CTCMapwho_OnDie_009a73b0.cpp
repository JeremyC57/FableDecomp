class CTCMapwho;
// Leaf: OnDie

extern "C" void helper_9f9020(void);
void __cdecl operator delete(void* block);   // MSVCR71 import thunk 0x00BFE9BC

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        helper_9f9020();
        operator delete(self);
    }
}
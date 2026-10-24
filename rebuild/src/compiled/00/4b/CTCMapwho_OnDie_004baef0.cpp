struct CTCMapwho;
// Leaf: OnDie
extern "C" void __fastcall Sub_4b9b90(CTCMapwho* self);
void operator delete(void*);
void __fastcall OnDie_3(CTCMapwho* self)
{
    if (self)
    {
        Sub_4b9b90(self);
        operator delete(self);
    }
}
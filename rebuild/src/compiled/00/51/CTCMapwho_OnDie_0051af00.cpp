class CTCMapwho;
// Leaf: OnDie
extern "C" void __fastcall CTCMapwho_dtor(CTCMapwho* self);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_dtor(self);
        operator delete(self);
    }
}
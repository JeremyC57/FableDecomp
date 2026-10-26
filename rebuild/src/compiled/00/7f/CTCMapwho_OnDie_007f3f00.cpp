class CTCMapwho;
// Leaf: OnDie
extern "C" void __fastcall Mapwho_Cleanup(CTCMapwho* self);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        Mapwho_Cleanup(self);
        operator delete(self);
    }
}
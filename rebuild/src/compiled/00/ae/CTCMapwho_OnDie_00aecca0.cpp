class CTCMapwho;
// Leaf: OnDie

void __fastcall CTCMapwho_Method(CTCMapwho* self);   // 0xaecb80, thiscall

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_Method(self);
        operator delete(self);
    }
}
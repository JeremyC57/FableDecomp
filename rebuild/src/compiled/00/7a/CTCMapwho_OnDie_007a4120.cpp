class CTCMapwho;
// Leaf: OnDie

extern "C" void __fastcall Mapwho_Member(CTCMapwho* self);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self) {
        Mapwho_Member(self);
        operator delete(self);
    }
}
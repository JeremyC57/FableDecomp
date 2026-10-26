#pragma optimize("t",on)
// Leaf: OnDie

class CTCMapwho { public: int dummy; };

extern "C" void __fastcall Mapwho_Dtor(CTCMapwho* self);
void operator delete(void*);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        Mapwho_Dtor(self);
        operator delete(self);
    }
}
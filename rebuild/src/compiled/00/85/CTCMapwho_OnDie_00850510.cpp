#include <stdlib.h>

struct CTCMapwho { int dummy; };

extern "C" void __fastcall Mapwho_PreDie(CTCMapwho* self);
void operator delete(void*);

void __fastcall OnDie_Mapwho(CTCMapwho* self)
{
    if (self)
    {
        Mapwho_PreDie(self);
        operator delete(self);
    }
}
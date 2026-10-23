#include <stdlib.h>

struct CTCMapwho { int dummy; };

extern "C" void __fastcall Mapwho_PreDie_00850360(CTCMapwho* self);
void operator delete(void*);

void __fastcall OnDie_Mapwho(CTCMapwho* self)
{
    if (self)
    {
        Mapwho_PreDie_00850360(self);
        operator delete(self);
    }
}
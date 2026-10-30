
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CCoinGameObstacleDef { public:
    char pad[0x28];
    unsigned long f28;
    void __fastcall Copy(int /*edx*/, CDefClassBase const *src);
};

void __fastcall CCoinGameObstacleDef::Copy(int /*edx*/, CDefClassBase const *src)
{
    ((CDefClassBase *)this)->operator=(*(const CDefClassBase*)(src));
    this->f28 = ((CCoinGameObstacleDef const *)src)->f28;
}
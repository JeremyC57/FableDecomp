#include "engine/CGoldDef.h"

class CDefClassBase { public: char pad[0x2c]; 
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CGoldDef_Methods : CGoldDef {  };

void __fastcall Copy(CGoldDef* self, int /*edx*/, const CDefClassBase* other)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)(other));
    self->Gold = ((const CGoldDef*)other)->Gold;
}

#include "engine/CResurrectionItemDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CSubField { int v; };
class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

void __fastcall CSpecialAbilitiesDrainLifeDataDef_Copy(CResurrectionItemDef* self, int _edx, const CResurrectionItemDef* other)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)other);
    self->OnUseParticleEffect = other->OnUseParticleEffect;
}
#include "engine/CTavernDef.h"

class CDefClassBase : CTavernDef { public: CDefClassBase& operator=(const CDefClassBase& other); };

void __fastcall CTavernDef_Copy(CTavernDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)(src));
    self->BedCost = ((const CTavernDef*)src)->BedCost;
}

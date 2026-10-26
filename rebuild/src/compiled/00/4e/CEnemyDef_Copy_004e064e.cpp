#include "engine/CEnemyDef.h"

class CDefClassBase : CEnemyDef { public: CDefClassBase& operator=(const CDefClassBase& other); };

void __fastcall CEnemyDef_Copy(CEnemyDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)(src));
    self->Faction = ((const CEnemyDef*)src)->Faction;
}

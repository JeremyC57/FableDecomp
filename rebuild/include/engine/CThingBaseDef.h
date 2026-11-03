#ifndef FABLE_ENGINE_CTHINGBASEDEF_H
#define FABLE_ENGINE_CTHINGBASEDEF_H
#include "engine/CParentDefClassBase.h"
// Original Ego_r CThingBaseDef derives CParentDefClassBase; native STLport sizeof70.
// Complete original Components through DefaultScriptData range retained opaque.
class CThingBaseDef : public CParentDefClassBase {
    unsigned char _pad_0x39[0x37];
};
FABLE_STATIC_ASSERT(sizeof(CThingBaseDef)==0x70);

#endif

#ifndef FABLE_ENGINE_CHEROSUITDEF_H
#define FABLE_ENGINE_CHEROSUITDEF_H
#include "engine/CSubDefClassBase.h"
#include "engine/EClothingSuitPart.h"
#include "stlport/fable_map.h"
class CHeroSuitDef:public CSubDefClassBase { public:std::map<EClothingSuitPart,long> SuitParts; };
FABLE_STATIC_ASSERT(offsetof(CHeroSuitDef,SuitParts)==0x28);
#endif

// CThingFilter_HasBuildingAsHome::operator()(const CThingAICreature& creature) const -- whether a creature-definition word (+0xe0 -> +0xc4) equals the filter's value (a second functor of the HasBuildingAsHome shape; propagated label)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982DB0, length 23 bytes.
// Leaf: ??RCThingFilter_HasBuildingAsHome
// Callees: the creature's definition getter @ 0x00662000 (`mov eax,[ecx+0E0h] ; mov eax,[eax+0C4h]`, a CThingCreatureBase
// accessor; manifest "CPixelShader::CountReferences" is propagated).
// Identity: the catalog label is propagated (the real HasBuildingAsHome is 0x00733020); no direct callers were found, so
// the class name is kept as catalogued.
// Layout (local model): the filter's value (long) at +0.
// Shape: `this` kept in esi across the call; `return creature.f() == m_value;` as `sub eax,[esi] ; neg ; sbb ; inc`.
// Name fix (NF93, round 8, q11293): 0x00662000 is CThingCreatureBase::GetCreatureType() const (Ego_r.pdb 0x0053CE50 / FableWin.pdb 0x01D9A450
//   ?GetCreatureType@CThingCreatureBase@@QBE?AW4ECreatureType@@XZ), not a made-up CThingAICreature::GetDefinitionValue: the local
//   CThingAICreature model replaced by the shared engine/CThingAICreature.h with engine/CThingCreatureBase.h's CREATURETYPE opt-in.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"                // GetCreatureType 0x00662000 through CThingCreatureBase (CREATURETYPE opt-in)

class CThingFilter_HasBuildingAsHome { public:
    long m_value;                               // +0x00
    bool operator()(const CThingAICreature& creature) const;
};

bool CThingFilter_HasBuildingAsHome::operator()(const CThingAICreature& creature) const
{
    return creature.GetCreatureType() == m_value;
}
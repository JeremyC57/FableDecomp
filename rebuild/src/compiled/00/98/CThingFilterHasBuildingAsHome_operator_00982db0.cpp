// CThingFilter_HasBuildingAsHome::operator()(const CThingAICreature& creature) const -- whether a creature-definition word (+0xe0 -> +0xc4) equals the filter's value (a second functor of the HasBuildingAsHome shape; propagated label)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982DB0, length 23 bytes.
// Leaf: ??RCThingFilter_HasBuildingAsHome
// Callees: the creature's definition getter @ 0x00662000 (`mov eax,[ecx+0E0h] ; mov eax,[eax+0C4h]`, a CThingCreatureBase
// accessor; manifest "CPixelShader::CountReferences" is propagated).
// Identity: the catalog label is propagated (the real HasBuildingAsHome is 0x00733020); no direct callers were found, so
// the class name is kept as catalogued.
// Layout (local model): the filter's value (long) at +0.
// Shape: `this` kept in esi across the call; `return creature.f() == m_value;` as `sub eax,[esi] ; neg ; sbb ; inc`.
struct CThingAICreature {
    long GetDefinitionValue() const;            // 0x00662000
};

struct CThingFilter_HasBuildingAsHome {
    long m_value;                               // +0x00
    bool operator()(const CThingAICreature& creature) const;
};

bool CThingFilter_HasBuildingAsHome::operator()(const CThingAICreature& creature) const
{
    return creature.GetDefinitionValue() == m_value;
}
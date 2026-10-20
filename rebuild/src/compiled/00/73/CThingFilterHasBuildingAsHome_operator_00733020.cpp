// CThingFilter_HasBuildingAsHome::operator()(const CThingAICreature& creature) const -- whether the creature's home building is the filter's building
// Grade: RELOCATION_MATCH -- the call result is held in a named local and compared `home == m_building` (gives retail `cmp eax,edx`); `m_building == f()` compares the other way round and `f() == m_building` folds into sub/neg/sbb/inc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733020, length 27 bytes.
// Leaf: ??RCThingFilter_HasBuildingAsHome
// Callees: the creature's home-building getter @ 0x00810FC0 (`mov ecx,[ecx+8] ; add ecx,50h ; jmp CIntelligentPointer::Get`;
// its neighbour 0x00810FD0 reads +0x58, the work building; manifest "_Cons_val<...>" is propagated).
// Identity: Ego_r.pdb `??RCThingFilter_HasBuildingAsHome@@QBE_NABVCThingAICreature@@@Z`.
// Layout (local model): the filter's building pointer at +0.
// Shape: `return creature.GetHomeBuilding() == m_building;` through a full register (`xor ecx,ecx ; cmp eax,edx ; sete cl ;
// mov al,cl`), the building loaded after the call (`mov edx,[esi]`).
struct CThingBuilding;

struct CThingAICreature {
    CThingBuilding* GetHomeBuilding() const;    // 0x00810FC0
};

struct CThingFilter_HasBuildingAsHome {
    const CThingBuilding* m_building;           // +0x00
    bool operator()(const CThingAICreature& creature) const;
};

bool CThingFilter_HasBuildingAsHome::operator()(const CThingAICreature& creature) const
{
    const CThingBuilding* home = creature.GetHomeBuilding();
    return home == m_building;
}
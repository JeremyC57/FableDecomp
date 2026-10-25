// CTCBase::PeekWorld() const -- the world through the global thing manager (CThingManager::World, retail +0x24)
// Leaf: ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686A80, 9 bytes: mov eax,[0x013B8A1C] ; mov eax,[eax+0x24] ; ret (this unused).
// 0x013B8A1C is the CThingManager pointer: the CTCBase::GetThingManager pair 0x00686AE0 / 0x00686AF0 return it bare,
//   and its retail members sit 0xC below Ego_r (Component +0x1C, DefinitionManager +0x20, World +0x24, WorldMap +0x28,
//   WorldSeed +0x2C, PlayerManager +0x30), read by the neighbouring CTCBase accessors 0x686D20..0x686DA0.
// Label: landed as CViewBase::GetDisplayEngine (descriptive). Ego_r names two const world accessors on CTCBase,
//   ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ and ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ, with identical bodies here at 0x686A70
//   and 0x686A80. The result of 0x686A80 only ever reaches const CWorld members (PeekMyPlayer, IsCreatureGenerationEnabled,
//   ...), while 0x686A70's result reaches non-const ones (HandleMoveHeroToRegionGameEvent, TeleportHeroToHSP), so this is
//   PeekWorld. Its 340 callers are CTC* component methods.
class CWorld;

class CThingManager {
public:
    unsigned char _unknown[0x24];
    CWorld* World;                                    // +0x24
};

extern CThingManager* g_pThingManager;                // 0x013B8A1C (descriptive: no PDB name)

class CTCBase {
protected:
    const CWorld& PeekWorld() const;
};

const CWorld& CTCBase::PeekWorld() const
{
    return *g_pThingManager->World;
}

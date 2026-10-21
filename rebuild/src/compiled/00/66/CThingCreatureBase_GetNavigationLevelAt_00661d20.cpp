// CThingCreatureBase::GetNavigationLevelAt(const C3DVector&) const -- the world map's navigation layer at a position
// Leaf: GetNavigationLevelAt@CThingCreatureBase
// Retail: Fable.exe @ 0x00661D20, 22 bytes, `ret 4`. Catalog label NUISystem::CFrontEndManager::CreateComponent is wrong.
// Callers pass a thing's own position (`mov ecx,thing ; call 0x004C73D0 ; push eax ; mov ecx,thing ; call here`:
// CThingPlayerCreature 0x006AA49C, CCombatManager::GetPreferredPos 0x006EA90D, CThingAICreature 0x0083432D, AI state group
// 0x0093DB46), and CGroundNavigatorExternalInterface::GetNavigationLevelAt 0x006CDA00 is `mov ecx,[ecx+4] ; jmp here`.
// Ego_r.pdb ?GetNavigationLevelAt@CThingCreatureBase@@QBEJABVC3DVector@@@Z is the matching const member.
// 0x004C79E0 returns [g_pThingManager 0x013B8A1C]+0x28, which is CThingManager::WorldMap (Ego_r.pdb +0x34; retail -0xc, cf.
// World +0x24, PlayerManager +0x30); its manifest name CViewBase::GetDisplayEngine is ICF-propagated. 0x0063E590 takes
// (pos, thing) and matches ?GetNavigationLayerAt@CWorldMap@@QBEJABVC3DVector@@PBVCThing@@@Z (manifest agrees).
class C3DVector;
class CThing;
class CWorldMap {
public:
    long GetNavigationLayerAt(const C3DVector& pos, const CThing* ignore) const;   // 0x0063E590
};
class CThingCreatureBase {
public:
    CWorldMap& GetWorldMap() const;                      // 0x004C79E0 (out of line)
    long GetNavigationLevelAt(const C3DVector& pos) const;
};
long CThingCreatureBase::GetNavigationLevelAt(const C3DVector& pos) const
{
    return GetWorldMap().GetNavigationLayerAt(pos, 0);
}
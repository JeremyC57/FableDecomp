// CThingManager::SetUpdateAndRenderAllFlag(bool flag) -- stores the update-and-render-all flag and refreshes every entity's appearance so the render lists pick them up
// Leaf: ?SetUpdateAndRenderAllFlag@CThingManager@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051EA90, 18 bytes, thiscall `ret 4`.
// Identity (proven): Ego_r.pdb names CThingManager::SetUpdateAndRenderAllFlag(bool); the store is to +0x99, which is
//   UpdateAndRenderAllFlag in the landed retail CThingManager model (ctor 0x00523540: UseMapSpecificUIDs +0x98,
//   UpdateAndRenderAllFlag +0x99). The only caller 0x0041986B gets the thing manager through
//   g_pMainGameComponent->PWorld (+0x24)->GetThingManager() (CWorld slot 4, 0x0049E1B0 by the primary vtable 0x012390F0)
//   and passes a flag computed from editor/debug globals.
// Callee: CThingManager::FillRenderListsWithAllEntities() 0x0051EA30 (private, same TU; parked row).
class CThingManager {
public:
    unsigned char _pad00[0x99];
    bool UpdateAndRenderAllFlag;                       // +0x99
    void SetUpdateAndRenderAllFlag(bool flag);
private:
    void FillRenderListsWithAllEntities();             // 0x0051EA30
};

void CThingManager::SetUpdateAndRenderAllFlag(bool flag)
{
    UpdateAndRenderAllFlag = flag;
    FillRenderListsWithAllEntities();
}
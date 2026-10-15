// CTCSwitchableNavigation::SetAsBlockingNavigation(bool blocking) -- block or unblock the thing's switchable navigation nodes in its (activated) navigation map
// Leaf: SetAsBlockingNavigation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787580, length 65 bytes (`ret 4`; not in the vtable). Label correction: the chunk calls this InitialActivate (propagated); Ego_r has ?SetAsBlockingNavigation@CTCSwitchableNavigation@@QAEX_N@Z and the body forwards its bool unchanged.
// Callees: CTCBase::GetWorld() @ 0x00686A70 (manifest "CViewBase::GetDisplayEngine", propagated),
// CNavigatorManager::IsNavigationMapActivated(long) const @ 0x00A14EB0, CNavigatorManager::SetSwitchableNodesAsBlocked(long,
// unsigned __int64, bool) @ 0x00A15470 (Ego_r ?SetSwitchableNodesAsBlocked@CNavigatorManager@@QAEXJ_K_N@Z).
// Layout: Ego_r.pdb CTCSwitchableNavigation (0x10 bytes): MapIndex +0x0c; CWorld::PNavigationManager +0x48; CThing::UniqueID
// (ulong64) +0x18, passed masked to 40 bits (`and ecx,0FFh` on the high word).
// Shape: the navigation manager is a named local (edi) across both calls.
struct C3DVector;

struct CNavigatorManager {
    bool IsNavigationMapActivated(long mapIndex) const;     // 0x00A14EB0
    void SetSwitchableNodesAsBlocked(long mapIndex, unsigned __int64 uid, bool blocked);   // 0x00A15470
};

struct CWorld {
    unsigned char      _pad[0x48];
    CNavigatorManager* PNavigationManager;                  // +0x48 scoped_ptr<CNavigatorManager>
};

struct CThing {
    unsigned char    _pad[0x18];
    unsigned __int64 UniqueID;                              // +0x18 (only the low 40 bits identify the thing)
};

struct CTCSwitchableNavigation {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    long    MapIndex;                                       // +0x0c
    CWorld* GetWorld();                                     // CTCBase::GetWorld @ 0x00686A70
    void SetAsBlockingNavigation(bool blocking);
};

void CTCSwitchableNavigation::SetAsBlockingNavigation(bool blocking)
{
    CNavigatorManager* navigation = GetWorld()->PNavigationManager;
    if (navigation->IsNavigationMapActivated(MapIndex))
        navigation->SetSwitchableNodesAsBlocked(MapIndex, ParentThing->UniqueID & 0xFFFFFFFFFFui64, blocking);
}
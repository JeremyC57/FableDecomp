// CTCSwitchableNavigation::~CTCSwitchableNavigation() -- when its navigation map is active, un-blocks the parent thing's switchable nodes, then ~CTCBase (tail jump)
// Leaf: ??1CTCSwitchableNavigation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007876D0, length 71 bytes. Vtable 0x0123C02C (CTCSwitchableNavigation).
// Callees: CTCBase::GetWorld() @ 0x00686A70 (returns the global world's +0x24; manifest "CViewBase::GetDisplayEngine" is
// propagated, as noted in the landed 0x008EC7E0), CNavigatorManager::IsNavigationMapActivated(long) @ 0x00A14EB0,
// CNavigatorManager::SetSwitchableNodesAsBlocked(long, unsigned __int64, bool) @ 0x00A15470, CTCBase::~CTCBase @ 0x00686830.
// Ego_r.pdb CTCSwitchableNavigation (0x10): MapIndex +0x0c. The world holds the navigator manager at +0x48; the parent
// thing's 64-bit id is read from +0x18/+0x1c and masked to 40 bits (`and eax,0FFh` on the high dword).
// Shape: a user destructor (it stores its own vtable first); the manager is a named local taken right after GetWorld
// (kept in edi for both calls).
struct CWorld;

struct CThing {
    unsigned char    _pad_0x00[0x18];
    unsigned __int64 m_id;                   // +0x18
};

struct CNavigatorManager {
    bool IsNavigationMapActivated(long mapIndex);                                    // 0x00A14EB0
    void SetSwitchableNodesAsBlocked(long mapIndex, unsigned __int64 id, bool blocked);   // 0x00A15470
};

struct CWorld {
    unsigned char      _pad_0x00[0x48];
    CNavigatorManager* PNavigatorManager;    // +0x48
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                     // +0x04
    bool    ToBeRemoved;                     // +0x08
    bool    IsInGlobalUpdateTCs;             // +0x09
    CWorld* GetWorld();                      // 0x00686A70
    ~CTCBase();                              // 0x00686830
};

struct CTCSwitchableNavigation : CTCBase {   // vtable 0x0123C02C
    long MapIndex;                           // +0x0c
    virtual void v00();
    ~CTCSwitchableNavigation();
};

CTCSwitchableNavigation::~CTCSwitchableNavigation()
{
    CNavigatorManager* navigator = GetWorld()->PNavigatorManager;
    if (navigator->IsNavigationMapActivated(MapIndex))
        navigator->SetSwitchableNodesAsBlocked(MapIndex, ParentThing->m_id & 0xFFFFFFFFFFui64, false);
}

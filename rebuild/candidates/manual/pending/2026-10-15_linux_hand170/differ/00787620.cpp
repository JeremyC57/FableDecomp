// CTCSwitchableNavigation::WouldBlockPos(const C3DVector& pos) -- find the navigation node under the position on its navigation layer and ask it whether this owner's thing blocks it
// Grade: DIFFER(107v107) -- register allocation of the UID push: retail thing in edx, lo/hi in ecx/edx, node vtable in esi; ours thing in ecx, hi in esi, vtable in edx
// Leaf: WouldBlockPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787620, length 107 bytes (`ret 4`).
// Callees: CTCBase::GetWorld @ 0x00686A70 (navigator manager at CWorld+0x48), CTCBase::GetWorldMap @ 0x00686D20,
// CWorldMap::GetNavigationLayerAt(const C3DVector&, CThing*) @ 0x0063E590 (null thing), CNavigatorManager::GetNodeAt(const C2DVector&,
// unsigned char layer) @ 0x00A14F70, the node's virtual slot 10 (+0x28) taking the owner's 64-bit UID (CThing+0x18/+0x1C).
// Layout: CTCSwitchableNavigation MapIndex +0x0C (Ego_r.pdb, unused here). CThing UID __int64 at +0x18.
// Shape: the 2D position is a named local built first (`C2DVector p(pos.X, pos.Y)`), the manager fetched before the layer call (kept in
// ebx); the node's answer is returned directly.
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CThing {
    unsigned char _pad00[0x18];
    __int64 UID;                                // +0x18
};
struct CANavQuadTreeNode {
    virtual void n00(); virtual void n01(); virtual void n02(); virtual void n03(); virtual void n04();
    virtual void n05(); virtual void n06(); virtual void n07(); virtual void n08(); virtual void n09();
    virtual bool WouldBeBlockedBy(__int64 thingUID);   // slot 10 (+0x28)
};
struct CNavigatorManager {
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer);   // 0x00A14F70
};
struct CWorld {
    unsigned char _pad00[0x48];
    CNavigatorManager* NavigatorManager;        // +0x48
};
struct CWorldMap {
    long GetNavigationLayerAt(const C3DVector& pos, CThing* thing);   // 0x0063E590
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CWorld* GetWorld();                         // 0x00686A70
    CWorldMap* GetWorldMap();                   // 0x00686D20
};
struct CTCSwitchableNavigation : CTCBase {
    long MapIndex;                              // +0x0c
    bool WouldBlockPos(const C3DVector& pos);
};

bool CTCSwitchableNavigation::WouldBlockPos(const C3DVector& pos)
{
    C2DVector p(pos.X, pos.Y);
    CNavigatorManager* navigator = GetWorld()->NavigatorManager;
    CANavQuadTreeNode* node = navigator->GetNodeAt(p, (unsigned char)GetWorldMap()->GetNavigationLayerAt(pos, 0));
    if (node)
        return node->WouldBeBlockedBy(ParentThing->UID);
    return false;
}

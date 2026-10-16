// CTCShop::IsShopKeeperNearerThan(float distance) -- TRUE when both the shop keeper and the player's controlled hero are alive and the keeper lies within `distance` of the hero on the XY plane
// Leaf: IsShopKeeperNearerThan
// Grade: DIFFER(153v153) -- register choice only plus x87 term order: retail keeps the hero in edi and the keeper in ebp (pushed in the block), ours swaps them; retail squares dx first (`fld st(1) ; fmul st(2)` twice), ours dy first (both sum orders tried); lane gap: x87 sum-of-squares term order
// Retail: Fable.exe 0x007723F0, 151 bytes (`ret 4`, BOOL result).
// Callees: CTCBase::GetPlayerManager() @ 0x00686D50 (manifest "CViewBase::GetDisplayEngine" is propagated),
// CPlayerManager::GetMainPlayer() @ 0x00449960 (manifest "Catch@0040d959" is wrong), CPlayer::GetControlledCreature()
// @ 0x00487DD0 (manifest "_Cons_val<...>" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3),
// CThing::GetPosition() const @ 0x004C73D0 (hero first, then keeper).
// Layout: retail CTCShop: PShopKeeper (CIntelligentPointer<CThingCreatureBase>) +0x10; CThing dead flag +0x91 bit 0.
// Shape: IS_THING_ALIVE inlined (two GetPItem calls) for the keeper, a plain null/dead test for the hero; the squared
// XY distance is compared against distance*distance (`fcompp ; test ah,41h ; jne`).
struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +4
    CThing* GetPItem() const;                       // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
// XY squared distance between two things (inline; arguments evaluate right to left, so the keeper pointer is
// fetched before the hero's position).
__forceinline float Distance2DSquared(const CThing* a, const CThing* b)
{
    const C3DVector& pa = a->GetPosition();
    const C3DVector& pb = b->GetPosition();
    float dx = pb.X - pa.X;
    float dy = pb.Y - pa.Y;
    return dx * dx + dy * dy;
}
struct CPlayer {
    CThingCreatureBase* GetControlledCreature();    // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                       // 0x00449960
};
struct CTCShop {
    virtual void v00();
    void*                   ParentThing;            // +0x04 (CTCBase)
    bool                    ToBeRemoved;            // +0x08
    bool                    IsInGlobalUpdateTCs;    // +0x09
    long                    StockState;             // +0x0c
    CBaseIntelligentPointer PShopKeeper;            // +0x10
    CPlayerManager* GetPlayerManager() const;       // 0x00686D50 (CTCBase)
    long IsShopKeeperNearerThan(float distance) const;
};

long CTCShop::IsShopKeeperNearerThan(float distance) const
{
    CThingCreatureBase* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
    if (IS_THING_ALIVE(PShopKeeper) && hero && !(hero->m_flags91 & 1)) {
        if (Distance2DSquared(hero, PShopKeeper.GetPItem()) < distance * distance)
            return 1;
    }
    return 0;
}

// Grade: DIFFER(688v680): gold check via bool GetInterface (690->688); residue is allocation -- ours CSEs &m_tcMask (lea +0x20 spilled) where retail re-tests [ebx+0x20] per HasTC, and keeps &Podium in ebp
// CTCBoastingPosition::FrameUpdate() -- while active with a living podium: find the nearest hero-type thing (interface 0x29); inside the position's radius and nearer the podium than the position itself, tell the podium the hero entered the boast area (once; with whether he can afford the position's gold) and notify his interface 8; otherwise tell the podium he left; then mark the parent updated and refresh its appearance
// Leaf: FrameUpdate
// Retail 0x007820E0, 688 bytes, `ret`. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0,
// CTCBase thing-search-tools getter 0x00686D80, CThingSearchTools::GetPBestThing<CThingFilter_HasTC, CThingCompare_Nearest>
// 0x00782670, LowerBound const 0x004365B0 (interface 4) / 0x0040F020 (0x70, 8), CTCBoastingPodium::OnHeroEnterBoastArea
// 0x008139C0 / OnHeroLeaveBoastArea 0x008139B0, interface 8's slot 34 (+0x88), CThing::SetAsFrameUpdated 0x004C7C40 /
// UpdateGraphicAppearance 0x004C7590. Global: boast radius squared 0x0125B2D4.
// Layout: CTCBoastingPosition Podium (CIntelligentPointer) +0x0c, Centre +0x14, Active +0x20, PositionIndex +0x24,
// RequiredGold +0x28, DetectHero +0x2c, HeroInArea +0x2d; hero gold TC (4) +0x3c.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline float GetLengthSquaredXY() const { return Y * Y + X * X; }
    __forceinline float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    __forceinline float GetLengthSquared() const { return Y * Y + X * X; }
};
extern const float g_BoastRadiusSquared;               // 0x0125B2D4
struct CThing;
struct CTCHeroStats {
    unsigned char _pad00[0x3c];
    long          Gold;                                 // +0x3c
};
struct CTCBoastingPodium {
    void OnHeroEnterBoastArea(long position, bool canAfford, CThing* hero);   // 0x008139C0
    void OnHeroLeaveBoastArea(long position, bool canAfford, CThing* hero);   // 0x008139B0
};
struct CTCHeroBoast {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void OnEnterBoastPosition(CThing* position);   // slot 34 (+0x88)
};
struct STCEntry { long type; void* component; };
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);                // 0x0040F020
    const STCEntry* LowerBound(const unsigned long& kind) const;    // 0x004365B0
    __forceinline void* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->type) ? m_pLast : it)->component;
    }
    __forceinline void* Find(long kind) const
    {
        const STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->type) ? m_pLast : it)->component;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                          // +0x20
    CTCMap        m_tcs;                                // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                             // +0x91 bit 0
    unsigned char _flags91 : 7;
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void SetAsFrameUpdated();                           // 0x004C7C40
    void UpdateGraphicAppearance(long reason);          // 0x004C7590
    __forceinline bool IsDead() const { return Dead; }
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline void* GetTC(long kind) { return m_tcs.Find(kind); }
    __forceinline const void* GetTC(long kind) const { return m_tcs.Find(kind); }
    template<class T> __forceinline bool GetInterface(T** out, long kind) { if (HasTC(kind)) { *out = (T*)GetTC(kind); return true; } return false; }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* PItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CThingFilter_HasTC {
    long TC;
    CThingFilter_HasTC(long tc) : TC(tc) {}
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
};
struct CThingSearchTools {
    bool GetPBestThing(const CThingFilter_HasTC& filter, const CThingCompare_Nearest& compare, CThing** out);   // 0x00782670
};
struct CTCBoastingPosition {
    void*                   __vftable;              // +0x00
    CThing*                 ParentThing;            // +0x04
    CThing& GetParentThing() { return *ParentThing; }   // Ego_r ?GetParentThing@CTCBase@@QAEAAVCThing@@XZ (inline)
    const CThing& GetParentThing() const { return *ParentThing; }   // Ego_r ?GetParentThing@CTCBase@@QBEABVCThing@@XZ (inline)
    unsigned char           _pad08[0x4];
    CBaseIntelligentPointer Podium;                 // +0x0c
    C3DVector               Centre;                 // +0x14
    bool                    Active;                 // +0x20
    long                    PositionIndex;          // +0x24
    long                    RequiredGold;           // +0x28
    bool                    DetectHero;             // +0x2c
    bool                    HeroInArea;             // +0x2d
    CThingSearchTools* GetThingSearchTools();       // 0x00686D80
    void FrameUpdate();
};

void CTCBoastingPosition::FrameUpdate()
{
    if (Podium.GetPItem() && !Podium.GetPItem()->IsDead() && Active) {
        CThing* found;
        GetThingSearchTools()->GetPBestThing(CThingFilter_HasTC(0x29), CThingCompare_Nearest(ParentThing->GetPosition()), &found);
        CThing* hero = found;
        if (hero && !hero->IsDead() && DetectHero) {
            CTCHeroStats* stats;
            bool canAfford = true;
            if (hero->GetInterface(&stats, 4))
                canAfford = stats->Gold >= RequiredGold;
            if ((hero->GetPosition() - Centre).GetLengthSquared() < g_BoastRadiusSquared) {
                C3DVector heroOffset = hero->GetPosition() - Podium.GetPItem()->GetPosition();
                C3DVector myOffset = ParentThing->GetPosition() - Podium.GetPItem()->GetPosition();
                if (heroOffset.GetLengthSquaredXY() < myOffset.GetLengthSquaredXY()) {
                    if (!HeroInArea) {
                        CTCBoastingPodium* podium;
                        Podium.GetPItem()->GetInterface(&podium, 0x70);
                        podium->OnHeroEnterBoastArea(PositionIndex, canAfford, hero);
                        HeroInArea = true;
                    }
                    if (hero->HasTC(8))
                        ((CTCHeroBoast*)hero->GetTC(8))->OnEnterBoastPosition(&GetParentThing());
                    goto done;
                }
            }
            if (HeroInArea) {
                CTCBoastingPodium* podium;
                Podium.GetPItem()->GetInterface(&podium, 0x70);
                podium->OnHeroLeaveBoastArea(PositionIndex, canAfford, hero);
                HeroInArea = false;
            }
        }
    }
done:
    ParentThing->SetAsFrameUpdated();
    ParentThing->UpdateGraphicAppearance(4);
}

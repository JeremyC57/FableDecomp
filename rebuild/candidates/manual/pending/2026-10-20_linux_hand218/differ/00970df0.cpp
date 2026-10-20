// CAIStateGroup_CreatureUseCoverBase::GetCoverThingList(vector<CThing*>& list) -- with a live target, gather the things within 15 units of the creature that can hide it from the target
// Grade: DIFFER(ours v retail) -- frame 0x58 v 0x50: retail reuses the away-vector temporary's slot for the Bind2 pointer pair, ours gives the pair its own 8 bytes (all offsets +8); code otherwise identical
// Retail: Fable.exe (TLC, VC7.1) @ 0x00970DF0, 307 bytes (virtual, `ret 4`).
// Ego_r.pdb: ?GetCoverThingList@CAIStateGroup_CreatureUseCoverBase@@MAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0, C2DBoxF -> C2DBoxI truncation
// 0x00A55A30 (manifest "CEditWorldMap::GetPenArea" is propagated), PeekThingSearchTools 0x008FD000, the search tool's
// GetAllThingsInRoughArea<CThing, vector<CThing*>, CFilter_Bind2<..> > 0x00971530; 15.0f pool 0x01297498.
// Shape: IS_THING_ALIVE(PTarget); the creature's XY copied first; the away-from-target direction member by member;
// the filter is `CThingFilter_NotSelf() && CThingFilter_IsCoverFrom(targetPos, away)` (a CFilter_Bind2 of two pointers,
// the empty filter in a frame byte), the area a C2DBoxF(+-15) truncated to a C2DBoxI.
// Leaf: GetCoverThingList@CAIStateGroup_CreatureUseCoverBase
struct CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct C2DVector {
    float X, Y;
};
struct C2DBoxI { long MinX, MinY, MaxX, MaxY; };
struct C2DBoxF {
    float MinX, MinY, MaxX, MaxY;
    C2DBoxF(float minX, float minY, float maxX, float maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
    C2DBoxI ToBoxI() const;                        // 0x00A55A30
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                               // +0x04
    CThing* GetPItem() const;                      // 0x00A01B50
};
#pragma pack(pop)

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                       // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;          // 0x004C73D0
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CThingFilter_NotSelf { CThingFilter_NotSelf() {} };
struct CThingFilter_IsCoverFrom {
    C3DVector TargetPos;
    C3DVector Away;
    CThingFilter_IsCoverFrom(const C3DVector& pos, C3DVector away) : TargetPos(pos), Away(away) {}
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }

struct CThingVector;
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind2<CThingFilter_NotSelf, CThingFilter_IsCoverFrom>& filter, CThingVector& out);   // 0x00971530
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                             // +0x18
};

#pragma pack(push, 1)
struct CAIStateGroup_CreatureUseCoverBase {
    virtual ~CAIStateGroup_CreatureUseCoverBase();
    CAIBrain* PBrain;                              // +0x04
    unsigned char _pad08[0x0c];                    // +0x08
    CBaseIntelligentPointer PTarget;               // +0x14
    CThingSearchTools* PeekThingSearchTools();     // 0x008FD000
    virtual void GetCoverThingList(CThingVector& list);
};
#pragma pack(pop)

void CAIStateGroup_CreatureUseCoverBase::GetCoverThingList(CThingVector& list)
{
    if (!IS_THING_ALIVE(PTarget))
        return;
    C2DVector centre = *(const C2DVector*)&PBrain->PCreature->GetPosition();
    CThing* creature = PBrain->PCreature;
    PeekThingSearchTools()->GetAllThingsInRoughArea(
        C2DBoxF(centre.X - 15.0f, centre.Y - 15.0f, centre.X + 15.0f, centre.Y + 15.0f).ToBoxI(),
        CThingFilter_NotSelf() && CThingFilter_IsCoverFrom(PTarget.GetPItem()->GetPosition(),
            creature->GetPosition() - PTarget.GetPItem()->GetPosition()), list);
}

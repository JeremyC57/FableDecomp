// Grade: DIFFER(872v843): flying arm matches; take-off arm differs -- retail CSEs the three squares of the hero distance into frame temps reused by the inline fast normalise and keeps the normalised forward on the x87 stack across the join into the CRightHandedSet / cross product; ours recomputes and spills
// CTCAmbientBird::FrameUpdate() -- a resting bird takes off when the hero comes within 5.6 units: it plays its fly animation and turns away from the hero (pitched up about the side axis); a flying bird climbs along its heading, staying on valid world-map ground, and is killed once it is 40 units above its take-off height
// Leaf: FrameUpdate
// Retail 0x007E1FC0, 843 bytes, `ret`. Callees: CThing::GetPosition 0x004C73D0, CThing::Kill(bool) 0x004C9B80, the
// placement object's (CThing +0x60) virtual slots 70 (+0x118, heading by hidden pointer), 30 (+0x78, SetPosition), 75
// (+0x12C, SetOrientation) and 38 (+0x98), CTCBase world-map getter 0x00686D30 (virtual slots 16 +0x40 / 10 +0x28),
// CTCBase::GetPlayerManager 0x00686D50, CPlayerManager::GetMainPlayer 0x00449960, CPlayer creature getter 0x00487DD0,
// LowerBound 0x0040F020, CCharString 0x0099EBF0 / dtor 0x0099EAE0, CTCAnimatingObject::PlayAnimation 0x007F90B0,
// CRightHandedSet::RotateAboutAxis 0x00A22F80. Globals: 40.0f 0x012700EC, 0.8f 0x012700E8, 31.36f 0x012700FC, epsilon
// 0x0129BA3C, the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0/1 0x0122DEDC/D8.
// Layout: CTCAmbientBird Flying +0x0c, TakeOffZ +0x10; CThing dead flag +0x91 bit 0, component bitset +0x20, map +0x44.
#include <math.h>
extern unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558
extern float g_SmallEpsilon;                               // 0x0129BA3C
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    __forceinline C3DVector GetSafeNormalised() const
    {
        float lengthSquared = X * X + Y * Y + Z * Z;
        if (lengthSquared > g_SmallEpsilon * g_SmallEpsilon) {
            unsigned long bits = *(unsigned long*)&lengthSquared;
            unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                     (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
            float r = *(float*)&estimate;
            r = (3.0f - r * r * lengthSquared) * r * 0.5f;
            return C3DVector(X * r, Y * r, Z * r);
        }
        return C3DVector(0.0f, 1.0f, 0.0f);
    }
};
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    __forceinline CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
    void RotateAboutAxis(const C3DVector& axis, float angle);   // 0x00A22F80
};
struct CThingPlacement {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);        // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37();
    virtual void SetPhysics(bool on);                           // slot 38 (+0x98)
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69();
    virtual C3DVector GetHeading() const;                       // slot 70 (+0x118)
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void SetOrientation(const CRightHandedSet& rhs);    // slot 75 (+0x12c)
};
struct CTCAnimatingObject {
    void PlayAnimation(const CCharString& name, bool a, bool b);   // 0x007F90B0
};
struct STCEntry { long type; CTCAnimatingObject* component; };
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);    // 0x0040F020
    __forceinline CTCAnimatingObject* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->type) ? m_pLast : it)->component;
    }
};
struct CThing {
    unsigned char    _pad00[0x20];
    unsigned long    m_tcMask[9];                       // +0x20
    CTCMap           m_tcs;                             // +0x44
    unsigned char    _pad50[0x60 - 0x50];
    CThingPlacement* Placement;                         // +0x60
    unsigned char    _pad64[0x91 - 0x64];
    unsigned char    m_stateFlags;                      // +0x91 (bit 0: dead)
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCAnimatingObject* GetTC(long kind) { return m_tcs.Find(kind); }
    bool IsDead() const { return (m_stateFlags & 1) != 0; }
};
struct CPlayer {
    CThing* GetCharacterThing();                        // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449960
};
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsValidSection(void* section);         // slot 10 (+0x28)
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void* GetSection(const C3DVector& position);   // slot 16 (+0x40)
};
struct CTCAmbientBird {
    void*         __vftable;                            // +0x00
    CThing*       ParentThing;                          // +0x04
    unsigned char _pad08[0x4];
    bool          Flying;                               // +0x0c
    float         TakeOffZ;                             // +0x10
    CWorldMap* GetWorldMap();                           // 0x00686D30
    CPlayerManager* GetPlayerManager();                 // 0x00686D50
    void FrameUpdate();
};

void CTCAmbientBird::FrameUpdate()
{
    if (Flying) {
        if (ParentThing->GetPosition().Z - TakeOffZ > 40.0f) {
            ParentThing->Kill(false);
            return;
        }
        C3DVector offset = ParentThing->Placement->GetHeading() * 0.8f;
        C3DVector target = offset + ParentThing->GetPosition();
        void* section = GetWorldMap()->GetSection(target);
        if (!GetWorldMap()->IsValidSection(section)) {
            const C3DVector& pos = ParentThing->GetPosition();
            target = C3DVector(pos.X, pos.Y, pos.Z + 0.8f);
        }
        ParentThing->Placement->SetPosition(target);
        return;
    }
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetCharacterThing();
    if (!hero || hero->IsDead())
        return;
    C3DVector away = ParentThing->GetPosition() - hero->GetPosition();
    float distanceSquared = away.GetLengthSquared();
    if (distanceSquared < 31.36f) {
        Flying = true;
        TakeOffZ = ParentThing->GetPosition().Z;
        if (ParentThing->HasTC(0x83))
            ParentThing->GetTC(0x83)->PlayAnimation("ST_FLY", true, true);
        C3DVector forward = away.GetSafeNormalised();
        C3DVector up(0.0f, 0.0f, 1.0f);
        CRightHandedSet rhs(up, forward);
        rhs.RotateAboutAxis(CrossProduct(forward, up), -0.17777778f);
        ParentThing->Placement->SetOrientation(rhs);
        ParentThing->Placement->SetPhysics(false);
    }
}

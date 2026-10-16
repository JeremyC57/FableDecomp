// CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity) -- ask the active navigator whether the route is possible, telling it whether the start is the creature's own position
// Grade: DIFFER(114v114) -- only the squared-distance term order: retail squares dz, dx, dy (`fld st(0);fmul st(1);fld st(3);fmul st(4);faddp;fld st(2);fmul st(3)`), ours dz, dy, dx; the float-local sum canonicalises the same for every operand order, (x+y)+z / x+(y+z) tried; a C3DVector temporary from operator- spills to the frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CB620, length 114 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0, CNavigatorBase slot 9 (+0x24).
// Shape: the bool local is set true first and cleared when the start is farther than the tolerance from the parent's position
// (`fcompp ; test ah,5 ; jp`); the difference vector stays on the x87 stack.
// Layout: Ego_r.pdb CTCCreatureNavigation: PNavigator +0xC (the active CNavigatorBase). Retail CThing::GetPosition @ 0x004C73D0.
// Globals: the 1e-4f tolerance @ 0x0129BA3C is an extern float variable (`fld [g] ; fmul [g]` squares it).
struct C3DVector {
    float X, Y, Z;
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector r;
        r.X = X - o.X;
        r.Y = Y - o.Y;
        r.Z = Z - o.Z;
        return r;
    }
};
inline float DistanceSquared(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    return dx * dx + (dy * dy + dz * dz);
}
inline float XYDistanceSquared(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}
extern float g_fTolerance;                      // 0x0129BA3C (1e-4f)
struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CWorldMap {
    bool IsPosInMap(const C3DVector& pos);      // 0x004FC600
};
struct CNavigatorBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual C3DVector GetRandomPosition(const C3DVector& centre, float radius, bool flee);     // slot 7 (+0x1c)
    virtual void v08();
    virtual bool IsAbleToNavigate(const C3DVector& from, const C3DVector& to, float proximity, bool fromCurrentPos);   // slot 9 (+0x24)
};
struct CTCCreatureNavigation {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CNavigatorBase* PNavigator;                 // +0x0c
    CWorldMap* GetWorldMap() const;             // 0x00686D20 (CTCBase; reads a global, the manifest's "GetDisplayEngine" is propagated)
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity);
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool flag);
    bool GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result);
};

bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity)
{
    bool fromCurrentPos = true;
    if (DistanceSquared(from, ParentThing->GetPosition()) > g_fTolerance * g_fTolerance)
        fromCurrentPos = false;
    return PNavigator->IsAbleToNavigate(from, to, proximity, fromCurrentPos);
}

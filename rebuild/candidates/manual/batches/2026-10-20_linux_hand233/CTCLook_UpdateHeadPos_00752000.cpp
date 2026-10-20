// CTCLook::UpdateHeadPos() -- refresh the cached head position from the parent creature, except while the cache timer runs and the thing is more than 8 units from the camera; restart the timer when the head barely moved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00752000, length 280 bytes (+ int3 padding), plain `ret`.
// Callees: CTCBase::GetWorld() @ 0x00686A70, CThing::GetPosition() const @ 0x004C73D0, the world's virtual slot 8 (+0x20,
// a C3DVector by hidden pointer: the camera position; name descriptive), CTCLook::ShouldGetExactHeadPos() @ 0x00751BC0
// (name descriptive: the bool handed to the creature), CThingCreatureBase::GetHeadPos(bool) @ 0x00661B40.
// Globals: 64.0f 0x0126973C (8 units squared), the .data tunables 0x013826A0 (float tolerance) and 0x01375550 (long frame
// count, halved with the signed cdq / sar idiom).
// Shape: world and thing position fetched into named locals before the camera call (retail calls GetWorld first); the
// differences go through C3DVector's inline operator- (ctor from components, kept on the x87 stack) and a single-expression
// MagnitudeSquared (VC7.1 orders the squares by stack position: ZZ, XX, YY for the camera test, ZZ, YY, XX for the head
// test); the early-out `--HeadCacheTimer; return;` has its own epilogue; AlwaysUpdateHeadPos skips the null test.
// Layout (retail CTCLook, packed past +0xa0 as in the landed GetHeadPos 0x00751B30): ParentThing +0x04, PParentCreature
// +0x10, PDef +0x14 (CLookDef AlwaysUpdateHeadPos +0x54), CachedHeadPos +0xa1, HeadCacheTimer +0xb0. No engine header.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float MagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetCameraPosition();     // slot 8 (+0x20)
};
struct CThing {
    const C3DVector& GetPosition() const;      // 0x004C73D0
};
struct CThingCreatureBase {
    C3DVector GetHeadPos(bool exact);          // 0x00661B40
};
struct CLookDef {
    unsigned char _pad00[0x54];
    bool AlwaysUpdateHeadPos;                  // +0x54
};
template<class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
extern float g_HeadPosCacheTolerance;          // 0x013826A0 (.data tunable, 0.0316)
extern long  g_HeadPosCacheFrames;             // 0x01375550 (.data, 15)

#pragma pack(push, 1)
struct CTCLook {
    void*               __vftable;              // +0x00
    CThing*             ParentThing;            // +0x04
    unsigned char       _pad08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;        // +0x10
    CDefPointer<const CLookDef> PDef;           // +0x14
    unsigned char       _pad18[0xa0 - 0x18];
    bool                TargetAcquired;         // +0xa0 (PDB +0xa4)
    C3DVector           CachedHeadPos;          // +0xa1 (PDB +0xa5)
    unsigned char       _padAD[0xb0 - 0xad];
    long                HeadCacheTimer;         // +0xb0 (PDB +0xb4)
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    bool ShouldGetExactHeadPos();               // 0x00751BC0
    void UpdateHeadPos();
};
#pragma pack(pop)

void CTCLook::UpdateHeadPos()
{
    if (!PDef->AlwaysUpdateHeadPos) {
        CWorld* world = GetWorld();
        const C3DVector& thingPos = ParentThing->GetPosition();
        C3DVector toCamera = world->GetCameraPosition() - thingPos;
        if (HeadCacheTimer > 0 && toCamera.MagnitudeSquared() > 64.0f) {
            --HeadCacheTimer;
            return;
        }
        if (PParentCreature == 0)
            return;
    }
    C3DVector head = PParentCreature->GetHeadPos(ShouldGetExactHeadPos());
    if ((CachedHeadPos - head).MagnitudeSquared() > g_HeadPosCacheTolerance)
        HeadCacheTimer = 0;
    else
        HeadCacheTimer = g_HeadPosCacheFrames / 2;
    CachedHeadPos = head;
}

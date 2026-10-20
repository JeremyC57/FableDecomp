// CTCDoor::UpdateOpenStatusTriggerTypePerson() -- a person-triggered door is open while a creature stands within the trigger radius (and within one unit of height); it closes one second after the last creature was seen; open/close the door when the state changed
// Grade: DIFFER(293v292) -- box/filter construction: ours spends one more frame word (0x48 v 0x44), computes maxX through fxch where retail does `fld st0 ; fadd st3` (radius + X) and stores the 1.0 z-range late; the search call, the frame tests and the open/close tail match (by-value C2DVector with a user copy ctor fixed the x/y FPU loads)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730F90, 292 bytes (+ int3 padding), plain `ret`.
// Callees: C2DBox::ToInteger @ 0x00A55A30 (manifest "CEditWorldMap::GetPenArea" is propagated), CTCBase::GetThingSearchTools
//   @ 0x00686D90, CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind2<CThingFilter_InArea, CThingFilter_IsWithinZRange>,
//   CThingCreatureBase> @ 0x007313B0, CTCBase::GetWorld const @ 0x00686A80, GFGetCurrentFrame @ 0x0049D870 (__fastcall),
//   CTCDoor::Open(bool) @ 0x00730C00, CTCDoor::Close @ 0x00730DD0.
// Globals: g_framesPerSecond @ 0x01375550.
// Layout: engine/CTCDoor.h (retail offsets): CurrentOpen +0x10, OldOpen +0x11, LastCreatureFoundWF +0x14, PDef +0x20;
//   CDoorDef TriggerRadius +0x2c; the parent's physics TC +0x60 with its position at +0x0c.
// Shape (approximate, parked): the area box from the 2D position and the radius, the in-area filter holding a copy of it,
//   the z-range filter (z, 1.0), both bound by reference; the frame read through an inline world accessor.
extern long g_framesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();            // 0x0049D870
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& p) : X(p.X), Y(p.Y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};
struct C2DBoxI { long minX, minY, maxX, maxY; };
struct C2DBox {
    float minX, minY, maxX, maxY;
    C2DBox(C2DVector p, float radius) { minX = p.X - radius; minY = p.Y - radius; maxX = p.X + radius; maxY = p.Y + radius; }
    C2DBoxI ToInteger() const;                  // 0x00A55A30
};
struct CThingFilter_InArea {
    C2DBox Area;
    CThingFilter_InArea(const C2DBox& area) : Area(area) { }
};
struct CThingFilter_IsWithinZRange {
    float Z;
    float Range;
    CThingFilter_IsWithinZRange(float z, float range) : Z(z), Range(range) { }
};
struct CInAreaZRangeFilter {                    // CFilter_Bind2<CThingFilter_InArea, CThingFilter_IsWithinZRange>
    const CThingFilter_InArea* First;
    const CThingFilter_IsWithinZRange* Second;
    CInAreaZRangeFilter(const CThingFilter_InArea& a, const CThingFilter_IsWithinZRange& b) : First(&a), Second(&b) { }
};
struct CThingCreatureBase;
struct CThingSearchTools {
    bool GetPFirstThingInRoughArea(const C2DBoxI& box, const CInAreaZRangeFilter& filter, CThingCreatureBase** out);   // 0x007313B0
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector Position;                         // +0x0c
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
struct CWorld {
    long GetFrame() const { return GFGetCurrentFrame(); }
};
struct CDoorDef {
    unsigned char _pad00[0x2c];
    float TriggerRadius;                        // +0x2c
};
struct CTCDoor {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    bool CurrentOpen;                           // +0x10
    bool OldOpen;                               // +0x11
    bool Opening;                               // +0x12
    bool Closing;                               // +0x13
    long LastCreatureFoundWF;                   // +0x14
    long UpdateID;                              // +0x18
    long TriggerType;                           // +0x1c
    const CDoorDef* PDef;                       // +0x20
    CThingSearchTools* GetThingSearchTools() const; // 0x00686D90 (CTCBase)
    const CWorld* GetWorld() const;             // 0x00686A80 (CTCBase)
    void Open(bool instant);                    // 0x00730C00
    void Close();                               // 0x00730DD0
    void UpdateOpenStatusTriggerTypePerson();
};

void CTCDoor::UpdateOpenStatusTriggerTypePerson()
{
    const C3DVector& pos = ParentThing->PhysicsTC->Position;
    C2DBox box(C2DVector(pos), PDef->TriggerRadius);
    CThingCreatureBase* creature = 0;
    CThingFilter_IsWithinZRange zRange(pos.Z, 1.0f);
    CThingFilter_InArea inArea(box);
    GetThingSearchTools()->GetPFirstThingInRoughArea(box.ToInteger(), CInAreaZRangeFilter(inArea, zRange), &creature);
    if (creature) {
        CurrentOpen = true;
        LastCreatureFoundWF = GetWorld()->GetFrame();
    } else {
        long framesPerSecond = g_framesPerSecond;
        long elapsed = GetWorld()->GetFrame() - LastCreatureFoundWF;
        if ((float)elapsed > (float)framesPerSecond)
            CurrentOpen = false;
    }
    if (CurrentOpen != OldOpen) {
        if (CurrentOpen)
            Open(true);
        else
            Close();
        OldOpen = CurrentOpen;
    }
}

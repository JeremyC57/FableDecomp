// Behaviour test for 00730f90: src/00730f90.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCDoor::UpdateOpenStatusTriggerTypePerson()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCDoor::UpdateOpenStatusTriggerTypePerson() -- a person-triggered door is open while a creature stands within the trigger radius (and within one unit of height); it closes one second after the last creature was seen; open/close the door when the state changed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730F90, 292 bytes (+ int3 padding), plain `ret`.
// Callees: C2DBox::ToInteger @ 0x00A55A30 (manifest "CEditWorldMap::GetPenArea" is propagated), CTCBase::GetThingSearchTools
//   @ 0x00686D90, CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind2<CThingFilter_InArea, CThingFilter_IsWithinZRange>,
//   CThingCreatureBase> @ 0x007313B0, CTCBase::GetWorld const @ 0x00686A80, GFGetCurrentFrame @ 0x0049D870 (__fastcall),
//   CTCDoor::Open(bool) @ 0x00730C00, CTCDoor::Close @ 0x00730DD0.
// Globals: g_framesPerSecond @ 0x01375550.
// Layout: engine/CTCDoor.h (retail offsets): CurrentOpen +0x10, OldOpen +0x11, LastCreatureFoundWF +0x14, PDef +0x20;
//   CDoorDef TriggerRadius +0x2c; the parent's physics TC +0x60 with its position at +0x0c.
// Shape: the 2D centre is a named vector and the box is built from its X/Y and the radius (keeps X and Y live on the
//   x87 stack: `fld st0 ; fadd st3`); C2DBox has a user copy ctor (minY copied at once, maxY stored from the FPU); both
//   filters are argument temporaries of the Bind2 filter; the out pointer lives in an inner block, so the else arm reuses
//   its slot for the elapsed-frames temp; the world pointer is read before the frames-per-second copy.
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
    C2DBox(float x, float y, float radius) { minX = x - radius; minY = y - radius; maxX = x + radius; maxY = y + radius; }
    C2DBox(const C2DBox& b) : minX(b.minX), minY(b.minY), maxX(b.maxX), maxY(b.maxY) { }
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
    C2DVector centre(pos);
    C2DBox box(centre.X, centre.Y, PDef->TriggerRadius);
    CThingCreatureBase* found;
    {
        CThingCreatureBase* creature = 0;
        GetThingSearchTools()->GetPFirstThingInRoughArea(box.ToInteger(), CInAreaZRangeFilter(CThingFilter_InArea(box), CThingFilter_IsWithinZRange(pos.Z, 1.0f)), &creature);
        found = creature;
    }
    if (found) {
        CurrentOpen = true;
        LastCreatureFoundWF = GetWorld()->GetFrame();
    } else {
        const CWorld* world = GetWorld();
        long framesPerSecond = g_framesPerSecond;
        long elapsed = world->GetFrame() - LastCreatureFoundWF;
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

// ---- stubs and driver ----
long g_framesPerSecond = 30;
static long g_frame = 0;
long __fastcall GFGetCurrentFrame() { return g_frame; }
static C2DBox g_toIntBox(0.0f, 0.0f, 0.0f);
C2DBoxI C2DBox::ToInteger() const
{
    g_toIntBox = *this;
    C2DBoxI r; r.minX = (long)minX; r.minY = (long)minY; r.maxX = (long)maxX; r.maxY = (long)maxY;
    return r;
}
static CThingSearchTools g_tools; static CWorld g_world;
static C2DBoxI g_searchBox; static C2DBox g_filterArea(0.0f, 0.0f, 0.0f); static float g_filterZ = 0, g_filterRange = 0;
static CThingCreatureBase* g_result = 0; static int g_searches = 0;
bool CThingSearchTools::GetPFirstThingInRoughArea(const C2DBoxI& box, const CInAreaZRangeFilter& filter, CThingCreatureBase** out)
{
    ++g_searches;
    g_searchBox = box;
    g_filterArea = filter.First->Area;
    g_filterZ = filter.Second->Z; g_filterRange = filter.Second->Range;
    CHECK(*out == 0);
    *out = g_result;
    return g_result != 0;
}
void CTCDoor::v00() { }
CThingSearchTools* CTCDoor::GetThingSearchTools() const { return &g_tools; }
const CWorld* CTCDoor::GetWorld() const { return &g_world; }
static int g_opens = 0, g_closes = 0; static bool g_lastInstant = false;
void CTCDoor::Open(bool instant) { ++g_opens; g_lastInstant = instant; }
void CTCDoor::Close() { ++g_closes; }
int main()
{
    CTCPhysicsBase phys; phys.Position.X = 10.0f; phys.Position.Y = 20.0f; phys.Position.Z = 5.0f;
    CThing thing; thing.PhysicsTC = &phys;
    CDoorDef def; def.TriggerRadius = 2.5f;
    CTCDoor door; door.ParentThing = &thing; door.PDef = &def;
    door.CurrentOpen = false; door.OldOpen = false; door.LastCreatureFoundWF = 0;
    // 1: a creature is found -> open now, remember the frame, open the door instantly
    static char creatureMem[4]; g_result = (CThingCreatureBase*)creatureMem; g_frame = 100;
    door.UpdateOpenStatusTriggerTypePerson();
    CHECK(g_searches == 1);
    CHECK(g_toIntBox.minX == 7.5f && g_toIntBox.minY == 17.5f && g_toIntBox.maxX == 12.5f && g_toIntBox.maxY == 22.5f);
    CHECK(g_searchBox.minX == 7 && g_searchBox.minY == 17 && g_searchBox.maxX == 12 && g_searchBox.maxY == 22);
    CHECK(g_filterArea.minX == 7.5f && g_filterArea.minY == 17.5f && g_filterArea.maxX == 12.5f && g_filterArea.maxY == 22.5f);
    CHECK(g_filterZ == 5.0f && g_filterRange == 1.0f);
    CHECK(door.CurrentOpen && door.OldOpen && door.LastCreatureFoundWF == 100);
    CHECK(g_opens == 1 && g_lastInstant && g_closes == 0);
    // 2: nobody, but within one second (30 frames) -> stays open, no door action
    g_result = 0; g_frame = 130;
    door.UpdateOpenStatusTriggerTypePerson();
    CHECK(door.CurrentOpen && door.OldOpen && door.LastCreatureFoundWF == 100);
    CHECK(g_opens == 1 && g_closes == 0);
    // 3: nobody for more than a second -> close
    g_frame = 131;
    door.UpdateOpenStatusTriggerTypePerson();
    CHECK(!door.CurrentOpen && !door.OldOpen && door.LastCreatureFoundWF == 100);
    CHECK(g_opens == 1 && g_closes == 1);
    // 4: still nobody -> no further action
    g_frame = 500;
    door.UpdateOpenStatusTriggerTypePerson();
    CHECK(!door.CurrentOpen && g_opens == 1 && g_closes == 1);
    if (fails == 0) printf("00730f90_TEST PASS\n");
    return fails;
}

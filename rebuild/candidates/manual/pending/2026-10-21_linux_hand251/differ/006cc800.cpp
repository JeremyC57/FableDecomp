// Grade: DIFFER(358v399) -- retail rounds the scan half-size with an inline-asm fistp helper (0.5 / 1.0 in ebp-frame locals, bare fistp, ebp frame); pure C++ cannot emit it (asm_bake family)
// CTCCreatureNavigation::FrameUpdate() -- every fifth frame (staggered by the thing's unique id) feeds the navigator the creature's position; when the nearby-thing scan is due (once a second, staggered) or the creature's scan radius grew, it re-reads the radius, schedules the next scan a second ahead, clears the nearby list and refills it with the things in a square of half-side max(2, round(radius * fps * k)) around the creature
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CC800, 399 bytes (`ret`, ebp frame).
// Callees: GFGetCurrentFrame @ 0x0049D870, _allrem @ 0x00BFEC30, CThing::GetPosition @ 0x006A4D00 / 0x004C73D0, the navigator's
// virtual slot 13 (+0x34), the scan-radius getter @ 0x00661CD0, memmove @ 0x00BFEAE6 (inlined vector erase), __ftol2 @ 0x00BFEA70,
// the world grid query @ 0x006CE800 (g_pWorld 0x013B86A0 +0x24 -> +0x20).
// Layout: ParentThing +0x04, navigator +0x0c, nearby-thing vector +0x40, NextScanFrame +0x4c, ScanRadius +0x5c; CThing::UniqueID +0x18.
extern long g_framesPerSecond;                              // 0x01375550
long __fastcall GFGetCurrentFrame();                        // 0x0049D870
struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned char    _pad00[0x18];
    __int64          UniqueID;                              // +0x18
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    float            GetScanRadius() const;                 // 0x00661CD0
};
struct CNavigator {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void SetPosition(const C3DVector& pos);         // slot 13 (+0x34)
};
struct CThingPtrVector {
    CThing** Start;
    CThing** Finish;
    CThing** EndOfStorage;
    void clear() { Finish = Start; }
};
struct CRect { long Left, Top, Right, Bottom; };
struct CThingGrid {
    void GetThingsInRect(const CRect* rect, CThing** thing, CThingPtrVector** out);   // 0x006CE800
};
struct CWorldMap { unsigned char _pad[0x20]; CThingGrid* Grid; };
struct CWorld { unsigned char _pad[0x24]; CWorldMap* Map; };
extern CWorld* g_pWorld;                                    // 0x013B86A0

inline long RoundToInt(float f)
{
    long i = (long)(f + 0.5f);
    if ((float)i - 1.0f == f)
        --i;
    return i;
}

struct CTCCreatureNavigation {
    void*           __vftable;                              // +0x00
    CThing*         ParentThing;                            // +0x04
    unsigned char   _pad08[0x4];
    CNavigator*     Navigator;                              // +0x0c
    unsigned char   _pad10[0x30];
    CThingPtrVector NearbyThings;                           // +0x40
    long            NextScanFrame;                          // +0x4c
    unsigned char   _pad50[0xc];
    float           ScanRadius;                             // +0x5c
    void FrameUpdate();
};

void CTCCreatureNavigation::FrameUpdate()
{
    if ((ParentThing->UniqueID + GFGetCurrentFrame()) % 5 == 0)
        Navigator->SetPosition(ParentThing->GetPosition());
    long next = NextScanFrame;
    if (GFGetCurrentFrame() < next || (next != 0 && (ParentThing->UniqueID + GFGetCurrentFrame()) % g_framesPerSecond != 0)) {
        if (!(ParentThing->GetScanRadius() > ScanRadius))
            return;
    }
    long fps = g_framesPerSecond;
    ScanRadius = ParentThing->GetScanRadius();
    NextScanFrame = GFGetCurrentFrame() + fps;
    NearbyThings.clear();
    long half = RoundToInt(fps * ScanRadius * 3.0f);
    if (half < 2)
        half = 2;
    const C3DVector& pos = ParentThing->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    CRect rect = { x - half, y - half, x + half, y + half };
    CThing* thing = ParentThing;
    CThingPtrVector* out = &NearbyThings;
    g_pWorld->Map->Grid->GetThingsInRect(&rect, &thing, &out);
}

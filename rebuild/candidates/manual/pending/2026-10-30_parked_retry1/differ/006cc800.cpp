// Grade: DIFFER(373v399) -- landed CeilToLong retail_asm helper, CWorld::GetFrame (PDB) with the landed __int64 uid stagger; residue: retail keeps the STLport erase(begin(), end()) of clear() (compares _M_finish with itself, memmove path) which the lane folds away, and the second stagger's register roles
// CTCCreatureNavigation::FrameUpdate() -- every fifth frame (staggered by the thing's unique id) feeds the navigator the creature's position; when the nearby-thing scan is due (once a second, staggered) or the creature's scan radius grew, it re-reads the radius, schedules the next scan a second ahead, clears the nearby list and refills it with the things in a square of half-side max(2, round(radius * fps * k)) around the creature
// Retail-asm: 0x006CC8FD fld dword ptr [ebp-10h] ; fadd dword ptr [ebp-0Ch] ; fistp dword ptr [ebp-4] ; dec dword ptr [ebp-4] -- the landed CeilToLong helper
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CC800, 399 bytes (`ret`, ebp frame).
// Callees: GFGetCurrentFrame @ 0x0049D870, _allrem @ 0x00BFEC30, CThing::GetPosition @ 0x006A4D00 / 0x004C73D0, the navigator's
// virtual slot 13 (+0x34), the scan-radius getter @ 0x00661CD0, memmove @ 0x00BFEAE6 (inlined vector erase), __ftol2 @ 0x00BFEA70,
// the world grid query @ 0x006CE800 (g_pWorld 0x013B86A0 +0x24 -> +0x20).
// Layout: ParentThing +0x04, navigator +0x0c, nearby-thing vector +0x40, NextScanFrame +0x4c, ScanRadius +0x5c; CThing::UniqueID +0x18.
extern long g_framesPerSecond;                              // 0x01375550
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
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int count);   // 0x00BFEAE6
struct CThingPtrVector {
    CThing** Start;
    CThing** Finish;
    CThing** EndOfStorage;
    CThing** erase(CThing** first, CThing** last)
    {
        CThing** i;
        if (last != Finish) {
            unsigned int bytes = (char*)Finish - (char*)last;
            i = (CThing**)((char*)memmove(first, last, bytes) + bytes);
        } else
            i = first;
        Finish = i;
        return first;
    }
    void clear() { erase(Start, Finish); }
};
struct CRect { long Left, Top, Right, Bottom; };
struct CThingGrid {
    void GetThingsInRect(const CRect* rect, CThing** thing, CThingPtrVector** out);   // 0x006CE800
};
struct CWorldMap { unsigned char _pad[0x20]; CThingGrid* Grid; };
class CWorld {
public:
    unsigned char _pad[0x24];
    CWorldMap*    Map;                                       // +0x24
    static long __fastcall GetFrame();                       // 0x0049D870
};
extern CWorld* g_pWorld;                                    // 0x013B86A0

// the engine's inline-assembly ceil helper (the landed CTCShop::GetMaxStock 0x00772720 / CThingPhysical::ModifyHealth 0x006A68F0 CeilToLong, verbatim)
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
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

static __forceinline bool IsStaggeredFrame(const CThing* thing, long period)
{
    __int64 uid = thing->UniqueID;
    return (CWorld::GetFrame() + uid) % period == 0;
}

void CTCCreatureNavigation::FrameUpdate()
{
    if (IsStaggeredFrame(ParentThing, 5))
        Navigator->SetPosition(ParentThing->GetPosition());
    long next = NextScanFrame;
    if (CWorld::GetFrame() < next || (next != 0 && !IsStaggeredFrame(ParentThing, g_framesPerSecond))) {
        if (!(ParentThing->GetScanRadius() > ScanRadius))
            return;
    }
    long fps = g_framesPerSecond;
    ScanRadius = ParentThing->GetScanRadius();
    NextScanFrame = CWorld::GetFrame() + fps;
    NearbyThings.clear();
    long half = CeilToLong(fps * ScanRadius * 3.0f);
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

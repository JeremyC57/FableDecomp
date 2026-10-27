// CTCCreatureNavigation::FrameUpdate() -- every fifth frame (staggered by the thing's unique id) feeds the navigator the creature's position; when the nearby-thing scan is due (once a second, staggered) or the creature's scan radius grew, it re-reads the radius, schedules the next scan a second ahead, clears the nearby list and refills it with the things in a square of half-side max(2, round(radius * fps * k)) around the creature
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CC800, 399 bytes (`ret`, ebp frame).
// Callees: GFGetCurrentFrame @ 0x0049D870, _allrem @ 0x00BFEC30, CThing::GetPosition @ 0x006A4D00 / 0x004C73D0, the navigator's
// virtual slot 13 (+0x34), the scan-radius getter @ 0x00661CD0, memmove @ 0x00BFEAE6 (inlined vector erase), __ftol2 @ 0x00BFEA70,
// the world grid query @ 0x006CE800 (g_pWorld 0x013B86A0 +0x24 -> +0x20).
// Layout: ParentThing +0x04, navigator +0x0c, nearby-thing vector +0x40, NextScanFrame +0x4c, ScanRadius +0x5c; CThing::UniqueID +0x18.
#pragma optimize("y", off)
// Retail-asm: 0x006CC8FD..0x006CC917 `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-18h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-18h] ; jne ; dec dword ptr [ebp-4]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
extern long g_framesPerSecond;                              // 0x01375550
long __fastcall GFGetCurrentFrame();                        // 0x0049D870
// the stagger test shared with 0x00785B40: the id and the period arrive by value (the id loaded into registers and the
// period homed in a frame word before the frame call), then the 64-bit remainder (__allrem)
__forceinline bool IsStaggeredFrame(__int64 uniqueId, long period)
{
    return (uniqueId + GFGetCurrentFrame()) % period == 0;
}
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
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
struct CThingPtrVector {                                    // STLport vector<CThing*>
    CThing** Start;
    CThing** Finish;
    CThing** EndOfStorage;
    // vector::erase(first, last): the tail moved down with memmove (the inlined pointer copy), the finish rebased
    __forceinline void erase(CThing** first, CThing** last)
    {
        CThing** newFinish;
        if (last != Finish) {
            unsigned long bytes = (unsigned long)((char*)Finish - (char*)last);
            newFinish = (CThing**)((char*)memmove(first, last, bytes) + bytes);
        } else
            newFinish = first;
        Finish = newFinish;
    }
};
struct CRect { long Left, Top, Right, Bottom; };
struct CThingGrid {
    void GetThingsInRect(const CRect* rect, CThing** thing, CThingPtrVector** out);   // 0x006CE800
};
struct CWorldMap { unsigned char _pad[0x20]; CThingGrid* Grid; };
struct CWorld { unsigned char _pad[0x24]; CWorldMap* Map; };
extern CWorld* g_pWorld;                                    // 0x013B86A0

// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
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

void CTCCreatureNavigation::FrameUpdate()
{
    if (IsStaggeredFrame(ParentThing->UniqueID, 5))
        Navigator->SetPosition(ParentThing->GetPosition());
    long next = NextScanFrame;
    if (GFGetCurrentFrame() < next || (next != 0 && !IsStaggeredFrame(ParentThing->UniqueID, g_framesPerSecond))) {
        if (!(ParentThing->GetScanRadius() > ScanRadius))
            return;
    }
    long fps = g_framesPerSecond;
    ScanRadius = ParentThing->GetScanRadius();
    NextScanFrame = GFGetCurrentFrame() + fps;
    CThingPtrVector* out = &NearbyThings;
    out->erase(out->Start, out->Finish);
    long half = CeilToLong(fps * ScanRadius * 3.0f);
    if (half < 2)
        half = 2;
    const C3DVector& pos = ParentThing->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    CRect rect = { x - half, y - half, x + half, y + half };
    CThing* thing = ParentThing;
    g_pWorld->Map->Grid->GetThingsInRect(&rect, &thing, &out);
}

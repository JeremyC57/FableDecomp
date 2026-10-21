// CAIStateGroup_LookAtInterestingThings::FindInterestingThing() -- pick a thing within 10 units that has an interest component, weighted by each one's interest value; with none nearby, push the next look 4 to 6 seconds out and return 0
// Grade: DIFFER(508v508) -- register permutations only: the first weighting loop keeps the thing in eax and the interest component in ecx (ours swaps them), the float-random argument and the second loop's key pointer use ecx/edx (ours edx/eax), `sar` lands on esi not eax, and the filter's 0xb3 store is scheduled one load earlier; frame, slots, control flow and calls all line up
// Shape: same-TU noinline stand-in for the collector (keeps the vector start in edi); the FrameToLook delay takes the random
// first and the frame rate into a local (fps loaded into ebp after the div, multiplied after GetWorldFrame).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00919240, length 508 bytes (+ int3 padding), plain `ret`.
// Callees: CThing::GetPosition @ 0x004C73D0, __ftol2 @ 0x00BFEA70, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, the
// rough-area collector @ 0x00919780 (box, interface filter, output vector), CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0,
// GFROR13 @ 0x00497890 (__fastcall), CWorld::GetWorldFrame @ 0x0049D870, GFFloatRandom(float, unsigned long*) @ 0x004978A0
// (__fastcall), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xb3, bit 19 of +0x34), the CRT free @ 0x00BFEA14.
// Globals: the frames-per-second long @ 0x01375550.
// Layout: CAIStateGroup_LookAtInterestingThings from Ego_r.pdb (FrameToLook +0x14); the interest component (interface 0xb3)
// keeps its def at +0x0c and the def its float weight at +0x28 (descriptive names).
struct C3DVector {
    float X, Y, Z;
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition();             // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* component;
        GetInterface(&component, interfaceType);
        return component;
    }
};
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};
struct CThingFilter_HasTC {
    long InterfaceType;
    CThingFilter_HasTC(long interfaceType) : InterfaceType(interfaceType) { }
};
extern "C" void __cdecl free(void* block);    // 0x00BFEA14
struct CThingPtrVector {                        // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingPtrVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingPtrVector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CThing*& operator[](unsigned int n) { return _M_start[n]; }
};
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasTC& filter, CThingPtrVector& out) const;   // 0x00919780
};
long CollectSink_919240(const void* self, long seen, CThing*** slots);   // stand-in: the area walk the collector does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for the rough-area collector @ 0x00919780 (a template instantiated in retail's TU, right after this
// function): it reads its const reference arguments and fills the output vector, and lets no argument escape.
__declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasTC& filter, CThingPtrVector& out) const
{
    out._M_finish = out._M_start + CollectSink_919240(this, area.MinX + filter.InterfaceType, 0);
}
#pragma auto_inline(on)
struct CInterestDef {
    unsigned char _pad0[0x28];
    float Interest;                             // +0x28
};
struct CTCInterest {                            // interface 0xb3
    unsigned char _pad0[0x0c];
    CInterestDef* Def;                          // +0x0c
};
long __fastcall GFROR13(unsigned long value);  // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThing* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    __forceinline unsigned long GetRandomBelow(unsigned long n)
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        return *seed % n;
    }
};
struct CAIStateGroup_LookAtInterestingThings : CAIStateGroupBase {
    long FrameToLook;                           // +0x14
    CThing* FindInterestingThing();
};

CThing* CAIStateGroup_LookAtInterestingThings::FindInterestingThing()
{
    CThingFilter_HasTC filter(0xb3);
    CThingPtrVector things;
    {
        const C3DVector& pos = GetCreature()->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        PeekThingSearchTools()->GetAllThingsInRoughArea(C2DBoxI(x - 10, y - 10, x + 10, y + 10), filter, things);
    }
    if (things.empty()) {
        unsigned long random = GetRandomBelow(120);
        long fps = g_framesPerSecond;
        FrameToLook = CWorld::GetWorldFrame() + (random + 240) * fps;
        return 0;
    }
    float total = 0.0f;
    unsigned int count = things.size();
    for (unsigned int i = 0; i < count; ++i)
        total += things[i]->GetTCIfPresent<CTCInterest>(0xb3)->Def->Interest;
    float pick = GFFloatRandom(total, GetPWorldSeed());
    total = 0.0f;
    for (unsigned int j = 0; j < count; ++j) {
        total += things[j]->GetTCIfPresent<CTCInterest>(0xb3)->Def->Interest;
        if (pick <= total)
            return things[j];
    }
    return 0;
}

// CAIStateGroup_KidHurtSelfRunToMother::IsRunnable() -- the kid runs to its mother when its recent XY movement is
// Retry 10b (chain-split sweep, FIXES B-16): the `GetOffsetSource()` result is a named local `offsetSource` before it is dereferenced, which gives retail's register colouring/order (the chained call gives the parked difference).
// large against the distance measure the creature reports (ratio > 0.8), and then only on a 1-in-100 roll.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00915080, length 266 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (inlined GetInterface, type 2 = bit 2 of +0x20), the +0x60 sub-object
// virtual slot 70 (+0x118, C3DVector by hidden pointer), CThing::GetPosition @ 0x004C73D0, creature virtual slot 81
// (+0x144, float of a const C3DVector&), CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;                             // +0x60 (typed per use below)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
};
struct CXYDelta {                               // a 12-byte delta whose z is never written (retail keeps the slot)
    float x, y, z;
    float GetLengthXY() const { return (float)sqrt(x * x + y * y); }
};

struct CTCMovement {                            // interface type 2: two XY positions (now +0x0c, before +0x18)
    unsigned char _pad[0x0c];
    float m_posX, m_posY;                       // +0x0c
    unsigned char _pad14[4];
    float m_lastX, m_lastY;                     // +0x18
};
struct CThingOffsetSource {                     // the creature's +0x60 sub-object
    V16(a) V16(b) V16(c) V16(d) V4(e)
    virtual void v68(); virtual void v69();
    virtual C3DVector GetOffset();              // slot 70 (+0x118)
};
struct CThingCreature : CThing {                // the brain's creature (+0x60: the offset source)
    CThingOffsetSource* GetOffsetSource() { return (CThingOffsetSource*)m_pSub60; }
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CThingCreatureV {                        // the creature seen through its vtable
    V16(a) V16(b) V16(c) V16(d) V16(e)
    virtual void v80();
    virtual float GetDistanceMeasure(const C3DVector& pos);   // slot 81 (+0x144)
};

struct CAIStateGroup_KidHurtSelfRunToMother : CAIStateGroupBase {
    bool AddedSadMode;                          // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_KidHurtSelfRunToMother::IsRunnable()
{
    CAIBrain* brain = PBrain;
    CTCMovement* movement;
    brain->PCreature->GetInterface(&movement, 2);
    CXYDelta moved;
    moved.x = movement->m_posX - movement->m_lastX;
    moved.y = movement->m_posY - movement->m_lastY;
    CThingCreature* creature = (CThingCreature*)brain->PCreature;
    CThingOffsetSource* const& offsetSource = creature->GetOffsetSource();
    C3DVector pos = creature->GetPosition() + offsetSource->GetOffset();
    float length = moved.GetLengthXY();
    if (length / ((CThingCreatureV*)PBrain->PCreature)->GetDistanceMeasure(pos) > 0.8f)
        return Random(100) < 1;
    return false;
}
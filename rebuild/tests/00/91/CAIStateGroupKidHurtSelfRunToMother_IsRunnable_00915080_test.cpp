// Behaviour test for 00915080: src/00915080.cpp copied verbatim; a creature whose movement component, offset source
// and distance measure are stubbed so the "moved too far relative to the distance measure" test and the 1-in-100
// random roll on the world seed are checked.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
    SEntry* LowerBound(const long* type) const;       // 0x0040F020 / 0x004365B0 (per-TU copies)
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
// ---- end source ----
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerThing::v00() {}
void CAIStateGroupBase::v01() {} void CAIStateGroupBase::v02() {} void CAIStateGroupBase::v03() {} void CAIStateGroupBase::v04() {} void CAIStateGroupBase::v05() {} void CAIStateGroupBase::v06() {} void CAIStateGroupBase::v07() {} void CAIStateGroupBase::v08() {} void CAIStateGroupBase::v13() {} void CAIStateGroupBase::v15() {} void CAIStateGroupBase::v16() {} void CAIStateGroupBase::v17() {}
void CThingOffsetSource::v68() {} void CThingOffsetSource::v69() {}
void CThingCreatureV::v80() {}
void CThingOffsetSource::aa0() {} void CThingOffsetSource::aa1() {} void CThingOffsetSource::aa2() {} void CThingOffsetSource::aa3() {} void CThingOffsetSource::ab0() {} void CThingOffsetSource::ab1() {} void CThingOffsetSource::ab2() {} void CThingOffsetSource::ab3() {} void CThingOffsetSource::ac0() {} void CThingOffsetSource::ac1() {} void CThingOffsetSource::ac2() {} void CThingOffsetSource::ac3() {} void CThingOffsetSource::ad0() {} void CThingOffsetSource::ad1() {} void CThingOffsetSource::ad2() {} void CThingOffsetSource::ad3() {} void CThingOffsetSource::ba0() {} void CThingOffsetSource::ba1() {} void CThingOffsetSource::ba2() {} void CThingOffsetSource::ba3() {} void CThingOffsetSource::bb0() {} void CThingOffsetSource::bb1() {} void CThingOffsetSource::bb2() {} void CThingOffsetSource::bb3() {} void CThingOffsetSource::bc0() {} void CThingOffsetSource::bc1() {} void CThingOffsetSource::bc2() {} void CThingOffsetSource::bc3() {} void CThingOffsetSource::bd0() {} void CThingOffsetSource::bd1() {} void CThingOffsetSource::bd2() {} void CThingOffsetSource::bd3() {} void CThingOffsetSource::ca0() {} void CThingOffsetSource::ca1() {} void CThingOffsetSource::ca2() {} void CThingOffsetSource::ca3() {} void CThingOffsetSource::cb0() {} void CThingOffsetSource::cb1() {} void CThingOffsetSource::cb2() {} void CThingOffsetSource::cb3() {} void CThingOffsetSource::cc0() {} void CThingOffsetSource::cc1() {} void CThingOffsetSource::cc2() {} void CThingOffsetSource::cc3() {} void CThingOffsetSource::cd0() {} void CThingOffsetSource::cd1() {} void CThingOffsetSource::cd2() {} void CThingOffsetSource::cd3() {} void CThingOffsetSource::da0() {} void CThingOffsetSource::da1() {} void CThingOffsetSource::da2() {} void CThingOffsetSource::da3() {} void CThingOffsetSource::db0() {} void CThingOffsetSource::db1() {} void CThingOffsetSource::db2() {} void CThingOffsetSource::db3() {} void CThingOffsetSource::dc0() {} void CThingOffsetSource::dc1() {} void CThingOffsetSource::dc2() {} void CThingOffsetSource::dc3() {} void CThingOffsetSource::dd0() {} void CThingOffsetSource::dd1() {} void CThingOffsetSource::dd2() {} void CThingOffsetSource::dd3() {}
void CThingOffsetSource::e0() {} void CThingOffsetSource::e1() {} void CThingOffsetSource::e2() {} void CThingOffsetSource::e3() {}
void CThingCreatureV::aa0() {} void CThingCreatureV::aa1() {} void CThingCreatureV::aa2() {} void CThingCreatureV::aa3() {} void CThingCreatureV::ab0() {} void CThingCreatureV::ab1() {} void CThingCreatureV::ab2() {} void CThingCreatureV::ab3() {} void CThingCreatureV::ac0() {} void CThingCreatureV::ac1() {} void CThingCreatureV::ac2() {} void CThingCreatureV::ac3() {} void CThingCreatureV::ad0() {} void CThingCreatureV::ad1() {} void CThingCreatureV::ad2() {} void CThingCreatureV::ad3() {} void CThingCreatureV::ba0() {} void CThingCreatureV::ba1() {} void CThingCreatureV::ba2() {} void CThingCreatureV::ba3() {} void CThingCreatureV::bb0() {} void CThingCreatureV::bb1() {} void CThingCreatureV::bb2() {} void CThingCreatureV::bb3() {} void CThingCreatureV::bc0() {} void CThingCreatureV::bc1() {} void CThingCreatureV::bc2() {} void CThingCreatureV::bc3() {} void CThingCreatureV::bd0() {} void CThingCreatureV::bd1() {} void CThingCreatureV::bd2() {} void CThingCreatureV::bd3() {} void CThingCreatureV::ca0() {} void CThingCreatureV::ca1() {} void CThingCreatureV::ca2() {} void CThingCreatureV::ca3() {} void CThingCreatureV::cb0() {} void CThingCreatureV::cb1() {} void CThingCreatureV::cb2() {} void CThingCreatureV::cb3() {} void CThingCreatureV::cc0() {} void CThingCreatureV::cc1() {} void CThingCreatureV::cc2() {} void CThingCreatureV::cc3() {} void CThingCreatureV::cd0() {} void CThingCreatureV::cd1() {} void CThingCreatureV::cd2() {} void CThingCreatureV::cd3() {} void CThingCreatureV::da0() {} void CThingCreatureV::da1() {} void CThingCreatureV::da2() {} void CThingCreatureV::da3() {} void CThingCreatureV::db0() {} void CThingCreatureV::db1() {} void CThingCreatureV::db2() {} void CThingCreatureV::db3() {} void CThingCreatureV::dc0() {} void CThingCreatureV::dc1() {} void CThingCreatureV::dc2() {} void CThingCreatureV::dc3() {} void CThingCreatureV::dd0() {} void CThingCreatureV::dd1() {} void CThingCreatureV::dd2() {} void CThingCreatureV::dd3() {} void CThingCreatureV::ea0() {} void CThingCreatureV::ea1() {} void CThingCreatureV::ea2() {} void CThingCreatureV::ea3() {} void CThingCreatureV::eb0() {} void CThingCreatureV::eb1() {} void CThingCreatureV::eb2() {} void CThingCreatureV::eb3() {} void CThingCreatureV::ec0() {} void CThingCreatureV::ec1() {} void CThingCreatureV::ec2() {} void CThingCreatureV::ec3() {} void CThingCreatureV::ed0() {} void CThingCreatureV::ed1() {} void CThingCreatureV::ed2() {} void CThingCreatureV::ed3() {}
CAIStateGroupBase::~CAIStateGroupBase() {}
void CAIStateGroupBase::Cleanup() {} void CAIStateGroupBase::OnReplace() {} bool CAIStateGroupBase::IsRunnable() { return false; }
void CAIStateGroupBase::Update() {} long CAIStateGroupBase::GetTimeToInitiate() { return 0; }
static unsigned long g_seed = 0;
unsigned long* CAIStateGroupBase::GetPWorldSeed() { return &g_seed; }
long __fastcall GFGetCurrentFrame() { return 0; }
static unsigned long g_rot = 0;
unsigned long __fastcall GFROR13(unsigned long) { return g_rot; }
static C3DVector g_pos(1, 2, 3); static C3DVector g_measuredAt;
const C3DVector& CThingCreature::GetPosition() { return g_pos; }
C3DVector CThingOffsetSource::GetOffset() { return C3DVector(10, 20, 30); }
static float g_measure = 10.0f;
float CThingCreatureV::GetDistanceMeasure(const C3DVector& pos) { g_measuredAt = pos; return g_measure; }
static CTCMovement g_move;
static CInterfaceMap::SEntry g_entries[1] = { { 2, (CTCBase*)&g_move } };
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { return *type <= 2 ? first : last; }
int main()
{
    static unsigned char creatureRaw[0x100]; memset(creatureRaw, 0, sizeof(creatureRaw));
    new (creatureRaw) CThingCreatureV;                                   // the vptr the distance measure is read through
    CThing* creature = (CThing*)creatureRaw;
    creature->m_interfaces.first = g_entries; creature->m_interfaces.last = g_entries + 1;
    creature->m_presentBits[0] |= 1UL << 2;
    static CThingOffsetSource offsetSource; creature->m_pSub60 = &offsetSource;
    static CAIBrain brain; brain.PCreature = creature;
    static unsigned char raw[sizeof(CAIStateGroup_KidHurtSelfRunToMother)]; memset(raw, 0, sizeof(raw));
    CAIStateGroup_KidHurtSelfRunToMother* g = (CAIStateGroup_KidHurtSelfRunToMother*)raw; g->PBrain = &brain;
    g_move.m_posX = 6; g_move.m_posY = 8; g_move.m_lastX = 0; g_move.m_lastY = 0;         // moved 10
    g_rot = 500;                                                          // 500 % 100 = 0 < 1
    CHECK(g->CAIStateGroup_KidHurtSelfRunToMother::IsRunnable() == true);
    CHECK(g_measuredAt.x == 11 && g_measuredAt.y == 22 && g_measuredAt.z == 33);
    CHECK(g_seed == 500);
    g_rot = 501;                                                          // roll fails
    CHECK(g->CAIStateGroup_KidHurtSelfRunToMother::IsRunnable() == false);
    g_rot = 500; g_measure = 20.0f;                                       // 10 / 20 = 0.5 <= 0.8
    CHECK(g->CAIStateGroup_KidHurtSelfRunToMother::IsRunnable() == false);
    if (fails == 0) printf("00915080_TEST PASS\n"); else printf("00915080_TEST FAIL\n");
    return fails;
}
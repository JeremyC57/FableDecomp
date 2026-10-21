// Behaviour test for 009191b0: src/009191b0.cpp copied verbatim; the world seed, GFROR13, the frame clock and the idle
// scheduler stubbed so the scheduled frame (now + (seed % 120 + 240) * fps) and the guarded ChooseNewIdle are observable.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// Retry 10b (B-12 sweep): the random delay is bound to a const REFERENCE (`const unsigned long& extraSeconds = Random(120);`, an address-taken temporary): the `% 120` div stays before the GFGetCurrentFrame call with the remainder in edi and the frame rate in ebx (a value local lets VC7.1 sink the div below the call).
// CAIStateGroup_LookAtInterestingThings::OnReplace() -- schedule the next look 240..359 seconds from now and let the idle scheduler choose a new idle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009191B0, length 140 bytes (+ int3 padding). Vtable slot 10 of
// CAIStateGroup_LookAtInterestingThings (0x01289BBC).
// Callees: CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890 (__fastcall), GFGetCurrentFrame
// @ 0x0049D870 (__fastcall), CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type 0xd3 = the idle
// scheduler), CTCIdleScheduler::ChooseNewIdle @ 0x00801A50. Global: the frame-rate long @ 0x01375550.
//
// Shape: `FrameToLook = GFGetCurrentFrame() + (Random(120) + 240) * g_framesPerSecond` -- the right operand (the
// inlined seed step `seed = GFROR13(seed*0x24A1 + 0x24DF)`, `% 120` as an unsigned `div` by a constant range so
// no zero test, the frame rate held in ebx) is evaluated before the frame call. Then the out-parameter
// GetInterface whose result guards the ChooseNewIdle call (component loaded straight into ecx).
// PDB: FrameToLook +0x14. The engine header CAIStateGroup_LookAtInterestingThings.h is flattened data-only:
// local model.
// Earlier forms: the single expression and value locals sink the div past the frame call.
class CTCBase;

long __fastcall GFGetCurrentFrame();                        // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);      // 0x00497890
extern long g_framesPerSecond;                              // 0x01375550

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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

struct CTCIdleScheduler {                       // interface type 0xd3
    void ChooseNewIdle();                       // 0x00801A50
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        if (range != 0)
            return rotated % range;
        return 0;
    }
    virtual void OnReplace();
};

struct CAIStateGroup_LookAtInterestingThings : CAIStateGroupBase {
    long FrameToLook;                           // +0x14
    virtual void OnReplace();
};

void CAIStateGroup_LookAtInterestingThings::OnReplace()
{
    const unsigned long& extraSeconds = Random(120);
    long framesPerSecond = g_framesPerSecond;
    FrameToLook = GFGetCurrentFrame() + (extraSeconds + 240) * framesPerSecond;
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetInterface(&scheduler, 0xd3))
        scheduler->ChooseNewIdle();
}
// ---- end source ----
long g_framesPerSecond = 30;
static unsigned long g_seed = 12345;
static int g_chosen = 0; static CTCIdleScheduler* g_chosenThis = 0;
long __fastcall GFGetCurrentFrame() { return 1000; }
unsigned long __fastcall GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
unsigned long* CAIStateGroupBase::GetPWorldSeed() { return &g_seed; }
CAIStateGroupBase::~CAIStateGroupBase() {}
void CAIStateGroupBase::OnReplace() {}
void CTCIdleScheduler::ChooseNewIdle() { ++g_chosen; g_chosenThis = this; }
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
int main()
{
    static CThing creature; memset(&creature, 0, sizeof(creature));
    static CTCIdleScheduler scheduler;
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0xd3; g_entries[1].component = (CTCBase*)&scheduler;
    creature.m_interfaces.first = g_entries; creature.m_interfaces.last = g_entries + 2;
    static CAIBrain brain; brain.PCreature = &creature;
    static unsigned char raw[sizeof(CAIStateGroup_LookAtInterestingThings)];
    CAIStateGroup_LookAtInterestingThings* g = (CAIStateGroup_LookAtInterestingThings*)raw;
    g->PBrain = &brain;
    unsigned long next = 12345UL * 0x24A1U + 0x24DFU; unsigned long rot = (next >> 13) | (next << 19);
    g->CAIStateGroup_LookAtInterestingThings::OnReplace();
    CHECK(g_seed == rot && g->FrameToLook == 1000 + (long)((rot % 120) + 240) * 30);
    CHECK(g_chosen == 0);                                        // no idle scheduler component bit
    creature.m_presentBits[0xd3 >> 5] |= 1UL << (0xd3 & 31);
    g->CAIStateGroup_LookAtInterestingThings::OnReplace();
    CHECK(g_chosen == 1 && g_chosenThis == &scheduler);
    CHECK(g->FrameToLook >= 1000 + 240 * 30 && g->FrameToLook < 1000 + 360 * 30);
    if (fails == 0) printf("009191b0_TEST PASS\n"); else printf("009191b0_TEST FAIL\n");
    return fails;
}
#include <stdio.h>
#include <string.h>
// CAIStateGroup_BirdFlyToFood::IsRunnable() -- runnable for a flying bird that has not eaten for 20 seconds, on a 74% roll: fly to the nearest dead creature (64% roll) or else the nearest BIRD_FOOD thing, if a flight position near it is found
// Leaf: IsRunnable@CAIStateGroup_BirdFlyToFood
// Model: the landed sibling CAIStateGroup_BirdGroundWander::IsRunnable 0x009143E0 (same TU: interface 7 lookup, mode getter,
//   frame-rate local) and GetRandomFlightPos 0x00912700; layout local: FlightPos +0x24 (C3DVector), FrameLastAte +0x30.
// Shape: the two GetPBestThing<Filter, Compare> instantiations have same-TU out-only stand-in bodies that read the
//   comparator's position by value, so neither the out pointer nor the comparator temporary escapes: VC7.1 then stores
//   the out pointer's zero after loading the creature and loads the whole position before the stores (as retail).
struct CTCBase;

long __fastcall GFGetCurrentFrame();            // 0x0049D870
extern long g_framesPerSecond;                  // 0x01375550
extern long g_flyingMode;                       // 0x0129CBA4 (the mode compared by BirdGroundWander too)

struct CWorld {
    static long GetFramesPerSecond() { return g_framesPerSecond; }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)


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
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct C3DVector { float X, Y, Z; };
struct CThingPhysical : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};
struct CThingFilter_None { CThingFilter_None() {} };
struct CThingFilter_DefName {
    CCharString DefName;
    CThingFilter_DefName(const CCharString& name) : DefName(name) {}
};
struct CThingCompare_Nearest {
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) : Position(position) {}
};
struct CThingSearchTools;
CThingPhysical* FindBest_009142B0(CThingSearchTools* tools, float x, float y, float z);
CThingPhysical* FindBest_00914330(CThingSearchTools* tools, const CThingFilter_DefName& filter, float x, float y, float z);
struct CThingSearchTools {
    bool GetPBestThing(const CThingFilter_None& filter, const CThingCompare_Nearest& compare, CThingPhysical** out);      // 0x009142B0
    bool GetPBestThing(const CThingFilter_DefName& filter, const CThingCompare_Nearest& compare, CThingPhysical** out);   // 0x00914330
};
#pragma auto_inline(off)
// Same-TU out-only stand-ins for the two GetPBestThing<Filter, Compare> instantiations (compiled in the caller's TU):
// they only write the out pointer, so VC7.1 knows the caller's out local does not escape.
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CThingFilter_None& filter, const CThingCompare_Nearest& compare, CThingPhysical** out)
{
    CThingPhysical* best = FindBest_009142B0(this, compare.Position.X, compare.Position.Y, compare.Position.Z);
    if (best == 0)
        return false;
    *out = best;
    return true;
}
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CThingFilter_DefName& filter, const CThingCompare_Nearest& compare, CThingPhysical** out)
{
    CThingPhysical* best = FindBest_00914330(this, filter, compare.Position.X, compare.Position.Y, compare.Position.Z);
    if (best == 0)
        return false;
    *out = best;
    return true;
}
#pragma auto_inline(on)

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

struct CTCCreatureModeManager {                 // interface 7
    long GetCurrentModeType();                  // 0x006CB6A0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingPhysical* Creature;                   // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld_008fcfa0();                        // 0x008FCFA0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {
    unsigned char _14[0x10];
    C3DVector FlightPos;                        // +0x24
    bool GetRandomFlightPos(const C3DVector& from, float radius, C3DVector* out, bool allowGround);   // 0x00912700
};
struct CAIStateGroup_BirdFlyToFood : CAIStateGroup_FlyingThingBase {
    unsigned long FrameLastAte;                 // +0x30
    bool IsRunnable();
};

bool CAIStateGroup_BirdFlyToFood::IsRunnable()
{
    CTCCreatureModeManager* modes;
    PBrain->Creature->GetInterface(&modes, 7);
    if (modes->GetCurrentModeType() == g_flyingMode) {
        long framesPerSecond;
        if ((FrameLastAte == 0
             || (framesPerSecond = GetPWorld_008fcfa0()->GetFramesPerSecond(),
                 GFGetCurrentFrame() - FrameLastAte > (unsigned long)(framesPerSecond * 20)))
            && WorldRandomRange(*GetPWorldSeed(), 100) < 74) {
            if (WorldRandomRange(*GetPWorldSeed(), 100) < 64) {
                CThingPhysical* creature = PBrain->Creature;
                CThingPhysical* corpse = 0;
                if (PeekThingSearchTools()->GetPBestThing(CThingFilter_None(), CThingCompare_Nearest(creature->GetPosition()), &corpse)
                    && GetRandomFlightPos(corpse->GetPosition(), 0.5f, &FlightPos, true))
                    return true;
            }
            CThingPhysical* food = 0;
            bool found;
            {
                CCharString name("BIRD_FOOD");
                found = PeekThingSearchTools()->GetPBestThing(CThingFilter_DefName(name), CThingCompare_Nearest(PBrain->Creature->GetPosition()), &food);
            }
            if (found && GetRandomFlightPos(food->GetPosition(), 1.5f, &FlightPos, true))
                return true;
        }
    }
    return false;
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
long g_framesPerSecond = 30;
long g_flyingMode = 5;
static long g_frame;
long __fastcall GFGetCurrentFrame() { return g_frame; }
static unsigned long g_rolls[8]; static int g_rollIndex;
unsigned long __fastcall GFROR13(unsigned long) { return g_rolls[g_rollIndex++]; }
static long g_mode;
long CTCCreatureModeManager::GetCurrentModeType() { return g_mode; }
static unsigned char g_creatureBuf[0x100], g_corpseBuf[0x100], g_foodBuf[0x100];
static C3DVector g_creaturePos = { 1.0f, 2.0f, 3.0f }, g_corpsePos = { 7.0f, 8.0f, 9.0f }, g_foodPos = { 4.0f, 5.0f, 6.0f };
const C3DVector& CThingPhysical::GetPosition() const
{
    if ((const void*)this == g_corpseBuf) return g_corpsePos;
    if ((const void*)this == g_foodBuf) return g_foodPos;
    return g_creaturePos;
}
static int g_strings, g_strCopies, g_strDtors; static const char* g_lastText;
CCharString::CCharString(const char* text, long) { g_lastText = text; PStringData = (void*)text; g_strings++; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; g_strCopies++; }
CCharString::~CCharString() { g_strDtors++; }
static CThingPhysical* g_corpseResult; static CThingPhysical* g_foodResult; static int g_corpseSearches, g_foodSearches;
static float g_searchX, g_searchY, g_searchZ; static const char* g_searchName; static CThingSearchTools* g_searchTools;
CThingPhysical* FindBest_009142B0(CThingSearchTools* tools, float x, float y, float z)
{ g_searchTools = tools; g_searchX = x; g_searchY = y; g_searchZ = z; g_corpseSearches++; return g_corpseResult; }
CThingPhysical* FindBest_00914330(CThingSearchTools* tools, const CThingFilter_DefName& filter, float x, float y, float z)
{ g_searchTools = tools; g_searchName = (const char*)filter.DefName.PStringData; g_searchX = x; g_searchY = y; g_searchZ = z; g_foodSearches++; return g_foodResult; }
CAIStateGroupBase::~CAIStateGroupBase() {}
static CWorld g_world; static unsigned long g_seed; static CThingSearchTools g_tools;
CWorld* CAIStateGroupBase::GetPWorld_008fcfa0() { return &g_world; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() { return &g_seed; }
CThingSearchTools* CAIStateGroupBase::PeekThingSearchTools() { return &g_tools; }
static bool g_flightOk[4]; static int g_flights; static C3DVector g_flightFrom[4]; static float g_flightRadius[4]; static C3DVector* g_flightOut[4]; static bool g_flightGround[4];
bool CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector& from, float radius, C3DVector* out, bool allowGround)
{ int i = g_flights++; g_flightFrom[i] = from; g_flightRadius[i] = radius; g_flightOut[i] = out; g_flightGround[i] = allowGround; return g_flightOk[i]; }

static CTCCreatureModeManager g_modes;
static CAIBrain g_brain;
static void Reset(unsigned long r0, unsigned long r1, bool f0, bool f1, CThingPhysical* corpse, CThingPhysical* food)
{
    g_rolls[0] = r0; g_rolls[1] = r1; g_rollIndex = 0; g_flightOk[0] = f0; g_flightOk[1] = f1; g_flights = 0;
    g_corpseResult = corpse; g_foodResult = food; g_corpseSearches = g_foodSearches = 0;
    g_strings = g_strCopies = g_strDtors = 0; g_searchName = 0; g_searchTools = 0;
}
int main()
{
    static CAIStateGroup_BirdFlyToFood state;
    CThingPhysical* creature = (CThingPhysical*)g_creatureBuf;
    CThingPhysical* corpse = (CThingPhysical*)g_corpseBuf;
    CThingPhysical* food = (CThingPhysical*)g_foodBuf;
    CInterfaceMap::SEntry e[] = { { 3, 0 }, { 7, (CTCBase*)&g_modes }, { 9, 0 } };
    creature->m_interfaces.first = e; creature->m_interfaces.last = e + 3;
    creature->m_presentBits[0] = 1UL << 7;
    g_brain.Creature = creature; state.PBrain = &g_brain;

    // not flying: nothing rolled
    g_mode = 4; state.FrameLastAte = 0; Reset(10, 10, true, true, corpse, food);
    CHECK(state.IsRunnable() == false && g_rollIndex == 0 && g_corpseSearches == 0 && g_foodSearches == 0);
    // flying, ate recently (elapsed == 20 s exactly): not runnable, no roll
    g_mode = 5; state.FrameLastAte = 1000; g_frame = 1600; Reset(10, 10, true, true, corpse, food);
    CHECK(state.IsRunnable() == false && g_rollIndex == 0);
    // hungry (elapsed > 20 s) but the 74% roll fails (74)
    g_frame = 1601; Reset(174, 10, true, true, corpse, food);
    CHECK(state.IsRunnable() == false && g_rollIndex == 1 && g_corpseSearches == 0 && g_foodSearches == 0);
    // never ate, roll 73 passes, corpse roll 63 passes, corpse found, flight found: runnable via the corpse
    state.FrameLastAte = 0; Reset(73, 263, true, true, corpse, food);
    CHECK(state.IsRunnable() == true && g_rollIndex == 2 && g_corpseSearches == 1 && g_foodSearches == 0 && g_strings == 0);
    CHECK(g_searchTools == &g_tools && g_searchX == 1.0f && g_searchY == 2.0f && g_searchZ == 3.0f);
    CHECK(g_flights == 1 && g_flightFrom[0].X == 7.0f && g_flightFrom[0].Z == 9.0f && g_flightRadius[0] == 0.5f && g_flightOut[0] == &state.FlightPos && g_flightGround[0]);
    // corpse found but no flight position: falls back to BIRD_FOOD, found, flight found
    Reset(0, 0, false, true, corpse, food);
    CHECK(state.IsRunnable() == true && g_corpseSearches == 1 && g_foodSearches == 1 && g_strings == 1 && g_strDtors == g_strings + g_strCopies);
    CHECK(strcmp(g_searchName, "BIRD_FOOD") == 0 && g_searchX == 1.0f && g_searchZ == 3.0f);
    CHECK(g_flights == 2 && g_flightFrom[1].X == 4.0f && g_flightFrom[1].Y == 5.0f && g_flightRadius[1] == 1.5f && g_flightOut[1] == &state.FlightPos && g_flightGround[1]);
    // no corpse, no food: not runnable, no flight query
    Reset(0, 0, true, true, 0, 0);
    CHECK(state.IsRunnable() == false && g_corpseSearches == 1 && g_foodSearches == 1 && g_flights == 0 && g_strDtors == g_strings + g_strCopies);
    // corpse roll fails (64): straight to food; food found but no flight position
    Reset(0, 64, false, false, corpse, food);
    CHECK(state.IsRunnable() == false && g_corpseSearches == 0 && g_foodSearches == 1 && g_flights == 1 && g_flightRadius[0] == 1.5f);
    // corpse roll fails, food found, flight found
    Reset(0, 99, true, true, corpse, food);
    CHECK(state.IsRunnable() == true && g_corpseSearches == 0 && g_foodSearches == 1 && g_flights == 1);
    if (fails == 0) printf("00913f30_TEST PASS\n"); else printf("00913f30_TEST FAIL\n");
    return fails;
}
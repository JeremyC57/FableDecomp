// Behaviour test for 0073ae10: src/0073ae10.cpp copied verbatim; AddMode(long) allocates a counted mode pointer,
// passes an add-ref'd copy to AddMode(CModePointer) (callee destroys it) and releases its local, returning the result.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCreatureModeManager::AddMode(long modeType) -- allocate a mode of the given type and add it (by-value counted pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AE10, length 92 bytes (+4 int3). Called by SetupDefaultMode
// (0x0073AF60) with the def's DefaultCreatureMode.
// Callees: CTCCreatureModeManager::AllocateMode(long) @ 0x0073A8E0 (returns CCountedPointer<CModeBase> through the hidden
// pointer = the 8-byte frame local), CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0 (by-value
// argument, destroyed by the callee), operator delete @ 0x00BFE9BC.
// Shape: the by-value argument is copy-constructed in the argument area (`sub esp,8 ; mov eax,esp`, the count block
// add-ref'd with `inc [esi]`); the bool result rides in bl across the local's destructor. The count block {references,
// __fastcall destroy function, object} is the CCountedPointer layout of the landed Reset family (0x00836580): the release
// is `--references == 0` -> destroy(object) -> delete block, with the block pointer cached in esi from the copy.
// AllocateMode gets a same-TU `auto_inline(off)` stand-in body that writes only the returned pointer: VC7.1 then knows
//   the local's address (the hidden return slot) does not escape, so AddMode cannot change it and the count block stays
//   in esi across the call, as retail (an extern declaration re-reads the slot after AddMode).
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CModeBase;

struct CModePointer {                       // CCountedPointer<CTCCreatureModeManager::CModeBase>
    CModeBase* m_pObject;                   // +0
    SCountBlock* m_pCount;                  // +4
    CModePointer(CModeBase* object, SCountBlock* count) : m_pObject(object), m_pCount(count) {}
    CModePointer(const CModePointer& other)
    {
        SCountBlock* count = other.m_pCount;
        m_pObject = other.m_pObject;
        m_pCount = count;
        if (count)
            ++count->m_references;
    }
    ~CModePointer()
    {
        SCountBlock* count = m_pCount;
        if (count) {
            if (--count->m_references == 0) {
                count->m_pfnDestroy(count->m_pObject);
                delete count;
            }
        }
    }
};

struct CThing;

struct CTCCreatureModeManager {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CModePointer AllocateMode(long modeType);           // 0x0073A8E0
    bool AddMode(CModePointer mode);                    // 0x0073ACC0
    bool AddMode(long modeType);
};

CModeBase* AllocateModeObject_standin(long modeType);   // stand-in callees of the stand-in body below
SCountBlock* AllocateModeCount_standin();
// Stand-in body (the real AllocateMode is out of line in retail): it writes only the returned pointer, so VC7.1
// knows the local's address does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) CModePointer CTCCreatureModeManager::AllocateMode(long modeType)
{
    return CModePointer(AllocateModeObject_standin(modeType), AllocateModeCount_standin());
}
#pragma auto_inline(on)

bool CTCCreatureModeManager::AddMode(long modeType)
{
    CModePointer mode = AllocateMode(modeType);
    return AddMode(mode);
}

// ---- harness ----
struct CModeBase { long type; };
static CModeBase g_mode;
static int g_destroyed = 0, g_addCalls = 0, g_seenRefs = 0;
static long g_seenType = -1;
static bool g_addResult = true, g_keep = true;
static void __fastcall DestroyMode(void* p) { if (p == &g_mode) ++g_destroyed; }
static SCountBlock* g_lastBlock = 0;
CModeBase* AllocateModeObject_standin(long modeType) { g_mode.type = modeType; return &g_mode; }
SCountBlock* AllocateModeCount_standin()
{
    SCountBlock* b = new SCountBlock;
    b->m_references = 1; b->m_pfnDestroy = DestroyMode; b->m_pObject = &g_mode;
    g_lastBlock = b;
    return b;
}
bool CTCCreatureModeManager::AddMode(CModePointer mode)
{
    ++g_addCalls;
    g_seenRefs = mode.m_pCount ? mode.m_pCount->m_references : -1;
    g_seenType = mode.m_pObject ? mode.m_pObject->type : -1;
    if (g_keep && mode.m_pCount) ++mode.m_pCount->m_references;   // keep one reference so the test sees the hand-off survive
    return g_addResult;
}
int main()
{
    CTCCreatureModeManager mgr = {};
    bool r = mgr.AddMode(7L);
    CHECK(r == true);
    CHECK(g_addCalls == 1);
    CHECK(g_seenRefs == 2);          // local + the add-ref'd by-value copy
    CHECK(g_seenType == 7);
    CHECK(g_destroyed == 0);         // the callee kept a reference
    CHECK(g_lastBlock && g_lastBlock->m_references == 1);
    // now the callee keeps nothing: the local's release destroys the mode and frees the block
    g_addResult = false;
    g_lastBlock = 0;
    {
        // second manager call where AddMode drops its extra reference afterwards
        bool r2 = mgr.AddMode(3L);
        CHECK(r2 == false);
        CHECK(g_seenRefs == 2);
        CHECK(g_seenType == 3);
        CHECK(g_lastBlock && g_lastBlock->m_references == 1);
        // release the reference AddMode kept: last reference -> destroy + delete
        SCountBlock* b = g_lastBlock;
        if (--b->m_references == 0) { b->m_pfnDestroy(b->m_pObject); delete b; }
        CHECK(g_destroyed == 1);
    }
    // the callee keeps nothing: its argument dies in the callee, the local's release destroys the mode and frees the block
    g_keep = false; g_addResult = true;
    CHECK(mgr.AddMode(9L) == true);
    CHECK(g_seenRefs == 2 && g_seenType == 9);
    CHECK(g_destroyed == 2);
    CHECK(g_addCalls == 3);
    if (fails == 0) printf("0073ae10_TEST PASS\n");
    return fails;
}
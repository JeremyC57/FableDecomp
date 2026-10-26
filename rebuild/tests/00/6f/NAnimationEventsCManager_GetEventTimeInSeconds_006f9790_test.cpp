// Behaviour test for 006f9790: src/006f9790.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. NAnimationEvents::CManager::GetEventTimeInSeconds(long animation, long event)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NAnimationEvents::CManager::GetEventTimeInSeconds(long animation, long event) -- the time of an animation event in seconds: the event's frame (from the embedded events when they are enabled and the animation carries the event, else from the event table) times the animation stats' seconds-per-frame, the counted stats released afterwards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F9790, length 141 bytes.
// Callees: the bank's GetAnimation @ 0x00A242C0, C3DAnimationInfo::GetAnimationStats @ 0x00A269C0 (counted pointer by hidden result),
//   AnimHasEmbeddedEvent @ 0x006F90D0, the embedded-event frame @ 0x006F9130, the event-table frame @ 0x006F96C0, operator delete @ 0x00BFE9BC.
// Layout: as the landed 0x006F91E0 / 0x006F9720 (bank +0x04, embedded-events flag +0x3d); C3DAnimationStats seconds-per-frame float +0x0c.
// Shape: the product is held in a `double` local and returned as `(float)seconds`: VC7.1 then stores it into the dead
//   `animation` argument slot (`fst [esp+18h]`) before popping edi/esi; a float local or the product returned directly
//   pops first.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class C3DAnimationStats { public:
    unsigned char _pad[0xc];
    float         m_secondsPerFrame;               // +0x0c
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(const C3DAnimationStats*);
    const C3DAnimationStats* m_pObject;
};
struct CCountedPointer_C3DAnimationStats {
    const C3DAnimationStats* m_pObject;
    SCountBlock*             m_pCount;
    __forceinline ~CCountedPointer_C3DAnimationStats()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
    const C3DAnimationStats* operator->() const { return m_pObject; }
};
class C3DAnimationInfo { public:
    CCountedPointer_C3DAnimationStats GetAnimationStats();   // 0x00A269C0
};
struct CAnimationBank {
    C3DAnimationInfo* GetAnimation(long animation);   // 0x00A242C0
};
struct CManager {
    void*           __vftable;
    CAnimationBank* m_pBank;                       // +0x04
    unsigned char   _pad08[0x35];
    bool            m_useEmbeddedEvents;           // +0x3d
    bool  AnimHasEmbeddedEvent(long animation, long event);     // 0x006F90D0
    float GetEmbeddedEventFrame(long animation, long event);    // 0x006F9130
    float GetEventFrame(long animation, long event);            // 0x006F96C0
    float GetEventTimeInSeconds(long animation, long event);
};
float CManager::GetEventTimeInSeconds(long animation, long event)
{
    CCountedPointer_C3DAnimationStats stats = m_pBank->GetAnimation(animation)->GetAnimationStats();
    float frame;
    if (m_useEmbeddedEvents && AnimHasEmbeddedEvent(animation, event))
        frame = GetEmbeddedEventFrame(animation, event);
    else
        frame = GetEventFrame(animation, event);
    double seconds = frame * stats->m_secondsPerFrame;
    return (float)seconds;
}

// ---- stubs and driver ----
static C3DAnimationStats g_stats; static int g_destroyed = 0; static SCountBlock* g_block = 0;
static bool g_frameFetched = false;
static void __fastcall DestroyStats(const C3DAnimationStats* s) { if (s == &g_stats && g_frameFetched) ++g_destroyed; }
static C3DAnimationInfo g_info; static long g_lastAnim = -1;
C3DAnimationInfo* CAnimationBank::GetAnimation(long animation) { g_lastAnim = animation; return &g_info; }
CCountedPointer_C3DAnimationStats C3DAnimationInfo::GetAnimationStats()
{
    // two references: the local below is destroyed when it is copied out (no NRVO in VC7.1), the caller holds the other
    g_block = new SCountBlock; g_block->m_references = 2; g_block->m_pfnDestroy = DestroyStats; g_block->m_pObject = &g_stats;
    CCountedPointer_C3DAnimationStats r; r.m_pObject = &g_stats; r.m_pCount = g_block;
    return r;
}
static bool g_has = false; static int g_embCalls = 0, g_tableCalls = 0, g_hasCalls = 0;
bool CManager::AnimHasEmbeddedEvent(long animation, long event) { ++g_hasCalls; return g_has && animation == 5 && event == 7; }
float CManager::GetEmbeddedEventFrame(long animation, long event) { ++g_embCalls; g_frameFetched = true; return 12.0f; }
float CManager::GetEventFrame(long animation, long event) { ++g_tableCalls; g_frameFetched = true; return 30.0f; }
int main()
{
    g_stats.m_secondsPerFrame = 0.5f;
    CManager m; CAnimationBank bank; m.m_pBank = &bank; m.m_useEmbeddedEvents = false;
    g_frameFetched = false; float t = m.GetEventTimeInSeconds(5, 7);
    CHECK(t == 15.0f && g_tableCalls == 1 && g_hasCalls == 0 && g_lastAnim == 5 && g_destroyed == 1);
    m.m_useEmbeddedEvents = true; g_has = true;
    g_frameFetched = false; t = m.GetEventTimeInSeconds(5, 7);
    CHECK(t == 6.0f && g_embCalls == 1 && g_hasCalls == 1 && g_destroyed == 2);
    g_has = false;
    g_frameFetched = false; t = m.GetEventTimeInSeconds(5, 7);
    CHECK(t == 15.0f && g_tableCalls == 2 && g_hasCalls == 2 && g_destroyed == 3);
    if (fails == 0) printf("006f9790_TEST PASS\n");
    return fails;
}

// Test: CSoundEventManager::GetPhysicalPlayer -- a free slot is returned at once; otherwise the entry found for the priority yields its player and the slot is fetched again; no entry gives 0.
#include <cstdio>
#include <cstring>
static char g_seq[16];
struct CManager;
struct CEvent { void YieldPhysicalPlayer(CManager*); };
struct CEntry { int pad; CEvent* m_pEvent; };
struct CSet { int freeSlots; CEntry* entry; int slot;
    void* GetFreeSlot() { std::strcat(g_seq, "G"); if (freeSlots) { --freeSlots; return &slot; } return 0; }
    CEntry* FindEntry(unsigned long) { std::strcat(g_seq, "F"); return entry; } };
struct CManager { CSet m_eventSet;
    void* GetPhysicalPlayer(unsigned long priority) {
        void* slot = m_eventSet.GetFreeSlot(); if (slot) return slot;
        CEntry* e = m_eventSet.FindEntry(priority); if (!e) return 0;
        e->m_pEvent->YieldPhysicalPlayer(this); return m_eventSet.GetFreeSlot(); } };
void CEvent::YieldPhysicalPlayer(CManager* m) { std::strcat(g_seq, "Y"); m->m_eventSet.freeSlots = 1; }
int main()
{
    CEvent ev; CEntry en; en.m_pEvent = &ev;
    CManager m; m.m_eventSet.freeSlots = 1; m.m_eventSet.entry = &en;
    bool ok = m.GetPhysicalPlayer(3) == &m.m_eventSet.slot && !std::strcmp(g_seq, "G");
    g_seq[0] = 0; ok = ok && m.GetPhysicalPlayer(3) == &m.m_eventSet.slot && !std::strcmp(g_seq, "GFYG");
    g_seq[0] = 0; m.m_eventSet.entry = 0; ok = ok && m.GetPhysicalPlayer(3) == 0 && !std::strcmp(g_seq, "GF");
    std::printf(ok ? "00c10c90_TEST PASS\n" : "00c10c90_TEST FAIL\n");
    return ok ? 0 : 1;
}
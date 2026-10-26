// Behaviour test for CTCCreatureModeManager::RemoveMode (0x0073ABA0): removing the current mode's type calls OnLeave on it,
// removes all of that type, resumes the new front and refreshes the look component; removing a background type only
// removes it; the idle timer is always frame + fps*30.
#include <cstdio>
#include <cstring>
long g_framesPerSecond = 15;
static long g_frame = 1000;
static char g_log[32];
static void Log(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CWorld { public: static long GetWorldFrame() { return g_frame; } };
class CTCLook { public: int Updates; void UpdateTurningAnims() { ++Updates; Log('u'); } };
struct CModeBase {
    unsigned long Type; char Name;
    virtual unsigned long GetModeType() { return Type; }
    virtual void OnResume() { Log('R'); Log(Name); }
    virtual void OnLeave() { Log('L'); Log(Name); }
    struct CIsType {
        unsigned long Type;
        CIsType(unsigned long type) : Type(type) { }
    };
};
struct SCountBlock;
struct CModePointer {
    CModeBase*   m_pObject;
    SCountBlock* m_pCount;
    CModeBase* operator->() const { return m_pObject; }
};
struct SModeNode {
    SModeNode*   m_pNext;
    SModeNode*   m_pPrev;
    CModePointer m_data;
};
struct CModeList {
    SModeNode* m_pHead;
    bool empty() const { return m_pHead->m_pNext == m_pHead; }
    CModePointer& front() { return m_pHead->m_pNext->m_data; }
    void remove_if(CModeBase::CIsType predicate)
    {
        for (SModeNode* n = m_pHead->m_pNext; n != m_pHead; ) {
            SModeNode* next = n->m_pNext;
            if (n->m_data.m_pObject->Type == predicate.Type) { n->m_pPrev->m_pNext = next; next->m_pPrev = n->m_pPrev; Log('x'); }
            n = next;
        }
    }
};
class CTCCreatureModeManager { public:
    virtual void v00() { }
    CThing* ParentThing;
    CModeList PModes;
    long TimeLeftForCurrentMode;
    long TimeForNextIdleMode;
    void RemoveMode(unsigned long modeType);
};

void CTCCreatureModeManager::RemoveMode(unsigned long modeType)
{
    bool removingCurrent = false;
    if (!PModes.empty() && PModes.front()->GetModeType() == modeType) {
        removingCurrent = true;
        PModes.front()->OnLeave();
    }
    PModes.remove_if(CModeBase::CIsType(modeType));
    if (removingCurrent && !PModes.empty()) {
        PModes.front()->OnResume();
        if (ParentThing->HasTC(0x43))
            ((CTCLook*)ParentThing->GetTC(0x43))->UpdateTurningAnims();
    }
    long idleDelay = (long)(g_framesPerSecond * 30.0f);
    TimeForNextIdleMode = CWorld::GetWorldFrame() + idleDelay;
}

int main()
{
    int bad = 0;
    CTCLook look; look.Updates = 0;
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    CInterfaceMap::SEntry e[1] = { { 0x43, (CTCBase*)&look } };
    thing.m_interfaces.first = e; thing.m_interfaces.last = e + 1; thing.m_presentBits[2] = 8;
    CModeBase a, b, c; a.Type = 4; a.Name = 'a'; b.Type = 9; b.Name = 'b'; c.Type = 4; c.Name = 'c';
    SModeNode head, na, nb, nc;
    head.m_pNext = &na; na.m_pNext = &nb; nb.m_pNext = &nc; nc.m_pNext = &head;
    head.m_pPrev = &nc; nc.m_pPrev = &nb; nb.m_pPrev = &na; na.m_pPrev = &head;
    na.m_data.m_pObject = &a; nb.m_data.m_pObject = &b; nc.m_data.m_pObject = &c;
    CTCCreatureModeManager m; m.ParentThing = &thing; m.PModes.m_pHead = &head; m.TimeForNextIdleMode = 0;
    g_log[0] = 0;
    m.RemoveMode(4);                                 // current type: leave a, drop a and c, resume b, refresh look
    if (std::strcmp(g_log, "LaxxRbu") || head.m_pNext != &nb || nb.m_pNext != &head || m.TimeForNextIdleMode != 1000 + 450 || look.Updates != 1) bad |= 1;
    g_log[0] = 0; g_frame = 2000; g_framesPerSecond = 10;
    m.RemoveMode(4);                                 // nothing of that type left: no hooks
    if (g_log[0] || m.TimeForNextIdleMode != 2300) bad |= 2;
    g_log[0] = 0;
    m.RemoveMode(9);                                 // removes the last mode: leave, no resume on the empty list
    if (std::strcmp(g_log, "Lbx") || !m.PModes.empty() || look.Updates != 1) bad |= 4;
    g_log[0] = 0;
    m.RemoveMode(1);                                 // empty list
    if (g_log[0]) bad |= 8;
    if (bad) { std::printf("0073aba0_TEST FAIL\n"); return 1; }
    std::printf("0073aba0_TEST PASS\n");
    return 0;
}
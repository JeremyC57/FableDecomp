#include <cstdio>

// Model of CCombatActionBase::IsActionWithStrikeEventHelperName @ 0x00857150

struct CStrikeEvent { bool Legacy; bool PrimaryLegacy; bool Weapon; void* HelperPointName; long StrikeEventIndex; long StartFrame; long EndFrame; };
struct CStrikeEventNode { CStrikeEventNode* m_pNext; CStrikeEventNode* m_pPrev; CStrikeEvent* m_pEvent; };
struct CStrikeEventIterator { CStrikeEventNode* m_pNode; CStrikeEventIterator(CStrikeEventNode* p) : m_pNode(p) { }
    bool operator!=(const CStrikeEventIterator& o) const { return m_pNode != o.m_pNode; } bool operator==(const CStrikeEventIterator& o) const { return m_pNode == o.m_pNode; }
    CStrikeEventIterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; } CStrikeEvent* operator*() const { return m_pNode->m_pEvent; } };
struct CStrikeEventList { CStrikeEventNode* m_pHead; long m_size; CStrikeEventIterator begin() const { return CStrikeEventIterator(m_pHead->m_pNext); } CStrikeEventIterator end() const { return CStrikeEventIterator(m_pHead); } };
static void Link(CStrikeEventNode* head, CStrikeEventNode* nodes, int n) { head->m_pNext = n ? &nodes[0] : head; head->m_pPrev = n ? &nodes[n-1] : head; for (int i = 0; i < n; ++i) { nodes[i].m_pNext = (i + 1 < n) ? &nodes[i+1] : head; nodes[i].m_pPrev = i ? &nodes[i-1] : head; } }

struct CGetActiveStrikeEventThatIsFirstToFinish {
    long m_currentFrame; CStrikeEvent* m_pFirstToFinish; long m_earliestEndFrame;
    CGetActiveStrikeEventThatIsFirstToFinish(long f) : m_currentFrame(f), m_pFirstToFinish(0), m_earliestEndFrame(1000000) { }
    void operator()(CStrikeEvent* e) { if (e->StartFrame <= m_currentFrame && m_currentFrame <= e->EndFrame && e->EndFrame < m_earliestEndFrame) { m_pFirstToFinish = e; m_earliestEndFrame = e->EndFrame; } }
};
static CGetActiveStrikeEventThatIsFirstToFinish for_each(CStrikeEventIterator first, CStrikeEventIterator last, CGetActiveStrikeEventThatIsFirstToFinish f) { for (; first != last; ++first) f(*first); return f; }
struct CCombatActionBaseCore {
    virtual void v00() {} unsigned char _pad04[0x1c - 4]; long FramesRunning; unsigned char _pad20[0xbc - 0x20]; CStrikeEventList StrikeEvents; unsigned char _padc4[0x114 - 0xc4];
    CStrikeEvent* GetPActiveStrikeEvent() const { return for_each(StrikeEvents.begin(), StrikeEvents.end(), CGetActiveStrikeEventThatIsFirstToFinish(FramesRunning)).m_pFirstToFinish; }
};
static void Setup(CCombatActionBaseCore& a, CStrikeEventNode* head, CStrikeEventNode* nodes, CStrikeEvent* e, int n)
{ for (int i = 0; i < n; ++i) { nodes[i].m_pEvent = &e[i]; e[i].Legacy = (i == 0); e[i].StrikeEventIndex = i + 1; e[i].StartFrame = 0; e[i].EndFrame = 10 * (n - i); } Link(head, nodes, n); a.StrikeEvents.m_pHead = head; a.StrikeEvents.m_size = n; a.FramesRunning = 5; }

struct CCombatActionBase : CCombatActionBaseCore { bool IsActionWithStrikeEventHelperName() const { CStrikeEvent* p = GetPActiveStrikeEvent(); if (p != 0) { if (!p->Legacy) return true; } return false; } };
int main()
{
    CStrikeEventNode head, nodes[3]; CStrikeEvent e[3]; CCombatActionBase a; Setup(a, &head, nodes, e, 3);   // active = e[2], not legacy
    if (!a.IsActionWithStrikeEventHelperName()) { std::printf("00857150_TEST FAIL\n"); return 1; }
    e[2].Legacy = true; if (a.IsActionWithStrikeEventHelperName()) { std::printf("00857150_TEST FAIL\n"); return 1; }
    Setup(a, &head, nodes, e, 0); if (a.IsActionWithStrikeEventHelperName()) { std::printf("00857150_TEST FAIL\n"); return 1; }
    std::printf("00857150_TEST PASS\n"); return 0;
}

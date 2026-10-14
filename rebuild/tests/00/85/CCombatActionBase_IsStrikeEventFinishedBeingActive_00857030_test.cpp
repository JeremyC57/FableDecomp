#include <cstdio>

// Model of CCombatActionBase::IsStrikeEventFinishedBeingActive @ 0x00857030

struct CStrikeEvent { bool Legacy; bool PrimaryLegacy; bool Weapon; void* HelperPointName; long StrikeEventIndex; long StartFrame; long EndFrame; };
struct CStrikeEventNode { CStrikeEventNode* m_pNext; CStrikeEventNode* m_pPrev; CStrikeEvent* m_pEvent; };
struct CStrikeEventIterator { CStrikeEventNode* m_pNode; CStrikeEventIterator(CStrikeEventNode* p) : m_pNode(p) { }
    bool operator!=(const CStrikeEventIterator& o) const { return m_pNode != o.m_pNode; } bool operator==(const CStrikeEventIterator& o) const { return m_pNode == o.m_pNode; }
    CStrikeEventIterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; } CStrikeEvent* operator*() const { return m_pNode->m_pEvent; } };
struct CStrikeEventList { CStrikeEventNode* m_pHead; long m_size; CStrikeEventIterator begin() const { return CStrikeEventIterator(m_pHead->m_pNext); } CStrikeEventIterator end() const { return CStrikeEventIterator(m_pHead); } };
static void Link(CStrikeEventNode* head, CStrikeEventNode* nodes, int n) { head->m_pNext = n ? &nodes[0] : head; head->m_pPrev = n ? &nodes[n-1] : head; for (int i = 0; i < n; ++i) { nodes[i].m_pNext = (i + 1 < n) ? &nodes[i+1] : head; nodes[i].m_pPrev = i ? &nodes[i-1] : head; } }

struct CIsStrikeEventActiveAfterFrame { long m_frame; CIsStrikeEventActiveAfterFrame(long f) : m_frame(f) { } bool operator()(const CStrikeEvent* e) const { return e->EndFrame > m_frame; } };
template<class I, class P> I find_if(I first, I last, P pred) { for (; first != last; ++first) { if (pred(*first)) break; } return first; }
struct CCombatActionBase {
    virtual void v00() {} unsigned char _pad04[0x1c - 4]; long FramesRunning; unsigned char _pad20[0xbc - 0x20]; CStrikeEventList StrikeEvents; unsigned char _padc4[0x114 - 0xc4];
    bool IsStrikeEventFinishedBeingActive() const {
        if (StrikeEvents.m_size <= 0) return false;
        CStrikeEventIterator it = find_if(StrikeEvents.begin(), StrikeEvents.end(), CIsStrikeEventActiveAfterFrame(FramesRunning));
        bool finished = (it == StrikeEvents.end()); return finished;
    }
};
int main()
{
    CStrikeEventNode head, nodes[2]; CStrikeEvent e0, e1; e0.EndFrame = 10; e1.EndFrame = 20; nodes[0].m_pEvent = &e0; nodes[1].m_pEvent = &e1;
    CCombatActionBase a; a.StrikeEvents.m_pHead = &head; Link(&head, nodes, 0); a.StrikeEvents.m_size = 0; a.FramesRunning = 100;
    if (a.IsStrikeEventFinishedBeingActive()) { std::printf("00857030_TEST FAIL\n"); return 1; }
    Link(&head, nodes, 2); a.StrikeEvents.m_size = 2;
    if (!a.IsStrikeEventFinishedBeingActive()) { std::printf("00857030_TEST FAIL\n"); return 1; }
    a.FramesRunning = 15; if (a.IsStrikeEventFinishedBeingActive()) { std::printf("00857030_TEST FAIL\n"); return 1; }
    a.FramesRunning = 20; if (!a.IsStrikeEventFinishedBeingActive()) { std::printf("00857030_TEST FAIL\n"); return 1; }
    std::printf("00857030_TEST PASS\n"); return 0;
}
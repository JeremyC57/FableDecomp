#include <cstdio>

static long g_worldFrame = 10; static int g_cleared = 0, g_listCleared = 0;
static long GetWorldFrame() { return g_worldFrame; }
struct CAGraphicAppearanceComponent {
    virtual ~CAGraphicAppearanceComponent() { }
    virtual void ClearPrimitives() { ++g_cleared; }
};
struct CComponentListNode { CComponentListNode* m_next; CComponentListNode* m_prev; CAGraphicAppearanceComponent* m_value; };
struct CPrimitiveListObject { void* m_head; void clear() { ++g_listCleared; } };
struct CComponentListObject {
    CComponentListNode* m_head;
    void remove(CAGraphicAppearanceComponent* const& value)
    {
        for (CComponentListNode* n = m_head->m_next; n != m_head; ) {
            CComponentListNode* next = n->m_next;
            if (n->m_value == value) { n->m_prev->m_next = next; next->m_prev = n->m_prev; }
            n = next;
        }
    }
};
struct CTCGraphicAppearance {
    CPrimitiveListObject Primitives;
    CComponentListObject Components;
    long FrameUpdatedWasRenderedFlag;
    void ClearAllPrimitives()
    {
        for (CComponentListNode* node = Components.m_head->m_next; node != Components.m_head; node = node->m_next)
            node->m_value->ClearPrimitives();
        Primitives.clear();
        FrameUpdatedWasRenderedFlag = GetWorldFrame() + 1;
    }
    void UnregisterComponent(const CAGraphicAppearanceComponent* component)
    {
        ClearAllPrimitives();
        Components.remove(const_cast<CAGraphicAppearanceComponent*>(component));
    }
};

int main()
{
    CAGraphicAppearanceComponent c1, c2;
    CComponentListNode head, n1, n2;
    head.m_next = &n1; n1.m_prev = &head; n1.m_next = &n2; n2.m_prev = &n1; n2.m_next = &head; head.m_prev = &n2;
    n1.m_value = &c1; n2.m_value = &c2;
    CTCGraphicAppearance a; a.Components.m_head = &head; a.FrameUpdatedWasRenderedFlag = 0;
    a.UnregisterComponent(&c1);
    if (g_cleared != 2 || g_listCleared != 1 || a.FrameUpdatedWasRenderedFlag != 11 || head.m_next != &n2 || n2.m_prev != &head)
        { std::printf("004c0000_TEST FAIL\n"); return 1; }
    std::printf("004c0000_TEST PASS\n");
    return 0;
}

#include <cstdio>

static int g_render = 0, g_cleared = 0;
struct CThing { void AddToRenderList() { ++g_render; } };
struct CListNode { CListNode* m_next; CListNode* m_prev; };
struct CPrimitiveList { CListNode* m_head; bool empty() const { return m_head->m_next == m_head; } };
struct CMessage;
struct CTCGraphicAppearance {
    CThing* ParentThing;
    CPrimitiveList Primitives;
    unsigned char UpdatePrimitiveReasons;
    void ClearAllPrimitives() { ++g_cleared; }
    void SetAsChanged(unsigned char reasons)
    {
        ParentThing->AddToRenderList();
        if ((reasons & 8) && !(UpdatePrimitiveReasons & 8) && !Primitives.empty())
            ClearAllPrimitives();
        UpdatePrimitiveReasons |= reasons;
    }
    void CatchMessage(const CMessage*) { SetAsChanged(8); }
};

int main()
{
    CThing t; CListNode head, node; head.m_next = &node; node.m_next = &head;
    CTCGraphicAppearance a; a.ParentThing = &t; a.Primitives.m_head = &head; a.UpdatePrimitiveReasons = 0;
    a.CatchMessage(0);                                               // first 8: primitives cleared
    if (g_render != 1 || g_cleared != 1 || a.UpdatePrimitiveReasons != 8) { std::printf("004bfce0_TEST FAIL\n"); return 1; }
    a.CatchMessage(0);                                               // already pending: not cleared again
    if (g_render != 2 || g_cleared != 1) { std::printf("004bfce0_TEST FAIL\n"); return 1; }
    a.UpdatePrimitiveReasons = 0; head.m_next = &head;   // empty list: nothing to clear
    a.CatchMessage(0);
    if (g_render != 3 || g_cleared != 1 || a.UpdatePrimitiveReasons == 0) { std::printf("004bfce0_TEST FAIL\n"); return 1; }
    std::printf("004bfce0_TEST PASS\n");
    return 0;
}
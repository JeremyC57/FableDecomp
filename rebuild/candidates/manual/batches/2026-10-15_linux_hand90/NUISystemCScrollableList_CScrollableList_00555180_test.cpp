#include <cstdio>
#include <cstring>
#include <new>
// Behaviour of NUISystem::CScrollableList::CScrollableList (0x00555180): base built with the argument, the scroll
// state (three dwords, two flags) cleared, the layout block left alone.
static long g_baseState = 0;
struct CComponentRoot_Retail { virtual long v00() { return 0; } };
struct CComponentBase2_Retail { virtual long v00() { return 0; } unsigned char _body[0x10]; };
struct CComponentBase3_Retail { virtual long v00() { return 0; } };
struct CChangingStateComponent : CComponentRoot_Retail, CComponentBase2_Retail, CComponentBase3_Retail {
    long m_state;
    CChangingStateComponent(long state) : m_state(state) { g_baseState = state; }
};
struct CScrollableList : CChangingStateComponent {
    unsigned char m_layout[0x10];
    long m_firstVisible;
    long m_selected;
    long m_count;
    bool m_scrollUp;
    bool m_scrollDown;
    virtual long v00() { return 2; }
    CScrollableList(long state)
        : CChangingStateComponent(state), m_firstVisible(0), m_selected(0), m_count(0),
          m_scrollUp(false), m_scrollDown(false)
    {
    }
};
union UStorage { double d; void* p; unsigned char bytes[sizeof(CScrollableList)]; };
int main()
{
    UStorage storage;
    std::memset(storage.bytes, 0xCD, sizeof(storage.bytes));
    CScrollableList* list = new (storage.bytes) CScrollableList(9);
    if (g_baseState != 9 || list->m_firstVisible != 0 || list->m_selected != 0 || list->m_count != 0
        || list->m_scrollUp || list->m_scrollDown || list->m_layout[0] != 0xCD || list->v00() != 2) {
        std::printf("00555180_TEST FAIL\n"); return 1;
    }
    std::printf("00555180_TEST PASS\n");
    return 0;
}

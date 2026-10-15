// Behaviour test for NUISystem::CDynamicList::ScrollSelectionDown @ 0x00540EC0
#include <cstdio>
struct CComponent {
    long m_state;
    virtual void SetState(long state) { m_state = state; }
};
template <class T> struct CRetailVector {
    T* m_first; T* m_last; T* m_endOfStorage;
    T* begin() const { return m_first; }
    unsigned long size() const { return (unsigned long)(m_last - m_first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CDynamicList {
    long TopIndex, SelectionIndex, NumVisible;
    CRetailVector<CComponent*> ScrollingChildren;
    int m_resets;
    virtual void ResetPositions() { ++m_resets; }
    void ScrollSelectionDown()
    {
        if (SelectionIndex >= 0 && (unsigned long)(SelectionIndex + 1) < ScrollingChildren.size()) {
            ScrollingChildren[SelectionIndex]->SetState(4);
            ++SelectionIndex;
            ScrollingChildren[SelectionIndex]->SetState(3);
            if (SelectionIndex >= TopIndex + NumVisible) {
                TopIndex = SelectionIndex - NumVisible + 1;
                ResetPositions();
            }
        }
    }
};
int main()
{
    CComponent c[4];
    CComponent* p[4];
    int i;
    for (i = 0; i < 4; ++i) { c[i].m_state = 0; p[i] = &c[i]; }
    CDynamicList l;
    l.ScrollingChildren.m_first = p; l.ScrollingChildren.m_last = p + 4; l.ScrollingChildren.m_endOfStorage = p + 4;
    l.TopIndex = 0; l.SelectionIndex = 0; l.NumVisible = 2; l.m_resets = 0;
    l.ScrollSelectionDown();
    if (l.SelectionIndex != 1 || c[0].m_state != 4 || c[1].m_state != 3 || l.TopIndex != 0 || l.m_resets != 0) { std::printf("00540ec0_TEST FAIL\n"); return 1; }
    l.ScrollSelectionDown();
    if (l.SelectionIndex != 2 || c[2].m_state != 3 || l.TopIndex != 1 || l.m_resets != 1) { std::printf("00540ec0_TEST FAIL\n"); return 1; }
    l.ScrollSelectionDown();
    if (l.SelectionIndex != 3 || l.TopIndex != 2 || l.m_resets != 2) { std::printf("00540ec0_TEST FAIL\n"); return 1; }
    l.ScrollSelectionDown();   // last child: no move
    if (l.SelectionIndex != 3 || l.m_resets != 2) { std::printf("00540ec0_TEST FAIL\n"); return 1; }
    l.SelectionIndex = -1;
    l.ScrollSelectionDown();   // no selection: no move
    if (l.SelectionIndex != -1) { std::printf("00540ec0_TEST FAIL\n"); return 1; }
    std::printf("00540ec0_TEST PASS\n");
    return 0;
}
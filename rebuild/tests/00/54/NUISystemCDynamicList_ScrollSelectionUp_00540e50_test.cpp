// Behaviour test for NUISystem::CDynamicList::ScrollSelectionUp @ 0x00540E50
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
    void ScrollSelectionUp()
    {
        if (SelectionIndex != 0) {
            ScrollingChildren[SelectionIndex]->SetState(4);
            --SelectionIndex;
            ScrollingChildren[SelectionIndex]->SetState(3);
            if (SelectionIndex < TopIndex) {
                TopIndex = SelectionIndex;
                ResetPositions();
            }
        }
    }
};
int main()
{
    CComponent c[5];
    CComponent* p[5];
    int i;
    for (i = 0; i < 5; ++i) { c[i].m_state = 0; p[i] = &c[i]; }
    CDynamicList l;
    l.ScrollingChildren.m_first = p; l.ScrollingChildren.m_last = p + 5; l.ScrollingChildren.m_endOfStorage = p + 5;
    l.TopIndex = 2; l.SelectionIndex = 3; l.NumVisible = 2; l.m_resets = 0;
    l.ScrollSelectionUp();
    if (l.SelectionIndex != 2 || c[3].m_state != 4 || c[2].m_state != 3 || l.TopIndex != 2 || l.m_resets != 0) { std::printf("00540e50_TEST FAIL\n"); return 1; }
    l.ScrollSelectionUp();
    if (l.SelectionIndex != 1 || c[2].m_state != 4 || c[1].m_state != 3 || l.TopIndex != 1 || l.m_resets != 1) { std::printf("00540e50_TEST FAIL\n"); return 1; }
    l.SelectionIndex = 0; c[0].m_state = 7;
    l.ScrollSelectionUp();
    if (l.SelectionIndex != 0 || c[0].m_state != 7 || l.m_resets != 1) { std::printf("00540e50_TEST FAIL\n"); return 1; }
    std::printf("00540e50_TEST PASS\n");
    return 0;
}
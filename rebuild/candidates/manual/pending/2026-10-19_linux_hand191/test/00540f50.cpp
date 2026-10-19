// Behaviour test for NUISystem::CDynamicList::SetSelectionIndex (0x00540F50): clamping, deselect/select states and the
// scroll window update.
#include <cstdio>
struct CComponent { long state; virtual void SetDisplayState(long s) { state = s; } };
template <class T> struct CRetailVector {
    T* m_start; T* m_finish; T* m_end;
    bool empty() const { return m_start == m_finish; }
    unsigned long size() const { return (unsigned long)(m_finish - m_start); }
    T& operator[](unsigned long i) { return m_start[i]; }
};
class CDynamicList {
public:
    int refreshes;
    virtual void UpdateScrollPosition() { ++refreshes; }
    long TopIndex, SelectionIndex, NumVisible; CRetailVector<CComponent*> ScrollingChildren;
    void SetSelectionIndex(unsigned long index);
};
void CDynamicList::SetSelectionIndex(unsigned long index)
{
    if (ScrollingChildren.empty())
        return;
    if (index >= ScrollingChildren.size())
        index = ScrollingChildren.size() - 1;
    if (SelectionIndex == (long)index)
        return;
    if (SelectionIndex >= 0 && (unsigned long)SelectionIndex < ScrollingChildren.size())
        ScrollingChildren[SelectionIndex]->SetDisplayState(4);
    SelectionIndex = index;
    ScrollingChildren[index]->SetDisplayState(3);
    if (SelectionIndex < TopIndex) {
        TopIndex = SelectionIndex;
        UpdateScrollPosition();
    } else if (SelectionIndex >= TopIndex + NumVisible) {
        TopIndex = SelectionIndex - NumVisible + 1;
        UpdateScrollPosition();
    }
}


int main()
{
    CComponent c[6]; CComponent* p[6];
    for (int i = 0; i < 6; ++i) { c[i].state = 0; p[i] = &c[i]; }
    CDynamicList l; l.refreshes = 0; l.TopIndex = 0; l.SelectionIndex = -1; l.NumVisible = 3;
    l.ScrollingChildren.m_start = p; l.ScrollingChildren.m_finish = p + 6; l.ScrollingChildren.m_end = p + 6;
    int bad = 0;
    l.SetSelectionIndex(1);
    if (l.SelectionIndex != 1 || c[1].state != 3 || l.refreshes) bad = 1;
    l.SetSelectionIndex(4);   // scroll down: top = 4 - 3 + 1
    if (c[1].state != 4 || c[4].state != 3 || l.TopIndex != 2 || l.refreshes != 1) bad = 2;
    l.SetSelectionIndex(99);  // clamped to 5
    if (l.SelectionIndex != 5 || l.TopIndex != 3 || l.refreshes != 2) bad = 3;
    l.SetSelectionIndex(5);   // unchanged
    if (l.refreshes != 2) bad = 4;
    l.SetSelectionIndex(0);   // scroll up
    if (l.TopIndex != 0 || c[5].state != 4 || c[0].state != 3 || l.refreshes != 3) bad = 5;
    l.ScrollingChildren.m_finish = p; l.SelectionIndex = 7;
    l.SetSelectionIndex(2);   // empty list: nothing
    if (l.SelectionIndex != 7) bad = 6;
    if (bad) { std::printf("00540f50_TEST FAIL\n"); return 1; }
    std::printf("00540f50_TEST PASS\n");
    return 0;
}

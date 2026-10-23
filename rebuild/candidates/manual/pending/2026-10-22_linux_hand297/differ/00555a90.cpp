// NUISystem::CScrollableList::UpdateVisibleList(float, float) -- lay the list out for its current scroll offset: show or hide the up arrow, hide the items above the window, unhide and stack the visible window of items one spacing step apart, show or hide the down arrow, hide the items below, and remember the move time
// Grade: DIFFER(595v595) -- register colouring only: the visible-window loop builds the by-value C2DVector with X in edx / Y in ecx and the component in eax (retail) where ours picks ecx/edx/ecx; the first and last loops pick edx where retail picks eax. Every instruction, stack slot and branch otherwise matches.
// Leaf: UpdateVisibleList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00555A90, length 608 bytes, ret 0x08.
// Callees: HideComponent 0x00555440, UnhideComponent 0x005556F0 and the arrows' virtual GetPosition (slot 16, +0x40).
// Layout: Items vector<CListItem> (8-byte elements) +0xb0, FirstVisible +0x15c, VisibleCount +0x160, Spacing +0x164,
// LastMoveTime +0x16c, UpArrow +0x170, DownArrow +0x174, UpArrowShown +0x178, DownArrowShown +0x179.
namespace std {
template <class T> inline const T& min(const T& a, const T& b) { return b < a ? b : a; }
}
namespace NUISystem {
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
    C2DVector operator-() const { return C2DVector(-X, -Y); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
};
struct CComponent {
    virtual void c000(); virtual void c001(); virtual void c002(); virtual void c003(); virtual void c004();
    virtual void c005(); virtual void c006(); virtual void c007(); virtual void c008(); virtual void c009();
    virtual void c010(); virtual void c011(); virtual void c012(); virtual void c013(); virtual void c014();
    virtual void c015();
    virtual C2DVector GetPosition() const;   // slot 16 (+0x40)
};
struct CListItem {
    CComponent* Component;
    long        Data;
};
struct CItemVector {                 // STLport vector<CListItem> (12 bytes)
    CListItem* _M_start; CListItem* _M_finish; CListItem* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CListItem& operator[](unsigned int n) { return *(_M_start + n); }
};
struct CScrollableList {
    unsigned char _pad000[0xb0];
    CItemVector   Items;             // +0xb0
    unsigned char _pad0bc[0x15c - 0xbc];
    unsigned int  FirstVisible;      // +0x15c
    unsigned int  VisibleCount;      // +0x160
    C2DVector     Spacing;           // +0x164
    float         LastMoveTime;      // +0x16c
    CComponent*   UpArrow;           // +0x170
    CComponent*   DownArrow;         // +0x174
    bool          UpArrowShown;      // +0x178
    bool          DownArrowShown;    // +0x179
    void HideComponent(CComponent* component, float positionTime, float fadeTime, C2DVector position, unsigned char alpha);
    void UnhideComponent(CComponent* component, float positionTime, float fadeTime, C2DVector position);
    void UpdateVisibleList(float moveTime, float fadeTime);
};

void CScrollableList::UpdateVisibleList(float moveTime, float fadeTime)
{
    C2DVector hidden = -Spacing;
    unsigned int i = 0;
    if (FirstVisible == 0) {
        HideComponent(UpArrow, 0, fadeTime, UpArrow->GetPosition(), 0);
        UpArrowShown = false;
    } else {
        UnhideComponent(UpArrow, 0, fadeTime, UpArrow->GetPosition());
        UpArrowShown = true;
    }
    for (; i < FirstVisible && i < Items.size(); ++i)
        HideComponent(Items._M_start[i].Component, moveTime, fadeTime, hidden, 0);
    C2DVector pos;
    pos.X = hidden.X + Spacing.X;
    pos.Y = hidden.Y + Spacing.Y;
    unsigned int end = std::min(FirstVisible + VisibleCount, Items.size());
    for (; i < end; ++i) {
        UnhideComponent(Items._M_start[i].Component, moveTime, fadeTime, pos);
        pos += Spacing;
    }
    if (i < Items.size()) {
        UnhideComponent(DownArrow, 0, fadeTime, DownArrow->GetPosition());
        DownArrowShown = true;
    } else {
        HideComponent(DownArrow, 0, fadeTime, DownArrow->GetPosition(), 0);
        DownArrowShown = false;
    }
    for (; i < Items.size(); ++i)
        HideComponent(Items._M_start[i].Component, moveTime, fadeTime, pos, 0);
    LastMoveTime = moveTime;
}

}

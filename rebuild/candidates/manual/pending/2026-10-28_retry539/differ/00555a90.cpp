// NUISystem::CScrollableList::UpdateVisibleList(float, float) -- lay the list out for its current scroll offset: show or hide the up arrow, hide the children above the window, unhide and stack the visible window one offset step apart, show or hide the down arrow, hide the children below, and remember the scroll time
// Grade: DIFFER(595v595) -- residue (16 mnemonic diffs): register colouring only (eax/edx/ecx choices in the three child loops and the by-value C2DVector argument copy, where retail stores Y before X); lane gap: register colouring | tried: real names (global C2DVector, Children vector<CCountedPointer<CComponent> > at +0xB0, the protected HideComponent / UnhideComponent leaves, std::min<unsigned long>), raw Children._M_start[i] element access (22 -> 16; the STLport begin()-based operator[] stays at 22), copy-ctor and position-construction forms, same-TU opaque Hide/Unhide bodies (inert); 1 RTM grade this pass | next: none in source
// Leaf: ?UpdateVisibleList@CScrollableList@NUISystem@@IAEXMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00555A90, 595 bytes, `ret 8`.
// Identity: Ego_r.pdb ?UpdateVisibleList@CScrollableList@NUISystem@@IAEXMM@Z (protected, two floats); its callees are the
//   class's own protected HideComponent 0x00555440 / UnhideComponent 0x005556F0 (Ego_r IAE, by-value C2DVector).
// Callees: HideComponent(CComponent*, float, float, C2DVector, unsigned char) 0x00555440, UnhideComponent(CComponent*, float,
//   float, C2DVector) 0x005556F0, CComponent::GetPosition() const (slot 16, +0x40, C2DVector by value).
// Layout (Ego_r names, retail offsets: STLport containers 12 bytes): CComponent::Children (vector<CCountedPointer<CComponent> >,
//   8-byte elements) +0xB0; CScrollableList TopOfList +0x15C, VisibleListItems +0x160, PositionOffset +0x164, ScrollTimer
//   +0x16C, PUpArrow +0x170, PDownArrow +0x174, UpArrowActive +0x178, DownArrowActive +0x179. Local model (no engine header
//   for CScrollableList; the CComponent base is modelled by its retail offsets).
namespace std {
template <class _Tp> inline const _Tp& min(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
};
}
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
    C2DVector operator-() const { return C2DVector(-X, -Y); }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                            // +0x00
    T* operator->() const { return Data; }
    CCPPointerInfo* Info;                            // +0x04
};
namespace NUISystem {
class CComponent {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual C2DVector GetPosition() const;           // slot 16 (+0x40)
    unsigned char _pad_0x04[0xAC];
    std::vector<CCountedPointer<CComponent> > Children;   // +0xB0
};
class CScrollableList : public CComponent {
public:
    unsigned char _pad_0xbc[0x15C - 0xBC];
    unsigned long TopOfList;                         // +0x15C
    unsigned long VisibleListItems;                  // +0x160
    C2DVector     PositionOffset;                    // +0x164
    float         ScrollTimer;                       // +0x16C
    CComponent*   PUpArrow;                          // +0x170
    CComponent*   PDownArrow;                        // +0x174
    bool          UpArrowActive;                     // +0x178
    bool          DownArrowActive;                   // +0x179
protected:
    void HideComponent(CComponent* component, float positionTime, float fadeTime, C2DVector position, unsigned char alpha);   // 0x00555440
    void UnhideComponent(CComponent* component, float positionTime, float fadeTime, C2DVector position);                       // 0x005556F0
    void UpdateVisibleList(float moveTime, float fadeTime);
};

void CScrollableList::UpdateVisibleList(float moveTime, float fadeTime)
{
    C2DVector hidden = -PositionOffset;
    unsigned long i = 0;
    if (TopOfList == 0) {
        HideComponent(PUpArrow, 0, fadeTime, PUpArrow->GetPosition(), 0);
        UpArrowActive = false;
    } else {
        UnhideComponent(PUpArrow, 0, fadeTime, PUpArrow->GetPosition());
        UpArrowActive = true;
    }
    for (; i < TopOfList && i < Children.size(); ++i)
        HideComponent(Children._M_start[i].Data, moveTime, fadeTime, hidden, 0);
    C2DVector pos;
    pos.X = hidden.X + PositionOffset.X;
    pos.Y = hidden.Y + PositionOffset.Y;
    unsigned long end = std::min<unsigned long>(TopOfList + VisibleListItems, Children.size());
    for (; i < end; ++i) {
        UnhideComponent(Children._M_start[i].Data, moveTime, fadeTime, pos);
        pos += PositionOffset;
    }
    if (i < Children.size()) {
        UnhideComponent(PDownArrow, 0, fadeTime, PDownArrow->GetPosition());
        DownArrowActive = true;
    } else {
        HideComponent(PDownArrow, 0, fadeTime, PDownArrow->GetPosition(), 0);
        DownArrowActive = false;
    }
    for (; i < Children.size(); ++i)
        HideComponent(Children._M_start[i].Data, moveTime, fadeTime, pos, 0);
    ScrollTimer = moveTime;
}

}

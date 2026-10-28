// NUISystem::CDynamicList::ResetPositions() -- lay the scrolling children out from the top index: the child at index i sits at (i - TopIndex) * PositionOffset in each of its six stored states and now, fully opaque when inside the visible window [TopIndex, TopIndex + NumVisible) and transparent otherwise
// Leaf: ?ResetPositions@CDynamicList@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005416B0, 379 bytes, thiscall, plain `ret`; CDynamicList vtable 0x01246E44 slot 142.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?ResetPositions@CDynamicList@NUISystem@@UAEXXZ (virtual void()); the vtable slot
//   (0x01246E44 + 142*4, the retry156 audit table) and the body's members are CDynamicList's: PositionOffset, TopIndex,
//   NumVisible and ScrollingChildren (Ego_r.pdb, here at retail +0x160 / +0x168 / +0x170 / +0x174, 0x30 below the donor).
// Callees: the child's virtual GetStates (slot 82, +0x148, Ego_r.pdb ?GetStates@CComponent@NUISystem@@UAEAAV?$map@KVCUIState...),
//   map<unsigned long, CUIState>::operator[](const unsigned long&) 0x0042D5B1, SetPosition(const C2DVector&) (slot 15),
//   GetColour() const (slot 14, CRGBColour by hidden pointer) and SetColour(const CRGBColour&) (slot 13).
// Data: the six state ids {0, 1, 4, 3, 5, 6} at 0x01246DFC (only reference: this function); name descriptive.
// Label correction: catalogued as "CDynamicList::ResetPositions" without the NUISystem namespace.
// Layout: local view -- no engine/ header for NUISystem::CDynamicList or CComponent (the PDB type dump lacks CComponent);
//   CUIState and CRGBColour members from Ego_r.pdb (Position +0x08, Colour +0x18, A its byte 3).
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector& operator+=(const C2DVector& other)
    {
        X += other.X;
        Y += other.Y;
        return *this;
    }
};
class CRGBColour {
public:
    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char A;
    CRGBColour() { }
};
namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(_M_start + __n); }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    _Tp& operator[](const _Key& __k);                       // 0x0042D5B1 (map<unsigned long, NUISystem::CUIState>)
    void*        _M_header;
    unsigned int _M_node_count;
};
}
namespace NUISystem {
enum EStateChangeType { };
class CUIState {                                            // 0x24 bytes
public:
    unsigned long    StateChangeFlag;                       // +0x00
    EStateChangeType StateChangeType;                       // +0x04
    C2DVector        Position;                              // +0x08
    C2DVector        Zoom;                                  // +0x10
    CRGBColour       Colour;                                // +0x18
    float            UpdateTime;                            // +0x1C
    bool             LinearChange;                          // +0x20
};
class CComponent {
public:
    virtual void Slot000();
    virtual void Slot001();
    virtual void Slot002();
    virtual void Slot003();
    virtual void Slot004();
    virtual void Slot005();
    virtual void Slot006();
    virtual void Slot007();
    virtual void Slot008();
    virtual void Slot009();
    virtual void Slot010();
    virtual void Slot011();
    virtual void Slot012();
    virtual void SetColour(const CRGBColour& colour);                   // slot 13 (+0x34)
    virtual CRGBColour GetColour() const;                               // slot 14 (+0x38)
    virtual void SetPosition(const C2DVector& position);                // slot 15 (+0x3C)
    virtual void Slot016();
    virtual void Slot017();
    virtual void Slot018();
    virtual void Slot019();
    virtual void Slot020();
    virtual void Slot021();
    virtual void Slot022();
    virtual void Slot023();
    virtual void Slot024();
    virtual void Slot025();
    virtual void Slot026();
    virtual void Slot027();
    virtual void Slot028();
    virtual void Slot029();
    virtual void Slot030();
    virtual void Slot031();
    virtual void Slot032();
    virtual void Slot033();
    virtual void Slot034();
    virtual void Slot035();
    virtual void Slot036();
    virtual void Slot037();
    virtual void Slot038();
    virtual void Slot039();
    virtual void Slot040();
    virtual void Slot041();
    virtual void Slot042();
    virtual void Slot043();
    virtual void Slot044();
    virtual void Slot045();
    virtual void Slot046();
    virtual void Slot047();
    virtual void Slot048();
    virtual void Slot049();
    virtual void Slot050();
    virtual void Slot051();
    virtual void Slot052();
    virtual void Slot053();
    virtual void Slot054();
    virtual void Slot055();
    virtual void Slot056();
    virtual void Slot057();
    virtual void Slot058();
    virtual void Slot059();
    virtual void Slot060();
    virtual void Slot061();
    virtual void Slot062();
    virtual void Slot063();
    virtual void Slot064();
    virtual void Slot065();
    virtual void Slot066();
    virtual void Slot067();
    virtual void Slot068();
    virtual void Slot069();
    virtual void Slot070();
    virtual void Slot071();
    virtual void Slot072();
    virtual void Slot073();
    virtual void Slot074();
    virtual void Slot075();
    virtual void Slot076();
    virtual void Slot077();
    virtual void Slot078();
    virtual void Slot079();
    virtual void Slot080();
    virtual void Slot081();
    virtual std::map<unsigned long, CUIState>& GetStates();             // slot 82 (+0x148)
};
extern const long DynamicListResetStates_01246dfc[6];       // 0x01246DFC {0, 1, 4, 3, 5, 6}
class CDynamicList {
public:
    virtual void ResetPositions();
    unsigned char _pad_0x04[0x160 - 0x04];
    C2DVector PositionOffset;                               // +0x160
    long      TopIndex;                                     // +0x168
    long      SelectionIndex;                               // +0x16C
    long      NumVisible;                                   // +0x170
    std::vector<CComponent*> ScrollingChildren;             // +0x174
};

void CDynamicList::ResetPositions()
{
    C2DVector position((float)-TopIndex * PositionOffset.X, (float)-TopIndex * PositionOffset.Y);
    for (unsigned long i = 0; i < ScrollingChildren.size(); ++i) {
        CComponent* child = ScrollingChildren[i];
        for (unsigned long s = 0; s < 6; ++s) {
            long state = DynamicListResetStates_01246dfc[s];
            child->GetStates()[state].Position = position;
            if (i >= (unsigned long)TopIndex && i < (unsigned long)(TopIndex + NumVisible))
                child->GetStates()[state].Colour.A = 0xff;
            else
                child->GetStates()[state].Colour.A = 0;
        }
        child->SetPosition(position);
        CRGBColour colour = child->GetColour();
        if (i >= (unsigned long)TopIndex && i < (unsigned long)(TopIndex + NumVisible))
            colour.A = 0xff;
        else
            colour.A = 0;
        child->SetColour(colour);
        position += PositionOffset;
    }
}
}   // namespace NUISystem
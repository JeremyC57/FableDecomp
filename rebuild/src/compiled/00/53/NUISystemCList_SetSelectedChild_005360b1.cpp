// NUISystem::CList::SetSelectedChild(long) -- select a child and, if it is outside the visible run of scrolling children, scroll the list by 25 units per row to bring it into view
// Leaf: ?SetSelectedChild@CList@NUISystem@@QAEXJ@Z (Ego_r.pdb UAEXJ @ 0x005B0359; virtual, modelled non-virtual)
// Retail: Fable.exe @ 0x005360B1, 216 bytes, `ret 4`.
// Body: SelectedChild = child; if child >= 0, child < ScrollingChildrenVisibility.size() and that child is not visible:
// find the first visible index and the last index of that first visible run (last = size-1 if the run reaches the end);
// if child > last scroll by C2DVector(0, (child-last) * -25.0f) (0x012462A4), else if a visible child exists scroll by
// C2DVector(0, (first-child) * 25.0f) (0x0123003C); the scroll is virtual slot 33 (+0x84, 0x0052EAF0) taking
// (const C2DVector&, float 0.0, bool false) -- one of Ego_r.pdb's CComponent::Change*Delta(const C2DVector&, float, bool)
// methods, modelled as ChangePositionDelta (not confirmed).
// Layout: retail CList = Ego_r.pdb CList - 0x30 after the STLport container shrink: SelectedChild +0x15c (Ego_r +0x18c),
// ScrollingChildrenVisibility +0x1c8 (Ego_r +0x204). Retail indexes the visibility vector as bytes
// (`cmp byte ptr [edi+edx], 0`), so it is modelled as a plain begin/end/eos vector of bool, not a bit vector.
// The generated engine/CList.h carries the Ego_r offsets, so a local layout is used.
// Opt: size TU (unaligned entry) with a frame pointer (`leave`).
#pragma optimize("s", on)
#pragma optimize("y", off)
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
class C2DVector {
public:
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
namespace NUISystem {
class CList {
public:
    virtual void _v00(); virtual void _v01(); virtual void _v02(); virtual void _v03(); virtual void _v04(); virtual void _v05();
    virtual void _v06(); virtual void _v07(); virtual void _v08(); virtual void _v09(); virtual void _v10(); virtual void _v11();
    virtual void _v12(); virtual void _v13(); virtual void _v14(); virtual void _v15(); virtual void _v16(); virtual void _v17();
    virtual void _v18(); virtual void _v19(); virtual void _v20(); virtual void _v21(); virtual void _v22(); virtual void _v23();
    virtual void _v24(); virtual void _v25(); virtual void _v26(); virtual void _v27(); virtual void _v28(); virtual void _v29();
    virtual void _v30(); virtual void _v31(); virtual void _v32();
    virtual void ChangePositionDelta(const C2DVector& delta, float time, bool flag);   // slot 33 (+0x84)
    void SetSelectedChild(long child);
    unsigned char     _pad004[0x15c - 4];
    long              SelectedChild;                     // +0x15c
    unsigned char     _pad160[0x1c8 - 0x160];
    std::vector<bool> ScrollingChildrenVisibility;       // +0x1c8
};
void CList::SetSelectedChild(long child)
{
    SelectedChild = child;
    if (child >= 0 && child < ScrollingChildrenVisibility.size() && !ScrollingChildrenVisibility[child])
    {
        long first = -1;
        long last = -1;
        for (long i = 0; i < ScrollingChildrenVisibility.size(); ++i)
        {
            if (first == -1 && ScrollingChildrenVisibility[i])
                first = i;
            if (first != -1 && !ScrollingChildrenVisibility[i])
            {
                last = i - 1;
                break;
            }
        }
        if (last == -1)
            last = ScrollingChildrenVisibility.size() - 1;
        if (child > last)
            ChangePositionDelta(C2DVector(0.0f, (child - last) * -25.0f), 0.0f, false);
        else if (first != -1)
            ChangePositionDelta(C2DVector(0.0f, (first - child) * 25.0f), 0.0f, false);
    }
}
}
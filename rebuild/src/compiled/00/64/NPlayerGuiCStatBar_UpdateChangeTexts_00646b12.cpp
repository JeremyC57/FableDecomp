// NPlayerGui::CStatBar::UpdateChangeTexts() -- while the bar is idle (state 0) or in state 3, move each floating change text along the bar box (start to end or end to start) by the frame step, fading its alpha as it goes; texts that finish (or are fully faded) are erased
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: UpdateChangeTexts
// Retail: Fable.exe (TLC, VC7.1) @ 0x00646B12, 306 bytes, `ret`.
// Callees: _ftol 0x00BFEA70, std::list<CChangeText>::erase(iterator) 0x00655A52 (iterator by value, hidden result).
// Globals: 1e-4f epsilon variable 0x0129BA3C (read as a variable, as the landed rows).
// Shape: frame-pointer size TU (`optimize("y", off)` + `("s", on)`); the start/end points are copied from the owner's
//   box; the delta is a named local ASSIGNED from the operator- result (an initialised one is built in place); the step
//   is the value of the `+=` (kept on the x87 stack for the test) and the test is `!(t >= 1 - eps)` (`test ah,1 ; je`;
//   a plain `<` gives `test ah,5 ; jp`); the new offset is start + delta * t; the alpha is `(1 - t) * alpha` through
//   _ftol; the list iterator has the STLport user copy ctor (erase's by-value argument is built with `mov eax,esp`).
// Layout: Ego_r.pdb NPlayerGui::CStatBar (retail list at +0x64, PDB +0x6C); the owner (+0x04) box TL +0x4CC, BR +0x4D4,
//   frame step +0x4DC; state +0x28. CChangeText: OldOffset +0, Offset +8, Text +0x10, Value +0x14, Colour +0x18
//   (alpha byte +0x1B), StartToEnd +0x1C, PercentageThrough +0x20.
extern const float g_epsilon_0129ba3c;          // 0x0129BA3C (1e-4f)
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
};
class CWideString { public: void* PStringData; };
class CRGBColour { public: unsigned char B, G, R, A; };
struct CStatBarOwner {
    unsigned char _pad000[0x4cc];
    C2DVector     BoxTL;                        // +0x4cc
    C2DVector     BoxBR;                        // +0x4d4
    float         FrameStep;                    // +0x4dc
};
namespace NPlayerGui {
class CStatBar {
public:
    class CChangeText { public:
        C2DVector   OldOffset;                  // +0x00
        C2DVector   Offset;                     // +0x08
        CWideString Text;                       // +0x10
        long        Value;                      // +0x14
        CRGBColour  Colour;                     // +0x18
        bool        StartToEnd;                 // +0x1c
        float       PercentageThrough;          // +0x20
    };
    struct SNode {
        SNode*      _M_next;
        SNode*      _M_prev;
        CChangeText _M_data;
    };
    struct iterator {
        SNode* _M_node;
        iterator() { }
        iterator(const iterator& o) : _M_node(o._M_node) { }
        CChangeText* operator->() const { return &_M_node->_M_data; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    };
    struct CChangeTextList {
        SNode* _M_node;
        iterator begin() { iterator it; it._M_node = _M_node->_M_next; return it; }
        iterator end() { iterator it; it._M_node = _M_node; return it; }
        iterator erase(iterator position);      // 0x00655A52
    };
    void*          __vftable;                   // +0x00
    CStatBarOwner* Owner;                       // +0x04
    unsigned char  _pad08[0x28 - 0x08];
    long           State;                       // +0x28
    unsigned char  _pad2c[0x64 - 0x2c];
    CChangeTextList ChangeTexts;                // +0x64
    void UpdateChangeTexts();
};
}

void NPlayerGui::CStatBar::UpdateChangeTexts()
{
    if (State != 0 && State != 3)
        return;
    iterator it = ChangeTexts.begin();
    while (it != ChangeTexts.end()) {
        C2DVector start, end;
        if (it->StartToEnd) {
            start = Owner->BoxTL;
            end = Owner->BoxBR;
        } else {
            start = Owner->BoxBR;
            end = Owner->BoxTL;
        }
        C2DVector delta;
        delta = end - start;
        if (!((it->PercentageThrough += Owner->FrameStep) >= 1.0f - g_epsilon_0129ba3c) && it->Colour.A) {
            it->OldOffset = it->Offset;
            it->Offset = start + delta * it->PercentageThrough;
            it->Colour.A = (unsigned char)((1.0f - it->PercentageThrough) * it->Colour.A);
            ++it;
        } else
            it = ChangeTexts.erase(it);
    }
}
// std::__uninitialized_fill_n(CUIMiniMapMarker* first, unsigned int n, const CUIMiniMapMarker& x, const __false_type&) -- copy-construct n copies of a mini-map marker into raw storage at first and return first + n
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x00613E30, 80 bytes, `ret 8`: __fastcall (first ecx, n edx; x and the empty tag by reference on
// the stack), returning the end pointer in eax. Call sites 0x0061495A, 0x006183AC, 0x0061891C (the
// vector<CUIMiniMapMarker> fill / insert paths; the fill twin is 0x00613180).
// Identity: the catalog's `_Fill<pair<float, CSubtitledMessage::CCameraTag>*>` is propagated -- the element has a
// CCharString at +4 (copy constructor 0x0099EC30) and strides 28 bytes: CTCInventoryMap::CUIMiniMapMarker (Ego_r.pdb,
// landed default constructor 0x005FA530): Region +0, UIDefName +4, Position C2DVector +8, DefNameLength +0x10,
// GraphicIndex +0x14, Angle +0x18.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: the marker's implicit copy constructor is memberwise because CCharString declares one: Region (loaded before
// the string argument push, stored after it), the string copy call, then the five words, each read from x and stored
// at its own offset from the new element. C2DVector needs its (memberwise) user copy constructor: with a trivially
// copyable C2DVector VC7.1 strength-reduces the destination into a second induction register at Position
// (`lea esi,[ebx+8]`, 86 v 80). Placement-new null check kept; n counts down in ebx.
#include <new>
class CCharString { public:
    void* m_pStringData;                        // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
};
class C2DVector { public:
    float X;                                    // +0x00
    float Y;                                    // +0x04
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
class CTCInventoryMap { public:
    class CUIMiniMapMarker { public:                   // 28 bytes
        long        Region;                     // +0x00
        CCharString UIDefName;                  // +0x04
        C2DVector   Position;                   // +0x08
        long        DefNameLength;              // +0x10
        long        GraphicIndex;               // +0x14
        float       Angle;                      // +0x18
    };
};
typedef CTCInventoryMap::CUIMiniMapMarker CUIMiniMapMarker;

struct __false_type { __false_type() {} };
namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template CUIMiniMapMarker* __fastcall __uninitialized_fill_n(CUIMiniMapMarker*, unsigned int, const CUIMiniMapMarker&, const __false_type&);
}
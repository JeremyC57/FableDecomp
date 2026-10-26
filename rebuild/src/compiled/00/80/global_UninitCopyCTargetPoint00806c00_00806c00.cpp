// _global::UninitCopy_CTargetPoint_00806c00(const CTargetPoint* first, const CTargetPoint* last, CTargetPoint* result, const __false_type&) -- STLport __uninitialized_copy<NPlayerTargeting::CTargetPoint*>: placement copy-construct 28-byte target points, return the advanced result
// Leaf: UninitCopy_CTargetPoint_00806c00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00806C00, length 122 bytes (+ int3 padding), `ret 8`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90.
// Vtable: CIntelligentPointer<CThing> 0x01238C6C (stored after the base constructor).
// Types (Ego_r.pdb): NPlayerTargeting::CTargetPoint (28 bytes) = PThing CIntelligentPointer<CThing> +0, Index +8,
//   TargetingPosition C3DVector +0xC, FrameRejected +0x18 (the model of the landed __linear_insert @ 0x0080AA60).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` (`__fastcall`: first in ecx, last in
// edx, result + the tag reference on the stack; `last` spilled to the frame word). Placement new (null test on the
// destination) of CTargetPoint's copy constructor: CBaseIntelligentPointer() + the derived vtable store +
// SetPItem(src.GetPItem()), then Index, the C3DVector (user copy constructor: direct dword stores) and FrameRejected
// through the strength-reduced src+0x14/dst+0x14 pointers. The CTargetPoint / CIntelligentPointer copies are
// __forceinline (VC7.1 otherwise calls an out-of-line copy). The advanced result (ebx) is returned.
// Label: the catalog's `_Copy_impl<NPlayerTargeting::CTargetPoint*,...>` names the element correctly (the body is STLport
// __uninitialized_copy, not Dinkumware _Copy_impl).
inline void* operator new(unsigned int, void* p) { return p; }

class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    void* PData;                                 // +0x04
    CBaseIntelligentPointer();                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();          // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for CThing
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace NPlayerTargeting {
class CTargetPoint { public:
    CIntelligentPointer<CThing> PThing;          // +0x00
    long                        Index;           // +0x08
    C3DVector                   TargetingPosition; // +0x0C
    long                        FrameRejected;   // +0x18
    __forceinline CTargetPoint(const CTargetPoint& other)
        : PThing(other.PThing), Index(other.Index), TargetingPosition(other.TargetingPosition),
          FrameRejected(other.FrameRejected) { }
};
}
using NPlayerTargeting::CTargetPoint;

struct __false_type { __false_type() { } };

CTargetPoint* __fastcall UninitCopy_CTargetPoint_00806c00(const CTargetPoint* first, const CTargetPoint* last, CTargetPoint* result, const __false_type&)
{
    CTargetPoint* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CTargetPoint(*first);
    return cur;
}
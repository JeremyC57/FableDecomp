// _global::CopyBackward_CTargetPoint_008076d0(CTargetPoint*, CTargetPoint*, CTargetPoint*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 28-byte NPlayerTargeting::CTargetPoint, returns the moved-back result
// Leaf: CopyBackward_CTargetPoint_008076d0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008076D0, length 136 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in ecx,
// last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n) *--result = *--last;`
// with the signed count computed once; the moved-back result is returned.
// Types (Ego_r.pdb, the manifest label is right): NPlayerTargeting::CTargetPoint (28 bytes) = PThing
// CIntelligentPointer<CThing> +0, Index +8, TargetingPosition C3DVector +0xC, FrameRejected +0x18. The count
// (0x92492493 multiply + add + `sar 4` = / 28) lives in the dead `result` argument slot; the assignment is inlined
// per element: SetPItem(src.GetPItem()), Index, the C3DVector through its IMPLICIT assignment (re-addressed in
// ecx/edx) and FrameRejected, with src/src+0xC/dst/dst+0xC stepped back by 0x1C. Backward twin of the landed
// __copy 0x00806EA0 (same model).
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
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace NPlayerTargeting {
class CTargetPoint { public:                            // Ego_r.pdb: 28 bytes
    CIntelligentPointer<CThing> PThing;          // +0x00
    long                        Index;           // +0x08
    C3DVector                   TargetingPosition; // +0x0C
    long                        FrameRejected;   // +0x18
    __forceinline CTargetPoint& operator=(const CTargetPoint& other)
    {
        PThing = other.PThing;
        Index = other.Index;
        TargetingPosition = other.TargetingPosition;
        FrameRejected = other.FrameRejected;
        return *this;
    }
};
}
using NPlayerTargeting::CTargetPoint;

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CTargetPoint* __fastcall CopyBackward_CTargetPoint_008076d0(CTargetPoint* first, CTargetPoint* last, CTargetPoint* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
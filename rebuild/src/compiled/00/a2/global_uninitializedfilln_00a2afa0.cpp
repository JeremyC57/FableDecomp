// __uninitialized_fill_n(C3DAnimationInfo* first, unsigned int n, const C3DAnimationInfo& x, __false_type) -- copy-construct n animation infos from one prototype
// Leaf: ??$__uninitialized_fill_n@PAVC3DAnimationInfo@@IV1@@std@@YIPAVC3DAnimationInfo@@PAV1@IABV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2AFA0, 107 bytes, `ret 8`. Only caller 0x00A2CB5B (inside 0x00A2CAC0, the
//   C3DAnimationInfo vector's grow path). The catalog label "fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha>" is
//   a mis-propagated ICF name: the element is the 0x28-byte C3DAnimationInfo (vtables 0x0129CD28 -> 0x0129CD64).
// Identity: STLport 4.6 `__uninitialized_fill_n(_ForwardIter, _Size, const _Tp&, const __false_type&)`; the element
//   copy is the implicit C3DAnimationInfo copy constructor landed at 0x00A2A4F0 (base Bank/Index, counted Data
//   addref'd, Duration, NonLoopingDuration, MovementVector, Rotation) -- Ego_r.pdb members.
// Calling convention: free template under /Gr = __fastcall: first in ecx (copied to eax and returned), n in edx,
//   the prototype and the tag reference on the stack.
// Shape: placement-new null test per element and a second induction register at +0x20 (MovementVector's own user
//   copy constructor keeps the memberwise copy split).
inline void* operator new(unsigned int, void* where) { return where; }

class CMeshDataBank;

class CMeshDataBankEntry {                               // vtable 0x0129CD28
public:
    virtual void v00();
    CMeshDataBank* Bank;                                 // +0x04
    unsigned long  Index;                                // +0x08
};

class CIVCountedPointeeBase {
public:
    virtual void Destroy(int flags);
    virtual void Delete();
    unsigned long IVRefCount;                            // +0x04
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

class C3DAnimationInfo : public CMeshDataBankEntry {    // vtable 0x0129CD64, 0x28 bytes
public:
    class CAnimationData;
    CIVCountedPointer<CAnimationData> Data;              // +0x0C
    float     Duration;                                  // +0x10
    float     NonLoopingDuration;                        // +0x14
    C3DVector MovementVector;                            // +0x18
    float     Rotation;                                  // +0x24
    virtual void v00();
    C3DAnimationInfo(const C3DAnimationInfo& other)
        : CMeshDataBankEntry(other), Data(other.Data), Duration(other.Duration), NonLoopingDuration(other.NonLoopingDuration),
          MovementVector(other.MovementVector), Rotation(other.Rotation)
    {
    }
};

namespace std {
struct __false_type { __false_type() {} };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

template C3DAnimationInfo* __fastcall std::__uninitialized_fill_n(C3DAnimationInfo*, unsigned int, const C3DAnimationInfo&, const std::__false_type&);
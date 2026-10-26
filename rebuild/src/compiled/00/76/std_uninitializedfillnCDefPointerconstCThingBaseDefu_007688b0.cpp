// std::uninitialized_fill_n<CDefPointer<const CThingBaseDef>*, unsigned int, CDefPointer<const CThingBaseDef> >(first, n, const CDefPointer& x) -- STLport three-argument front end (__uninitialized_fill_n with __false_type inlined): copy-construct n add-ref'd thing-def pointers into raw storage, return first + n
// Leaf: ??$uninitialized_fill_n@PAV?$CDefPointer@$$CBVCThingBaseDef@@@@IV1@@std@@YIPAV?$CDefPointer@$$CBVCThingBaseDef@@@@PAV1@IABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007688B0, 41 bytes, __fastcall (first ecx, n edx, &x on the stack, `ret 4`).
// Identity (sweep3; FINDINGS chunk 478 "0x7688B0 element is CDefPointer<const CThingBaseDef> (not CIVCountedPointer)"; was the
//   descriptive _global::UninitializedFillN_IVCountedPointer_007688b0): its only caller is the landed
//   vector<CDefPointer<const CThingBaseDef> >::_M_fill_insert 0x0076B950 (CTCObjectAugmentations::Augmentations, Ego_r.pdb
//   vector<CDefPointer<const CThingBaseDef> >), which already binds this address as the symbol above; the 4-byte element's
//   add-ref is the def's count at +4 (the CDefPointer copy constructor).
// Shape: unsigned n (`jbe`), the placement-new null test, the copy constructor's null test before `inc [edx+4]`.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CDefPointeeBase {                                     // counted def object: vptr, reference count at +4
public:
    virtual void v00() ;
    virtual void Destroy() ;                            // slot 1
    long m_refCount;                                        // +0x04
};

class CThingBaseDef;

template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                                // +0x00 (Ego_r.pdb)
    CDefPointer() { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                      const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

}   // namespace std

template CDefPointer<const CThingBaseDef>* __fastcall std::uninitialized_fill_n(CDefPointer<const CThingBaseDef>*, unsigned int, const CDefPointer<const CThingBaseDef>&);

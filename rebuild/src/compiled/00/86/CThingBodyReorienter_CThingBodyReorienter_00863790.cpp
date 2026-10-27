// CThingBodyReorienter::~CThingBodyReorienter() -- straighten the body for slopes one last time, then destroy the intersecting-things vector
// Leaf: ??1CThingBodyReorienter@@QAE@XZ
// Retail: Fable.exe @ 0x00863790, 55 bytes, thiscall plain `ret`.
// Identity (proven): reloc_bindings already binds ??1CThingBodyReorienter@@QAE@XZ to 0x00863790 (FableWin.pdb has the
// non-virtual ??1CThingBodyReorienter@@QAE@XZ and the boost::checked_delete / scoped_ptr<CThingBodyReorienter> users);
// its caller 0x006634F0 is a scoped_ptr reset shape (`call 0x00863790 ; push edi ; call operator delete`). The body works
// on +0x18/+0x1C/+0x24 = FableWin.pdb PossibleIntersectingThings (STLport vector) and Body. The row is not a catch block:
// it builds its own frame (push ebx/esi/edi) and returns with `ret`.
// Label correction: catalog "_global::Catch@0047e8cc" (gap-scan label); reloc_bindings also holds a stray
// ??1CTCMapwho@@QAE@XZ for this address.
// Callees: 0x008633B0 ?ReorientBodyYZForSlopes@CThingBodyReorienter@@QAEXXZ; each element's virtual destructor (slot 0, flag
// 0); 0x00BFEA14 free (STLport allocator::deallocate behind the null test).
// Body: STLport 4.6 ~vector inlined -- `_Destroy(_M_start, _M_finish)` (each CIntelligentPointer<CThing> through its
// virtual destructor, stride 8), then `if (_M_start) free(_M_start)`.
// Layout: local model from FableWin.pdb (no engine header), retail offsets as in 0x00863250's source.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

namespace std {
struct __true_type { };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* Item;                                      // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
};

class CThingBodyReorienter {
public:
    ~CThingBodyReorienter();
    void ReorientBodyYZForSlopes();                         // 0x008633B0

    bool BodyReorientationDone;                             // +0x00
    bool ReorientingBody;                                   // +0x01
    long ReorientationCount;                                // +0x04
    bool StillCheckingForNewBodyReorientationAngle;         // +0x08
    bool HaveFinalReorientationAngle;                       // +0x09
    float FinalReorientationAngleXY;                        // +0x0C
    long RotationDirection;                                 // +0x10 ERotationDirection
    bool HasPossibleIntersectingThings;                     // +0x14
    std::vector<CIntelligentPointer<CThing> > PossibleIntersectingThings;   // +0x18
    CThing* Body;                                           // +0x24
};

CThingBodyReorienter::~CThingBodyReorienter()
{
    ReorientBodyYZForSlopes();
}
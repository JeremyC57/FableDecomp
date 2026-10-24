// std::vector<CKeyCamera>::insert(CKeyCamera* position, const CKeyCamera& x) -- STLport single-element insert of a scripted-camera key: construct at the end, or shift the tail up one slot and assign x into position, or grow; return the iterator at the insert index
// Leaf: ?insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075FAE0, 230 bytes (+int3 pad to 240), thiscall `ret 8`, the iterator (a plain pointer)
//   returned in eax.
// Caller: CLipSyncDataBank::Close-labelled 0x0075E460 (call site 0x0075E49C; the landed CLipSyncDataBank::InsertSample
//   twin 0x0075E460 sits in the CTCCameraPointScriptedSpline key-camera TU).
// Callees: CKeyCamera copy constructor 0x0075F1F0 (three sites: end construct, back-element construct, the x_copy
//   local), __copy_backward 0x0075F0E0 (first, last, result, random_access_iterator_tag&, null distance),
//   CKeyCamera::operator= 0x0075EF00, ~CCharString 0x0099EAE0 (x_copy's EventName: the implicit ~CKeyCamera),
//   _M_insert_overflow(pos, x, __false_type&, 1, false) 0x0075F860.
// Identity: the 0x38 stride (`imul 92492493h` + `sar edx,5`) and every callee are the landed CKeyCamera vector helpers
//   (CKeyCamera from Ego_r.pdb, 56 bytes, EventName +0x30).
// Shape: STLport 4.x `vector::insert(position, x)` verbatim: n = position - begin() first; room: at end ->
//   `_Construct(_M_finish, x)`, else `_Construct(_M_finish, *(_M_finish - 1))`, `_Tp x_copy = x`,
//   `__copy_backward_ptrs(position, _M_finish - 2, _M_finish - 1, __false_type)` (inline, forwarding to the
//   out-of-line __copy_backward with a fresh tag homed in the dead `x` slot), `*position = x_copy`; no room ->
//   `_M_insert_overflow(position, x, __false_type(), 1UL)`; then `return begin() + n` duplicated into each tail.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;                                      // +0x00
    ~CCharString();                                         // 0x0099EAE0
};

class C3DVector {
public:
    float X, Y, Z;
};

class CKeyCamera {                                          // Ego_r.pdb, 56 bytes
public:
    float       DeltaIndex;                                 // +0x00
    C3DVector   Pos;                                        // +0x04
    C3DVector   LookDirection;                              // +0x10
    float       FOV;                                        // +0x1c
    float       Roll;                                       // +0x20
    float       ShuttleSpeed;                               // +0x24
    float       Duration;                                   // +0x28
    float       PauseTime;                                  // +0x2c
    CCharString EventName;                                  // +0x30
    float       SpeedFraction;                              // +0x34
    CKeyCamera(const CKeyCamera& other);                    // 0x0075F1F0
    CKeyCamera& operator=(const CKeyCamera& other);         // 0x0075EF00
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*);   // 0x0075F0E0

template <class _Tp>
inline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class V> struct _STLP_alloc_proxy {
    V _M_data;
};

template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*> _M_end_of_storage;                // +0x08

    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void _M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x0075F860
    T* insert(T* __position, const T& __x);
};

template <class T>
T* vector<T>::insert(T* __position, const T& __x)
{
    unsigned int __n = __position - begin();
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        if (__position == end()) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else {
            _Construct(this->_M_finish, *(this->_M_finish - 1));
            ++this->_M_finish;
            T __x_copy = __x;
            __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, __false_type());
            *__position = __x_copy;
        }
    }
    else
        _M_insert_overflow(__position, __x, __false_type(), 1UL);
    return begin() + __n;
}

template class vector<CKeyCamera>;

}
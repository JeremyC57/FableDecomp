// std::vector::_M_fill_insert(CDialogueLayerDef* position, unsigned int n, const CDialogueLayerDef& x) -- (vector<NSpeechGainManager::CDialogueLayerDef>) STLport insert of n copies of a dialogue-layer def: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCDialogueLayerDef@NSpeechGainManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00472C2A, 333 bytes, thiscall `ret 0Ch`; size TU with a frame pointer.
// Identity: the only caller is vector<CDialogueLayerDef>::insert(position, n, x) 0x00472BD3; stride 0x40 with the
//   element copy constructor 0x004663E7 and assignment 0x004663AE (both landed under NSpeechGainManager). The catalog
//   label "Vector_InsertDialogueLayersAndShift" is behavioural.
// Callees: CDialogueLayerDef(const CDialogueLayerDef&) 0x004663E7, CDialogueLayerDef::operator= 0x004663AE,
//   uninitialized_fill_n(first, n, x) 0x00472E58 (__fastcall), CResource::~CResource 0x009FC550 (the x_copy destructor's
//   only out-of-line step), _M_insert_overflow(position, x, const __false_type&, n, atend) 0x00472D79 (landed).
// Shape: STLport 4.x `_M_fill_insert` verbatim, as 0x0046AD59: `x_copy` is copy-constructed in the frame and destroyed
//   at the end of the in-place branch (the (size-TU __forceinline) destructor chain inlines to the CDefPointeeBase vtable store
//   0x01230BA0 and the CResource destructor call); the inlined `_Construct` is the null-checked placement copy.
// Model: CResource (0x1C, virtual destructor) <- CDefPointeeBase (vtable 0x01230BA0) <- CDefClassBase (PDefManager,
//   GlobalIndex, DefaultValsApplied; 0x28 padded) <- CDialogueLayerDef (six floats, 0x40), member names from Ego_r.pdb.
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* where) { return where; }

class CResourceList;
class CDefinitionManager;

class CResource {
public:
    virtual ~CResource();                                // 0x009FC550
    unsigned long _pad04;                                // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource* PrevResource;                             // +0x0C
    CResource* NextResource;                             // +0x10
    unsigned long ResourceSize;                          // +0x14
    unsigned long LastUsedFrame;                         // +0x18
};

class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    __forceinline ~CDefPointeeBase() { }
};

class CDefClassBase : public CDefPointeeBase {
public:
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long GlobalIndex;                           // +0x20
    unsigned char DefaultValsApplied;                    // +0x24
    __forceinline ~CDefClassBase() { }
};

namespace NSpeechGainManager {
class CDialogueLayerDef : public CDefClassBase {         // vtable 0x0123543C
public:
    float ResponseTimeSecsAttack;                        // +0x28
    float ResponseTimeSecsRelease;                       // +0x2C
    float GainLevelHigh;                                 // +0x30
    float GainLevelMid;                                  // +0x34
    float GainLevelLow;                                  // +0x38
    float FocusShiftDistanceThreshold;                   // +0x3C
    CDialogueLayerDef(const CDialogueLayerDef& other);               // 0x004663E7
    CDialogueLayerDef& operator=(const CDialogueLayerDef& other);    // 0x004663AE
    __forceinline ~CDialogueLayerDef() { }
};
}

namespace std {

struct __false_type { };
struct random_access_iterator_tag { };

NSpeechGainManager::CDialogueLayerDef* __fastcall uninitialized_fill_n(NSpeechGainManager::CDialogueLayerDef* first, unsigned int n, const NSpeechGainManager::CDialogueLayerDef& x);  // 0x00472E58

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _Tp> __forceinline _Tp* __uninitialized_copy(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    _Tp* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(__cur, *__first);
    return __cur;
}

template <class _Tp> __forceinline _Tp* __copy_backward(_Tp* __first, _Tp* __last, _Tp* __result,
                                                        const random_access_iterator_tag&, int*)
{
    for (int __n = int(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _Tp> __forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> __forceinline void fill(_Tp* __first, _Tp* __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x00472D79
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if ((unsigned int)(_M_end_of_storage - _M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = _M_finish - __position;
            pointer __old_finish = _M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(_M_finish - __n, _M_finish, _M_finish, __false_type());
                _M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                _M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, _M_finish, __false_type());
                _M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<NSpeechGainManager::CDialogueLayerDef>::_M_fill_insert(NSpeechGainManager::CDialogueLayerDef*, unsigned int, const NSpeechGainManager::CDialogueLayerDef&);

}
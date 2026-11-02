// CFrameRateSmoother::Uninitialise() -- if the smoother is valid, empty the Times array (STLport clear = erase(begin, end)), then the base Uninitialise
// Leaf: ?Uninitialise@CFrameRateSmoother@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049BA20, 65 bytes (+ int3 padding to 0x0049BA70), thiscall; both exits tail-jump
//   CInitBaseClass::Uninitialise 0x0099A360.
// Identity (proven, two sources): the only caller 0x00417674 is CMainGameComponent::Shutdown (0x004175E5) with
//   `lea ecx,[esi+16178h]`, the Ego_r.pdb CMainGameComponent member FrameRateSmoother (CFrameRateSmoother, +90488 = +0x16178);
//   Ego_r.pdb and FableWin.pdb ?Uninitialise@CFrameRateSmoother@@QAEXXZ; the row sits in the CFrameRateSmoother block of retail
//   .text between the landed AddFrameTime 0x0049B9E0 and Init 0x0049BA70. The body is the CInitBaseClass pattern: if IsValid(), empty
//   the Times array, then the base Uninitialise. The catalog label "CThingCreatureBase::EditWorldUpdate" is propagated and the
//   landed caller's binding ?Shutdown@CEditWorld@@QAEXXZ is wrong (NAME_FIXES.tsv).
// Callees: ?IsValid@CInitBaseClass@@QBE_NXZ 0x0099A330 (`mov al,[ecx+4]`), _memmove 0x00BFEAE6 (STLport __copy_trivial),
//   ?Uninitialise@CInitBaseClass@@IAEXXZ 0x0099A360 (protected, both PDBs; tail).
// Shape: STLport 4.6 `clear()` = `erase(begin(), end())`, whose trivial-assignment path is `__copy_ptrs(last, finish, first,
//   __true_type)` -> `__copy_trivial`: `(last == first) ? result : memmove(result, first, n) + n` -- with last == finish the
//   compiler still emits retail's `cmp eax,eax ; jne` and the memmove arm; the trivial _Destroy is empty.
// Model: the landed sibling CFrameRateSmoother::Init 0x0049BA70 (CArray<T> : std::vector<T>, CInitBaseClass : CBaseClass with
//   Valid +0x04, Times +0x08 .. SmoothedTime +0x28), with the real STLport 4.6 erase / __copy_ptrs / __copy_trivial text.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}

template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _TrivialAss;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}

template <class T> class CArray : public std::vector<T> { };

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CInitBaseClass : public CBaseClass {
public:
    bool IsValid() const;                            // 0x0099A330 ?IsValid@CInitBaseClass@@QBE_NXZ
protected:
    bool Initialise();                               // 0x0099A350 ?Initialise@CInitBaseClass@@IAE_NXZ
    void Uninitialise();                             // 0x0099A360 ?Uninitialise@CInitBaseClass@@IAEXXZ
public:
    bool Valid;                                      // +0x04
};
class CFrameRateSmoother : public CInitBaseClass {
public:
    bool Init(long maxNoTimes, long noFramesToChangeFrameRateOver);
    void Uninitialise();
    CArray<float> Times;                             // +0x08
    long          MaxNoTimes;                        // +0x14
    long          NoTimes;                           // +0x18
    long          NoFramesToChangeFrameRateOver;     // +0x1C
    long          FirstTime;                         // +0x20
    long          LastTime;                          // +0x24
    double        SmoothedTime;                      // +0x28
};

void CFrameRateSmoother::Uninitialise()
{
    if (IsValid())
        Times.clear();
    CInitBaseClass::Uninitialise();
}
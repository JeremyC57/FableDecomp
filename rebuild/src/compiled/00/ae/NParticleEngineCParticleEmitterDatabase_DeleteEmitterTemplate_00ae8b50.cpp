// NParticleEngine::CParticleEmitterDatabase::DeleteEmitterTemplate(long handle) -- drop the emitter template(s) registered under a handle: erase the handle's [lower_bound, upper_bound) range from the Emitters vector map (releasing their counted emitter pointers)
// Leaf: ?DeleteEmitterTemplate@CParticleEmitterDatabase@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE8B50, 82 bytes, __fastcall `ret 4`. No direct caller (an editor-side entry kept in the
//   image).
// Identity: Ego_r.pdb `?DeleteEmitterTemplate@CParticleEmitterDatabase@NParticleEngine@@QAEXJ@Z`; Emitters is
//   CVectorMap<long, CCountedPointer<NParticleEngine::CParticleEmitter>, CKeyPairCompareLess<..> > at retail +0x28 (the
//   landed ctor 0x00AE8BF0 / GetEmitterTemplate 0x00AE89E0 model).
// Callees: CVectorMap::LowerBound(const long&) 0x00AE9520, CVectorMap::UpperBound(const long&) 0x00AE95E0 (landed),
//   __copy 0x00AE9CB0 (the entries above the range moved down; manifest `_Move<pair<long,CCountedPointer<CParticleEmitter>>*>`),
//   __destroy_aux 0x00AE9D50 (the vacated tail's counted pointers released).
// Label: catalog "CVectorMap_EraseByRange" is descriptive.
// Shape: the map's inline Erase (`first = LowerBound(key); if (first != end()) m_vector.erase(first, UpperBound(key));`) over
//   STLport's inline `erase(first, last) { i = __copy_ptrs(last, _M_finish, first, __false_type()); _Destroy(i, _M_finish);
//   _M_finish = i; }`. The two bound searches are same-TU noinline bodies (their landed code), so VC7.1 knows they write
//   no memory and reuses the end pointer read after LowerBound for the copy (`mov edx,ebx`); the opaque __copy forces the
//   finish reload for _Destroy. Both tag temporaries are homed in the dead handle slot.
namespace NParticleEngine { class CParticleEmitter; }
struct SCountBlock;

template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0x0
    SCountBlock* Block;                                     // +0x4
};

namespace _STL {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _InputIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_InputIter __first, _InputIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);              // 0x00AE9CB0
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00AE9D50

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> struct _STLP_alloc_proxy {             // the end-of-storage pointer over an empty allocator
    _Tp* _M_data;
};

template <class _Tp> class vector {                        // STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp> _M_end_of_storage;               // +0x08
    iterator end() { return _M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
}

template <class K, class V> struct CKeyPairCompareLess { };

template <class K, class V, class C> class CVectorMap {
public:
    typedef _STL::pair<K, V> value_type;
    typedef value_type* iterator;
    _STL::vector<value_type> m_vector;                      // +0x00
    C    Compare;                                           // +0x0c (empty)
    bool Dirty;                                             // +0x0d
#pragma auto_inline(off)
    __declspec(noinline) iterator LowerBound(const K& key)  // 0x00AE9520
    {
        iterator first = m_vector._M_start;
        long count = m_vector._M_finish - m_vector._M_start;
        while (0 < count) {
            long half = count / 2;
            iterator mid = first + half;
            if (mid->first < key) {
                first = ++mid;
                count -= half + 1;
            }
            else
                count = half;
        }
        return first;
    }
    __declspec(noinline) iterator UpperBound(const K& key)  // 0x00AE95E0 (landed)
    {
        iterator first = m_vector._M_start;
        long count = m_vector._M_finish - m_vector._M_start;
        while (0 < count) {
            long half = count / 2;
            iterator mid = first + half;
            if (!(key < mid->first)) {
                first = ++mid;
                count -= half + 1;
            }
            else
                count = half;
        }
        return first;
    }
#pragma auto_inline(on)
    void Erase(const K& key)
    {
        iterator first = LowerBound(key);
        if (first != m_vector.end())
            m_vector.erase(first, UpperBound(key));
    }
};

namespace NParticleEngine {
struct CSymbolMap { unsigned char m_data[0x24]; };          // retail 0x24 bytes (PDB 0x2c: STLport members)

class CParticleEmitterDatabase {
public:
    CSymbolMap SymbolMap;                                   // +0x00
    bool       UsingSymbolMap;                              // +0x24
    CVectorMap<long, CCountedPointer<CParticleEmitter>, CKeyPairCompareLess<long, CCountedPointer<CParticleEmitter> > > Emitters;   // +0x28
    void DeleteEmitterTemplate(long handle);
};

void CParticleEmitterDatabase::DeleteEmitterTemplate(long handle)
{
    Emitters.Erase(handle);
}
}
// NThingManager::CEntityRuntimePersistence::CEntityScriptInfo::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CEntityScriptInfo inlined (SaveData freed, then Script destroyed), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052AAD0, 48 bytes, thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: NThingManager::CEntityRuntimePersistence::CEntityScriptInfo -- proven; evidence: FableWin.pdb names ??_GCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@QAEPAXI@Z (a class: V); the layout is Ego_r.pdb CEntityScriptInfo (CCharString Script +0, CArray<unsigned char> SaveData +4, long UncompressedSize, bool Valid), and the adjacent constructor 0x0052AB00 builds exactly that: CCharString() at +0, zeroes +4/+8/+0xC/+0x10 and the bool at +0x14
// Label: the manifest's "CParticleEffect::`scalar_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (the inlined _Vector_base destructor: allocator::deallocate behind the null test), CCharString::~CCharString 0x0099EAE0, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 stores the vptr only in user-written destructors and CEntityScriptInfo's is implicit; members are
//   destroyed in reverse declaration order (the vector's trivially destructible elements need no loop: STLport's
//   __type_traits<T>::has_trivial_destructor selects the empty __destroy_aux).
//   The non-polymorphic deleting destructor is emitted where a delete expression needs it; retail keeps it although
//   nothing references it (no call or pointer in Fable.exe), so DeleteEntityScriptInfo is that delete expression, nothing more.
class CCharString {
public:
    CCharString();                                            // 0x0099E4B0
    ~CCharString();                                           // 0x0099EAE0
    void* PStringData;
};
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <> struct __type_traits<unsigned char> { typedef __true_type has_trivial_destructor; };
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
template <class T> class CArray : public std::vector<T> { };     // retail CArray: an STLport vector (12 bytes)
namespace NThingManager {
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo {                                 // 0x18 bytes
    public:
        CCharString           Script;                         // +0x00
        CArray<unsigned char> SaveData;                       // +0x04
        long                  UncompressedSize;               // +0x10
        bool                  Valid;                          // +0x14
    };
};
}
void DeleteEntityScriptInfo(NThingManager::CEntityRuntimePersistence::CEntityScriptInfo* p) { delete p; }
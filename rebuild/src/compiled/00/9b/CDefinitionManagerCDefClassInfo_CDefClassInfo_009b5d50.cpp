// CDefinitionManager::CDefClassInfo::~CDefClassInfo() -- the compiler-generated destructor: free the GlobalIndices block, then destroy Name
// Leaf: ??1CDefClassInfo@CDefinitionManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B5D50, 27 bytes, thiscall, tail `jmp ~CCharString`.
// Identity (proven): unreferenced in retail (inlined at its uses; the linker kept the COMDAT). FableWin.pdb has the pair
//   ??0CDefClassInfo@CDefinitionManager@@QAE@XZ (0x0305A7B0) then ??1CDefClassInfo@CDefinitionManager@@QAE@XZ (0x0305A820)
//   back to back; retail has the same pair: the landed ctor 0x009B5D30 then this row. Ego_r.pdb CDefClassInfo is
//   {CCharString Name +0, AllocFunc +4, CArray<unsigned long> GlobalIndices +8}: exactly the members this body destroys
//   (vector start at +8, CCharString at +0). Not polymorphic (no vtable). The catalog label "CTCInventoryItem::OnDie" is
//   propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), CCharString::~CCharString 0x0099EAE0.
// Shape: members in reverse order: GlobalIndices (CArray over the STLport vector: trivial _Destroy, null-tested free), then
//   Name's out-of-line destructor as the tail jump. DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this
//   out-of-line implicit `??1`.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
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
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std
template <class T> class CArray : public std::vector<T> { };   // Lionhead CArray over STLport vector (0xc)

class CCharStringData;
class CCharString {
public:
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;                           // +0x00
};
class CDefClassBase;
class CDefinitionManager {
public:
    class CDefClassInfo {                                   // Ego_r.pdb, 0x14 bytes in retail
    public:
        CDefClassInfo() { }                                 // 0x009B5D30
        CCharString           Name;                         // +0x00
        CDefClassBase* (__cdecl* AllocFunc)();              // +0x04
        CArray<unsigned long> GlobalIndices;                // +0x08
    };
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CDefinitionManager::CDefClassInfo* info)
{
    info->CDefinitionManager::CDefClassInfo::~CDefClassInfo();
}
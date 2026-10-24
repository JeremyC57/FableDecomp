// std::__destroy_aux(std::pair<unsigned long, CDefinitionManager::CDefClassInfo>* first, std::pair<unsigned long, CDefinitionManager::CDefClassInfo>* last, const __false_type&) -- destroy a range of DefClassInfo map entries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B8070, length 55 bytes.
// Leaf: __destroy_aux
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The caller at 0x009B038C
// (CDefinitionManager::Cleanup) passes the bounds of the CVectorMap<unsigned long, CDefClassInfo> DefClassInfo vector
// ([esi+0x78], [esi+0x7c]) and the address of an empty tag (`lea eax,[esp+13h] ; push eax`), so this is STLport
// `__destroy_aux(first, last, const __false_type&)` over pair<unsigned long, CDefinitionManager::CDefClassInfo>.
// Layout: Ego_r.pdb CDefClassInfo = Name (CCharString) +0, AllocFunc +4, GlobalIndices (CArray<unsigned long>) +8; the
// retail CArray is a 12-byte STLport vector, so the pair is 0x18 bytes (key +0, Name +4, AllocFunc +8, GlobalIndices +0xc).
// Convention: LTCG register form -- first in ecx, last in edx, the tag pointer on the stack (`ret 4`).
// Callees: free @ 0x00BFEA14 (vector block), CCharString::~CCharString @ 0x0099EAE0.
// Shape: the implicit ~pair -> ~CDefClassInfo -> ~CArray -> STLport ~vector / ~_Vector_base chain, all inline. Only the
// real STLport structure (_Vector_base + _STLP_alloc_proxy, trivial `_Destroy(start, finish)`, `deallocate(p, n)`)
// gives retail's strength-reduced element pointer (`lea esi,[edi+0Ch]`, CCharString at `[esi-8]`); a flat
// three-pointer vector with `if (p) free(p)` addresses everything from `first` (52 v 53 bytes).
extern "C" void __cdecl free(void* p);      // 0x00BFEA14
struct CCharString {
    void* m_pData;
    ~CCharString();                         // 0x0099EAE0
};
namespace std {
struct __true_type { };
struct __false_type { };
template <class T> inline void __destroy_trivial(T*, T*, const __true_type&) { }
template <class T> inline void _Destroy(T* first, T* last) { __destroy_trivial(first, last, __true_type()); }
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> struct allocator {
    void deallocate(T* p, unsigned int) { free(p); }
};
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class T> struct vector : _Vector_base<T> {
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
template <class T> struct CArray : std::vector<T> { };
struct CDefClassBase;
struct CDefinitionManager {
    struct CDefClassInfo {
        CCharString Name;                       // +0x00
        CDefClassBase* (__cdecl* AllocFunc)();  // +0x04
        CArray<unsigned long> GlobalIndices;    // +0x08
    };
};
typedef std::pair<unsigned long, CDefinitionManager::CDefClassInfo> SDefClassInfoEntry;

namespace std {
void __fastcall __destroy_aux(SDefClassInfoEntry* first, SDefClassInfoEntry* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}
// CFactionManager::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the inline destructor destroys the pair<unsigned long, CCountedPointer<CFaction> > range of its vector through the out-of-line __destroy_aux and frees the storage, then the object is freed when bit 0 of the flags is set
// Leaf: ??_GCFactionManager@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE4D0, 55 bytes (+ int3 padding to 64), thiscall `ret 4`, returns this.
// Identity (proven): CFactionManager::`scalar deleting destructor'; evidence: its callee 0x004AD9E0 is __destroy_aux<pair<unsigned long, CCountedPointer<CFaction> >*> (the landed vector<pair<unsigned long, CCountedPointer<CFaction> > >::_M_insert_overflow 0x006BAA90 calls it), the element type of Ego_r.pdb CFactionManager::Factions (CVectorMap<unsigned long, CCountedPointer<CFaction> >, the class's only member); the landed CWorld::~CWorld 0x004A6AB0 deletes the CWorld::PFactionManager scoped_ptr<CFactionManager> through the same inline body (0x004A6D13 calls 0x004AD9E0 then operator delete)
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H064: six deleting destructors that differ only in the
//   __destroy_aux they call; retail has no identical-code folding, so each address is one class's COMDAT). No call or
//   data reference reaches this address: VC7.1 emits the ??_G COMDAT for every `delete p` and explicit `p->~T()` of a
//   class with a destructor (checked with a two-line probe), inlines it at the site, and the retail link kept it.
// Callees: std::__destroy_aux<std::pair<unsigned long, CCountedPointer<CFaction> >*> @ 0x004AD9E0 (`__fastcall` first/last, the empty __false_type tag by reference,
//   homed on the flags argument's slot: `lea eax,[esp+8]`), free 0x00BFEA14 (the null-tested
//   ~_Vector_base deallocate), operator delete 0x00BFE9BC.
// Layout: local models (no engine header for these classes); STLport 4.6 vector = 12 bytes.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIter>
void __fastcall __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&);   // out of line
template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x0
    _Tp* _M_finish;                                     // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
class CFaction;
template <class T> class CCountedPointer {             // 8 bytes: object, shared count block
public:
    T*    Data;
    void* Info;
    ~CCountedPointer();
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {   // 16 bytes in retail
public:
    C    Compare;                                       // +0xC
    bool Dirty;                                         // +0xD
};
class CFactionManager {                               // 16 bytes in retail (Ego_r.pdb: one member)
public:
    CVectorMap<unsigned long, CCountedPointer<CFaction>, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > > Factions;   // +0x0
};

// Same-TU stand-in for the out-of-line __destroy_aux 0x004AD9E0: a plain function above the caller under auto_inline(off)
// that never touches the tag, so VC7.1 homes the empty tag temporary on the live flags slot as retail does (a declared-only
// callee opens a `push ecx` frame word; a template stand-in is inlined despite the pragma -- GOTCHAS 2026-10-20).
namespace std {
#pragma auto_inline(off)
void __fastcall __destroy_aux(std::pair<unsigned long, CCountedPointer<CFaction> >* __first, std::pair<unsigned long, CCountedPointer<CFaction> >* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(__first);
}
#pragma auto_inline(on)
}

// scoped_ptr<CFactionManager>::~scoped_ptr / reset (CWorld::PFactionManager): the delete that emits the COMDAT.
void DeleteFactionManager_004ae4d0(CFactionManager* p) { delete p; }
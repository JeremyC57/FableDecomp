// CRegionConnectionGraph::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the inline destructor destroys the pair<long, CCountedPointer<CRegionConnectionNode> > range of its vector through the out-of-line __destroy_aux and frees the storage, then the object is freed when bit 0 of the flags is set
// Leaf: ??_GCRegionConnectionGraph@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051C8E0, 55 bytes (+ int3 padding to 64), thiscall `ret 4`, returns this.
// Identity (proven): CRegionConnectionGraph::`scalar deleting destructor'; evidence: its callee 0x00519050 is the __destroy_aux the CCountedPointer deleter 0x0051CCD0 runs on the deleted object's vector before `operator delete` (the same inline body, null-tested); that deleter is stored only by CCountedPointer::Reset 0x0051CEA0, whose only caller is CWorldMap::LoadRegionGraph 0x00506D40, resetting the member at +0x44 with a `new` 0x10-byte object whose ctor zeroes three words and the +0xD Dirty byte (a CVectorMap); Ego_r.pdb CWorldMap::PRegionGraph is CCountedPointer<CRegionConnectionGraph> and CRegionConnectionGraph is one CVectorMap<long, CCountedPointer<CRegionConnectionNode> > GraphNodes
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H064: six deleting destructors that differ only in the
//   __destroy_aux they call; retail has no identical-code folding, so each address is one class's COMDAT). No call or
//   data reference reaches this address: VC7.1 emits the ??_G COMDAT for every `delete p` and explicit `p->~T()` of a
//   class with a destructor (checked with a two-line probe), inlines it at the site, and the retail link kept it.
// Callees: std::__destroy_aux<std::pair<long, CCountedPointer<CRegionConnectionNode> >*> @ 0x00519050 (`__fastcall` first/last, the empty __false_type tag by reference,
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
class CRegionConnectionNode;
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
class CRegionConnectionGraph {                        // 16 bytes in retail (Ego_r.pdb: one member)
public:
    CVectorMap<long, CCountedPointer<CRegionConnectionNode>, CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> > > GraphNodes;   // +0x0
};

// Same-TU stand-in for the out-of-line __destroy_aux 0x00519050: a plain function above the caller under auto_inline(off)
// that never touches the tag, so VC7.1 homes the empty tag temporary on the live flags slot as retail does (a declared-only
// callee opens a `push ecx` frame word; a template stand-in is inlined despite the pragma -- GOTCHAS 2026-10-20).
namespace std {
#pragma auto_inline(off)
void __fastcall __destroy_aux(std::pair<long, CCountedPointer<CRegionConnectionNode> >* __first, std::pair<long, CCountedPointer<CRegionConnectionNode> >* __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(__first);
}
#pragma auto_inline(on)
}

// The CCountedPointer<CRegionConnectionGraph> deleter (retail 0x0051CCD0): the delete that emits the COMDAT.
void __fastcall DeleteRegionConnectionGraph_0051c8e0(CRegionConnectionGraph* p) { delete p; }
// __destroy_aux(CSeaBodyDesc* first, CSeaBodyDesc* last, const __false_type&) -- STLport range destroy for 16-byte {CCharString Identifier; CArray<long> Maps} elements: free each Maps block, then ~CCharString
// Leaf: ??$__destroy_aux
//
// Retail 0x00519EB0, 51 bytes (+ int3 padding to 64) (`ret 4`: first in ecx, last in edx -- the retail TU is /Gr, spelled here as an explicit
//   __fastcall -- and the empty tag on the stack).
// Callees: 0x00BFEA14 free (vector<long> storage), 0x0099EAE0 CCharString::~CCharString.
// Callers: the vector's erase/clear in 0x00507070 (after __copy 0x00517200), _M_insert_overflow 0x0051C110 and four
//   more vector bodies (0x0051ABE6 .. 0x0051BF65 call sites).
// Element: 16 bytes in retail STLport: CCharString +0, CArray<long> (= vector<long>: start +4, finish +8, eos +0xC).
//   Ego_r.pdb CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc = { CCharString Identifier; CArray<long> Maps } is the
//   one PDB type of that shape; its copy goes through the vector<long> copy ctor 0x00514160 (as CRegion::Maps does).
// Shape: `for (; first != last; ++first) _Destroy(&*first);` -- the implicit ~CSeaBodyDesc runs ~vector<long> (trivial
//   element destroy, `_Vector_base` frees a non-null start) then ~CCharString; the tag parameter is unnamed and unused.
// Label: catalog "_Copy_backward<CPixelFormat*,CPixelFormat*>" is a propagated Dinkumware label; this is STLport __destroy_aux.
extern "C" void __cdecl free(void* p);

class CCharString {
public:
    const char* PString;
    ~CCharString();                            // 0x0099EAE0
};

namespace std {
struct __false_type {};
template <class T> class allocator {
public:
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) allocator<T>().deallocate(_M_start, _M_end_of_storage - _M_start); }
};
template <class T> class vector : public _Vector_base<T> {
public:
    ~vector() {}
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator first, _ForwardIterator last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}

template <class T> class CArray : public std::vector<T> {};

class CStaticMapGenerationVisibiilityInfo {
public:
    struct CSeaBodyDesc {
        CCharString  Identifier;               // +0x0
        CArray<long> Maps;                     // +0x4
    };
};
typedef CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc CSeaBodyDesc;

template void __fastcall std::__destroy_aux<CSeaBodyDesc*>(CSeaBodyDesc*, CSeaBodyDesc*, const std::__false_type&);
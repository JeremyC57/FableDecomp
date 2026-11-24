// std::uninitialized_fill_n(CMeshMorphSetParts* first, unsigned int n, const CMeshMorphSetParts& x) -- STLport public non-POD fill (the __false_type dispatch inlined): placement-copy n copies of a mesh morph set part (CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts, 16 bytes) and return the end
// Flags: /Gr
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728B50, length 50 bytes (+ int3 padding), __fastcall (first in ecx, n in edx), `ret 4`.
// Caller: _M_fill_insert 0x00728DF0 (landed). Callee: vector<CEngineTextureMorph>::vector(const vector&) 0x005BBDF1 (landed).
// Name (NF392 / NF395, T3 nf393 0x00728B10): the reloc table's spelling of this address (witness 00728df0), the STLport 4.6 _uninitialized.h
//   signature over CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts* (vendor-source-derived: no PDB names an __uninitialized_*
//   instance; constness from the retail callers, which all pass non-const pointers). Was the Dinkumware-style stand-in
//   ?UninitFillN_SPairLongCSecond_005bbdf1_00728b50@@YIPAUSPairLongCSecond_005bbdf1@@PAU1@KABU1@@Z.
// Flags note (NF395): retail's TU is /Gr -- the unreferenced out-of-line _Construct<CMeshMorphSetParts> at 0x00728A20 takes p in ecx and
//   the value in edx (`test ecx,ecx ; je ; mov eax,[edx]`), and STLport's _Construct carries no calling-convention keyword -- so the row
//   compiles /Gr and none of the STLport templates below carries a keyword (build entry: CompilerFlags '/O2 /Oy /W3 /Gr').
// Model: the landed same-TU sibling 0x00728D10's local STLport model (same-TU stand-in bodies of the out-of-line helpers; CArray<T> derives
//   from the STLport vector with no special members of its own; Ego_r.pdb CEngineTextureMorphSet {CArray<CEngineTextureMorph> Morphs;} and
//   CMeshMorphSetParts {unsigned long MeshIndex; CEngineTextureMorphSet TextureMorphs;}, the shapes of engine/CEngineTextureMorph.h /
//   engine/CTCAppearanceMorphBase.h). The element copy is the implicit CMeshMorphSetParts copy expanded down to STLport's
//   ??0?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAE@ABV01@@Z (0x005BBDF1, FableWin.pdb), not the
//   stand-in ??0CSecond_005bbdf1@@QAE@ABU0@@Z. Placement new is <new>'s (explicit __cdecl, as HEAD).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
#include <new>

class CEngineTextureMorph;
namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

// Same-TU stand-in bodies of the out-of-line helpers (the STLport text of 0x00728B10 / 0x00728A40 / 0x00722130): VC7.1
// learns that the empty tags are never read and homes the __false_type temporaries in the atend slot, as retail.
#pragma auto_inline(off)
template <class InputIter, class ForwardIter>
__declspec(noinline) ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class ForwardIter, class Size, class T>
inline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class ForwardIter>
__declspec(noinline) void __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
#pragma auto_inline(on)
#pragma auto_inline(off)
template <class ForwardIter, class Size, class T>
__declspec(noinline) ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}
#pragma auto_inline(on)
template <class T> inline void _Destroy(T* first, T* last) { __destroy_aux(first, last, __false_type()); }

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};
template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;      // +0x08
};
template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    vector(const vector<T, Alloc>& x);                     // 0x005BBDF1 for T = CEngineTextureMorph
    ~vector();                                              // 0x005BC0D3 for T = CEngineTextureMorph
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    public:
    protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
    public:
};

}

template <class T> class CArray : public std::vector<T> {   // retail: CArray<T> is an STLport vector (12 bytes); its copy is vector's
public:
};
class CEngineTextureMorphSet {                              // Ego_r.pdb (retail 12 bytes)
public:
    CArray<CEngineTextureMorph> Morphs;                     // +0x00
};
class CTCAppearanceMorphBase {
public:
    class CMeshMorphSet {
    public:
        class CMeshMorphSetParts {                          // Ego_r.pdb (retail 16 bytes)
        public:
            unsigned long          MeshIndex;               // +0x00
            CEngineTextureMorphSet TextureMorphs;           // +0x04
        };
    };
};

typedef CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts TMeshMorphSetParts;
template TMeshMorphSetParts* std::uninitialized_fill_n(TMeshMorphSetParts*, unsigned int, const TMeshMorphSetParts&);

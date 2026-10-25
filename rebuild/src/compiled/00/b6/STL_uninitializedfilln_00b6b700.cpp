// std::__uninitialized_fill_n(CVertexShader* first, unsigned int n, const CVertexShader& x, const __false_type&) -- STLport placement-copy of n vertex-shader handles into raw storage (shared data ReferenceCount bumped); returns the end of the run
// Leaf: ??$__uninitialized_fill_n@PAUCVertexShader@@IU1@@std@@YIPAUCVertexShader@@PAU1@IABU1@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6B700, 49 bytes, `__fastcall` (first ecx, n edx, &x and the
//   __false_type reference on the stack), `ret 8`. No callees.
// Identity: the only caller 0x00B6C120 (vector<CVertexShader>::_M_insert_overflow, the n > 1 arm: STLport calls
//   `__uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type())` there) passes a tag reference, not the by-value
//   allocator of the Dinkumware `_Uninit_fill_n` the landed name and the manifest carried. Its ret-4 sibling 0x00B6BB10 is
//   the 3-argument uninitialized_fill_n wrapper that _M_fill_insert 0x00B6C5D0 calls.
//   The element is CVertexShader {vptr 0x0129B3D0 (RTTI CVertexShader; slot 2 CountReferences 0x009BE210, slot 0 the
//   deleting dtor 0x009C1AE0), Data} (engine/CVertexShader.h is a flattened data-only record, so the polymorphic copy needs
//   this local virtual view, as the landed CVertexShader ctor 0x00A63020); the copy constructor shares the
//   CVertexShaderData block and bumps its ReferenceCount (+0x10, Ego_r CVertexShaderData).
// Shape: STLport 4.6 `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);` (unsigned count: `test edx,edx ; jbe`).
#include <new>

struct CVertexShaderData {
    void* __vftable;                              // +0
    void* VertexDeclaration;                      // +4
    void* VertexShader;                           // +8
    const void* FormatDecl;                       // +0xc
    unsigned long ReferenceCount;                 // +0x10
};

struct CVertexShader {                            // vtable 0x0129B3D0
    virtual void v00();
    CVertexShaderData* Data;                      // +4
    CVertexShader(const CVertexShader& other) : Data(other.Data)
    {
        if (Data)
            ++Data->ReferenceCount;
    }
};

struct __false_type { __false_type() {} };

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

template CVertexShader* __fastcall std::__uninitialized_fill_n<CVertexShader*, unsigned int, CVertexShader>(CVertexShader*,
    unsigned int, const CVertexShader&, const __false_type&);

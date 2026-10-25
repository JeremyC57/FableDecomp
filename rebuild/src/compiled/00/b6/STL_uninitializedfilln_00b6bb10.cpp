// std::uninitialized_fill_n(CVertexShader* first, unsigned int n, const CVertexShader& x) -- STLport uninitialized_fill_n over vertex-shader handles (the non-POD __uninitialized_fill_n inline): placement-copy n handles, bumping the shared data ReferenceCount; returns the end of the run
// Leaf: ??$uninitialized_fill_n@PAUCVertexShader@@IU1@@std@@YIPAUCVertexShader@@PAU1@IABU1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6BB10, 49 bytes (+15 int3 pad), `__fastcall` (first ecx, n edx, &x the
//   only stack argument), `ret 4`. No callees.
// Identity: the only caller 0x00B6C5D0 (vector<CVertexShader>::_M_fill_insert: STLport calls the 3-argument
//   `uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy)` there) passes no tag, so this is the public
//   wrapper with __uninitialized_fill_n(__false_type) inlined; its ret-8 sibling 0x00B6B700 is that __uninitialized_fill_n
//   (called by _M_insert_overflow 0x00B6C120). The landed name and the manifest carried the Dinkumware `_Uninit_fill_n`.
//   The element is CVertexShader {vptr 0x0129B3D0 (RTTI CVertexShader; slot 2 CountReferences 0x009BE210, slot 0 the
//   deleting dtor 0x009C1AE0), Data} (engine/CVertexShader.h is a flattened data-only record, so the polymorphic copy needs
//   this local virtual view, as the landed CVertexShader ctor 0x00A63020); the copy constructor shares the
//   CVertexShaderData block and bumps its ReferenceCount (+0x10, Ego_r CVertexShaderData).
// Shape: STLport 4.6 `return __uninitialized_fill_n(__first, __n, __x, _IS_POD_ITER(__first, _Tp));` with the loop
//   `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);` inline (unsigned count: `test edx,edx ; jbe`); VC7.1
//   keeps the inner helper out of line unless it is __forceinline (it then emits a 23-byte forwarder).
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
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
}

template CVertexShader* __fastcall std::uninitialized_fill_n<CVertexShader*, unsigned int, CVertexShader>(CVertexShader*,
    unsigned int, const CVertexShader&);

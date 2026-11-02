// CShaderBankEntry::~CShaderBankEntry() -- implicit destructor of a shader bank entry: VSConstantLayout, the ShaderConstants strings and block, the ShaderCode block, then ~CBaseClassNonCopyable
// Leaf: ??1CShaderBankEntry@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A628D0, 79 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable` 0x0099A300.
// Identity (proven, two sources): its only caller is the scalar deleting dtor 0x00A628B0, slot 0 of vtable 0x0129D698 (rtti-vtables.tsv:
//   RTTI .?AVCShaderBankEntry@@ : CBaseClassNonCopyable : CBaseClass), the vtable the ctor 0x00A62120 stores (it zeroes the two vectors
//   at +0x8 / +0x14 and builds the string at +0x20); the members it destroys are Ego_r.pdb CShaderBankEntry's VSConstantLayout
//   (CCharString), ShaderConstants (CArray<CCharString>) and ShaderCode (CArray<unsigned long>) at their retail STLport offsets. It
//   stores no vtable of its own: the compiler-generated (virtual) destructor ??1CShaderBankEntry@@UAE@XZ (Ego_r lists only the
//   ??_E 0x00BE02C0: it inlines implicit dtors). The catalog label (CEnginePrimitiveRenderer::ResetVisibilityRatios) is propagated
//   and wrong.
// Callees: ??1CCharString@@QAE@XZ 0x0099EAE0 (VSConstantLayout, then each ShaderConstants element), _free 0x00BFEA14 (x2: the two
//   vector blocks, STLport allocator over malloc), ??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300 (tail).
// Layout: Ego_r.pdb CShaderBankEntry (0x2C with 16-byte Dinkumware vectors) at retail STLport sizes: ShaderType +0x4, ShaderCode +0x8,
//   ShaderConstants +0x14, VSConstantLayout +0x20, sizeof 0x24. engine/CBaseClassNonCopyable.h, fable_string.h (CCharString); CArray<T>
//   is Fable's CArray over the STLport vector (FableWin ??0?$CArray@K@@QAE@XZ / ??1?$CArray@K@@QAE@XZ).
// TU: retail's own ctor ??0CShaderBankEntry@@QAE@KKPAVCShaderDataBank@@@Z is defined with its member construction (the shader
//   loading that follows in retail's body is not reconstructed here); it emits the vtable, the ??_G and this implicit destructor.
#include "engine/CBaseClassNonCopyable.h"                  // CBaseClassNonCopyable : CBaseClass (~ 0x0099A300)
#include "fable_string.h"                                   // CCharString (0x0099E4B0 / 0x0099EAE0)

extern "C" void __cdecl free(void* block);                  // 0x00BFEA14 _free (MSVCR71 import thunk)

namespace std {                                             // STLport 4.6 (_alloc.h / _construct.h / _vector.h), the members this body expands
template <class _Tp> class allocator { public:
    typedef unsigned int size_type;
    void deallocate(_Tp* __p, size_type) { if (__p != 0) free((void*)__p); }   // __sgi_alloc = the malloc allocator
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) {}
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) {}
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

class CShaderDataBank;

class CShaderBankEntry : public CBaseClassNonCopyable {     // vtable 0x0129D698
public:
    CShaderBankEntry(unsigned long shaderType, unsigned long index, CShaderDataBank* bank);   // ??0CShaderBankEntry@@QAE@KKPAVCShaderDataBank@@@Z
    unsigned long          ShaderType;                      // +0x04
    CArray<unsigned long>  ShaderCode;                      // +0x08
    CArray<CCharString>    ShaderConstants;                 // +0x14
    CCharString            VSConstantLayout;                // +0x20
};
FABLE_STATIC_ASSERT(sizeof(CShaderBankEntry) == 0x24);

CShaderBankEntry::CShaderBankEntry(unsigned long shaderType, unsigned long, CShaderDataBank*)
    : ShaderType(shaderType)
{
}
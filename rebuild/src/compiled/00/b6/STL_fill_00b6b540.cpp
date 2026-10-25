// std::fill(CVertexShader* first, CVertexShader* last, const CVertexShader& val) -- assign val to every vertex shader handle of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6B540, 60 bytes. __fastcall (first ecx, last edx), `ret 4` (&val on
//   the stack).
// Callees: none (CVertexShader::operator= inline; the release calls CVertexShaderData's virtual deleting destructor).
// Callers: vector<CVertexShader>::_M_fill_insert 0x00B6C5D0 (call 0x00B6C6B4) in the CEngineLandscapeRenderer TU.
// Identity: the manifest label "_Copy_backward<CVertexShader*,CVertexShader*>" is propagated and wrong (a fill, one
//   source element); STLport `fill(_ForwardIter, _ForwardIter, const _Tp&)`.
// Shape: `for (; first != last; ++first) *first = val;` -- val's address is loaded once (ebx), the release test and the
//   share / count bump of the inlined operator= per element.
// The element is CVertexShader (8 bytes: vptr 0x0129B3D0, CVertexShaderData* Data). engine/CVertexShader.h is a flattened
// data-only record, so the class is modelled locally with its (inlined) assignment operator, written with the
// FableWin.pdb members `??4CVertexShader@@QAEABV0@ABV0@@Z`, `?Release@CVertexShaderData@@AAEXXZ` and
// `?AddReference@CVertexShaderData@@AAEXXZ`: release the old shared data (`--ReferenceCount`, then `delete this` -- the
// virtual deleting destructor at slot 0, no null test), share rhs.Data and add a reference (no null test; load/inc/store).
class CVertexShaderData {                               // vtable 0x0129DA50 (CBaseClassNonCopyable-derived)
    friend class CVertexShader;
public:
    virtual ~CVertexShaderData();                       // slot 0: deleting destructor 0x00A630D0
    void* VertexDeclaration;                            // +0x04 IDirect3DVertexDeclaration9*
    void* VertexShader;                                 // +0x08 IDirect3DVertexShader9*
    const void* FormatDecl;                             // +0x0C _D3DVERTEXELEMENT9*
    unsigned long ReferenceCount;                       // +0x10
private:
    void AddReference() { ++ReferenceCount; }
    void Release()
    {
        if (--ReferenceCount == 0)
            delete this;
    }
};

class CVertexShader {                                   // vtable 0x0129B3D0
public:
    virtual ~CVertexShader();
    CVertexShaderData* Data;                            // +0x04
    const CVertexShader& operator=(const CVertexShader& rhs)
    {
        if (Data)
            Data->Release();
        Data = rhs.Data;
        Data->AddReference();
        return *this;
    }
};

namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}

template void __fastcall fill<CVertexShader*, CVertexShader>(CVertexShader*, CVertexShader*, const CVertexShader&);
}
// _STL::__copy(CVertexShader* first, CVertexShader* last, CVertexShader* result, const random_access_iterator_tag&, int*) -- counted forward copy of vertex shader handles (assignment, shared data re-counted); returns the end of the destination
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6B610, 75 bytes. __fastcall (first ecx, last edx), `ret 0Ch` (result,
//   the tag reference and the null distance pointer on the stack), result in eax.
// Callees: none (CVertexShader::operator= inline).
// Callers: vector<CVertexShader>::erase 0x00B6C050 (call 0x00B6C069) and 0x00B6BA0C, in the CEngineLandscapeRenderer TU.
// Identity: the manifest label "_Copy_backward<CVertexShader*,CVertexShader*>" is propagated and wrong (a forward copy);
//   STLport `__copy(_RandomAccessIter, _RandomAccessIter, _OutputIter, const random_access_iterator_tag&, _Distance*)`,
//   the out-of-line tail of copy() -> __copy_aux -> __copy_ptrs(__false_type) for a non-trivially-assignable element.
// Shape: `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;` --
//   first is strength-reduced to &first->Data (`lea edi,[ecx+4]`), the count in ebx.
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

namespace _STL {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template CVertexShader* __fastcall __copy<CVertexShader*, CVertexShader*, int>(CVertexShader*, CVertexShader*,
    CVertexShader*, const random_access_iterator_tag&, int*);
}
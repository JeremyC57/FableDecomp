// _global::UninitFillN_CVertexShader_00b6bb10(CVertexShader* first, unsigned long count, const CVertexShader& value) -- placement-copy `count` CVertexShader handles (shared data ReferenceCount bumped), return the advanced pointer
// Leaf: UninitFillN_CVertexShader_00b6bb10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6BB10, 49 bytes (+15 int3 pad). No callees.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; mov ecx,[value] ; push esi ; mov esi,edx ;
//   L: test eax,eax ; je S ; mov [eax],vtbl 0x0129B3D0 ; mov edx,[ecx+4] ; test edx,edx ;
//   mov [eax+4],edx ; je S ; inc [edx+0x10] ; S: add eax,8 ; dec esi ; jne L ; pop esi ; E: ret 4
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, the value reference the
// only stack argument, `ret 4`: this instantiation's caller passes no allocator). The manifest label is propagated: the vtable store is
// RTTI CVertexShader (0x0129B3D0; slot 2 CountReferences 0x009BE210, slot 0 the deleting dtor
// 0x009C1AE0), and the 8-byte element is CVertexShader {vptr, Data} (engine/CVertexShader.h).
// The copy constructor shares the CVertexShaderData block and bumps its ReferenceCount (+0x10,
// PDB CVertexShaderData: vptr, VertexDeclaration, VertexShader, FormatDecl, ReferenceCount).
// engine/CVertexShader.h is a flattened data-only record, so the polymorphic copy needs this
// local virtual view (same choice as the landed CVertexShader ctor 0x00A63020). The ret-8 twin
// with the by-value allocator is 0x00B6B700.
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

CVertexShader* __fastcall UninitFillN_CVertexShader_00b6bb10(CVertexShader* first, unsigned long count, const CVertexShader& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CVertexShader(value);
    return first;
}
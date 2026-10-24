// _global::UninitCopy_CVertexShader_00b6bab0(const CVertexShader* first, const CVertexShader* last, CVertexShader* result, const __false_type&) -- STLport __uninitialized_copy over 8-byte CVertexShader handles: placement-copy each handle (vtable, then the shared data pointer with its add-ref), return the advanced result
// Leaf: UninitCopy_CVertexShader_00b6bab0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6BAB0, length 53 bytes, `ret 8`.
// Callers: the vector<CVertexShader> regrowth @ 0x00B6C120 (calls 0x00B6C17F / 0x00B6C1D3) and range insert @ 0x00B6C5D0
//   (calls 0x00B6C63A / 0x00B6C697).
// Callees: none (the copy constructor is inline). Vtable 0x0129B3D0 = CVertexShader (rtti-vtables.tsv).
// Layout (Ego_r.pdb): CVertexShader (8 bytes) = vptr +0x0, CVertexShaderData* Data +0x4; CVertexShaderData (20 bytes)
//   keeps its ReferenceCount at +0x10.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack): placement new (null test) of CVertexShader's copy
//   constructor -- the vtable store, the data pointer, `if (Data) ++Data->ReferenceCount` (`inc [esi+10h]`).
// Label correction: the catalog's `_Uninit_copy<..CScriptThing..>` is propagated (CScriptThing is 12 bytes).
inline void* operator new(unsigned int, void* where) { return where; }

struct IDirect3DVertexDeclaration9;
struct IDirect3DVertexShader9;
struct _D3DVERTEXELEMENT9;

struct CVertexShaderData {                     // Ego_r.pdb, 20 bytes
    virtual ~CVertexShaderData();              // +0x00 vptr
    IDirect3DVertexDeclaration9* VertexDeclaration;  // +0x04
    IDirect3DVertexShader9* VertexShader;      // +0x08
    _D3DVERTEXELEMENT9* FormatDecl;            // +0x0C
    unsigned long ReferenceCount;              // +0x10
};

class CVertexShader {
public:
    virtual ~CVertexShader();                  // vtable 0x0129B3D0
    CVertexShader(const CVertexShader& other) : Data(other.Data)
    {
        if (Data)
            ++Data->ReferenceCount;
    }
    CVertexShaderData* Data;                   // +0x4
};

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

CVertexShader* __fastcall UninitCopy_CVertexShader_00b6bab0(const CVertexShader* first, const CVertexShader* last, CVertexShader* result, const __false_type&)
{
    CVertexShader* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}
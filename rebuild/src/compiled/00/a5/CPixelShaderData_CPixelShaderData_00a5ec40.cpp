// CPixelShaderData::CPixelShaderData(IDirect3DPixelShader9* shader, const vector<long>& constIndices, long unused) -- wrap a created pixel shader with one reference and a private copy of its constant-index list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5EC40, length 107 bytes (+ int3 padding).
// Leaf: ??0CPixelShaderData
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, operator new[] @ 0x00BFEB22, memmove @ 0x00BFEAE6 (STLport copy of a POD range).
// Layout: Ego_r.pdb CPixelShaderData (0x14 bytes, vtable 0x0129D58C): PixelShader +4, ReferenceCount +8, ConstCount
// +0xc, ConstIndices +0x10.
// Shape: the shader stored from the argument before the vtable store, ReferenceCount = 1, ConstCount = the vector's
// signed `>> 2` size, ConstIndices = 0 after it (initialiser list); the body allocates `count` longs and copies the
// range with STLport's trivial copy (skipped for an empty range); the third argument is unused; `ret 0Ch`.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22

struct IDirect3DPixelShader9;
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};

struct CLongVector {                               // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    const long* begin() const { return _M_start; }
    const long* end() const { return _M_finish; }
    long size() const { return _M_finish - _M_start; }
};

inline long* CopyTrivial(const long* first, const long* last, long* result)   // STLport __copy_trivial
{
    if (last != first)
        memmove(result, first, (const char*)last - (const char*)first);
    return result;
}

struct CPixelShaderData : CBaseClassNonCopyable {                  // vtable 0x0129D58C, 0x14 bytes
    IDirect3DPixelShader9* PixelShader;            // +0x04
    unsigned long          ReferenceCount;         // +0x08
    unsigned long          ConstCount;             // +0x0c
    long*                  ConstIndices;           // +0x10
    CPixelShaderData(IDirect3DPixelShader9* shader, const CLongVector& constIndices, long unused);
};

CPixelShaderData::CPixelShaderData(IDirect3DPixelShader9* shader, const CLongVector& constIndices, long unused)
    : PixelShader(shader), ReferenceCount(1), ConstCount(constIndices.size()), ConstIndices(0)
{
    if (ConstCount) {
        ConstIndices = new long[ConstCount];
        CopyTrivial(constIndices.begin(), constIndices.end(), ConstIndices);
    }
}
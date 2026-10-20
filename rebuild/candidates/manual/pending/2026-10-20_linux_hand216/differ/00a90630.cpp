// Grade: DIFFER(203v202) -- VC7.1 jump-threads the consecutive `compressed` choices (normal, uv) into one test; retail tests esi again for each (`test esi,esi ; lea ecx ; je ; push imm`); ternary arguments compile branchless (sbb) instead
// N3DPrimitive::CVertexShaderInputAnimated::Initialise(unsigned long flags, bool b) -- builds the animated vertex stream format (position 13 or 2, two blend elements of type 4, normal 12/2, uv 7/1, optional colour 11/3 when flags & 2) and adds it as stream 0
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A90630, 202 bytes (padded to 208), `ret 8`.
//   PDB name ?Initialise@CVertexShaderInputAnimated@N3DPrimitive@@QAEXK_N@Z.
// Callees: CVertexStreamFormat::CVertexStreamFormat() 0x00AB37F0, CVertexStreamFormat::AddDataElement(unsigned long,
//   EVertexStreamDataType) 0x00AB3810, CVertexShaderInput::AddStream(unsigned long, const CVertexStreamFormat&, bool)
//   0x00AB3B40, CRT free 0x00BFEA14 (the DataElements array), CBaseClass::~CBaseClass 0x0099A2E0.
//   PDB names: ??0CVertexStreamFormat@@QAE@XZ, ?AddDataElement@CVertexStreamFormat@@QAEXKW4EVertexStreamDataType@@@Z,
//   ?AddStream@CVertexShaderInput@@QAEXKABVCVertexStreamFormat@@_N@Z.
// Shape: the stream format is a 16-byte local (CBaseClass vptr + 12-byte CArray<CDataElement>); its destructor is
//   compiler-generated (no vtable re-store: the array's inline free, then the out-of-line base destructor); the
//   compressed bit `flags & 4` is a named local (esi); the position choice (an `&&` condition) is an if/else of two
//   complete AddDataElement calls (tail-merged: `push imm` per arm, then the shared `push 0 ; call`), the other
//   choices are ternary arguments over enum constants (a ternary over an `&&` gives `mov eax,imm` instead, and
//   if/else pairs on the same test are jump-threaded together); `!b` is the `sete al` argument.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
struct CBaseClass {
    virtual ~CBaseClass();                         // 0x0099A2E0
};
enum EVertexStreamDataType {                       // values follow D3DDECLTYPE
    VSDT_FLOAT2 = 1, VSDT_FLOAT3 = 2, VSDT_FLOAT4 = 3, VSDT_D3DCOLOR = 4, VSDT_SHORT4 = 7,
    VSDT_USHORT2N = 11, VSDT_USHORT4N = 12, VSDT_UDEC3 = 13
};
template <class T> struct CArray {                 // retail CArray (STLport vector, 12 bytes)
    T* m_first;
    T* m_last;
    T* m_end;
    ~CArray()
    {
        if (m_first)
            free(m_first);
    }
};
struct CVertexStreamFormat : CBaseClass {          // 16 bytes
    struct CDataElement { unsigned long Usage; EVertexStreamDataType Type; };
    CArray<CDataElement> DataElements;             // +0x04
    CVertexStreamFormat();                         // 0x00AB37F0
    void AddDataElement(unsigned long usage, EVertexStreamDataType type);   // 0x00AB3810
};
struct CVertexShaderInput {
    void AddStream(unsigned long stream, const CVertexStreamFormat& format, bool flag);   // 0x00AB3B40
};
namespace N3DPrimitive {
struct CVertexShaderInputAnimated : CVertexShaderInput {
    void Initialise(unsigned long flags, bool b);
};
void CVertexShaderInputAnimated::Initialise(unsigned long flags, bool b)
{
    CVertexStreamFormat format;
    unsigned long compressed = flags & 4;
    if (compressed && !(flags & 0x10))
        format.AddDataElement(0, VSDT_UDEC3);
    else
        format.AddDataElement(0, VSDT_FLOAT3);
    format.AddDataElement(1, VSDT_D3DCOLOR);
    format.AddDataElement(2, VSDT_D3DCOLOR);
    compressed ? format.AddDataElement(3, VSDT_USHORT4N) : format.AddDataElement(3, VSDT_FLOAT3);
    compressed ? format.AddDataElement(4, VSDT_SHORT4) : format.AddDataElement(4, VSDT_FLOAT2);
    if (flags & 2)
        compressed ? format.AddDataElement(5, VSDT_USHORT2N) : format.AddDataElement(5, VSDT_FLOAT4);
    AddStream(0, format, !b);
}
}

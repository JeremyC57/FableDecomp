// N3DPrimitive::CVertexShaderInputStatic::Initialise(unsigned long flags, bool b) -- builds the static vertex stream format (compressed: position 13 or 2, normal 12, uv 7; else 2, 2, 1; optional colour 11/3 when flags & 2) and adds it as stream 0
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A90560, 195 bytes (padded to 208), `ret 8`.
//   PDB name ?Initialise@CVertexShaderInputStatic@N3DPrimitive@@QAEXK_N@Z.
// Callees: CVertexStreamFormat::CVertexStreamFormat() 0x00AB37F0, CVertexStreamFormat::AddDataElement(unsigned long,
//   EVertexStreamDataType) 0x00AB3810, CVertexShaderInput::AddStream(unsigned long, const CVertexStreamFormat&, bool)
//   0x00AB3B40, CRT free 0x00BFEA14 (the DataElements array), CBaseClass::~CBaseClass 0x0099A2E0.
//   PDB names: ??0CVertexStreamFormat@@QAE@XZ, ?AddDataElement@CVertexStreamFormat@@QAEXKW4EVertexStreamDataType@@@Z,
//   ?AddStream@CVertexShaderInput@@QAEXKABVCVertexStreamFormat@@_N@Z.
// Shape: the stream format is a 16-byte local (CBaseClass vptr + 12-byte CArray<CDataElement>); its destructor is
//   compiler-generated (no vtable re-store: the array's inline free, then the out-of-line base destructor); the
//   compressed bit `flags & 4` is a named local (esi); the compressed/plain element sets are an if/else whose last
//   calls tail-merge (`push 7 ; jmp` / `push 1`); the position type is a local defaulted then overridden
//   (`mov eax,13 ; je ; mov eax,2` -- a ternary compiles branchless with sbb); the optional colour is an if/else of
//   two complete calls; `!b` is the `sete al` argument.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
enum EVertexStreamDataType {                       // values follow D3DDECLTYPE
    VSDT_FLOAT2 = 1, VSDT_FLOAT3 = 2, VSDT_FLOAT4 = 3, VSDT_D3DCOLOR = 4, VSDT_SHORT4 = 7,
    VSDT_USHORT2N = 11, VSDT_USHORT4N = 12, VSDT_UDEC3 = 13
};
template <class T> class CArray { public:                 // retail CArray (STLport vector, 12 bytes)
    T* m_first;
    T* m_last;
    T* m_end;
    ~CArray()
    {
        if (m_first)
            free(m_first);
    }
};
class CVertexStreamFormat : public CBaseClass { public:          // 16 bytes
    class CDataElement { public: unsigned long Usage; EVertexStreamDataType Type; };
    CArray<CDataElement> DataElements;             // +0x04
    CVertexStreamFormat();                         // 0x00AB37F0
    void AddDataElement(unsigned long usage, EVertexStreamDataType type);   // 0x00AB3810
};
class CVertexShaderInput { public:
    void AddStream(unsigned long stream, const CVertexStreamFormat& format, bool flag);   // 0x00AB3B40
};
namespace N3DPrimitive {
struct CVertexShaderInputStatic : CVertexShaderInput {
    void Initialise(unsigned long flags, bool b);
};
void CVertexShaderInputStatic::Initialise(unsigned long flags, bool b)
{
    CVertexStreamFormat format;
    unsigned long compressed = flags & 4;
    if (compressed) {
        EVertexStreamDataType position = VSDT_UDEC3;
        if (flags & 0x10)
            position = VSDT_FLOAT3;
        format.AddDataElement(0, position);
        format.AddDataElement(1, VSDT_USHORT4N);
        format.AddDataElement(2, VSDT_SHORT4);
    }
    else {
        format.AddDataElement(0, VSDT_FLOAT3);
        format.AddDataElement(1, VSDT_FLOAT3);
        format.AddDataElement(2, VSDT_FLOAT2);
    }
    if (flags & 2) {
        if (compressed)
            format.AddDataElement(3, VSDT_USHORT2N);
        else
            format.AddDataElement(3, VSDT_FLOAT4);
    }
    AddStream(0, format, !b);
}
}
// Behaviour test for 00a90630 N3DPrimitive::CVertexShaderInputAnimated::Initialise: final.cpp body verbatim; the stream
// format callees record the (usage, type) pairs and the AddStream arguments.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
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
//   compressed bit `flags & 4` is written out in every condition (CSE keeps it in esi; a named local lets VC7.1
//   jump-thread the normal/uv choices into one test); the position choice (an `&&` condition) is an if/else of two
//   complete AddDataElement calls (tail-merged: `push imm` per arm, then the shared `push 0 ; call`), the other
//   choices are conditional expressions over two complete calls (ternary arguments give `mov eax,imm` forms);
//   `!b` is the `sete al` argument.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
class CBaseClass { public:
    virtual ~CBaseClass();                         // 0x0099A2E0
};
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
struct CVertexShaderInputAnimated : CVertexShaderInput {
    void Initialise(unsigned long flags, bool b);
};
void CVertexShaderInputAnimated::Initialise(unsigned long flags, bool b)
{
    CVertexStreamFormat format;
    if ((flags & 4) && !(flags & 0x10))
        format.AddDataElement(0, VSDT_UDEC3);
    else
        format.AddDataElement(0, VSDT_FLOAT3);
    format.AddDataElement(1, VSDT_D3DCOLOR);
    format.AddDataElement(2, VSDT_D3DCOLOR);
    (flags & 4) ? format.AddDataElement(3, VSDT_USHORT4N) : format.AddDataElement(3, VSDT_FLOAT3);
    (flags & 4) ? format.AddDataElement(4, VSDT_SHORT4) : format.AddDataElement(4, VSDT_FLOAT2);
    if (flags & 2)
        (flags & 4) ? format.AddDataElement(5, VSDT_USHORT2N) : format.AddDataElement(5, VSDT_FLOAT4);
    AddStream(0, format, !b);
}
}
static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }
static unsigned long g_usage[8]; static int g_type[8]; static int g_count = 0;
static int g_streams = 0, g_baseDtors = 0; static unsigned long g_stream = 99; static bool g_flag = false;
static const CVertexStreamFormat* g_format = 0;
CBaseClass::~CBaseClass() { ++g_baseDtors; }
CVertexStreamFormat::CVertexStreamFormat() { DataElements.m_first = DataElements.m_last = DataElements.m_end = 0; }
void CVertexStreamFormat::AddDataElement(unsigned long usage, EVertexStreamDataType type)
{ if (g_count < 8) { g_usage[g_count] = usage; g_type[g_count] = type; } ++g_count; }
void CVertexShaderInput::AddStream(unsigned long stream, const CVertexStreamFormat& format, bool flag)
{ ++g_streams; g_stream = stream; g_flag = flag; g_format = &format; }
static bool Expect(const int* types, int n)
{
    if (g_count != n) return false;
    for (int i = 0; i < n; ++i) if (g_usage[i] != (unsigned long)i || g_type[i] != types[i]) return false;
    return true;
}
static void Run(unsigned long flags, bool b) { g_count = 0; N3DPrimitive::CVertexShaderInputAnimated input; input.Initialise(flags, b); }
int main()
{
    int plain[] = { 2, 4, 4, 2, 1 };
    Run(0, false);
    Check(Expect(plain, 5), "uncompressed elements");
    Check(g_streams == 1 && g_stream == 0 && g_flag && g_format != 0 && g_baseDtors == 1, "stream added, format destroyed");
    int comp[] = { 13, 4, 4, 12, 7 };
    Run(4, true);
    Check(Expect(comp, 5) && !g_flag, "compressed elements");
    int compColour[] = { 2, 4, 4, 12, 7, 11 };
    Run(4 | 0x10 | 2, false);
    Check(Expect(compColour, 6), "compressed, 0x10 keeps float3 position, colour ushort2n");
    int plainColour[] = { 2, 4, 4, 2, 1, 3 };
    Run(2, false);
    Check(Expect(plainColour, 6) && g_streams == 4 && g_baseDtors == 4, "uncompressed colour float4");
    printf(g_fail ? "00a90630_TEST FAIL\n" : "00a90630_TEST PASS\n");
    return g_fail;
}
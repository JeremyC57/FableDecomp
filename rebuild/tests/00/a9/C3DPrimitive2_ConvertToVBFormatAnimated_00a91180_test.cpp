// Behaviour test for 00a91180: final.cpp copied verbatim, the callees stubbed below so the staging path, the transforms,
// the range check and the conversion are observable. C3DPrimitive2::ConvertToVBFormatAnimated
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#define _snprintf snprintf
#endif
#define free Test_free
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// C3DPrimitive2::ConvertToVBFormatAnimated(void*, unsigned long, unsigned long, CVertexCompressionParams*) -- transform every animated block into the destination (or a compressed staging buffer) and convert the stream when the compression bit is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A91180, length 519 bytes.
// Shape (chunk 277 park, AdvA2): the empty iterator tag's slot is retail's dead flags argument slot once DestroyRange has a
//   same-TU OUT-ONLY stand-in (the tag is never read, so the temporary needs no frame word); the staging byte count is
//   multiplied twice -- `count` for the 31-bit range check and `stride * vertexCount` again in resize -- which keeps
//   `stride` live across the check and gives retail's `mov eax,edi ; imul eax,[esp+14h]` (a single `count` is multiplied
//   in place: `imul edi ; mov eax,edi`).
extern "C" void __cdecl free(void*); // 0x00BFEA14
class CVertexCompressionParams { public: float PosScale[4]; float PosBias[4]; };
void __fastcall ConvertAnimatedVertices(const void*, unsigned long, void*, unsigned long, unsigned long, CVertexCompressionParams&); // 0x00A90FC0
struct CVertexFormat { unsigned long GetVertexSize() const; }; // 0x00AB37B0
struct CStreamInfo { long Unnamed00; CVertexFormat Format; };
struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };
void __fastcall DestroyRange(CStreamInfo* first, CStreamInfo* last, const SNonscalarPtrIteratorTag& tag);   // 0x00A5E900
void __fastcall DestroyRange_005e900(CStreamInfo* first, CStreamInfo* last);   // the real work of 0x00A5E900
// Same-TU out-only stand-in for the DestroyRange instantiation 0x00A5E900: the tag is never read, so VC7.1 knows the
// temporary's slot carries nothing.
#pragma auto_inline(off)
__declspec(noinline) void __fastcall DestroyRange(CStreamInfo* first, CStreamInfo* last, const SNonscalarPtrIteratorTag& tag)
{
    DestroyRange_005e900(first, last);
}
#pragma auto_inline(on)
struct CStreamInfoArray {
    CStreamInfo* first;
    CStreamInfo* last;
    CStreamInfo* end;
    ~CStreamInfoArray()
    {
        SNonscalarPtrIteratorTag tag;
        DestroyRange(first, last, tag);
        if (first != 0)
            free(first);
    }
};
struct D3DVERTEXELEMENT9;
struct CVertexElementArray {
    D3DVERTEXELEMENT9* first;
    D3DVERTEXELEMENT9* last;
    D3DVERTEXELEMENT9* end;
    ~CVertexElementArray() { if (first) free(first); }
};
class CBaseClass { public:
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
};
class CVertexShaderInput : public CBaseClass { public:           // vtable 0x0129EA08
    CStreamInfoArray    DataStreams;               // +0x04
    CVertexElementArray D3DDeclaration;            // +0x10
    CVertexShaderInput();                          // 0x00AB3B10
};
namespace N3DPrimitive {
struct CVertexShaderInputAnimated : CVertexShaderInput {   // vtable 0x0129DFB0
    CVertexShaderInputAnimated() { }
    void Initialise(unsigned long, bool);          // 0x00A90630
};
}
struct CByteVector {
    unsigned char* first;
    unsigned char* last;
    unsigned char* end;
    void insert(unsigned char*, unsigned long, const unsigned char&); // 0x00485F3A
    unsigned long size() const { return (unsigned long)(last - first); }
    __forceinline void resize(unsigned long count, const unsigned char& value)
    {
        if (count < size())
            last = first + count;
        else
            insert(last, count - size(), value);
    }
    ~CByteVector() { if (first) free(first); }
};
struct CDeviceIndependent;
struct CAnimatedVertexWithNormals { unsigned char bytes[0x38]; };
struct CAnimatedVertex { unsigned char bytes[0x28]; };
struct CAnimatedBlock {                            // Layout: Ego_r.pdb CAnimatedBlock, size 0x30
    unsigned char pad0[0x10];
    unsigned long VertexCount;                     // +0x10
    unsigned char pad14[0x1c];
    void TransformVerticesWithNormals(CAnimatedVertexWithNormals*, const CDeviceIndependent*); // 0x00A90040
    void TransformVertices(CAnimatedVertex*, const CDeviceIndependent*);                       // 0x00A8FF50
};
class C3DPrimitive2 { public:                             // Layout: retail offsets (Ego_r.pdb - 4)
    unsigned char pad0[0x2c];
    CDeviceIndependent* Data;                      // +0x2c
    unsigned char pad30[0x18];
    unsigned long AnimatedBlockCount;              // +0x48
    CAnimatedBlock* AnimatedBlocks;                // +0x4c
    void ConvertToVBFormatAnimated(void*, unsigned long, unsigned long, CVertexCompressionParams*);
};

void C3DPrimitive2::ConvertToVBFormatAnimated(void* destination, unsigned long sourceFormat,
                                              unsigned long flags, CVertexCompressionParams* params)
{
    unsigned long vertexCount = 0;
    for (unsigned long b = 0; b < AnimatedBlockCount; ++b)
        vertexCount += AnimatedBlocks[b].VertexCount;
    CByteVector bytes;
    bytes.first = 0;
    bytes.last = 0;
    bytes.end = 0;
    unsigned long convertedFormat = flags & ~4;
    void* transformed = destination;
    if (flags != convertedFormat) {
        unsigned long stride;
        {
            N3DPrimitive::CVertexShaderInputAnimated input;
            input.Initialise(convertedFormat, true);
            stride = input.DataStreams.first->Format.GetVertexSize();
        }
        unsigned long count = stride * vertexCount;
        __int64 checkedCount = (__int64)count;
        if (!(checkedCount >= 0 && checkedCount <= 0x7fffffffLL))
            return;
        unsigned char zero = 0;
        bytes.resize(stride * vertexCount, zero);
        transformed = (unsigned long)(bytes.last - bytes.first) != 0 ? (void*)bytes.first : (void*)&bytes;
    }
    unsigned long vertexOffset = 0;
    for (unsigned long i = 0; i < AnimatedBlockCount; ++i) {
        CAnimatedBlock& block = AnimatedBlocks[i];
        if (flags & 2)
            block.TransformVerticesWithNormals((CAnimatedVertexWithNormals*)transformed + vertexOffset, Data);
        else
            block.TransformVertices((CAnimatedVertex*)transformed + vertexOffset, Data);
        vertexOffset += block.VertexCount;
    }
    if (flags != convertedFormat)
        ConvertAnimatedVertices(transformed, convertedFormat, destination, flags, vertexCount, *params);
}
// ---- stubs ----
#undef free
extern "C" void Test_free(void* p) { note("free"); free(p); }
static unsigned long g_vertexSize = 0x38;
static unsigned char g_destination[4096];
static void* g_staging = 0;
unsigned long CVertexFormat::GetVertexSize() const { note("vsize"); return g_vertexSize; }
void __fastcall DestroyRange_005e900(CStreamInfo* first, CStreamInfo* last) { char b[48]; _snprintf(b, sizeof b, "destroy(%d)", (int)(last - first)); note(b); }
void CBaseClass::v00() { }
CBaseClass::~CBaseClass() { note("~base"); }
CVertexShaderInput::CVertexShaderInput() { DataStreams.first = DataStreams.last = DataStreams.end = 0; D3DDeclaration.first = D3DDeclaration.last = D3DDeclaration.end = 0; }
void N3DPrimitive::CVertexShaderInputAnimated::Initialise(unsigned long format, bool animated)
{
    char b[48]; _snprintf(b, sizeof b, "init(%lx,%d)", format, animated ? 1 : 0); note(b);
    DataStreams.first = (CStreamInfo*)malloc(sizeof(CStreamInfo)); DataStreams.last = DataStreams.end = DataStreams.first + 1;
}
void CByteVector::insert(unsigned char* pos, unsigned long n, const unsigned char& value)
{
    char b[48]; _snprintf(b, sizeof b, "insert(%lu,%d)", n, (int)value); note(b);
    if (n) { first = (unsigned char*)malloc(n); memset(first, value, n); last = end = first + n; g_staging = first; }
}
static bool InDest(const void* p) { return p >= (const void*)g_destination && p < (const void*)(g_destination + sizeof g_destination); }
static long Offset(const void* p) { return (long)((const char*)p - (InDest(p) ? (const char*)g_destination : (const char*)g_staging)); }
void CAnimatedBlock::TransformVerticesWithNormals(CAnimatedVertexWithNormals* out, const CDeviceIndependent* data) { char b[64]; _snprintf(b, sizeof b, "normals(%s+%ld,%lx)", InDest(out) ? "dest" : "staging", Offset(out), (unsigned long)(size_t)data); note(b); }
void CAnimatedBlock::TransformVertices(CAnimatedVertex* out, const CDeviceIndependent* data) { char b[64]; _snprintf(b, sizeof b, "plain(%s+%ld)", InDest(out) ? "dest" : "staging", Offset(out)); note(b); }
void __fastcall ConvertAnimatedVertices(const void* src, unsigned long format, void* dst, unsigned long flags, unsigned long count, CVertexCompressionParams& params)
{
    char b[96]; _snprintf(b, sizeof b, "convert(%s,%lx,%s,%lx,%lu,%g)", src == g_staging ? "staging" : "other", format, dst == g_destination ? "dest" : "?", flags, count, params.PosScale[0]); note(b);
}
int main()
{
    C3DPrimitive2 prim; memset(&prim, 0, sizeof prim);
    CAnimatedBlock blocks[2]; memset(blocks, 0, sizeof blocks); blocks[0].VertexCount = 3; blocks[1].VertexCount = 5;
    prim.AnimatedBlocks = blocks; prim.AnimatedBlockCount = 2; prim.Data = (CDeviceIndependent*)0x1234;
    CVertexCompressionParams params; memset(&params, 0, sizeof params); params.PosScale[0] = 2.5f;
    // 1. no compression bit: transform straight into the destination (normals: 0x38 per vertex), no conversion
    g_log[0] = 0; prim.ConvertToVBFormatAnimated(g_destination, 0, 2, &params);
    CHECKLOG("normals(dest+0,1234);normals(dest+168,1234);");
    // 2. compression bit with normals: staging buffer of stride * 8 vertices, transforms into it, then the conversion
    g_log[0] = 0; prim.ConvertToVBFormatAnimated(g_destination, 0, 6, &params);
    CHECKLOG("init(2,1);vsize;destroy(1);free;~base;insert(448,0);normals(staging+0,1234);normals(staging+168,1234);convert(staging,2,dest,6,8,2.5);free;");
    // 3. compression without normals: plain transforms (0x28 per vertex) into the staging buffer
    g_vertexSize = 0x28; g_log[0] = 0; prim.ConvertToVBFormatAnimated(g_destination, 0, 4, &params);
    CHECKLOG("init(0,1);vsize;destroy(1);free;~base;insert(320,0);plain(staging+0);plain(staging+120);convert(staging,0,dest,4,8,2.5);free;");
    // 4. no vertices: an empty staging vector, the conversion gets the vector itself as the source
    prim.AnimatedBlockCount = 0; g_log[0] = 0; prim.ConvertToVBFormatAnimated(g_destination, 0, 4, &params);
    CHECKLOG("init(0,1);vsize;destroy(1);free;~base;insert(0,0);convert(other,0,dest,4,0,2.5);");
    // 5. byte count beyond 31 bits: give up after the shader input is torn down
    prim.AnimatedBlockCount = 2; g_vertexSize = 0x40000000; blocks[0].VertexCount = 1; blocks[1].VertexCount = 1;
    g_log[0] = 0; prim.ConvertToVBFormatAnimated(g_destination, 0, 4, &params);
    CHECKLOG("init(0,1);vsize;destroy(1);free;~base;");
    if (fails == 0) printf("00a91180_TEST PASS\n"); else printf("00a91180_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}
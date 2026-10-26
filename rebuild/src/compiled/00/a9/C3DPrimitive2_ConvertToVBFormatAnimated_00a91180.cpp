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
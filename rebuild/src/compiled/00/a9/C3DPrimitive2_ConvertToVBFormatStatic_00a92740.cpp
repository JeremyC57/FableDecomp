// C3DPrimitive2::ConvertToVBFormatStatic(void*, unsigned long, unsigned long, CVertexCompressionParams*)
// Retail 0x00A92740, 295 bytes (ret 0x10). Field-free C3DPrimitive2 adapter: RepeatingMeshReps +8,
// Data +0x2c, VertexCount +0x58. CVertexShaderInput is reconstructed from the landed adjacent
// destructor: CBaseClass vptr then DataStreams CArray +4 and D3DDeclaration CArray +0x10; true
// virtual destructor chain and static vtable 0x0129DFB8. Native getter @0x00AB37B0 receives
// DataStreams.first +4 (the stream's Format subobject), as shown by A927A7's mov [local+4]; add 4.
// Empty staging uses the vector object as its native sentinel.
// Invalid wrapped 32-bit byte count skips both transform paths and conversion while preserving cleanup.
#include <string.h>

extern "C" void __cdecl free(void*); // 0x00BFEA14
struct CVertexCompressionParams;
extern "C" void __fastcall ConvertStaticVertices(const void*, unsigned long, void*, unsigned long, unsigned long, CVertexCompressionParams&); // 0x00A91390

struct CVertexCompressionParams { float PosScale[4]; float PosBias[4]; };
struct CVertexFormat { unsigned long GetVertexSize() const; }; // 0x00AB37B0
struct CStreamInfo { long Unnamed00; CVertexFormat Format; };
struct D3DVERTEXELEMENT9;
template<class T> struct CArray {
    T* first;
    T* last;
    T* end;
};
struct CBaseClass {
    virtual ~CBaseClass(); // vtable 0x01231710, deleting destructor 0x004371F0
    CBaseClass();           // 0x0099A2D0
};
struct CVertexShaderInput : CBaseClass { // vtable 0x0129EA08, size 0x1c
    CArray<CStreamInfo> DataStreams;               // +4
    CArray<D3DVERTEXELEMENT9> D3DDeclaration;      // +0x10
    virtual ~CVertexShaderInput();                 // 0x00A5EB70 (AB4690 is deleting wrapper)
    CVertexShaderInput();                          // 0x00AB3B10
};
namespace N3DPrimitive {
struct CVertexShaderInputStatic : CVertexShaderInput { // vtable 0x0129DFB8
    CVertexShaderInputStatic() { }
    virtual ~CVertexShaderInputStatic();          // 0x00A98640
    void Initialise(unsigned long, bool);          // 0x00A90560
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

struct CStaticVertex { unsigned char bytes[32]; };
struct CArrayStaticVertex { CStaticVertex* first; CStaticVertex* last; CStaticVertex* end; };
struct CDeviceIndependent { long pad; CArrayStaticVertex Vertices; };
struct C3DPrimitive2 {
    unsigned char pad0[8];
    long RepeatingMeshReps;
    unsigned char pad0c[0x20];
    CDeviceIndependent* Data;
    unsigned char pad30[0x28];
    unsigned long VertexCount;
    void ConvertToVBFormatRepeatingMesh(void*); // 0x00A8F770
    void TransformVertexBufferWithNormals(void*); // 0x00A8F1B0
    void TransformVertexBuffer(void*);            // 0x00A8EFA0
    void ConvertToVBFormatStatic(void*, unsigned long, unsigned long, CVertexCompressionParams*);
};

void C3DPrimitive2::ConvertToVBFormatStatic(void* destination, unsigned long sourceFormat,
                                            unsigned long flags, CVertexCompressionParams* params)
{
    if (RepeatingMeshReps > 0) {
        ConvertToVBFormatRepeatingMesh(destination);
        return;
    }
    unsigned long convertedFormat = flags & ~4;
    CByteVector bytes;
    bytes.first = 0;
    bytes.last = 0;
    bytes.end = 0;
    void* transformed = destination;
    if (flags != convertedFormat) {
        unsigned long stride;
        {
            N3DPrimitive::CVertexShaderInputStatic input;
            input.Initialise(convertedFormat, true);
            stride = input.DataStreams.first->Format.GetVertexSize();
        }
        unsigned long count = VertexCount * stride;
        __int64 checkedCount = (__int64)count;
        if (!(checkedCount >= 0 && checkedCount <= 0x7fffffffLL))
            return;
        unsigned char zero = 0;
        bytes.resize(count, zero);
        transformed = (unsigned long)(bytes.last - bytes.first) != 0 ? (void*)bytes.first : (void*)&bytes;
    }
    {
        if (flags & 2)
            TransformVertexBufferWithNormals(transformed);
        else
            TransformVertexBuffer(transformed);
        if (flags != convertedFormat)
            ConvertStaticVertices(transformed, convertedFormat, destination, flags, VertexCount, *params);
    }
}
// C3DPrimitive2::CalculateVertexSize(unsigned long vertexFlags, bool animated, bool compressedLayout) -- byte size of one vertex for a primitive format: a fixed 0x24 for the third-flag layout, else the first stream's size from a temporary animated or static vertex-shader input built for the flags
// Leaf: ?CalculateVertexSize@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A910E0, 146 bytes (+ int3 padding), static `__fastcall` (vertexFlags in
//   ecx -> esi, animated in dl, the third bool on the stack), `ret 4`.
// Identity: Ego_r.pdb `?CalculateVertexSize@C3DPrimitive2@@SIKK_N0@Z` (static __fastcall unsigned long (unsigned long, bool,
//   bool)); the two temporaries carry the vtables of N3DPrimitive::CVertexShaderInputAnimated 0x0129DFB0 and
//   ...Static 0x0129DFB8 (rtti-vtables.tsv), whose Initialise(unsigned long, bool) are 0x00A90630 / 0x00A90560 (landed).
//   The meaning of the third bool (which short-circuits to 0x24 = 36 bytes) is not recoverable from these bytes.
// Callees: CVertexShaderInput::CVertexShaderInput 0x00AB3B10, CVertexShaderInputAnimated/Static::Initialise 0x00A90630 /
//   0x00A90560, CVertexStreamFormat::CalculateVertexSize 0x00AB37B0, ~CVertexShaderInputAnimated 0x00A986D0 and
//   ~CVertexShaderInputStatic 0x00A98640 (out of line, called directly on the known-type temporaries).
// Layout (retail, STLport 12-byte arrays, cf. the landed 0x00A90FC0): CVertexShaderInput 0x1C = {vptr, DataStreams +4,
//   D3DDeclaration +0x10}; CStreamInfo = {StreamIndex +0, CVertexStreamFormat StreamFormat +4}.
// Shape: the third bool tested first (early `return 0x24`), then one block per input kind: the derived temporary's
//   implicit constructor (base constructor call, then its own vtable store), Initialise(flags, true), the first stream
//   format's CalculateVertexSize() kept in esi across the temporary's destructor.
class CVertexStreamFormat {
public:
    virtual ~CVertexStreamFormat();
    unsigned char DataElements[0x0c];                   // +0x04 CArray<CDataElement> (12-byte STLport vector)
    unsigned long CalculateVertexSize() const;          // 0x00AB37B0
};

class CVertexShaderInput {
public:
    class CStreamInfo {
    public:
        unsigned long       StreamIndex;                // +0x00
        CVertexStreamFormat StreamFormat;               // +0x04
    };
    struct SArray {                                     // CArray<T>: the 12-byte STLport vector
        void* _M_start;
        void* _M_finish;
        void* _M_end_of_storage;
    };
    CVertexShaderInput();                               // 0x00AB3B10
    virtual ~CVertexShaderInput();
    CStreamInfo*  DataStreamsBegin;                     // +0x04 DataStreams._M_start
    CStreamInfo*  DataStreamsEnd;                       // +0x08
    CStreamInfo*  DataStreamsCapacity;                  // +0x0c
    SArray        D3DDeclaration;                       // +0x10
    const CVertexStreamFormat& GetStreamFormat(unsigned long index) const { return DataStreamsBegin[index].StreamFormat; }
};

namespace N3DPrimitive {
class CVertexShaderInputAnimated : public CVertexShaderInput {    // vtable 0x0129DFB0
public:
    virtual ~CVertexShaderInputAnimated();              // 0x00A986D0
    void Initialise(unsigned long vertexFlags, bool flag);   // 0x00A90630
};
class CVertexShaderInputStatic : public CVertexShaderInput {      // vtable 0x0129DFB8
public:
    virtual ~CVertexShaderInputStatic();                // 0x00A98640
    void Initialise(unsigned long vertexFlags, bool flag);   // 0x00A90560
};
}

class C3DPrimitive2 {
public:
    static unsigned long __fastcall CalculateVertexSize(unsigned long vertexFlags, bool animated, bool compressedLayout);
};

unsigned long __fastcall C3DPrimitive2::CalculateVertexSize(unsigned long vertexFlags, bool animated, bool compressedLayout)
{
    if (compressedLayout)
        return 0x24;
    if (animated) {
        N3DPrimitive::CVertexShaderInputAnimated input;
        input.Initialise(vertexFlags, true);
        return input.GetStreamFormat(0).CalculateVertexSize();
    } else {
        N3DPrimitive::CVertexShaderInputStatic input;
        input.Initialise(vertexFlags, true);
        return input.GetStreamFormat(0).CalculateVertexSize();
    }
}
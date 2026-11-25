// CVertexStreamConverter::ConvertStream(const void* src, long srcStride, const CVertexStreamFormat& srcFormat,
// void* dst, long dstStride, const CVertexStreamFormat& dstFormat, unsigned long count,
// const CVertexCompressionParams& params) -- for each data element the two formats share, read every vertex's element
// into a C4DVector and write it back in the destination type, then advance both element offsets by the type sizes
// Leaf: ConvertStream
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC4D70, 320 bytes (padded to 336), `ret 0x20`.
// Signature: Ego_r.pdb ?ConvertStream@CVertexStreamConverter@@QAEXPBXJABVCVertexStreamFormat@@PAXJ1KABVCVertexCompressionParams@@@Z.
// Callees: ConvertFrom(const char*, EVertexStreamDataType, C4DVector&, const CVertexCompressionParams&) 0x00AC44D0,
//   ConvertTo(char*, EVertexStreamDataType, const C4DVector&, const CVertexCompressionParams&) 0x00AC4910 (both private,
//   Ego_r.pdb), GetDataTypeSize(EVertexStreamDataType) 0x00AB3740 (`__fastcall` static).
// Layout: CVertexStreamFormat {vptr?, CArray<CDataElement> DataElements +0x04} (Ego_r.pdb), CDataElement {Register,
//   DataType +0x04} (8 bytes), C4DVector {X, Y, Z, W}.
// Shape: the element count is the `a < b ? a : b` of the two sizes (each recomputed in its arm).
enum EVertexStreamDataType { VERTEX_STREAM_DATA_TYPE_NONE };
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};
#include "engine/CArray.h"
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00AB3740 ?GetDataTypeSize@CVertexStreamFormat@@SIKW4EVertexStreamDataType@@@Z (was ?GetDataTypeSize@CVertexStreamConverter@@SIKW4EVertexStreamDataType@@@Z)
class CVertexStreamFormat { public:
    class CDataElement { public:
        unsigned long         Register;                // +0x00
        EVertexStreamDataType DataType;                // +0x04
    };
    unsigned long         _f00;                        // +0x00
    CArray<CDataElement>  DataElements;                // +0x04
 public: static unsigned long __fastcall GetDataTypeSize(EVertexStreamDataType); };
class CVertexCompressionParams { public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
};
struct CVertexStreamConverter {
    private: void ConvertFrom(const char* data, EVertexStreamDataType type, C4DVector& v, const CVertexCompressionParams& params); public:   // 0x00AC44D0
    private: void ConvertTo(char* data, EVertexStreamDataType type, const C4DVector& v, const CVertexCompressionParams& params); public:     // 0x00AC4910
    void ConvertStream(const void* src, long srcStride, const CVertexStreamFormat& srcFormat, void* dst, long dstStride,
                       const CVertexStreamFormat& dstFormat, unsigned long count, const CVertexCompressionParams& params);
};

void CVertexStreamConverter::ConvertStream(const void* src, long srcStride, const CVertexStreamFormat& srcFormat, void* dst,
                                           long dstStride, const CVertexStreamFormat& dstFormat, unsigned long count,
                                           const CVertexCompressionParams& params)
{
    unsigned long srcOffset = 0;
    unsigned long dstOffset = 0;
    unsigned long numElements = srcFormat.DataElements.size() < dstFormat.DataElements.size()
                                    ? srcFormat.DataElements.size() : dstFormat.DataElements.size();
    for (unsigned long i = 0; i < numElements; ++i) {
        EVertexStreamDataType srcType = srcFormat.DataElements[i].DataType;
        EVertexStreamDataType dstType = dstFormat.DataElements[i].DataType;
        const char* s = (const char*)src + srcOffset;
        char* d = (char*)dst + dstOffset;
        C4DVector v(0.0f, 0.0f, 0.0f, 0.0f);
        for (unsigned long j = 0; j < count; ++j) {
            ConvertFrom(s, srcType, v, params);
            ConvertTo(d, dstType, v, params);
            s += srcStride;
            d += dstStride;
        }
        srcOffset += CVertexStreamFormat::GetDataTypeSize(srcType);
        dstOffset += CVertexStreamFormat::GetDataTypeSize(dstType);
    }
}
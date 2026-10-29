// CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::TransferBinaryIn(CDataInputStream*, CReplaceableMeshes*) -- read the mesh-graphic count, resize the vector, then read the raw CEngineGraphic block
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EA2C9, 207 bytes, `ret 8` (__fastcall on the traits; called from the
//   replaceable-mesh transfer helper 0x004EA21E with the 12-byte traits in ecx; the write twin is 0x004EA26B).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path, x2),
//   vector<CEngineGraphic>::resize(unsigned long) @ 0x004EA398 (the default-value overload).
// Layout: engine/CDataInputStream.h; PDB CReplaceableMeshes = { CArray<CEngineGraphic> Vector } (retail STLport vector,
//   12 bytes); CEngineGraphic is packed to 0x0E bytes in retail (PDB sizeof 14). Local model with the PDB names.
// Shape: the WoundMorphs read idiom (landed 0x00757990): count read into the dead stream-argument slot, resize, data =
//   `size() > 0 ? first : (void*)&vector`, `if (bytes > 0)` Read(data, bytes); the inline Read tests
//   `size <= remaining` (`cmp edx,[ebx+14h] ; jg`) and copies with the intrinsic memcpy (rep movsd / rep movsb).
//   Size-optimised TU: #pragma optimize("s", on) (ebp frame).
#pragma optimize("s", on)
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStreamReader   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CDataInputStreamReader : CDataInputStream {
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

#pragma pack(push, 1)              // PDB sizeof(CEngineGraphic) == 0x0E: the class is packed in retail
class CEngineGraphic { public:
    unsigned long BankIndex;        // +0x00
    float         AnimStep;         // +0x04
    float         RenderSizeX;      // +0x08
    bool          AdditiveAlpha;    // +0x0C
    unsigned char Type;             // +0x0D
};
#pragma pack(pop)

template<class T> class CArray { public:   // STLport vector, 12 bytes in retail
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline unsigned long size() const { return _M_finish - _M_start; }
    __forceinline T* data() { return size() > 0 ? _M_start : (T*)this; }
    void resize(unsigned long count);                   // 0x004EA398
};

namespace NGraphicAppearance {
class CReplaceableMeshes { public:
    CArray<CEngineGraphic> Vector;  // +0x00
};
}

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<NGraphicAppearance::CReplaceableMeshes> {
    void TransferBinaryIn(CDataInputStream* stream, NGraphicAppearance::CReplaceableMeshes* value);
};

void CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::TransferBinaryIn(CDataInputStream* stream, NGraphicAppearance::CReplaceableMeshes* value)
{
    CDataInputStreamReader* in = (CDataInputStreamReader*)stream;
    unsigned long count;
    in->Read(&count, sizeof(count));
    value->Vector.resize(count);
    CEngineGraphic* data = value->Vector.data();
    long bytes = value->Vector.size() * sizeof(CEngineGraphic);
    if (bytes > 0)
        in->Read(data, bytes);
}
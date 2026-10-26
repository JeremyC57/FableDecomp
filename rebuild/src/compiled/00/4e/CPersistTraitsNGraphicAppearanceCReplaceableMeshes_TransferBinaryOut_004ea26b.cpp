// CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::TransferBinaryOut(CDataOutputStream*, const CReplaceableMeshes*) -- write the mesh-graphic count, then the raw CEngineGraphic block (or the vector object itself when empty)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EA26B, 67 bytes, `ret 8` (__fastcall on the traits; called from the
//   replaceable-mesh transfer helper 0x004EA21E, which builds the 12-byte traits at [ebp-0Ch] and passes it in ecx).
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0, CDataOutputStream::Write(const void*, unsigned long) = virtual
//   slot 4 (+0x10).
// Layout: PDB NGraphicAppearance::CReplaceableMeshes = { CArray<CEngineGraphic> Vector } (retail STLport vector,
//   12 bytes); CEngineGraphic is 0x0E bytes (BankIndex, AnimStep, RenderSizeX, AdditiveAlpha, Type), so the counts are
//   `(last - first) / 14` (cdq ; idiv). No engine header; local model with the PDB names.
// Shape: same data/size idiom as the landed CSimVoice::TransferBinaryOut 0x006808C0: data = `Size() > 0 ? first :
//   (void*)&vector` (unsigned `ja`), byte count Size() * sizeof; with both accessors __forceinline (the size TU
//   otherwise keeps data() out of line) the second Size() is computed once and shared.
//   Size-optimised TU: #pragma optimize("s", on) (`push 0Eh ; pop ecx`).
#pragma optimize("s", on)

class CDataOutputStream { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, unsigned long size);   // slot 4 (+0x10)
public:
    void WriteSLONG(long);   // 0x00993EB0
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
    __forceinline const T* data() const { return size() > 0 ? _M_start : (const T*)this; }
};

namespace NGraphicAppearance {
class CReplaceableMeshes { public:
    CArray<CEngineGraphic> Vector;  // +0x00
};
}

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<NGraphicAppearance::CReplaceableMeshes> {
    void TransferBinaryOut(CDataOutputStream* stream, const NGraphicAppearance::CReplaceableMeshes* value);
};

void CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::TransferBinaryOut(CDataOutputStream* stream, const NGraphicAppearance::CReplaceableMeshes* value)
{
    stream->WriteSLONG(value->Vector.size());
    stream->Write(value->Vector.data(), value->Vector.size() * sizeof(CEngineGraphic));
}
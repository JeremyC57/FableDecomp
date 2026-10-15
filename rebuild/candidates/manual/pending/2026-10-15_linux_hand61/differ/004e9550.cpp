// CPersistContext::TransferHelper(CObjectAugmentationParticleSet* value) -- persist one particle set: CRC-check the empty key, then run CPersistTraits<CObjectAugmentationParticleSet> in the context's direction (mode 2 load, mode 3 save)
// Grade: DIFFER(ours v retail) -- 80v77: retail forms the traits `this` (`lea ecx,[ebp-2Ch]`) once above the mode switch's first `je` and lets the default arm fall straight into the destructor call; the lane re-forms it in every arm (+3 bytes); tried: switch (this file), if/else chain (83), a __forceinline traits TransferBinary(context, value) holding the switch (80)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E9550, length 77 bytes (`ret 4`). Callees: CPersistContext::CheckCRC
// @ 0x00404500 with "" @ 0x0122D70E, the traits constructor @ 0x004E9627 (manifest "CDateAndTime"), TransferBinaryIn
// @ 0x004E9642 / TransferBinaryOut @ 0x004E959D, the traits destructor @ 0x004E25F1. The manifest calls it
// CObjectAugmentationParticleSet::TransferHelper, but `this` is the persist context (mode at +0x18, input stream +0x24,
// output stream +0x28, as in the landed CPersistContext::TransferBinary_00467062); the 0x2C-byte frame is the traits
// object (it holds a default CObjectAugmentationParticleSet, which is 0x2C bytes in retail). Frame-pointer size TU:
// #pragma optimize("y", off) + ("s", on).
// Leaf: TransferHelper
#pragma optimize("y", off)
#pragma optimize("s", on)
class CDataOutputStream;
class CMemoryDataInputStream;
class CObjectAugmentationParticleSet;

class CPersistTraits_ParticleSet {                 // CPersistTraits<CObjectAugmentationParticleSet>, 0x2C bytes
public:
    unsigned char _body[0x2C];
    CPersistTraits_ParticleSet();                  // 0x004E9627
    ~CPersistTraits_ParticleSet();                 // 0x004E25F1
    void TransferBinaryOut(CDataOutputStream* stream, CObjectAugmentationParticleSet* value);     // 0x004E959D
    void TransferBinaryIn(CMemoryDataInputStream* stream, CObjectAugmentationParticleSet* value); // 0x004E9642
};

class CPersistContext {
public:
    unsigned char           _pad00[0x18];
    long                    Mode;                  // +0x18 (2 = load)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* InputStream;           // +0x24
    CDataOutputStream*      OutputStream;          // +0x28
    void CheckCRC(const char* name);               // 0x00404500
    void TransferHelper(CObjectAugmentationParticleSet* value);
};

void CPersistContext::TransferHelper(CObjectAugmentationParticleSet* value)
{
    CheckCRC("");
    CPersistTraits_ParticleSet traits;
    switch (Mode) {
    case 2:
        traits.TransferBinaryIn(InputStream, value);
        break;
    case 3:
        traits.TransferBinaryOut(OutputStream, value);
        break;
    }
}

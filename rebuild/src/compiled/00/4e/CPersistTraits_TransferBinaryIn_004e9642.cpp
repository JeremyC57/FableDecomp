// CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryIn(CMemoryDataInputStream& is, CObjectAugmentationParticleSet& value) -- binary read of one augmentation particle set: PeriodSecs, the four particle-effect handles, then the StartPoint and EndPoint def-string arrays
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCObjectAugmentationParticleSet@@@@QAEXAAVCMemoryDataInputStream@@AAVCObjectAugmentationParticleSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E9642, 460 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCObjectAugmentationParticleSet@@@@QAEXAAVCMemoryDataInputStream@@AAVCObjectAugmentationParticleSet@@@Z
//   (a member of the explicit specialisation CPersistTraits<CObjectAugmentationParticleSet>, overlay opt-in
//   FABLE_RETAIL_CPERSISTTRAITS_OBJECTAUGMENTATIONPARTICLESET; both arguments by reference).
// CObjectAugmentationParticleSet (engine/CObjectAugmentationParticleSet.h) = { float PeriodSecs +0, long ParticleEffect +4,
//   long ParticleEffectToBlendTo +8, long AttackParticleEffect +0xC, long AttackParticleEffectToBlendTo +0x10, CArray<CDefString>
//   StartPoint +0x14, CArray<CDefString> EndPoint +0x20 } (retail STLport vectors are 12 bytes).
// Callees: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out,
//   0x00465852 GFSerialiseVectorBinaryIn<CDefString>(CMemoryDataInputStream&, vector<CDefString>&) (landed; __fastcall).
// Shape: CMemoryDataInputStream::Read (the shared CDataInputStream inline Read) expanded five times through the inline
//   CPersistTraits<float> / CPersistTraits<long>::TransferBinaryIn (overlay opt-ins FABLE_RETAIL_CPERSISTTRAITS_SCALAR_BINARYIN_INLINE, FABLE_RETAIL_CPERSISTTRAITS_OBJECTAUGMENTATIONPARTICLESET).
//   EBP frame + size TU (`#pragma optimize("y", off)` + `("s", on)`).
// NF62 (2026-11-17, round 7): ported onto the shared CPersistTraits / CDataInputStream / CMemoryDataInputStream /
//   CObjectAugmentationParticleSet / CDefString / CArray headers (the local CDataInputStream / CDefString / CObjectAugmentationParticleSet
//   models, the CMemoryDataInputStreamReader stand-in and the local CPersistTraits specialisations are gone); the self leaf is now the
//   PDB by-reference spelling (was ...QAEXPAVCMemoryDataInputStream@@PAVCObjectAugmentationParticleSet@@@Z, pointers).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_SCALAR_BINARYIN_INLINE
#define FABLE_RETAIL_CPERSISTTRAITS_OBJECTAUGMENTATIONPARTICLESET
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CObjectAugmentationParticleSet.h"
#include "stlport/fable_containers.h"

template<class T> void FABLE_FASTCALL GFSerialiseVectorBinaryIn(CMemoryDataInputStream&,std::vector<T,std::allocator<T> >&);

void CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryIn(CMemoryDataInputStream& is, CObjectAugmentationParticleSet& value)
{
    CPersistTraits<float> floatTraits;
    floatTraits.TransferBinaryIn(is, value.PeriodSecs);
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(is, value.ParticleEffect);
    longTraits.TransferBinaryIn(is, value.ParticleEffectToBlendTo);
    longTraits.TransferBinaryIn(is, value.AttackParticleEffect);
    longTraits.TransferBinaryIn(is, value.AttackParticleEffectToBlendTo);
    GFSerialiseVectorBinaryIn(is, value.StartPoint);
    GFSerialiseVectorBinaryIn(is, value.EndPoint);
}

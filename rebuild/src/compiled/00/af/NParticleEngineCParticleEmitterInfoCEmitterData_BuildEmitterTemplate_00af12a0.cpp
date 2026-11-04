// NParticleEngine::CParticleEmitterInfo::CEmitterData::BuildEmitterTemplate() -- read the loaded bank data into a fresh particle-emitter template through a memory stream, then release the file data
// Leaf: ?BuildEmitterTemplate@CEmitterData@CParticleEmitterInfo@NParticleEngine@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF12A0, 144 bytes; Ego_r.pdb ?BuildEmitterTemplate@CEmitterData@CParticleEmitterInfo@NParticleEngine@@QAEXXZ
//   (the class is nested in NParticleEngine::CParticleEmitterInfo, a CResource; layout Ego_r CEmitterData: FileData +0x1C,
//   EmitterTemplate +0x24, Owner +0x2C).
// Callees: CBankFileAsyncData::PeekData() const 0x009D5580 (Sizeof() inline: the +0x10 Length word, evaluated first as the
//   last ctor argument), CMemoryDataInputStream(const void*, unsigned long) 0x00994930 / ~CMemoryDataInputStream 0x00993B10,
//   operator new 0x00BFEA1A (0x16C), CParticleEmitter() 0x00AF0380, CCountedPointer<CParticleEmitter>::Reset 0x00AF1860 (the
//   shared header's inline Reset, emitted out of line by VC7.1 here as in retail), CParticleEmitter::ReadBinary 0x00AEFAA0,
//   operator delete 0x00BFE9BC (the file data's CCPPointerInfo through the inline ReleaseData: Info cleared before Data).
// Shape: the old draft's register tie (template object loaded after the stream lea) came from local CCountedPointer / stream
//   models; on the shared CCountedPointer, CMemoryDataInputStream and CParticleEmitter retail headers it is retail's order.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include "engine/CCountedPointer.h"
#include "engine/CResource.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CParticleEmitter.h"

class CBankFileAsyncData {                               // Ego_r.pdb (0x48); no shared header
public:
    const void* PeekData() const;                        // 0x009D5580 ?PeekData@CBankFileAsyncData@@QBEPBXXZ
    unsigned long Sizeof() const { return Length; }      // ?Sizeof@CBankFileAsyncData@@QBEKXZ (inline in retail)
private:
    unsigned char _pad_00[0x10];
    unsigned long Length;                                // +0x10
};

namespace NParticleEngine {
class CParticleEmitterInfo {
public:
    class CEmitterData : public CResource {
    public:
        void BuildEmitterTemplate();
        CCountedPointer<CBankFileAsyncData> FileData;          // +0x1c
        CCountedPointer<CParticleEmitter>   EmitterTemplate;   // +0x24
        CParticleEmitterInfo*               Owner;             // +0x2c
    };
};
}

void NParticleEngine::CParticleEmitterInfo::CEmitterData::BuildEmitterTemplate()
{
    CMemoryDataInputStream stream(FileData->PeekData(), FileData->Sizeof());
    EmitterTemplate.Reset(new CParticleEmitter);
    EmitterTemplate->ReadBinary(stream);
    FileData.ReleaseData();
}
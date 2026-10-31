// CEmitterData::BuildEmitterTemplate() -- read the loaded bank data into a fresh particle-emitter template through a memory stream, then drop the file data
// Grade: DIFFER(144v144) -- moved onto engine/CCountedPointer.h (Reset stays out of line at 0x00AF1860 as retail, Clear
//   = ReleaseData); residue unchanged: retail loads the template object (mov ecx,[edi]) before forming the stream
//   argument, ours after the push; the count-block delete pushes ecx (retail) vs edx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF12A0, length 144 bytes.
// Callees: CBankFileAsyncData::GetData() @ 0x009D5580 (thiscall, no arguments), CMemoryDataInputStream::CMemoryDataInputStream(const void* data, long size)
// @ 0x00994930 / ~CMemoryDataInputStream @ 0x00993B10 (manifest "~CChunkedFileChunk" is propagated), operator new @ 0x00BFEA1A (0x16c bytes),
// NParticleEngine::CParticleEmitter::CParticleEmitter() @ 0x00AF0380, CCountedPointer<CParticleEmitter>::Reset(CParticleEmitter*) @ 0x00AF1860
// (out of line), CParticleEmitter::ReadBinary(CDataInputStream&) @ 0x00AEFAA0, operator delete @ 0x00BFE9BC (the file data's count block).
// Layout = Ego_r.pdb CEmitterData (0x30 bytes): FileData (CCountedPointer<CBankFileAsyncData>) +0x1c, EmitterTemplate
// (CCountedPointer<CParticleEmitter>) +0x24, Owner +0x2c; the async data's size word at +0x10 is descriptive.
// Shape: the 0x1c-byte stream in the frame; its two constructor arguments evaluated right to left (the size read first, then GetData);
// `new` with the null-checked constructor (`xor eax,eax` arm) handed to the out-of-line Reset; ReadBinary on the template's object word;
// then the file data dropped by an inline release (`if (block) { --count; if (count == 0) { deleter(object); delete block; } }`) that
// clears the block word before the object word; the stream destructor last.
#include "engine/CCountedPointer.h"

struct CDataInputStream;

struct CMemoryDataInputStream {
    unsigned char _body[0x1c];
    CMemoryDataInputStream(const void* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();               // 0x00993B10
};

struct CBankFileAsyncData {
    unsigned char _pad[0x10];
    long          m_dataSize;                // +0x10
    const void* GetData();                   // 0x009D5580
};

namespace NParticleEngine {
struct CParticleEmitter {
    unsigned char _body[0x16c];
    CParticleEmitter();                      // 0x00AF0380
    void ReadBinary(CMemoryDataInputStream& stream);   // 0x00AEFAA0
};
}

struct CParticleEmitterInfo;

struct CEmitterData {
    unsigned char _pad00[0x1c];
    CCountedPointer<CBankFileAsyncData>                FileData;          // +0x1c
    CCountedPointer<NParticleEngine::CParticleEmitter> EmitterTemplate;   // +0x24
    CParticleEmitterInfo*                              Owner;             // +0x2c
    void BuildEmitterTemplate();
};

void CEmitterData::BuildEmitterTemplate()
{
    CMemoryDataInputStream stream(FileData->GetData(), FileData->m_dataSize);
    EmitterTemplate.Reset(new NParticleEngine::CParticleEmitter());
    EmitterTemplate->ReadBinary(stream);
    FileData.ReleaseData();
}

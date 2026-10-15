// CEmitterData::BuildEmitterTemplate() -- read the loaded bank data into a fresh particle-emitter template through a memory stream, then drop the file data
// Grade: DIFFER(144v144) -- register allocation only: retail loads the template object into ecx BEFORE forming the stream argument (`mov ecx,[edi] ; lea eax,[esp+8] ; push eax`), the lane pushes `lea ecx,[esp+8]` first and loads ecx after; and the count block reloaded for operator delete is ecx in retail, edx in the lane. Tried: `EmitterTemplate->ReadBinary(stream)` and the object read into a named local first (identical output).
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
struct SCountBlock {                         // the shared count block of CCountedPointer: count, per-type deleter, object
    long  m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4 (called with the object in ecx)
    void* m_pObject;                         // +8
};

template <class T>
struct CCountedPointer {                     // {object, count block}
    T*           Object;
    SCountBlock* Count;
    void Reset(T* object);                   // 0x00AF1860 for T = CParticleEmitter
    void Clear()
    {
        if (Count) {
            --Count->m_references;
            if (Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
        Count = 0;
        Object = 0;
    }
    T* operator->() const { return Object; }
};

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
    NParticleEngine::CParticleEmitter* emitter = EmitterTemplate.Object;
    emitter->ReadBinary(stream);
    FileData.Clear();
}

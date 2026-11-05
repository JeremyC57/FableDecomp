// NParticleEngine::CEmitterData::BeginLoading() -- if the emitter data is not loaded yet, make room in the bank's resource list for its entry, start an asynchronous read of the whole entry, keep the counted file data and record the resource size
// Leaf: BeginLoading
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members Object->Data are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF1360, length 223 bytes (+ int3 padding), `ret`.
// Callees: CBankFile::GetEntryDataSize(ulong) @ 0x009CC240, CResourceList::MakeFreeSpace(ulong) @ 0x009FC4F0,
// CBankFileAsync::CreateEntry(ulong) @ 0x009D58D0 (CCountedPointer<CBankFileAsyncEntry> by hidden pointer; catalog
// "CEngineWorldMap::CreateNewBankMap"), CBankFileAsyncEntry::BeginReadDataAutoAlloc(ulong, ulong, EThreadedFilePriority)
// @ 0x009D6100 (CCountedPointer<CBankFileAsyncData> by hidden pointer), the counted pointer operator= @ 0x009A4440,
// CBankFileAsyncData::Sizeof @ 0x009D5620 (catalog "CVertexBufferWin32::DoSizeof"), CResource::SetResourceSize(ulong)
// @ 0x009FBF20, operator delete @ 0x00BFE9BC (count blocks); the read priority global @ 0x0139954C.
// Layout: CEmitterData = CResource (engine/CResource.h, 0x1c bytes) + FileData (CCountedPointer: object +0x1c, block
// +0x20) + EmitterTemplate +0x24 + Owner (CParticleEmitterInfo*) +0x2c; the owner keeps its bank (CBankFileAsync) at +4
// and the entry index at +8; the bank's resource list is at +0x168.
// Shape: the entry and the read result are counted-pointer locals (the read result an unnamed temporary assigned through
// the out-of-line operator= and released inline right after); the resource size is `(data ? data->Sizeof() + 8 : 8) +
// 12` (a ternary local then one add).
#define CResource CResourceLayout
#include "engine/CResource.h"
#undef CResource

class CResource : public CResourceLayout
{
public:
    void SetResourceSize(unsigned long);   // 0x009FBF20
};

struct CResourceMethods : CResource {
};
class CResourceList { public:
public:
    bool MakeFreeSpace(unsigned long);   // 0x009FC4F0
};

void __fastcall DestroyCountedObject(void* object);
#include "engine/CCountedPointer.h"

enum EThreadedFilePriority { };
extern EThreadedFilePriority g_ParticleReadPriority;   // 0x0139954C
class CBankFileAsyncData { public:
    unsigned long Sizeof() const;                  // 0x009D5620
};
class CBankFileAsyncEntry { public:
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};
class CBankFile { public:
    unsigned long GetEntryDataSize(unsigned long a0) const;   // 0x009CC240 (Ego_r.pdb ?GetEntryDataSize@CBankFile@@QBEKK@Z)
};
class CBankFileAsync { public:
    unsigned char _pad[0x168];
    CResourceList ResourceList;             // +0x168
    CCountedPointer<CBankFileAsyncEntry> CreateEntry(unsigned long index);   // 0x009D58D0
};
struct CParticleEmitterInfo {
    void* __vftable;                               // +0x00
    CBankFileAsync* Bank;                          // +0x04
    unsigned long EntryIndex;                      // +0x08
};

namespace NParticleEngine {
class CParticleEmitter;
struct CEmitterData : CResourceMethods {
    CCountedPointer<CBankFileAsyncData> FileData;  // +0x1c
    CCountedPointer<CParticleEmitter> EmitterTemplate;   // +0x24
    CParticleEmitterInfo* Owner;                   // +0x2c
    void BeginLoading();
};

void CEmitterData::BeginLoading()
{
    if (FileData.Data)
        return;
    unsigned long size = ((CBankFile*)(Owner->Bank))->GetEntryDataSize(Owner->EntryIndex);
    Owner->Bank->ResourceList.MakeFreeSpace(size);
    CCountedPointer<CBankFileAsyncEntry> entry = Owner->Bank->CreateEntry(Owner->EntryIndex);
    FileData = entry->BeginReadDataAutoAlloc(0, size, g_ParticleReadPriority);
    unsigned long dataSize = FileData.Data ? FileData->Sizeof() + 8 : 8;
    SetResourceSize(dataSize + 12);
}
}
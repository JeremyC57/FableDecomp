// NParticleEngine::CParticleEmitterInfo::CEmitterData::BeginLoading() -- if the emitter data is not loaded yet, make room in the bank's
// resource list for its entry, start an asynchronous read of the whole entry, keep the counted file data and record the resource size
// Leaf: ?BeginLoading@CEmitterData@CParticleEmitterInfo@NParticleEngine@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF1360, length 223 bytes (+ int3 padding), `ret`.
// Callees: CBankFile::GetEntryDataSize(ulong) const @ 0x009CC240, CResourceList::MakeFreeSpace(ulong) @ 0x009FC4F0,
//   CBankFileAsync::GetAsyncEntry(ulong) @ 0x009D58D0, CBankFileAsyncEntry::BeginReadDataAutoAlloc(ulong, ulong, EThreadedFilePriority)
//   @ 0x009D6100, the counted pointer operator= @ 0x009A4440, CBankFileAsyncData::Sizeof @ 0x009D5620, CResource::SetResourceSize(ulong)
//   @ 0x009FBF20, operator delete @ 0x00BFE9BC (count blocks); the read priority global @ 0x0139954C.
// Shape: the entry and the read result are counted-pointer locals (the read result an unnamed temporary assigned through the out-of-line
//   operator= and released inline right after); the resource size is `(data ? data->Sizeof() + 8 : 8) + 12`.
// NF72 (round 7, q10881): ported off the renaming-macro CResource model (`#define CResource CResourceLayout`) and the local CBankFile /
//   CBankFileAsync / CResourceList / CBankFileAsyncData models onto engine/CResource.h (SETRESOURCESIZE), CResourceList.h (MAKEFREESPACE),
//   CResourceBank.h, CBankFile.h (ENTRYDATASIZE), CBankFileAsync.h (GETASYNCENTRY: Ego_r.pdb 0x00B29950 / FableWin.pdb 0x02FF69A0
//   ?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z; was the made-up CreateEntry) and CBankFileAsyncData.h
//   (SIZEOF). The class is Ego_r.pdb's nested NParticleEngine::CParticleEmitterInfo::CEmitterData (?BeginLoading@CEmitterData@
//   CParticleEmitterInfo@NParticleEngine@@QAEXXZ, Ego_r 0x00AB0C40; its landed caller 0x00AF1480 binds that spelling). CParticleEmitterInfo
//   (Ego_r: Data +0, Bank +4 CParticleDataBank*, Index +8), its CEmitterData, CParticleDataBank (RTTI: CBankFileAsync, then CResourceBank at
//   +0x164, so ResourceList +0x168) and CBankFileAsyncEntry have no shared header and stay local models.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST_MAKEFREESPACE
#define FABLE_RETAIL_CRESOURCE_SETRESOURCESIZE
#define FABLE_RETAIL_CBANKFILEASYNCDATA_SIZEOF
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"
#include "engine/CBankFileAsyncData.h"
#include "engine/CResource.h"
#include "engine/CResourceBank.h"
#include "engine/CIVCountedPointer.h"
#include "engine/EThreadedFilePriority.h"

class CBankFileAsyncEntry {
public:
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};

extern EThreadedFilePriority g_ParticleReadPriority;        // 0x0139954C (landed descriptive name; no PDB name found)

namespace NParticleEngine {
class CParticleEmitter;
class CParticleDataBank : public CBankFileAsync, public CResourceBank {
};
class CParticleEmitterInfo {
public:
    class CEmitterData : public CResource {
    public:
        void BeginLoading();
        CCountedPointer<CBankFileAsyncData> FileData;        // +0x1c
        CCountedPointer<CParticleEmitter>   EmitterTemplate; // +0x24
        CParticleEmitterInfo*               Owner;           // +0x2c
    };
    CIVCountedPointer<CEmitterData> Data;                    // +0x00
    CParticleDataBank*              Bank;                    // +0x04
    unsigned long                   Index;                   // +0x08
};
}

void NParticleEngine::CParticleEmitterInfo::CEmitterData::BeginLoading()
{
    if (FileData.Data)
        return;
    unsigned long size = Owner->Bank->GetEntryDataSize(Owner->Index);
    Owner->Bank->ResourceList.MakeFreeSpace(size);
    CCountedPointer<CBankFileAsyncEntry> entry = Owner->Bank->GetAsyncEntry(Owner->Index);
    FileData = entry->BeginReadDataAutoAlloc(0, size, g_ParticleReadPriority);
    unsigned long dataSize = FileData.Data ? FileData->Sizeof() + 8 : 8;
    SetResourceSize(dataSize + 12);
}

// CPhysicsMeshInfo::BeginLoading() const -- if the physics mesh data is not loaded yet, create it, open the bank entry, queue the data on
// the bank's physics-mesh resource bank, start an asynchronous read of the whole entry and record the resource size
// Leaf: ?BeginLoading@CPhysicsMeshInfo@@QBEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?BeginLoading@CPhysicsMeshInfo@@QBEXXZ (const; the landed caller 0x00724630 binds this spelling,
//   0x00A262B0 still binds the QAE stand-in: NAME_FIXES.tsv).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A25BD0, 321 bytes, plain `ret`.
// Callees: operator new 0x00BFEA1A (0x2C bytes), CResource::CResource 0x009FBEC0 (CMeshData vtable 0x0129CD78), CBankFileAsync::CreateEntry
//   0x009D58D0, CBankFile::GetEntryDataSize 0x009CC240 (on the entry's BankFile), CResourceBank::AddToLoadingQueue 0x009FC3D0 (nf44:83),
//   CBankFileAsyncEntry::BeginReadDataAutoAlloc 0x009D6100, CCountedPointer<CBankFileAsyncData>::operator= 0x009A4440,
//   CPhysicsMesh::Sizeof 0x00A6C900, CBankFileAsyncData::Sizeof 0x009D5620, CResource::SetResourceSize 0x009FBF20, operator delete 0x00BFE9BC.
// Port (NF68): the local CResourceBase / CResourceBank / CMeshDataBank / CCountedPointer / CIVCountedPointer / CBankFile models are gone:
//   engine/CResource.h + CIVCountedPointeeBase.h (FABLE_RETAIL_DEF_CLASS_BASE), engine/CIVCountedPointer.h, engine/CCountedPointer.h,
//   engine/CBankFile.h + CBankFileAsync.h, engine/CMeshDataBank.h (retail CResourceBank members, CBankFileAsync base),
//   engine/CMeshDataBankEntry.h (Bank +4 / Index +8), engine/CBankFileAsyncData.h, engine/CPhysicsMesh.h (overlay opt-in
//   FABLE_RETAIL_CPHYSICSMESH_SIZEOF). Local models remain only for the classes with no shared header: CPhysicsMeshInfo (Ego_r.pdb:
//   CMeshDataBankEntry base, MeshData +0xC) with its CMeshData (FileData +0x1C, Mesh +0x24, Owner +0x28) and CBankFileAsyncEntry (Ego_r.pdb
//   BankFile +4, Index +8, PaddedSize +0xC). MeshData is `mutable` (the PDB function is const and stores the new data; the bytes cannot
//   show whether the source wrote mutable or a const_cast).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANK_CTOR
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE_SETRESOURCESIZE
#define FABLE_RETAIL_CBANKFILEASYNCDATA_SIZEOF
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CPHYSICSMESH_SIZEOF
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"
#include "engine/CBankFileAsyncData.h"
#include "engine/CResource.h"
#include "engine/CResourceBank.h"
#include "engine/CIVCountedPointer.h"
#include "engine/EThreadedFilePriority.h"
#include "engine/CMeshDataBank.h"
#include "engine/CMeshDataBankEntry.h"
#include "engine/CPhysicsMesh.h"

class CBankFileAsyncEntry {
public:
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
    virtual ~CBankFileAsyncEntry();
    CBankFileAsync* BankFile;                                // +0x04
    unsigned long   Index;                                   // +0x08
    unsigned long   PaddedSize;                              // +0x0c
};

extern EThreadedFilePriority g_PhysicsMeshReadPriority;     // 0x0139723C (landed descriptive name; no PDB name found)

class CPhysicsMeshInfo : public CMeshDataBankEntry {
public:
    class CMeshData : public CResource {
    public:
        CMeshData(const CPhysicsMeshInfo* owner) : Owner(owner) {}
        virtual ~CMeshData();
        virtual bool UnloadResource();
        virtual bool IsResourceLoaded() const;
        virtual void OnResourceLoaded();
        unsigned long Sizeof() const
        {
            unsigned long meshSize = !Mesh.IsNull() ? Mesh->Sizeof() + sizeof(Mesh) : sizeof(Mesh);
            unsigned long fileSize = FileData.Data ? FileData->Sizeof() + sizeof(FileData) : sizeof(FileData);
            return sizeof(CMeshData) + meshSize + fileSize;
        }
        CCountedPointer<CBankFileAsyncData>   FileData;      // +0x1c
        CIVCountedPointer<const CPhysicsMesh> Mesh;          // +0x24
        const CPhysicsMeshInfo*               Owner;         // +0x28
    };
    void BeginLoading() const;
    mutable CIVCountedPointer<CMeshData> MeshData;           // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshInfo::CMeshData) == 0x2c);
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshInfo) == 0x10);

void CPhysicsMeshInfo::BeginLoading() const
{
    if (!MeshData.IsNull())
        return;
    MeshData.Reset(new CMeshData(this));
    CCountedPointer<CBankFileAsyncEntry> entry = Bank->GetAsyncEntry(Index);
    unsigned long size = entry->BankFile->GetEntryDataSize(entry->Index);
    Bank->PhysicsMeshResourceBank.AddToLoadingQueue(*MeshData);
    MeshData->FileData = entry->BeginReadDataAutoAlloc(0, size, g_PhysicsMeshReadPriority);
    MeshData->SetResourceSize(MeshData->Sizeof());
}

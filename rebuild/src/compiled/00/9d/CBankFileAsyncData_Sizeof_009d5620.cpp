// CBankFileAsyncData::Sizeof() const -- the bytes this async bank-file load holds: its internally allocated buffer plus
// the object itself (InternalAllocatedBufferSize + sizeof(CBankFileAsyncData) = +0x48).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5620, length 7 bytes.
// Callers (30): the bank entries' Sizeof / BeginLoading (CTextEntry::Sizeof 0x009C7D90, C3DMeshLODInfo::Sizeof 0x00A244E0,
//   C3DMeshInfo::BeginLoading 0x00A256F0, CPhysicsMeshInfo::BeginLoading 0x00A25BD0, ...).
// Name: Ego_r.pdb 0x00B29690 / FableWin.pdb 0x02FFA870 ?Sizeof@CBankFileAsyncData@@QBEKXZ; the PDB layout
//   (struct_layouts_egor) puts InternalAllocatedBufferSize at +0x24 and sizeof at 72 = 0x48, the two constants here.
//   Was catalogued `CVertexBufferWin32::DoSizeof` (module DoSizeof), whose PDB form is the private virtual
//   ?DoSizeof@CVertexBufferWin32@@EBEKXZ.
// Model: local class (no engine/CBankFileAsyncData.h); members and offsets from Ego_r.pdb.
// Leaf: ?Sizeof@CBankFileAsyncData@@QBEKXZ
class CMovableResourceMemoryPool;
class CBankFileAsync;

class CBankFileAsyncData {
public:
    unsigned char               _pad00[0x8];                   // +0x00 (vptr / base)
    void*                       Data;                          // +0x08
    void*                       Buffer;                        // +0x0C
    unsigned long               Length;                        // +0x10
    unsigned long               SkipBytes;                     // +0x14
    unsigned long               ThreadedFileUID;               // +0x18
    CMovableResourceMemoryPool* MemoryPool;                    // +0x1C
    long                        MemoryHandle;                  // +0x20
    unsigned long               InternalAllocatedBufferSize;   // +0x24
    CBankFileAsync*             BankFile;                      // +0x28
    unsigned char               LoadDesc[0x18];                // +0x2C CThreadedFileObjectDesc
    bool                        Finished;                      // +0x44
    bool                        BufferLocked;                  // +0x45
    bool                        AllowRestart;                  // +0x46
    bool                        Aborted;                       // +0x47
    unsigned long Sizeof() const;
};

unsigned long CBankFileAsyncData::Sizeof() const
{
    return InternalAllocatedBufferSize + sizeof(CBankFileAsyncData);
}

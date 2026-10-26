// C3DMeshFile::GetRoot() const -- the mesh file's root chunk as a const counted pointer: the chunked file's super-chunk pointer converted to const, static-cast into a fresh pointer, returned by value
// Leaf: ?GetRoot@C3DMeshFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A32620, 101 bytes (+ int3 padding), thiscall `ret 4` (hidden result).
// Identity: FableWin.pdb `?GetRoot@C3DMeshFile@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileRootChunk@@@@XZ` with the
//   instantiations it needs (`CCountedPointer<const CChunkedFileSuperChunk>` converting ctor from the non-const pointer,
//   `CCountedPointer<const C3DMeshFileRootChunk>::SetStaticCast<const CChunkedFileSuperChunk>`, ShareData, ReleaseData).
//   Callers: CPhysicsMesh::GetMeshDataFromFileData / GetHelperDataFromFileData / GetContainmentDataFromFileData
//   (`const C3DMeshFile&` in ecx) and 0x00A2F786. The catalog's "SharedPtr_Assign" is descriptive. C3DAnimFile::GetRootChunk
//   @ 0x00A537F0 is the byte-identical C3DAnimFile twin.
// Layout: CChunkedFile (Ego_r.pdb, 20 bytes): vptr +0, CCountedPointer<CChunkedFileSuperChunk> RootChunk +4 (Data +4,
//   Info +8), Version +0xc, CCharString FileComment +0x10; C3DMeshFile derives from it. The count block is
//   {RefCount +0, DeleteFunc +4 (`__fastcall`, Data in ecx), Data +8}, freed with operator delete @ 0x00BFE9BC.
// Shape: `CCountedPointer<const Root> root; root.SetStaticCast(CCountedPointer<const Super>(RootChunk)); return root;` -- the
//   converting temporary (add-ref), ShareData's `Info != info` test against the empty local (so a null block leaves the
//   result {0,0} even when Data is set: `xor ebx,ebx ; xor edi,edi`), its add-ref, the temporary's release, then the
//   copy into the hidden result (no NRVO in VC7.1) and the local's release -- every block test folds onto the one Info.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCPPointerInfo { public:
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)
    {
        ShareData(static_cast<T*>(other.Data), other.Info);
    }
};

class CChunkedFileSuperChunk { };
class C3DMeshFileRootChunk : public CChunkedFileSuperChunk { };
class CCharString { public: void* PStringData; };

class CChunkedFile {
public:
    virtual ~CChunkedFile();
    CCountedPointer<CChunkedFileSuperChunk> RootChunk;  // +0x04
    unsigned long                           Version;    // +0x0c
    CCharString                             FileComment; // +0x10
};

class C3DMeshFile : public CChunkedFile {
public:
    CCountedPointer<const C3DMeshFileRootChunk> GetRoot() const;
};

CCountedPointer<const C3DMeshFileRootChunk> C3DMeshFile::GetRoot() const
{
    CCountedPointer<const C3DMeshFileRootChunk> root;
    root.SetStaticCast(CCountedPointer<const CChunkedFileSuperChunk>(RootChunk));
    return root;
}
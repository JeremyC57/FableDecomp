// C3DMeshFile::GetRoot() -- the mesh file's root chunk as a counted pointer (non-const overload): the chunked file's root super-chunk copied out, static-cast into a fresh pointer, returned by value
// Leaf: ?GetRoot@C3DMeshFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD1E80, 101 bytes, thiscall `ret 4` (hidden result).
// Identity: Ego_r.pdb `?GetRoot@C3DMeshFile@@QAE?AV?$CCountedPointer@VC3DMeshFileRootChunk@@@@XZ` (the non-const overload;
//   its const twin `QBE` is landed at 0x00A32620 with the same 101-byte body). The PDB also carries the pieces this
//   needs: `CChunkedFile::GetChunkedFileRoot() QAE` (by-value copy of RootChunk = the add-ref temporary) and
//   `CCountedPointer<C3DMeshFileRootChunk>::SetStaticCast<CChunkedFileSuperChunk>`. Caller: the landed
//   QuadricOptimiserInternals::COptimisedMesh::CopyTo 0x00ACD5A0 (takes a non-const `C3DMeshFile&`; it declares this
//   address as `C3DMeshFile::GetMeshChunk()`, which should read `GetRoot()` returning the root chunk).
// Label correction: catalogued `CCountedPointer_MoveAssign`.
// Callees: the count block's DeleteFunc (`call [Info+4]`, Data in ecx), operator delete 0x00BFE9BC.
// Layout: CChunkedFile (Ego_r.pdb, 20 bytes): vptr +0, CCountedPointer<CChunkedFileSuperChunk> RootChunk +4 (Data +4,
//   Info +8), Version +0xc, CCharString FileComment +0x10; C3DMeshFile derives from it. Count block {RefCount +0,
//   DeleteFunc +4, Data +8}.
// Shape: `root; root.SetStaticCast(GetChunkedFileRoot()); return root;` -- the by-value temporary (add-ref), ShareData's
//   `Info != info` test against the empty local (`xor ebx,ebx ; xor edi,edi`), its add-ref, the temporary's release,
//   then the copy into the hidden result (no NRVO in VC7.1) and the local's release.
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
    CCountedPointer<CChunkedFileSuperChunk> GetChunkedFileRoot() { return RootChunk; }
    CCountedPointer<CChunkedFileSuperChunk> RootChunk;  // +0x04
    unsigned long                           Version;    // +0x0c
    CCharString                             FileComment; // +0x10
};

class C3DMeshFile : public CChunkedFile {
public:
    CCountedPointer<C3DMeshFileRootChunk> GetRoot();
};

CCountedPointer<C3DMeshFileRootChunk> C3DMeshFile::GetRoot()
{
    CCountedPointer<C3DMeshFileRootChunk> root;
    root.SetStaticCast(GetChunkedFileRoot());
    return root;
}
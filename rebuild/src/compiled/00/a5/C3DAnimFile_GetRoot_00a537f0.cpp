// C3DAnimFile::GetRoot() const -- the animation file's root chunk as a const counted pointer: the chunked file's super-chunk pointer converted to const, static-cast into a fresh pointer, returned by value
// Leaf: ?GetRoot@C3DAnimFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A537F0, length 101 bytes (+11 int3 pad), thiscall `ret 4` (hidden result).
// Identity: Ego_r.pdb `?GetRoot@C3DAnimFile@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileRootChunk@@@@XZ` (the const overload: the
//   converting `CCountedPointer<const CChunkedFileSuperChunk>` temporary is the extra add-ref/release pair the non-const
//   overload would not need). It is the byte-identical twin of the landed C3DMeshFile::GetRoot() const 0x00A32620, whose
//   model this reuses. Callers: C3DAnimationStats::InitialiseStats 0x00A4EBC6 (landed; it declares this address as
//   `C3DAnimFile::GetRootChunk() const`), 0x00A4FE9B, 0x00A4FEEB, 0x00A50038, 0x00A50148, 0x00A50164, 0x00A5023A.
// Label correction: catalogued `SharedPtr_Copy`; the landed caller's `GetRootChunk` should read `GetRoot`.
// Layout: CChunkedFile (Ego_r.pdb, 20 bytes): vptr +0, CCountedPointer<CChunkedFileSuperChunk> RootChunk +4 (Data +4,
//   Info +8), Version +0xc, CCharString FileComment +0x10; C3DAnimFile derives from it. The count block is
//   {RefCount +0, DeleteFunc +4 (`__fastcall`, Data in ecx), Data +8}, freed with operator delete @ 0x00BFE9BC.
// Shape: `CCountedPointer<const Root> root; root.SetStaticCast(CCountedPointer<const Super>(RootChunk)); return root;` -- the
//   converting temporary (add-ref), ShareData's `Info != info` test against the empty local (so a null block leaves the
//   result {0,0}: `xor ebx,ebx ; xor edi,edi`), its add-ref, the temporary's release, then the copy into the hidden result
//   (no NRVO in VC7.1) and the local's release.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

struct CCPPointerInfo {
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
class C3DAnimFileRootChunk : public CChunkedFileSuperChunk { };
class CCharString { public: void* PStringData; };

class CChunkedFile {
public:
    virtual ~CChunkedFile();
    CCountedPointer<CChunkedFileSuperChunk> RootChunk;  // +0x04
    unsigned long                           Version;    // +0x0c
    CCharString                             FileComment; // +0x10
};

class C3DAnimFile : public CChunkedFile {
public:
    CCountedPointer<const C3DAnimFileRootChunk> GetRoot() const;
};

CCountedPointer<const C3DAnimFileRootChunk> C3DAnimFile::GetRoot() const
{
    CCountedPointer<const C3DAnimFileRootChunk> root;
    root.SetStaticCast(CCountedPointer<const CChunkedFileSuperChunk>(RootChunk));
    return root;
}
// C3DAnimFileLoader::LoadAnimFile(CDataInputStream&) -- new root chunk from the registered prototype, a new C3DAnimFile around it, load the chunks, return the file
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A99420, length 240 bytes (`ret 8`: hidden result + stream).
// Ego_r.pdb: ?LoadAnimFile@C3DAnimFileLoader@@QAE?AV?$CCountedPointer@VC3DAnimFile@@@@AAVCDataInputStream@@@Z.
// Callees: the root prototype's CreateNewInstance (vtable slot 9, +0x24), operator new @ 0x00BFEA1A,
// C3DAnimFile::C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>&) @ 0x00A98F40, the two counted pointers'
// __fastcall DeleteData hooks @ 0x00A99FE0 (root chunk) / 0x00A9A000 (file), CChunkedFileLoader::LoadChunkedFile
// (CChunkedFile&, CDataInputStream&) @ 0x00AA79C0, operator delete @ 0x00BFE9BC.
// Shape: two `CCountedPointer<T> p(raw)` locals (count block only for a non-null object), the load on the raw
// file, then `return file;` (no NRVO: merged inc/dec copy-out) and the root pointer's register-form release.
// Layout: the root chunk prototype pointer at +0x10 of the loader (CChunkedFileLoader base).
// Leaf: LoadAnimFile@C3DAnimFileLoader
struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
    SCountBlock(void (__fastcall* destroy)(void*), void* object) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};

template<class T> struct CCountedPointer {
    T* Object;                                      // +0
    SCountBlock* Count;                             // +4
    static void __fastcall DeleteData(void* object);   // 0x00A99FE0 (root chunk) / 0x00A9A000 (file)
    CCountedPointer(T* object) : Object(object), Count(object ? new SCountBlock(DeleteData, object) : 0) {}
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    ~CCountedPointer()
    {
        if (Count) {
            if (--Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
    }
    template<class U> CCountedPointer(const CCountedPointer<U>& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    T& operator*() const { return *Object; }
};

struct CDataInputStream;
struct CChunkedFileChunk {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8();
    virtual CChunkedFileChunk* CreateNewInstance() const;   // slot 9 (+0x24)
};
struct CChunkedFileSuperChunk : CChunkedFileChunk {};
struct C3DAnimFileRootChunk : CChunkedFileSuperChunk {};

struct CChunkedFile {
    CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root);   // 0x00AA7DB0
    virtual ~CChunkedFile();
    unsigned char _body[0x10];                      // RootChunk, Version, FileComment (sizeof 0x14)
};
struct C3DAnimFile : CChunkedFile {
    C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>& root);   // 0x00A98F40
};

struct CChunkedFileLoader {
    virtual ~CChunkedFileLoader();
    unsigned char _pad04[0x0c];
    CChunkedFileChunk* RootChunkPrototype;          // +0x10
    void LoadChunkedFile(CChunkedFile& file, CDataInputStream& in);   // 0x00AA79C0
};

struct C3DAnimFileLoader : CChunkedFileLoader {
    CCountedPointer<C3DAnimFile> LoadAnimFile(CDataInputStream& in);
};

// The C3DAnimFile constructor shares the TU (0x00A98F40, defined above the caller as in retail): VC7.1 then
// knows it leaves the root pointer alone and keeps its count word in ebp across the call.
#pragma auto_inline(off)
C3DAnimFile::C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>& root) : CChunkedFile(root)
{
}
#pragma auto_inline(on)

CCountedPointer<C3DAnimFile> C3DAnimFileLoader::LoadAnimFile(CDataInputStream& in)
{
    CCountedPointer<C3DAnimFileRootChunk> root((C3DAnimFileRootChunk*)RootChunkPrototype->CreateNewInstance());
    CCountedPointer<C3DAnimFile> file(new C3DAnimFile(root));
    LoadChunkedFile(*file, in);
    return file;
}
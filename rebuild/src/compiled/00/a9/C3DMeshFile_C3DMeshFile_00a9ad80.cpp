// C3DMeshFile::C3DMeshFile() -- build the file around a new C3DMeshFileRootChunk, stamp the copyright comment and version 100
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9AD80, length 224 bytes.
// Identity: stores the C3DMeshFile vtable 0x0129E0D8 (rtti-vtables.tsv); Ego_r.pdb has ??0C3DMeshFile@@QAE@XZ.
// Callees: operator new @ 0x00BFEA1A (0x2c-byte C3DMeshFileRootChunk, 0x0C-byte count block), C3DMeshFileChunk(const char*) @ 0x00AA8360
// (catalog "CShopItemDef"; the root chunk's base), CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&)
// @ 0x00AA7DB0, the counted pointer's __fastcall DeleteData hook @ 0x00A99FD0, operator delete @ 0x00BFE9BC,
// CCharString(const char*, long) @ 0x0099EBF0, CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0.
// Shape: the base is built from a CCountedPointer<CChunkedFileSuperChunk> temporary over `new C3DMeshFileRootChunk` (its inline
// ctor: C3DMeshFileChunk("3DRT") then vtable 0x0129E090), released after the base ctor; then the vtable store,
// FileComment assigned from a named CCharString local in its own {} block (retail's `lea ; push`, sharing the dead temporary's frame word) and Version = 100.
// Layout: CChunkedFile (PDB 0x14): vptr, RootChunk +4, Version +0xc, FileComment +0x10.
// Leaf: ??0C3DMeshFile
struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
    SCountBlock(void (__fastcall* destroy)(void*), void* object) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};

template<class T> struct CCountedPointer {
    T* Object;                                      // +0
    SCountBlock* Count;                             // +4
    static void __fastcall DeleteData(void* object);   // 0x00A99FD0
    CCountedPointer(T* object) : Object(object) { Count = Object ? new SCountBlock(DeleteData, Object) : 0; }
    ~CCountedPointer()
    {
        if (Count) {
            --Count->m_references;
            if (Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
    }
};

struct CCharString {
    CCharString(const char* text, long length);     // 0x0099EBF0 (-1 = strlen)
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    void* m_pStringData;
};

struct CChunkedFileSuperChunk {
    virtual ~CChunkedFileSuperChunk();
};
struct C3DMeshFileChunk : CChunkedFileSuperChunk {
    C3DMeshFileChunk(const char* chunkType);        // 0x00AA8360
    unsigned char _base_body[0x28];                 // +0x04 .. +0x2b
};
struct C3DMeshFileRootChunk : C3DMeshFileChunk {
    C3DMeshFileRootChunk() : C3DMeshFileChunk("3DRT") {}
    virtual ~C3DMeshFileRootChunk();
};

struct CChunkedFile {
    CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root);   // 0x00AA7DB0
    virtual ~CChunkedFile();
    CCountedPointer<CChunkedFileSuperChunk> RootChunk;  // +0x04
    unsigned long Version;                          // +0x0c
    CCharString FileComment;                        // +0x10
};

struct C3DMeshFile : CChunkedFile {
    C3DMeshFile();
    virtual ~C3DMeshFile();
};

C3DMeshFile::C3DMeshFile() : CChunkedFile(CCountedPointer<CChunkedFileSuperChunk>(new C3DMeshFileRootChunk))
{
    {
        CCharString comment("Copyright Big Blue Box Studios Ltd.", -1);
        FileComment = comment;
    }
    Version = 100;
}
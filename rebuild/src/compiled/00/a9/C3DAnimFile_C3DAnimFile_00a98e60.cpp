// C3DAnimFile::C3DAnimFile() -- build the file around a new C3DAnimFileRootChunk, stamp the copyright comment and version 100
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98E60, length 224 bytes.
// Identity: stores the C3DAnimFile vtable 0x0129E034 (rtti-vtables.tsv); Ego_r.pdb has ??0C3DAnimFile@@QAE@XZ.
// Callees: operator new @ 0x00BFEA1A (0x34-byte C3DAnimFileRootChunk, 0x0C-byte count block), C3DMeshFileChunk(const char*) @ 0x00AA8360
// (catalog "CShopItemDef"; the root chunk's base), CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&)
// @ 0x00AA7DB0, the counted pointer's __fastcall DeleteData hook @ 0x00A99FD0, operator delete @ 0x00BFE9BC,
// CCharString(const char*, long) @ 0x0099EBF0, CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0.
// Shape: the base is built from a CCountedPointer<CChunkedFileSuperChunk> temporary over `new C3DAnimFileRootChunk` (its inline
// ctor: C3DMeshFileChunk("ANRT") then vtable 0x0129DFF0), released after the base ctor; then the vtable store,
// FileComment assigned from a named CCharString local in its own {} block (retail's `lea ; push`, sharing the dead temporary's frame word) and Version = 100.
// Layout: CChunkedFile (PDB 0x14): vptr, RootChunk +4, Version +0xc, FileComment +0x10.
// Leaf: ??0C3DAnimFile
struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
    SCountBlock(void (__fastcall* destroy)(void*), void* object) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};

template<class T> class CCountedPointer { public:
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

class CCharString { public:
    CCharString(const char* text, long length);     // 0x0099EBF0 (-1 = strlen)
    ~CCharString();                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    void* m_pStringData;
};

class CChunkedFileSuperChunk { public:
    virtual ~CChunkedFileSuperChunk();
};
struct C3DMeshFileChunk : CChunkedFileSuperChunk {
    C3DMeshFileChunk(const char* chunkType);        // 0x00AA8360
    unsigned char _base_body[0x28];                 // +0x04 .. +0x2b
};
class C3DAnimFileRootChunk : public C3DMeshFileChunk { public:
    C3DAnimFileRootChunk() : C3DMeshFileChunk("ANRT") {}
    virtual ~C3DAnimFileRootChunk();
    bool  LoopFlag;                                 // +0x2c
    float Duration;                                 // +0x30
};

class CChunkedFile { public:
    CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root);   // 0x00AA7DB0
    virtual ~CChunkedFile();
    CCountedPointer<CChunkedFileSuperChunk> RootChunk;  // +0x04
    unsigned long Version;                          // +0x0c
    CCharString FileComment;                        // +0x10
};

class C3DAnimFile : public CChunkedFile { public:
    C3DAnimFile();
    virtual ~C3DAnimFile();
};

C3DAnimFile::C3DAnimFile() : CChunkedFile(CCountedPointer<CChunkedFileSuperChunk>(new C3DAnimFileRootChunk))
{
    {
        CCharString comment("Copyright Big Blue Box Studios Ltd.", -1);
        FileComment = comment;
    }
    Version = 100;
}
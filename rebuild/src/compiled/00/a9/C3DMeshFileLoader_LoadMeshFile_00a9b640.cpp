// C3DMeshFileLoader::LoadMeshFile(CDataInputStream&) -- new root chunk from the registered prototype, a new C3DMeshFile around it, load the chunks, return the file
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9B640, length 244 bytes (`ret 8`: hidden result + stream).
// Ego_r.pdb: ?LoadMeshFile@C3DMeshFileLoader@@QAE?AV?$CCountedPointer@VC3DMeshFile@@@@AAVCDataInputStream@@@Z.
// Callees: the root prototype's CreateNewInstance (vtable slot 9, +0x24), operator new @ 0x00BFEA1A,
// C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>&) @ 0x00A9AE60, the two counted pointers'
// __fastcall DeleteData hooks @ 0x00A9CB20 (root chunk) / 0x00A9CB50 (file), CChunkedFileLoader::LoadChunkedFile
// (CChunkedFile&, CDataInputStream&) @ 0x00AA79C0, operator delete @ 0x00BFE9BC.
// Shape: two `CCountedPointer<T> p(raw)` locals (count block only for a non-null object), the load on the raw
// file, then `return file;` (no NRVO: merged inc/dec copy-out) and the root pointer's register-form release.
// Layout: the root chunk prototype pointer at +0x10 of the loader (CChunkedFileLoader base).
// Leaf: LoadMeshFile@C3DMeshFileLoader
#include "engine/CDataInputStream.h"
#include "engine/CChunkedFileChunk.h"
#include "engine/CChunkedFileSuperChunk.h"
// Shared flattened headers describe storage; these method-only views recover virtual calls.
// Only the vptr is accessed through these views; the chunk headers' donor container sizes are unused.
struct CInputStreamMethods { virtual void Reserved0(); virtual void SetPosition(unsigned long); virtual unsigned long GetPosition(); unsigned char StreamStorage[sizeof(CDataInputStream)-sizeof(void*)]; };
struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
    SCountBlock(void (__fastcall* destroy)(void*), void* object) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};

template<class T> class CCountedPointer { public:
    T* Object;                                      // +0
    SCountBlock* Count;                             // +4
    static void __fastcall DeleteData(void* object);   // 0x00A9CB20 (root chunk) / 0x00A9CB50 (file)
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
    template<class U> CCountedPointer(const CCountedPointer<U>& other) : Object((T*)other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    T& operator*() const { return *Object; }
};

struct CChunkPrototypeMethods {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8();
    virtual CChunkedFileChunk* CreateNewInstance() const;   // slot 9 (+0x24)
};
class C3DMeshFileRootChunk : public CChunkedFileSuperChunk { public:};

class CChunkedFile { public:
    CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root);   // 0x00AA7DB0
    virtual ~CChunkedFile();
    unsigned char _body[0x10]; // RootChunk +4, Version +0xc, FileComment +0x10; total 0x14
};
class C3DMeshFile : public CChunkedFile { public:
    C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root);   // 0x00A9AE60
};

class CChunkedFileLoader { public:
    virtual ~CChunkedFileLoader();
    unsigned char _pad04[0x0c];
    CChunkedFileChunk* RootChunkPrototype;          // +0x10
    void LoadChunkedFile(CChunkedFile& file, CDataInputStream& in);   // 0x00AA79C0
};

struct C3DMeshFileLoader : CChunkedFileLoader {
    CCountedPointer<C3DMeshFile> LoadMeshFile(CDataInputStream& in);
};

// The C3DMeshFile constructor shares the TU (0x00A9AE60, defined above the caller as in retail): VC7.1 then
// knows it leaves the root pointer alone and keeps its count word in ebp across the call.
#pragma auto_inline(off)
C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root) : CChunkedFile(root)
{
}
#pragma auto_inline(on)

CCountedPointer<C3DMeshFile> C3DMeshFileLoader::LoadMeshFile(CDataInputStream& in)
{
    CCountedPointer<C3DMeshFileRootChunk> root((C3DMeshFileRootChunk*)((CChunkPrototypeMethods*)RootChunkPrototype)->CreateNewInstance());
    CCountedPointer<C3DMeshFile> file(new C3DMeshFile(root));
    CInputStreamMethods& stream = *(CInputStreamMethods*)&in;
    unsigned long position = stream.GetPosition();
    stream.SetPosition(position);
    LoadChunkedFile(*file, in);
    return file;
}
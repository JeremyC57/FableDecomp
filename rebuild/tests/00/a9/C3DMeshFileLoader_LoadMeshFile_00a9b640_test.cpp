#include <stdio.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define FABLE_ENGINE_CDATAINPUTSTREAM_H
#define FABLE_ENGINE_CCHUNKEDFILECHUNK_H
#define FABLE_ENGINE_CCHUNKEDFILESUPERCHUNK_H
struct CDataInputStream {void* vptr;unsigned char storage[0x14];};
struct CChunkedFileChunk {void* vptr;};
struct CChunkedFileSuperChunk {void* vptr;};
#endif
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

template<class T> struct CCountedPointer {
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
struct C3DMeshFileRootChunk : CChunkedFileSuperChunk {};

struct CChunkedFile {
    CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root);   // 0x00AA7DB0
    virtual ~CChunkedFile();
    unsigned char _body[0x10]; // RootChunk +4, Version +0xc, FileComment +0x10; total 0x14
};
struct C3DMeshFile : CChunkedFile {
    C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root);   // 0x00A9AE60
};

struct CChunkedFileLoader {
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
static int stage, failures, rootsDestroyed, filesDestroyed, loaded;
static bool emptyRoot;
static C3DMeshFileRootChunk rootStorage;
static CDataInputStream* expectedStream;
static CChunkedFileLoader* expectedLoader;
static void Check(bool value) { if (!value) ++failures; }
void CChunkPrototypeMethods::v0() {} void CChunkPrototypeMethods::v1() {}
void CChunkPrototypeMethods::v2() {} void CChunkPrototypeMethods::v3() {}
void CChunkPrototypeMethods::v4() {} void CChunkPrototypeMethods::v5() {}
void CChunkPrototypeMethods::v6() {} void CChunkPrototypeMethods::v7() {}
void CChunkPrototypeMethods::v8() {}
CChunkedFileChunk* CChunkPrototypeMethods::CreateNewInstance() const {
 Check(stage++ == 0); return emptyRoot ? 0 : (CChunkedFileChunk*)&rootStorage;
}
void CInputStreamMethods::Reserved0() {}
unsigned long CInputStreamMethods::GetPosition() {
 Check((CDataInputStream*)this == expectedStream); Check(stage++ == 2); return 0x87654321UL;
}
void CInputStreamMethods::SetPosition(unsigned long pos) {
 Check((CDataInputStream*)this == expectedStream); Check(stage++ == 3); Check(pos == 0x87654321UL);
}
CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root) {
 Check(stage++ == 1);
 Check(root.Object == (emptyRoot ? 0 : (CChunkedFileSuperChunk*)&rootStorage));
 Check(emptyRoot ? root.Count == 0 : root.Count && root.Count->m_references == 2);
 new (_body) CCountedPointer<CChunkedFileSuperChunk>(root);
}
CChunkedFile::~CChunkedFile() {
 ((CCountedPointer<CChunkedFileSuperChunk>*)_body)->~CCountedPointer<CChunkedFileSuperChunk>();
}
CChunkedFileLoader::~CChunkedFileLoader() {}
template<> void __fastcall CCountedPointer<C3DMeshFileRootChunk>::DeleteData(void* p) {
 Check(p == &rootStorage); ++rootsDestroyed;
}
template<> void __fastcall CCountedPointer<C3DMeshFile>::DeleteData(void* p) {
 ++filesDestroyed; delete (C3DMeshFile*)p;
}
void CChunkedFileLoader::LoadChunkedFile(CChunkedFile& file, CDataInputStream& input) {
 Check(this == expectedLoader); Check(&input == expectedStream); Check(stage++ == 4); ++loaded;
 CCountedPointer<CChunkedFileSuperChunk>* root = (CCountedPointer<CChunkedFileSuperChunk>*)file._body;
 Check(root->Object == (emptyRoot ? 0 : (CChunkedFileSuperChunk*)&rootStorage));
 Check(emptyRoot ? root->Count == 0 : root->Count && root->Count->m_references == 2);
}
static void Run(bool noRoot) {
 emptyRoot = noRoot; stage = 0; rootsDestroyed = filesDestroyed = loaded = 0;
 CChunkPrototypeMethods prototype; CInputStreamMethods stream; C3DMeshFileLoader loader;
 loader.RootChunkPrototype = (CChunkedFileChunk*)&prototype;
 expectedLoader = &loader; expectedStream = (CDataInputStream*)&stream;
 {
  CCountedPointer<C3DMeshFile> result = loader.LoadMeshFile(*expectedStream);
  Check(result.Object != 0 && result.Count != 0); Check(result.Count->m_references == 1);
  Check(loaded == 1 && stage == 5 && !rootsDestroyed && !filesDestroyed);
  CCountedPointer<CChunkedFileSuperChunk>* root = (CCountedPointer<CChunkedFileSuperChunk>*)result.Object->_body;
  Check(noRoot ? root->Count == 0 : root->Count->m_references == 1);
  { CCountedPointer<C3DMeshFile> copy(result); Check(copy.Count->m_references == 2); }
  Check(result.Count->m_references == 1);
 }
 Check(filesDestroyed == 1 && rootsDestroyed == (noRoot ? 0 : 1));
}
int main() { Run(false); Run(true); printf("00a9b640_TEST %s\n", failures ? "FAIL" : "PASS"); return failures ? 1 : 0; }
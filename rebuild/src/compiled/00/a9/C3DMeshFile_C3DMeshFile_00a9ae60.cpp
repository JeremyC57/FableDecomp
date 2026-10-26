// C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root) -- chunked file over a typed root chunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9AE60, length 96 bytes.
// Callees: CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&) @ 0x00AA7DB0,
//          operator delete @ 0x00BFE9BC (the count block). Vtable 0x0129E0D8 (rtti-vtables.tsv).
// The derived-typed counted pointer is converted to the base-typed one the CChunkedFile
// constructor takes: the converting constructor copies object + count block into an 8-byte
// temporary and bumps the count, the base constructor gets its address, and the temporary is
// released (decrement, re-read, destroy + delete on zero) before this class's vtable is stored.
// Layout from the PDB: CChunkedFile is 0x14 bytes (vptr, RootChunk +4, Version +0xc,
// FileComment +0x10); this class adds nothing.
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4: per-type deleter, object in ecx
    void* m_pObject;                        // +8
};

template <class T>
class CCountedPointer { public:
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    template <class U>
    CCountedPointer(const CCountedPointer<U>& other)
        : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};

class CChunkedFileSuperChunk { public: virtual void v00(); };
class C3DMeshFileRootChunk : public CChunkedFileSuperChunk { public: };

class CChunkedFile { public:
    virtual void v00();
    unsigned char _body[0x10];              // RootChunk +4, Version +0xc, FileComment +0x10
    CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root);   // 0x00AA7DB0
};

class C3DMeshFile : public CChunkedFile { public:
    virtual void v00();
    C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root);
};

C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root)
    : CChunkedFile(root)
{
}
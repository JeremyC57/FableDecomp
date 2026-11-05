// C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root) -- chunked file over a typed root chunk
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9AE60, length 96 bytes.
// Callees: CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&) @ 0x00AA7DB0,
//          operator delete @ 0x00BFE9BC (the count block). Vtable 0x0129E0D8 (rtti-vtables.tsv).
// The derived-typed counted pointer is converted to the base-typed one the CChunkedFile
// constructor takes: the converting constructor copies object + count block into an 8-byte
// temporary and bumps the count, the base constructor gets its address, and the temporary is
// released (decrement, re-read, destroy + delete on zero) before this class's vtable is stored.
// Layout from the PDB: CChunkedFile is 0x14 bytes (vptr, RootChunk +4, Version +0xc,
// FileComment +0x10); this class adds nothing.
#include "engine/CCountedPointer.h"


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
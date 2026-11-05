// C3DMeshFile::C3DMeshFile() -- build the file around a new C3DMeshFileRootChunk, stamp the copyright comment and version 100
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9AD80, length 224 bytes.
// Identity: stores the C3DMeshFile vtable 0x0129E0D8 (rtti-vtables.tsv); Ego_r.pdb has ??0C3DMeshFile@@QAE@XZ.
// Callees: operator new @ 0x00BFEA1A (0x2c-byte C3DMeshFileRootChunk, 0x0C-byte count block), CChunkedFileSuperChunk(const char*) @ 0x00AA8360
// (catalog "CShopItemDef"; the root chunk's base), CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&)
// @ 0x00AA7DB0, the counted pointer's __fastcall DeleteData hook @ 0x00A99FD0, operator delete @ 0x00BFE9BC,
// CCharString(const char*, long) @ 0x0099EBF0, CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0.
// Shape: the base is built from a CCountedPointer<CChunkedFileSuperChunk> temporary over `new C3DMeshFileRootChunk` (its inline
// ctor: CChunkedFileSuperChunk("3DRT") then vtable 0x0129E090), released after the base ctor; then the vtable store,
// FileComment assigned from a named CCharString local in its own {} block (retail's `lea ; push`, sharing the dead temporary's frame word) and Version = 100.
// Layout: CChunkedFile (PDB 0x14): vptr, RootChunk +4, Version +0xc, FileComment +0x10.
// Leaf: ??0C3DMeshFile
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members Object->Data, Count->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
#include "engine/CCountedPointer.h"


class CCharString { public:
    CCharString(const char* text, long length);     // 0x0099EBF0 (-1 = strlen)
    ~CCharString();                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    void* m_pStringData;
};

class CChunkedFileSuperChunk { public:
    virtual ~CChunkedFileSuperChunk();
protected:
    CChunkedFileSuperChunk(const char* chunkType);        // 0x00AA8360 (Ego_r / FableWin protected IAE; RTTI: the direct base of the root chunks)
public:
    unsigned char _base_body[0x28];                 // +0x04 .. +0x2b
};
class C3DMeshFileRootChunk : public CChunkedFileSuperChunk { public:
    C3DMeshFileRootChunk() : CChunkedFileSuperChunk("3DRT") {}
    virtual ~C3DMeshFileRootChunk();
};

class CChunkedFile { public:
    CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root);   // 0x00AA7DB0
    virtual ~CChunkedFile();
    CCountedPointer<CChunkedFileSuperChunk> RootChunk;  // +0x04
    unsigned long Version;                          // +0x0c
    CCharString FileComment;                        // +0x10
};

class C3DMeshFile : public CChunkedFile { public:
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
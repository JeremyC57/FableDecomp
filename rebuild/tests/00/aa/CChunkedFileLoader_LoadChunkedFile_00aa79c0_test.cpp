// Behaviour test for 0x00AA79C0 CChunkedFileLoader::LoadChunkedFile: checks the ">>>>" file id (retail 0x0129E454) and the loader's
// file type, reads the version, the comment and the root chunk's type and size, and hands the stream to the root chunk.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined; it is called through a
// __fastcall shim aliased to the row symbol). The stream and the root chunk are fixture classes laid out as the row's models, so a
// file that starts ">>>>" loads; a mutated literal makes the row throw, which the test catches and reports as FAIL.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CCharString { public: void* PStringData; const CCharString& operator=(const CCharString& other); ~CCharString(); };
class CChunkedFileLoader;
class CDataInputStream { public:
    unsigned long StreamPos; long StreamSize; const unsigned char* CurrentSourcePtr; unsigned long SourceChunkStreamPos; long SourceChunkBytesRemaining;
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadString();                   // slot 6
};
class CChunkedFileSuperChunk { public:
    char ChunkType[5];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);   // slot 7
};
struct CRootPointer { CChunkedFileSuperChunk* Data; void* Info; ~CRootPointer(); };
static char g_comment[] = "made by test"; static int g_bad = 0; static unsigned long g_rootSize = 0; static void* g_loader = 0; static void* g_comm = 0;
const CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
CCharString::~CCharString() { }
CRootPointer::~CRootPointer() { }
void CDataInputStream::v0() { } void CDataInputStream::v1() { } void CDataInputStream::v2() { } void CDataInputStream::v3() { } void CDataInputStream::v4() { } void CDataInputStream::v5() { }
CCharString CDataInputStream::ReadString() { CCharString s; s.PStringData = g_comment; return s; }
void CChunkedFileSuperChunk::v0() { } void CChunkedFileSuperChunk::v1() { } void CChunkedFileSuperChunk::v2() { } void CChunkedFileSuperChunk::v3() { }
void CChunkedFileSuperChunk::v4() { } void CChunkedFileSuperChunk::v5() { } void CChunkedFileSuperChunk::v6() { }
void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{ g_rootSize = size; g_loader = &loader; if (stream.StreamPos != 20) g_bad = 1; }
extern "C" void __fastcall NF420_Row(void* self, int edx, void* chunk, CDataInputStream* stream);
#pragma comment(linker, "/alternatename:@NF420_Row@16=?LoadChunkedFile@CChunkedFileLoader@@IAEXAAVCChunkedFile@@AAVCDataInputStream@@@Z")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
extern "C" void __cdecl NF420Trap0() { NF420TrapHit("?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z"); }
#pragma comment(linker, "/alternatename:?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z=_NF420Trap0")
int main()
{
    static unsigned char file[20]; memcpy(file, ">>>>TYPE", 8); *(unsigned long*)(file + 8) = 3; memcpy(file + 12, "ROOT", 4); *(unsigned long*)(file + 16) = 99;
    CDataInputStream stream; stream.StreamPos = 0; stream.StreamSize = 20; stream.CurrentSourcePtr = file; stream.SourceChunkStreamPos = 0; stream.SourceChunkBytesRemaining = 20;
    CChunkedFileSuperChunk root; memcpy(root.ChunkType, "ROOT", 5);
    static unsigned char loader[0x10]; memcpy(loader + 8, "TYPE", 5);
    static unsigned char chunk[0x14]; *(CChunkedFileSuperChunk**)(chunk + 4) = &root;
    bool threw = false;
    try { NF420_Row(loader, 0, chunk, &stream); } catch (...) { threw = true; }
    bool ok = !threw && *(unsigned long*)(chunk + 0xc) == 3 && *(void**)(chunk + 0x10) == g_comment && g_rootSize == 99 && g_loader == loader && !g_bad;
    if (ok) printf("00aa79c0_TEST PASS\n");
    else printf("FAIL threw=%d version=%lu size=%lu bad=%d\n", (int)threw, *(unsigned long*)(chunk + 0xc), g_rootSize, g_bad);
    return ok ? 0 : 1;
}

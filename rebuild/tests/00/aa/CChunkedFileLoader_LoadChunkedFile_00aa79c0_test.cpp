// Behaviour test for CChunkedFileLoader::LoadChunkedFile (0x00AA79C0): body verbatim, callees stubbed. Covers the good path
// (version/comment stored, root chunk called with stream/size/loader, reference counts restored), the overflow read path,
// and the three CChunkedFileException exits (bad file id, bad file type, root type mismatch).
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef long long __int64;
#define long int   // g++ host: keep Fable's 4-byte long
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
class CCharString { public:
    const char* PStringData;
    const CCharString& operator=(const CCharString& other);
    ~CCharString();
};
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
class CChunkedFileLoader;
class CDataInputStream { public:
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {}
    virtual CCharString ReadString();                   // slot 6 (+0x18)
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, 4); return value; }
};
class CChunkedFileSuperChunk { public:
    char ChunkType[5];                                  // +0x04
    virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {}
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);   // slot 7 (+0x1c)
};
struct CRootPointer {
    CChunkedFileSuperChunk* Data;
    CCPPointerInfo*         Info;
    ~CRootPointer();
    CChunkedFileSuperChunk* operator->() const { return Data; }
};
struct CRootView {
    CChunkedFileSuperChunk* Data;
    CCPPointerInfo*         Info;
    __forceinline CRootView(const CRootPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CRootView() { if (Info) { if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } }
    CChunkedFileSuperChunk* operator->() const { return Data; }
};
class CChunkedFile { public:
    void*        _vf;                                   // +0x00
    CRootPointer RootChunk;                             // +0x04
    unsigned long Version;                              // +0x0c
    CCharString  FileComment;                           // +0x10
};
struct CChunkedFileException { CChunkedFileException() {} };
extern const char CHUNKED_FILE_ID[];
static __forceinline bool SameID(const void* a, const void* b) { return memcmp(a, b, 4) == 0; }
class CChunkedFileLoader { public:
    virtual void v0() {}
    long ReadUnknownChunkTypes;                         // +0x04
    char SupportedFileType[5];                          // +0x08
    void LoadChunkedFile(CChunkedFile& chunk, CDataInputStream& stream);
};

// ---- body verbatim (src/00aa79c0.cpp) ----
void CChunkedFileLoader::LoadChunkedFile(CChunkedFile& chunk, CDataInputStream& stream)
{
    char id[4] = { 0, 0, 0, 0 };
    stream.Read(id, 4);
    if (!SameID(id, CHUNKED_FILE_ID))
        throw CChunkedFileException();
    unsigned long fileType = 0;
    stream.Read(&fileType, 4);
    if (!SameID(&fileType, SupportedFileType))
        throw CChunkedFileException();
    chunk.Version = stream.ReadULong();
    CCharString comment = stream.ReadString();
    chunk.FileComment = comment;
    unsigned long rootType;
    stream.Read(&rootType, 4);
    if (!SameID(&rootType, CRootView(chunk.RootChunk)->ChunkType))
        throw CChunkedFileException();
    unsigned long rootSize = stream.ReadULong();
    CRootPointer root = chunk.RootChunk;
    CChunkedFileSuperChunk* rootChunk = root.Data;
    if (root.Info) ++root.Info->RefCount;
    rootChunk->ReadFromFile(stream, rootSize, *this);
}

// ---- stubs ----
const char CHUNKED_FILE_ID[] = ">>>>";
static int g_assigns = 0, g_strDtors = 0, g_overflow = 0, g_rootCalls = 0, g_rootDtors = 0, g_deletes = 0;
static CDataInputStream* g_rootStream = 0; static unsigned long g_rootSize = 0; static CChunkedFileLoader* g_rootLoader = 0;
const CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; g_assigns++; return *this; }
CCharString::~CCharString() { g_strDtors++; }
CCharString CDataInputStream::ReadString() { CCharString s; s.PStringData = "a comment"; return s; }
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    g_overflow++; memcpy(buffer, CurrentSourcePtr, size); CurrentSourcePtr += size; StreamPos += size;
}
void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    g_rootCalls++; g_rootStream = &stream; g_rootSize = size; g_rootLoader = &loader;
}
CRootPointer::~CRootPointer() { g_rootDtors++; if (Info) { if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } }
static void __fastcall DeleteRoot(void*) { g_deletes++; }

static void Put(unsigned char* p, const char* four) { memcpy(p, four, 4); }
static void PutU(unsigned char* p, unsigned int v) { p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); p[2] = (unsigned char)(v >> 16); p[3] = (unsigned char)(v >> 24); }
static void MakeStream(CDataInputStream& s, const unsigned char* bytes, long avail)
{
    s.StreamPos = 0; s.StreamSize = 20; s.CurrentSourcePtr = bytes; s.SourceChunkStreamPos = 0; s.SourceChunkBytesRemaining = avail;
}
static int Load(CChunkedFileLoader& loader, CChunkedFile& file, CDataInputStream& s)
{
    try { loader.LoadChunkedFile(file, s); } catch (CChunkedFileException&) { return 1; }
    return 0;
}

int main()
{
    unsigned char bytes[20];
    Put(bytes, ">>>>"); Put(bytes + 4, "TYPE"); PutU(bytes + 8, 7); Put(bytes + 12, "ROOT"); PutU(bytes + 16, 0x40);
    CChunkedFileSuperChunk root; memcpy(root.ChunkType, "ROOT", 5);
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 1; info->DeleteFunc = DeleteRoot; info->Data = &root;
    CChunkedFile file; file._vf = 0; file.RootChunk.Data = &root; file.RootChunk.Info = info; file.Version = 0; file.FileComment.PStringData = "";
    CChunkedFileLoader loader; loader.ReadUnknownChunkTypes = 0; memcpy(loader.SupportedFileType, "TYPE", 5);
    CDataInputStream s;

    // 1. good path, all from the current source chunk
    MakeStream(s, bytes, 20);
    CHECK(Load(loader, file, s) == 0);
    CHECK(file.Version == 7 && strcmp(file.FileComment.PStringData, "a comment") == 0 && g_assigns == 1);
    CHECK(g_rootCalls == 1 && g_rootStream == &s && g_rootSize == 0x40 && g_rootLoader == &loader);
    CHECK(g_rootDtors == 1 && info->RefCount == 1 && g_deletes == 0 && s.StreamPos == 20 && g_overflow == 0);

    // 2. good path, source chunk short: every word through the overflow read
    file.Version = 0; g_rootCalls = 0;
    MakeStream(s, bytes, 2);
    CHECK(Load(loader, file, s) == 0);
    CHECK(file.Version == 7 && g_rootCalls == 1 && g_rootSize == 0x40 && g_overflow == 5 && info->RefCount == 1);

    // 3. bad file id
    g_rootCalls = 0; bytes[0] = '<';
    MakeStream(s, bytes, 20); file.Version = 0;
    CHECK(Load(loader, file, s) == 1 && file.Version == 0 && g_rootCalls == 0 && s.StreamPos == 4);
    bytes[0] = '>';

    // 4. wrong file type
    memcpy(loader.SupportedFileType, "TYPX", 5);
    MakeStream(s, bytes, 20);
    CHECK(Load(loader, file, s) == 1 && file.Version == 0 && g_rootCalls == 0 && s.StreamPos == 8);
    memcpy(loader.SupportedFileType, "TYPE", 5);

    // 5. root type mismatch: version/comment already stored, root not called, reference restored by the inline release
    memcpy(root.ChunkType, "RXXT", 5); g_assigns = 0;
    MakeStream(s, bytes, 20);
    CHECK(Load(loader, file, s) == 1 && file.Version == 7 && g_assigns == 1 && g_rootCalls == 0 && info->RefCount == 1 && g_deletes == 0);

    // 6. the root-type check's temporary release deletes when it drops the last reference
    memcpy(root.ChunkType, "ROOT", 5);
    CCPPointerInfo* lone = new CCPPointerInfo; lone->RefCount = 0; lone->DeleteFunc = DeleteRoot; lone->Data = &root;
    file.RootChunk.Info = lone; memcpy(root.ChunkType, "RXXT", 5);
    MakeStream(s, bytes, 20);
    CHECK(Load(loader, file, s) == 1 && g_deletes == 1);
    file.RootChunk.Info = 0;
    delete info;

    if (g_fail == 0) printf("00aa79c0_TEST PASS\n"); else printf("00aa79c0_TEST FAIL\n");
    return g_fail != 0;
}
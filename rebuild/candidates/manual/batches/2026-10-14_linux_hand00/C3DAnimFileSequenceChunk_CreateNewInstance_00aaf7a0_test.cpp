#include <cstdio>
#include <cstring>
#include <cstdlib>
struct CChunkedFileSaver { };
struct CCharString { const char* m_pData; CCharString() : m_pData(0) {} };
struct CDataOutputStream {
    char m_log[512]; unsigned long m_bytes;
    CDataOutputStream() : m_bytes(0) { m_log[0] = 0; }
    void Append(const char* s) { std::strcat(m_log, s); }
    virtual void WriteBytes(const void* data, unsigned long size) { char b[64]; std::sprintf(b, "B%lu;", size); Append(b); m_bytes = size; m_data = data; }
    const void* m_data;
    void WriteLONG(long v) { char b[64]; std::sprintf(b, "L%ld;", v); Append(b); }
    void WriteULONG(unsigned long v) { char b[64]; std::sprintf(b, "U%lu;", v); Append(b); }
    void WriteFLOAT(float v) { char b[64]; std::sprintf(b, "F%g;", (double)v); Append(b); }
    void WriteBOOL(bool v) { Append(v ? "T;" : "f;"); }
    void WriteNullTerminatedString(const CCharString& s) { Append("S:"); Append(s.m_pData ? s.m_pData : ""); Append(";"); }
};
template <class T> struct CArray {
    T* m_pBegin; T* m_pEnd; T* m_pEndOfStorage;
    CArray() : m_pBegin(0), m_pEnd(0), m_pEndOfStorage(0) {}
    unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
    const T& operator[](unsigned long i) const { return m_pBegin[i]; }
    void Use(T* p, unsigned long n) { m_pBegin = p; m_pEnd = p + n; m_pEndOfStorage = p + n; }
};
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static int g_destroyed = 0;
static void DestroyChunk(void*) { ++g_destroyed; }
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* object) : m_pObject(object), m_pCount(0) { if (object) { m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroyChunk; m_pCount->m_pObject = object; } }
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } }
};
struct CChunkedFileSuperChunk;
static int g_initCalls = 0;
struct CChunkedFileChunk {
    virtual ~CChunkedFileChunk() {}
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const { out.Append("base;"); }
    virtual CChunkedFileChunk* CreateNewInstance() const { return 0; }
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk* chunk) const { ++g_initCalls; return chunk; }
    char ChunkType[5]; CChunkedFileSuperChunk* Parent;
    CChunkedFileChunk(const char* type) : Parent(0) { std::strncpy(ChunkType, type, 4); ChunkType[4] = 0; }
};
struct CChunkedFileSuperChunk : CChunkedFileChunk {
    CChunkedFileChunk* m_pFound;
    CChunkedFileSuperChunk(const char* type) : CChunkedFileChunk(type), m_pFound(0) {}
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const { out.Append("super;"); }
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const { return CCountedPointer<CChunkedFileChunk>(std::strcmp(type, m_wanted) == 0 ? m_pFound : 0); }
    const char* m_wanted;
};
struct C3DAnimFileSequenceChunk : CChunkedFileSuperChunk {
    long BoneIndex; long ParentIndex; CCharString BoneName; float SamplesPerSecond; long BoneType; CArray<unsigned char> Matrices;
    C3DAnimFileSequenceChunk() : CChunkedFileSuperChunk("ASEQ"), BoneIndex(-1), BoneName(), BoneType(0), Matrices() {}
    virtual CChunkedFileChunk* CreateNewInstance() const { return InitNewInstance(new C3DAnimFileSequenceChunk); }
};
int main()
{
    C3DAnimFileSequenceChunk factory; CChunkedFileChunk* made = factory.CreateNewInstance();
    if (made == 0 || g_initCalls != 1 || std::strcmp(made->ChunkType, "ASEQ") != 0) { std::printf("00aaf7a0_TEST FAIL\n"); return 1; }
    if (static_cast<C3DAnimFileSequenceChunk*>(made)->BoneIndex != -1 || static_cast<C3DAnimFileSequenceChunk*>(made)->BoneType != 0) { std::printf("00aaf7a0_TEST FAIL\n"); return 1; }
    delete made;
    std::printf("00aaf7a0_TEST PASS\n");
    return 0;
}

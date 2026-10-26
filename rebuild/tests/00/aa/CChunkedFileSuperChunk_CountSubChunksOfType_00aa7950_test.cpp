// Behaviour test for 0x00AA7950 (label CountSubChunksOfType): the registered prototype chunk with a given code, or null.
#include <cstdio>
#include <cstring>
class CChunkedFileSuperChunk;
class CDataOutputStream { public:
    unsigned char buf[256]; long pos; long end;
    CDataOutputStream() : pos(0), end(0) { std::memset(buf, 0, sizeof(buf)); }
    void SetPosition(long p) { pos = p; }
    long GetPosition() { return pos; }
    void Write(const void* d, long n) { std::memcpy(buf + pos, d, n); pos += n; if (pos > end) end = pos; }
    void WriteLong(long v) { int x = (int)v; Write(&x, 4); }
};
class CChunkedFileChunk { public:
    long unknown; long bodySize;
    char ChunkType[5];
    CChunkedFileSuperChunk* Parent;
    CChunkedFileChunk(const char* t, long size, long unk) : unknown(unk), bodySize(size), Parent(0) { std::memcpy(ChunkType, t, 5); }
    long IsUnknownType() const { return unknown; }
    void WriteToFile(CDataOutputStream& s) const { for (long i = 0; i < bodySize; ++i) { unsigned char b = (unsigned char)(0xA0 + i); s.Write(&b, 1); } }
};
struct SCountBlock { long m_references; };
struct CChunkPointer { CChunkedFileChunk* Object; SCountBlock* Count; };
struct CChunkArray {
    CChunkPointer* m_pFirst; CChunkPointer* m_pLast; CChunkPointer* m_pEnd;
    CChunkPointer* begin() const { return m_pFirst; }
    CChunkPointer* end() const { return m_pLast; }
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    CChunkPointer* erase(CChunkPointer* p) { for (CChunkPointer* q = p; q + 1 < m_pLast; ++q) *q = *(q + 1); --m_pLast; return p; }
};
class CChunkedFileSuperChunk { public:
    CChunkArray SubChunks, RegisteredSubChunkTypes;
    CChunkedFileChunk* FindRegisteredSubChunkType(const char* type) const
    {
        for (CChunkPointer* it = RegisteredSubChunkTypes.begin(); it != RegisteredSubChunkTypes.end(); ++it)
            if (std::strcmp(type, it->Object->ChunkType) == 0) return it->Object;
        return 0;
    }
};
int main()
{
    CChunkedFileChunk a("MESH", 0, 0), b("ANIM", 0, 0);
    CChunkPointer items[2] = { { &a, 0 }, { &b, 0 } };
    CChunkedFileSuperChunk s; s.RegisteredSubChunkTypes.m_pFirst = items; s.RegisteredSubChunkTypes.m_pLast = items + 2;
    if (s.FindRegisteredSubChunkType("ANIM") != &b || s.FindRegisteredSubChunkType("MESH") != &a || s.FindRegisteredSubChunkType("TEXT") != 0) {
        std::printf("00aa7950_TEST FAIL\n"); return 1;
    }
    std::printf("00aa7950_TEST PASS\n");
    return 0;
}
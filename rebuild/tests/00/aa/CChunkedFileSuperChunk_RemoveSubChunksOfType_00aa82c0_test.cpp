// Behaviour test for 0x00AA82C0 CChunkedFileSuperChunk::RemoveSubChunksOfType: matches detached and erased, others kept in order.
#include <cstdio>
#include <cstring>
struct CChunkedFileSuperChunk;
struct CDataOutputStream {
    unsigned char buf[256]; long pos; long end;
    CDataOutputStream() : pos(0), end(0) { std::memset(buf, 0, sizeof(buf)); }
    void SetPosition(long p) { pos = p; }
    long GetPosition() { return pos; }
    void Write(const void* d, long n) { std::memcpy(buf + pos, d, n); pos += n; if (pos > end) end = pos; }
    void WriteLong(long v) { int x = (int)v; Write(&x, 4); }
};
struct CChunkedFileChunk {
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
struct CChunkedFileSuperChunk {
    CChunkArray SubChunks;
    void RemoveSubChunksOfType(const char* type)
    {
        for (unsigned long i = 0; i < SubChunks.size(); ++i) {
            CChunkedFileChunk* chunk = (*(SubChunks.begin() + i)).Object;
            if (std::strcmp(type, chunk->ChunkType) == 0) { chunk->Parent = 0; SubChunks.erase(SubChunks.begin() + i); --i; }
        }
    }
};
int main()
{
    CChunkedFileSuperChunk s;
    CChunkedFileChunk a("MESH", 0, 0), b("MESH", 0, 0), c("ANIM", 0, 0), d("MESH", 0, 0);
    a.Parent = b.Parent = c.Parent = d.Parent = &s;
    CChunkPointer items[4] = { { &a, 0 }, { &b, 0 }, { &c, 0 }, { &d, 0 } };
    s.SubChunks.m_pFirst = items; s.SubChunks.m_pLast = items + 4; s.SubChunks.m_pEnd = items + 4;
    s.RemoveSubChunksOfType("MESH");
    if (s.SubChunks.size() != 1 || items[0].Object != &c || a.Parent != 0 || b.Parent != 0 || d.Parent != 0 || c.Parent != &s) {
        std::printf("00aa82c0_TEST FAIL\n"); return 1;
    }
    std::printf("00aa82c0_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x00AA7EA0 CChunkedFileSuperChunk::CountSubChunksOfType: known chunks of the code; counted copies balanced.
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
struct CChunkRef {
    CChunkedFileChunk* Object; SCountBlock* Count;
    CChunkRef(const CChunkPointer& o) : Object(o.Object), Count(o.Count) { if (Count) ++Count->m_references; }
    ~CChunkRef() { if (Count) --Count->m_references; }
    CChunkedFileChunk* operator->() const { return Object; }
};
struct CChunkedFileSuperChunk {
    CChunkArray SubChunks;
    long CountSubChunksOfType(const char* type) const
    {
        long count = 0;
        for (CChunkPointer* it = SubChunks.begin(); it != SubChunks.end(); ++it) {
            CChunkRef chunk(*it);
            if (!chunk->IsUnknownType() && std::strcmp(type, chunk->ChunkType) == 0) ++count;
        }
        return count;
    }
};
int main()
{
    CChunkedFileChunk a("MESH", 0, 0), b("MESH", 0, 1), c("MESH", 0, 0), d("ANIM", 0, 0);
    SCountBlock blk = { 1 };
    CChunkPointer items[4] = { { &a, &blk }, { &b, 0 }, { &c, 0 }, { &d, 0 } };
    CChunkedFileSuperChunk s; s.SubChunks.m_pFirst = items; s.SubChunks.m_pLast = items + 4;
    if (s.CountSubChunksOfType("MESH") != 2 || s.CountSubChunksOfType("ANIM") != 1 || s.CountSubChunksOfType("NONE") != 0 || blk.m_references != 1) {
        std::printf("00aa7ea0_TEST FAIL\n"); return 1;
    }
    std::printf("00aa7ea0_TEST PASS\n");
    return 0;
}
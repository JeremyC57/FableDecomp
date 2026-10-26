// Behaviour test for 0x00AA78C0 CChunkedFileSuperChunk::WriteToFile: [fourcc][size][body] per sub-chunk with the size back-patched.
#include <cstdio>
#include <cstring>
class CChunkedFileSuperChunk;
class CDataOutputStream { public:
    unsigned char buf[256]; long pos; long end;
    CDataOutputStream() : pos(0), end(0) { std::memset(buf, 0, sizeof(buf)); }
    void SetPosition(long p) { pos = p; }
    long GetPosition() { return pos; }
    void Write(const void* d, long n) { std::memcpy(buf + pos, d, n); pos += n; if (pos > end) end = pos; }
    void WriteLong_00993ea0(long v) { int x = (int)v; Write(&x, 4); }
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
    CChunkArray SubChunks;
    void WriteToFile(CDataOutputStream& stream) const
    {
        for (CChunkPointer* it = SubChunks.begin(); it != SubChunks.end(); ++it) {
            stream.Write(it->Object->ChunkType, 4);
            stream.WriteLong_00993ea0(0);
            long start = stream.GetPosition();
            it->Object->WriteToFile(stream);
            long finish = stream.GetPosition();
            stream.SetPosition(start - 4);
            stream.WriteLong_00993ea0(finish - start);
            stream.SetPosition(finish);
        }
    }
};
int main()
{
    CChunkedFileChunk a("ABCD", 3, 0), b("WXYZ", 1, 0);
    CChunkPointer items[2] = { { &a, 0 }, { &b, 0 } };
    CChunkedFileSuperChunk s; s.SubChunks.m_pFirst = items; s.SubChunks.m_pLast = items + 2; s.SubChunks.m_pEnd = items + 2;
    CDataOutputStream out;
    s.WriteToFile(out);
    int size0, size1; std::memcpy(&size0, out.buf + 4, 4); std::memcpy(&size1, out.buf + 15, 4);
    if (out.pos != 20 || std::memcmp(out.buf, "ABCD", 4) != 0 || size0 != 3 || out.buf[8] != 0xA0 || out.buf[10] != 0xA2 ||
        std::memcmp(out.buf + 11, "WXYZ", 4) != 0 || size1 != 1) { std::printf("00aa78c0_TEST FAIL\n"); return 1; }
    std::printf("00aa78c0_TEST PASS\n");
    return 0;
}
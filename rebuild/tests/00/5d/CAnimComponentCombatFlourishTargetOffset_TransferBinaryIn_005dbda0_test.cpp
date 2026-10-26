// Behaviour test for CAnimComponentCombatFlourishTargetOffset::TransferBinaryIn (0x005DBDA0).
#include <cstdio>
#include <cstring>

class C2DVector { public: float X, Y; };

class CMemoryDataInputStream { public:
    unsigned long m_position; const unsigned char* m_pData; long m_remaining; int overflowReads;
    const unsigned char* m_pNext;     // the next chunk, used by the overflow reader
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    { ++overflowReads; std::memcpy(buffer, m_pNext, size); m_pNext += size; m_position += size; }
    C2DVector ReadC2DVector() { C2DVector v; v.X = 1.5f; v.Y = -2.0f; return v; }
    static bool IsValidPosition(long long p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(m_position + size);
        if (!IsValidPosition(newPosition)) return;
        if (m_remaining >= size) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size; m_remaining -= size; m_position += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CAnimComponentCombatFlourishTargetOffset { public:
    bool TargetOffset; C2DVector TargetOffsetVector;
    void TransferBinaryIn(CMemoryDataInputStream& stream)
    {
        unsigned char targetOffset = 0;
        stream.Read(&targetOffset, 1);
        TargetOffset = targetOffset != 0;
        TargetOffsetVector = stream.ReadC2DVector();
    }
};

int main()
{
    unsigned char chunk[1] = { 7 }; unsigned char next[1] = { 0 };
    CMemoryDataInputStream s; s.m_position = 10; s.m_pData = chunk; s.m_remaining = 1; s.overflowReads = 0; s.m_pNext = next;
    CAnimComponentCombatFlourishTargetOffset c; c.TargetOffset = false;
    c.TransferBinaryIn(s);
    if (!c.TargetOffset || s.m_position != 11 || s.m_remaining != 0 || c.TargetOffsetVector.X != 1.5f) return std::printf("005dbda0_TEST FAIL\n"), 1;
    c.TransferBinaryIn(s);           // chunk exhausted: overflow reader returns 0
    if (c.TargetOffset || s.overflowReads != 1 || c.TargetOffsetVector.Y != -2.0f) return std::printf("005dbda0_TEST FAIL\n"), 1;
    std::printf("005dbda0_TEST PASS\n");
    return 0;
}
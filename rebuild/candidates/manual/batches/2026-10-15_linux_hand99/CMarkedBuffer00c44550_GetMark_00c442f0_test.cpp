#include <cstdio>
struct SBufferMark { void* m_pBuffer; void* m_pBlock; void* m_pCursor; long m_markStart; long m_markEnd; };
struct CMarkedBuffer_00c44550 {
    void* m_pBlock; void* m_pCursor; void* m_pBuffer; long m_reserved; long m_markStart; long m_markEnd;
    SBufferMark& GetMark(SBufferMark& mark)
    {
        mark.m_pBuffer = m_pBuffer;
        mark.m_markEnd = m_markEnd;
        mark.m_markStart = m_markStart;
        mark.m_pBlock = m_pBlock;
        mark.m_pCursor = m_pCursor;
        return mark;
    }
};
int main()
{
    char a, b, c;
    CMarkedBuffer_00c44550 buf; buf.m_pBlock = &a; buf.m_pCursor = &b; buf.m_pBuffer = &c; buf.m_reserved = 99; buf.m_markStart = 4; buf.m_markEnd = 17;
    SBufferMark m;
    SBufferMark& r = buf.GetMark(m);
    if (&r != &m || m.m_pBuffer != &c || m.m_pBlock != &a || m.m_pCursor != &b || m.m_markStart != 4 || m.m_markEnd != 17) { std::printf("00c442f0_TEST FAIL\n"); return 1; }
    std::printf("00c442f0_TEST PASS\n");
    return 0;
}

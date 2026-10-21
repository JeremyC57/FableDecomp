#include <cstdio>
#include <cstring>
struct C2DVector { float X, Y; C2DVector() {} };
struct CMemoryDataInputStream {
    void* m_pVtable;
    unsigned int m_position;
    int m_08;
    const unsigned char* m_pData;
    int m_10;
    int m_remaining;
    int m_overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size)
    {
        ++m_overflowCalls;
        std::memset(buffer, 0x11, size);
        m_position += size;
    }
    static bool IsValidPosition(long long position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        long long newPosition = (long long)(unsigned int)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    int ReadLong() { int v; Read(&v, sizeof(v)); return v; }
    C2DVector ReadC2DVector() { C2DVector v; Read(&v.X, 4); Read(&v.Y, 4); return v; }
};
struct CActionInputControl {
    int GameAction, ControllerType, KeyboardKey, XboxButton, MouseButton;
    C2DVector ControlDirection;
    void TransferBinaryIn(CMemoryDataInputStream& stream)
    {
        GameAction = stream.ReadLong();
        ControllerType = stream.ReadLong();
        KeyboardKey = stream.ReadLong();
        XboxButton = stream.ReadLong();
        MouseButton = stream.ReadLong();
        ControlDirection = stream.ReadC2DVector();
    }
};
int main()
{
    int words[7] = { 12, 1, 0x41, 3, 2, 0, 0 };
    float fx = 0.5f, fy = -1.0f;
    std::memcpy(&words[5], &fx, 4); std::memcpy(&words[6], &fy, 4);
    CMemoryDataInputStream s; std::memset(&s, 0, sizeof(s));
    s.m_pData = (const unsigned char*)words; s.m_remaining = sizeof(words);
    CActionInputControl c; std::memset(&c, 0, sizeof(c));
    c.TransferBinaryIn(s);
    if (c.GameAction != 12 || c.ControllerType != 1 || c.KeyboardKey != 0x41 || c.XboxButton != 3 || c.MouseButton != 2
        || c.ControlDirection.X != 0.5f || c.ControlDirection.Y != -1.0f || s.m_position != 28 || s.m_overflowCalls != 0) {
        std::printf("00410120_TEST FAIL\n"); return 1;
    }
    // short chunk: the 5th word and the vector come from the overflow reader
    CMemoryDataInputStream t; std::memset(&t, 0, sizeof(t));
    t.m_pData = (const unsigned char*)words; t.m_remaining = 16;
    c.TransferBinaryIn(t);
    if (c.XboxButton != 3 || c.MouseButton != 0x11111111 || t.m_overflowCalls != 3 || t.m_position != 28) {
        std::printf("00410120_TEST FAIL\n"); return 1;
    }
    std::printf("00410120_TEST PASS\n");
    return 0;
}

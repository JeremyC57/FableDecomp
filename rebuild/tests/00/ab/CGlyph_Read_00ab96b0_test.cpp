// behaviour test for CGlyph::Read (0x00AB96B0): four floats then three shorts, fast path and overflow path
#include <cstdio>
#include <cstring>
class CDataInputStream { public:
    unsigned long StreamPos; long StreamSize; const unsigned char* CurrentSourcePtr; long SourceChunkBytesRemaining;
    const unsigned char* Base; int OverflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++OverflowCalls;
        std::memcpy(buffer, Base + StreamPos, size); StreamPos += size; CurrentSourcePtr = Base + StreamPos;
    }
    void Read(void* dst, long size)
    {
        long long np = (long long)(StreamPos + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
};
class CGlyph { public:
    float Left, Top, Right, Bottom; short Offset, Width, Advance;
    void Read(CDataInputStream* in)
    {
        { float f; in->Read(&f, sizeof(f)); Left = f; }
        { float f; in->Read(&f, sizeof(f)); Top = f; }
        { float f; in->Read(&f, sizeof(f)); Right = f; }
        { float f; in->Read(&f, sizeof(f)); Bottom = f; }
        { short s; in->Read(&s, sizeof(s)); Offset = s; }
        { short s; in->Read(&s, sizeof(s)); Width = s; }
        { short s; in->Read(&s, sizeof(s)); Advance = s; }
    }
};
int main()
{
    unsigned char buf[22]; float fv[4] = { 0.25f, 0.5f, 0.75f, 1.0f }; short sv[3] = { -3, 12, 14 };
    std::memcpy(buf, fv, 16); std::memcpy(buf + 16, sv, 6);
    CDataInputStream in = { 0, 22, buf, 18, buf, 0 };      // chunk runs out before the last two shorts
    CGlyph g; g.Read(&in);
    if (g.Left != 0.25f || g.Top != 0.5f || g.Right != 0.75f || g.Bottom != 1.0f || g.Offset != -3 || g.Width != 12 || g.Advance != 14
        || in.StreamPos != 22 || in.OverflowCalls != 2) { std::printf("00ab96b0_TEST FAIL\n"); return 1; }
    std::printf("00ab96b0_TEST PASS\n");
    return 0;
}
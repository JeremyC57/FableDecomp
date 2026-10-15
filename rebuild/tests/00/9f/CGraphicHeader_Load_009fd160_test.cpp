// behaviour test for 0x009FD160 CGraphicHeader::Load
#include <cstdio>
#include <cstring>
struct CPixelFormat { int FormatIndex; CPixelFormat() {} };
struct CDataInputStream {
    unsigned int StreamPos; int StreamSize; unsigned char* CurrentSourcePtr; unsigned int SourceChunkStreamPos; int SourceChunkBytesRemaining;
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++overflowCalls; std::memset(buffer, 0xEE, size); StreamPos += size; }
    CPixelFormat ReadPixelFormat() { CPixelFormat f; f.FormatIndex = 42; return f; }
    static bool IsValidPosition(long long position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        long long newPosition = (long long)(StreamPos + size);
        if (!IsValidPosition(newPosition)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
struct CGraphicHeader {
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;
    CPixelFormat PixelFormat;
    unsigned char TransparencyType, MipmapLevels, IsFlatAnimatedGraphic;
    unsigned int FrameDataSize; unsigned int CompressedMipmapSizes[1];
    void Load(CDataInputStream& stream) { stream.Read(this, sizeof(CGraphicHeader)); PixelFormat = stream.ReadPixelFormat(); }
};
int main()
{
    CGraphicHeader src; std::memset(&src, 0, sizeof(src));
    src.Width = 256; src.Height = 128; src.FrameCount = 3; src.PixelFormat.FormatIndex = 7; src.FrameDataSize = 999;
    unsigned char buf[64]; std::memcpy(buf, &src, sizeof(src));
    CDataInputStream s = { 10, 100, buf, 0, 64, 0 };
    CGraphicHeader h; std::memset(&h, 0, sizeof(h));
    h.Load(s);
    int bad = h.Width != 256 || h.Height != 128 || h.FrameCount != 3 || h.FrameDataSize != 999 || h.PixelFormat.FormatIndex != 42
        || s.StreamPos != 10 + sizeof(CGraphicHeader) || s.CurrentSourcePtr != buf + sizeof(CGraphicHeader) || s.overflowCalls != 0;
    CDataInputStream t = { 0, 100, buf, 0, 4, 0 };
    h.Load(t);
    bad |= t.overflowCalls != 1 || h.Width != 0xEEEE || h.PixelFormat.FormatIndex != 42;
    if (bad) { std::printf("009fd160_TEST FAIL\n"); return 1; }
    std::printf("009fd160_TEST PASS\n");
    return 0;
}
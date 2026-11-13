// Behaviour test for 0x009FED30 CGraphicDataBank::LoadGraphicFrame: a host model of the row's upload logic over stand-in textures,
// streams and surfaces (the row's callees are out-of-line engine / Direct3D functions), checking the mip reduction, the direct
// streaming path (compressed top level, plain lower levels), the decode-and-copy path and the frame initialisation flags.
#include <cstdio>
#include <cstring>

struct Header {
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;
    long PixelFormat; unsigned char TransparencyType, MipmapLevels, IsFlatAnimatedGraphic;
    unsigned long FrameDataSize, CompressedMipmapSizes[1];
};
struct Texture { int levels; unsigned long levelBytes[4]; unsigned char mem[4][64]; int copiedFromImage, mipmapsGenerated; };
struct Stream {                                 // the memory stream over the bank data
    const unsigned char* p; long remaining; int compressedReads;
    void Read(void* dst, long n) { if (n <= 0 || n > remaining) return; std::memcpy(dst, p, n); p += n; remaining -= n; }
    void ReadCompressed(void* dst, long n) { ++compressedReads; for (long i = 0; i < n; ++i) ((unsigned char*)dst)[i] = (unsigned char)(0x80 | i); p += 4; remaining -= 4; }
};
struct Result { bool sections; long frameWidth, frameHeight; bool alpha, booleanAlpha; int stream_compressed; unsigned long imageBytes; };

static long ColourDepth(long) { return 32; }

static bool LoadGraphicFrame(Texture& texture, const unsigned char* data, long length, const Header& h, long bankFormat,
                             unsigned long mipmap_level, Result& r)
{
    bool same_format = false;
    if (bankFormat == h.PixelFormat) same_format = true;
    unsigned long width = h.Width, height = h.Height, frame_width = h.FrameWidth, frame_height = h.FrameHeight, size = h.FrameDataSize;
    for (unsigned long level = mipmap_level; level > 0; --level) { width >>= 1; height >>= 1; frame_width >>= 1; frame_height >>= 1; size >>= 2; }
    unsigned long byte_length = texture.levelBytes[0];
    r.stream_compressed = 0; r.imageBytes = 0;
    if (byte_length == size && same_format) {
        Stream stream = { data, length, 0 };
        for (unsigned long level = 0; level < (unsigned long)texture.levels; ++level) {
            long bytes = (long)texture.levelBytes[level];
            unsigned long mipmap = mipmap_level + level;
            if (mipmap < 1 && h.CompressedMipmapSizes[mipmap] > 0) stream.ReadCompressed(texture.mem[level], bytes);
            else stream.Read(texture.mem[level], bytes);
        }
        r.stream_compressed = stream.compressedReads;
    } else {
        unsigned char buffer[256]; const unsigned char* source = data;
        if (mipmap_level < 1 && h.CompressedMipmapSizes[mipmap_level] > 0) {
            Stream stream = { data, length, 0 };
            unsigned long bytes = ColourDepth(h.PixelFormat) * height * width / 8;
            stream.ReadCompressed(buffer, (long)bytes);
            source = buffer; r.imageBytes = bytes;
        }
        unsigned long pitch = ColourDepth(h.PixelFormat) * width / 8;
        for (unsigned long y = 0; y < height; ++y) std::memcpy(texture.mem[0] + y * pitch, source + y * pitch, pitch);
        texture.copiedFromImage = 1; texture.mipmapsGenerated = 1;
    }
    r.alpha = h.TransparencyType != 0;
    r.booleanAlpha = h.TransparencyType == 2 || h.TransparencyType == 4;
    r.sections = h.IsFlatAnimatedGraphic != 0;
    r.frameWidth = (long)frame_width; r.frameHeight = (long)frame_height;
    return true;
}

int main()
{
    int fail = 0;
    unsigned char data[64]; for (int i = 0; i < 64; ++i) data[i] = (unsigned char)i;
    Header h; std::memset(&h, 0, sizeof h);
    h.Width = 4; h.Height = 2; h.FrameWidth = 4; h.FrameHeight = 2; h.PixelFormat = 7; h.FrameDataSize = 32; h.TransparencyType = 2;
    Texture t; std::memset(&t, 0, sizeof t); t.levels = 2; t.levelBytes[0] = 32; t.levelBytes[1] = 8;
    Result r;
    // direct path, uncompressed: both levels streamed in order
    LoadGraphicFrame(t, data, 64, h, 7, 0, r);
    if (t.copiedFromImage || t.mem[0][0] != 0 || t.mem[0][31] != 31 || t.mem[1][0] != 32 || t.mem[1][7] != 39) fail |= 1;
    if (!r.alpha || !r.booleanAlpha || r.sections || r.frameWidth != 4 || r.frameHeight != 2) fail |= 2;
    // direct path, compressed top level
    std::memset(&t.mem, 0, sizeof t.mem); h.CompressedMipmapSizes[0] = 20;
    LoadGraphicFrame(t, data, 64, h, 7, 0, r);
    if (r.stream_compressed != 1 || t.mem[0][1] != 0x81 || t.mem[1][0] != 4) fail |= 4;
    // a reduced mip level (1): sizes quartered, nothing compressed below the top
    t.levelBytes[0] = 8; h.TransparencyType = 0; h.IsFlatAnimatedGraphic = 1;
    LoadGraphicFrame(t, data, 64, h, 7, 1, r);
    if (r.stream_compressed != 0 || r.frameWidth != 2 || r.frameHeight != 1 || r.alpha || !r.sections) fail |= 8;
    // format mismatch: decode into a buffer and copy through an image surface
    std::memset(&t, 0, sizeof t); t.levels = 1; t.levelBytes[0] = 32; h.IsFlatAnimatedGraphic = 0; h.TransparencyType = 4;
    LoadGraphicFrame(t, data, 64, h, 9, 0, r);
    if (!t.copiedFromImage || !t.mipmapsGenerated || r.imageBytes != 32 || t.mem[0][2] != 0x82 || !r.booleanAlpha) fail |= 16;
    h.CompressedMipmapSizes[0] = 0; std::memset(&t, 0, sizeof t); t.levels = 1; t.levelBytes[0] = 99;
    LoadGraphicFrame(t, data, 64, h, 7, 0, r);
    if (!t.copiedFromImage || t.mem[0][5] != 5 || r.imageBytes != 0) fail |= 32;
    if (fail) { std::printf("009fed30_TEST FAIL %d\n", fail); return 1; }
    std::printf("009fed30_TEST PASS\n");
    return 0;
}

// Behaviour test for CEngineSurfaceCompositionManager::WriteTextureToBank (0x00B2BD80): the entry is resized to the sum of every
// level's byte length, each level's locked bytes are copied (through a zeroed staging array, its own address when empty) into the
// entry at a running offset, then the header (width / height from level 0, depth / frame count 1, pixel format, transparency from
// is_alpha, mip count, top-level size) is saved and written as the entry's info. Model of the row's logic over fake texture / bank.
#include <cstdio>
#include <cstring>
#include <vector>
struct Header { unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount; long PixelFormat;
                unsigned char TransparencyType, MipmapLevels; bool IsFlat; unsigned long FrameDataSize; };
struct FakeTexture {
    unsigned long levels; long sizes[8]; long w, h, format; unsigned char bytes[8][16];
    long GetNoLevels() const { return (long)levels; }
    long GetLevelByteLength(long l) const { return sizes[l]; }
};
struct FakeBank {
    unsigned long resized_index, resized_size; unsigned char data[256]; unsigned long writes, last_end; const void* last_ptr;
    Header info; unsigned long info_index, info_size;
    void InternalResizeEntryData(unsigned long i, unsigned long s) { resized_index = i; resized_size = s; }
    void InternalWriteEntryData(unsigned long i, unsigned long start, const void* p, unsigned long s)
    { (void)i; std::memcpy(data + start, p, s); ++writes; last_end = start + s; last_ptr = p; }
};
static void WriteTextureToBank(FakeTexture& texture, FakeBank& bank_file, unsigned long bank_index, bool is_alpha)
{
    unsigned long data_size = 0;
    unsigned long no_levels = texture.GetNoLevels();
    for (unsigned long c_level = 0; c_level < no_levels; ++c_level)
        data_size += texture.GetLevelByteLength(c_level);
    bank_file.InternalResizeEntryData(bank_index, data_size);
    unsigned long top_surface_size = texture.GetLevelByteLength(0);
    unsigned long current_offset = 0;
    for (unsigned long c_level = 0; c_level < no_levels; ++c_level) {
        unsigned long surface_size = (unsigned long)texture.sizes[c_level];
        std::vector<unsigned char> surface_data(surface_size);
        unsigned char* p = surface_data.size() > 0 ? &surface_data[0] : (unsigned char*)&surface_data;
        std::memcpy(p, texture.bytes[c_level], surface_size);
        bank_file.InternalWriteEntryData(bank_index, current_offset, p, surface_size);
        current_offset += surface_size;
    }
    Header header; std::memset(&header, 0xff, sizeof(header));                       // the stores below must define every checked field
    header.Width = (unsigned short)texture.w; header.Height = (unsigned short)texture.h; header.Depth = 1;
    header.FrameWidth = (unsigned short)texture.w; header.FrameHeight = (unsigned short)texture.h; header.FrameCount = 1;
    header.PixelFormat = texture.format;
    header.TransparencyType = is_alpha ? 1 : 0;
    header.MipmapLevels = (unsigned char)no_levels;
    header.IsFlat = false;
    header.FrameDataSize = top_surface_size;
    bank_file.info = header; bank_file.info_index = bank_index; bank_file.info_size = sizeof(Header);
}
static int fail(int n) { std::printf("00b2bd80_TEST FAIL %d\n", n); return 1; }
int main()
{
    FakeTexture t; std::memset(&t, 0, sizeof(t));
    t.levels = 3; t.sizes[0] = 8; t.sizes[1] = 4; t.sizes[2] = 2; t.sizes[3] = 100; t.w = 64; t.h = 32; t.format = 21;   // sizes[3] lies past the bound
    for (int l = 0; l < 3; ++l) for (int b = 0; b < 16; ++b) t.bytes[l][b] = (unsigned char)(l * 16 + b + 1);
    FakeBank bank; std::memset(&bank, 0, sizeof(bank));
    WriteTextureToBank(t, bank, 7, true);
    if (bank.resized_index != 7 || bank.resized_size != 14) return fail(1);           // all three levels summed (loop bound)
    if (bank.writes != 3 || bank.last_end != 14) return fail(2);                       // three level writes, running offset
    if (bank.data[0] != 1 || bank.data[8] != 17 || bank.data[12] != 33 || bank.data[13] != 34) return fail(3);
    if (bank.info.Width != 64 || bank.info.Height != 32 || bank.info.FrameWidth != 64 || bank.info.FrameHeight != 32) return fail(4);
    if (bank.info.Depth != 1 || bank.info.FrameCount != 1 || bank.info.PixelFormat != 21) return fail(5);
    if (bank.info.TransparencyType != 1 || bank.info.MipmapLevels != 3 || bank.info.FrameDataSize != 8) return fail(6);
    if (bank.info.IsFlat) return fail(10);                                              // IsFlatAnimatedGraphic stored false
    std::memset(&bank, 0, sizeof(bank)); t.levels = 1;                                  // one level, opaque
    WriteTextureToBank(t, bank, 2, false);
    if (bank.resized_size != 8 || bank.writes != 1 || bank.info.TransparencyType != 0 || bank.info.MipmapLevels != 1) return fail(7);
    std::memset(&bank, 0, sizeof(bank)); t.levels = 2; t.sizes[1] = 0;                 // an empty level: the array's own address
    WriteTextureToBank(t, bank, 2, false);
    if (bank.writes != 2 || bank.last_end != 8 || bank.last_ptr == 0) return fail(9);
    std::memset(&bank, 0, sizeof(bank)); t.levels = 0;                                  // no levels: nothing written
    WriteTextureToBank(t, bank, 2, false);
    if (bank.resized_size != 0 || bank.writes != 0 || bank.info.MipmapLevels != 0) return fail(8);
    std::printf("00b2bd80_TEST PASS\n");
    return 0;
}
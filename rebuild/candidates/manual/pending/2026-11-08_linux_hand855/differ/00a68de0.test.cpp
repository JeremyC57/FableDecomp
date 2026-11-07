// Behaviour model of CVolumeTexture::LoadFromDataStream (0x00A68DE0): read the 21-byte file header, initialise the texture it
// describes, then per mip level lock the box and either read the whole compressed level (GetLevelByteLength bytes) or, for an
// uncompressed level, give up when the row size or level size overflows 32 bits, read Y*Z rows of depth*X bytes into the locked
// memory and walk them row by row (row pitch, then slice pitch) into a packed buffer; every level is unlocked.
#include <cstdio>
#include <cstring>
#include <vector>
struct Stream { std::vector<unsigned char> Data; size_t Pos; void Read(void* p, size_t n) { std::memcpy(p, &Data[Pos], n); Pos += n; } };
struct Level { long X, Y, Z; long RowPitch, SlicePitch; std::vector<unsigned char> Mem; };
struct Texture { bool Compressed; long Depth; std::vector<Level> Levels; int Unlocked; };
static bool Load(Texture& t, Stream& s)
{
    unsigned char header[21];
    s.Read(header, sizeof(header));
    for (size_t level = 0; level < t.Levels.size(); ++level) {
        Level& box = t.Levels[level];
        if (t.Compressed) {
            s.Read(&box.Mem[0], box.Mem.size());
        } else {
            unsigned long long row = (unsigned long long)t.Depth * box.X;
            if (row > 0xffffffffULL) return false;
            if ((unsigned long long)box.Y * box.Z * row > 0xffffffffULL) return false;
            unsigned long size = (unsigned long)(box.Y * box.Z * row);
            std::vector<unsigned char> buffer(size + box.Y * box.Z * box.RowPitch + box.Z * box.SlicePitch);
            s.Read(&box.Mem[0], size);
            unsigned char* dst = &buffer[0]; const unsigned char* src = &box.Mem[0];
            for (long z = box.Z; z > 0; --z) {
                for (long y = box.Y; y > 0; --y) { std::memcpy(dst, src, (size_t)row); src += row; dst += box.RowPitch; }
                dst += box.SlicePitch;
            }
        }
        ++t.Unlocked;
    }
    return true;
}
int main()
{
    Texture t; t.Compressed = false; t.Depth = 4; t.Unlocked = 0;
    Level l0 = { 2, 2, 2, 8, 0, std::vector<unsigned char>(32) }, l1 = { 1, 1, 1, 4, 0, std::vector<unsigned char>(4) };
    t.Levels.push_back(l0); t.Levels.push_back(l1);
    Stream s; s.Pos = 0;
    for (int i = 0; i < 21 + 32 + 4; ++i) s.Data.push_back((unsigned char)i);
    if (!Load(t, s) || s.Pos != 57 || t.Unlocked != 2) { std::printf("00a68de0_TEST FAIL uncompressed\n"); return 1; }
    if (t.Levels[0].Mem[0] != 21 || t.Levels[1].Mem[3] != 56) { std::printf("00a68de0_TEST FAIL data\n"); return 1; }
    Texture c = t; c.Compressed = true; c.Unlocked = 0; s.Pos = 0;
    if (!Load(c, s) || s.Pos != 57 || c.Unlocked != 2) { std::printf("00a68de0_TEST FAIL compressed\n"); return 1; }
    std::printf("00a68de0_TEST PASS\n");
    return 0;
}

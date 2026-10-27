#include <cstdio>
#include <cstring>
// Behaviour model: count, then (when non-zero) the compressed size and block; a failed resize stops before the block.
struct Stream { const unsigned char* p; long left; bool read(void* d, long n) { if (n <= 0 || n > left) return false; std::memcpy(d, p, n); p += n; left -= n; return true; } };
static int resized = -1, decompressed = 0;
static bool load(Stream& s, bool resizeOk)
{
    long count = 0; s.read(&count, 4);
    resized = (int)count;
    if (!resizeOk) return false;
    if (count > 0) { long size = 0; s.read(&size, 4); unsigned char buf[16]; if (s.read(buf, size)) decompressed = size; }
    return true;
}
int main()
{
    unsigned char data[16] = { 3, 0, 0, 0, 2, 0, 0, 0, 0xaa, 0xbb };
    Stream s = { data, 10 };
    if (!load(s, true) || resized != 3 || decompressed != 2 || s.left != 0) { std::printf("00bfcbc0_TEST FAIL\n"); return 1; }
    Stream t = { data, 10 }; decompressed = 0;
    if (load(t, false) || decompressed != 0) { std::printf("00bfcbc0_TEST FAIL\n"); return 1; }
    std::printf("00bfcbc0_TEST PASS\n");
    return 0;
}
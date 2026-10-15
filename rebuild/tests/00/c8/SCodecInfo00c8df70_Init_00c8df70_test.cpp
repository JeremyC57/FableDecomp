#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SCodecInfo_00c8df70 {
    long version, channels, rate, bitrate_upper, bitrate_nominal, bitrate_lower, bitrate_window;
    void* codec_setup;
    void Init() { std::memset(this, 0, sizeof(*this)); codec_setup = std::calloc(1, 0xE80); }
};
int main()
{
    SCodecInfo_00c8df70 vi;
    std::memset(&vi, 0x5A, sizeof(vi));
    vi.Init();
    bool ok = vi.version == 0 && vi.channels == 0 && vi.rate == 0 && vi.bitrate_upper == 0 && vi.bitrate_nominal == 0
        && vi.bitrate_lower == 0 && vi.bitrate_window == 0 && vi.codec_setup != 0;
    if (ok) {
        const unsigned char* p = (const unsigned char*)vi.codec_setup;
        for (int i = 0; i < 0xE80; ++i) if (p[i]) { ok = false; break; }
    }
    std::free(vi.codec_setup);
    if (!ok) { std::printf("00c8df70_TEST FAIL\n"); return 1; }
    std::printf("00c8df70_TEST PASS\n");
    return 0;
}
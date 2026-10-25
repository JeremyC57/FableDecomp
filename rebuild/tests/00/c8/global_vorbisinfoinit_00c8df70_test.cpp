// Behaviour test for 00c8df70: vorbis_info_init -- zeroes the info and allocates a zeroed 0xE80-byte codec setup
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct vorbis_info { int version; int channels; long rate; long bitrate_upper; long bitrate_nominal; long bitrate_lower; long bitrate_window; void* codec_setup; };
static void vorbis_info_init(vorbis_info* vi) { std::memset(vi, 0, sizeof(*vi)); vi->codec_setup = std::calloc(1, 0xE80); }
int main()
{
    vorbis_info vi; std::memset(&vi, 0x55, sizeof vi); vorbis_info_init(&vi);
    int ok = vi.version == 0 && vi.channels == 0 && vi.rate == 0 && vi.bitrate_window == 0 && vi.codec_setup != 0 && ((unsigned char*)vi.codec_setup)[0xE7F] == 0 && sizeof(vorbis_info) == 0x20;
    std::free(vi.codec_setup);
    std::printf(ok ? "00c8df70_TEST PASS\n" : "00c8df70_TEST FAIL\n");
    return ok ? 0 : 1;
}

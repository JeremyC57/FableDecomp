// Behaviour test for 00c8df60: vorbis_info_blocksize (Xiph libvorbis info.c)
#include <cstdio>
#include <cstring>
struct vorbis_info {                    // 0x20 bytes
    int   version;                      // +0x00
    int   channels;                     // +0x04
    long  rate;                         // +0x08
    long  bitrate_upper;                // +0x0c
    long  bitrate_nominal;              // +0x10
    long  bitrate_lower;                // +0x14
    long  bitrate_window;               // +0x18
    void* codec_setup;                  // +0x1c (codec_setup_info, 0xE80 bytes)
};
struct codec_setup_info {               // 0xE80 bytes; only its head is read here
    long blocksizes[2];                 // +0x00
};

static int vorbis_info_blocksize(vorbis_info* vi, int zo)
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    return ci ? ci->blocksizes[zo] : -1;
}

int main()
{
    int ok;
    codec_setup_info ci = {{256, 2048}}; vorbis_info vi = {0, 2, 44100, 0, 0, 0, 0, &ci};
    ok = vorbis_info_blocksize(&vi, 0) == 256 && vorbis_info_blocksize(&vi, 1) == 2048;
    vi.codec_setup = 0; ok = ok && vorbis_info_blocksize(&vi, 1) == -1;
    std::printf(ok ? "00c8df60_TEST PASS\n" : "00c8df60_TEST FAIL\n");
    return ok ? 0 : 1;
}

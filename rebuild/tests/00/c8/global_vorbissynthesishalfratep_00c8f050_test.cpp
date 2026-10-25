// Behaviour test for 00c8f050: vorbis_synthesis_halfrate_p (Xiph libvorbis synthesis.c)
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
struct codec_setup_info {               // 0xE80 bytes
    unsigned char _head[0xe78];
    int           halfrate_flag;        // +0xE78
    unsigned char _tail[0xe80 - 0xe7c];
};

static int vorbis_synthesis_halfrate_p(vorbis_info* vi)
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    return ci->halfrate_flag;
}

int main()
{
    int ok;
    static codec_setup_info ci; ci.halfrate_flag = 1; vorbis_info vi = {0, 2, 44100, 0, 0, 0, 0, &ci};
    ok = vorbis_synthesis_halfrate_p(&vi) == 1 && sizeof(codec_setup_info) == 0xE80;
    std::printf(ok ? "00c8f050_TEST PASS\n" : "00c8f050_TEST FAIL\n");
    return ok ? 0 : 1;
}

// Behaviour test for 00c8b9b0: ov_pcm_tell (Xiph vorbisfile.c)
#include <cstdio>
#include <cstring>
typedef __int64 ogg_int64_t;
struct vorbis_info;
struct vorbis_comment;
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;
    int storage, fill, returned, unsynced, headerbytes, bodybytes;
};
struct OggVorbis_File {                 // libpk's fork: no ov_callbacks member
    void*           datasource;         // +0x00
    int             seekable;           // +0x04
    ogg_int64_t     offset;             // +0x08
    ogg_int64_t     end;                // +0x10
    ogg_sync_state  oy;                 // +0x18
    int             links;              // +0x34
    ogg_int64_t*    offsets;            // +0x38
    ogg_int64_t*    dataoffsets;        // +0x3c
    long*           serialnos;          // +0x40
    ogg_int64_t*    pcmlengths;         // +0x44
    vorbis_info*    vi;                 // +0x48
    vorbis_comment* vc;                 // +0x4c
    ogg_int64_t     pcm_offset;         // +0x50
    int             ready_state;        // +0x58
    long            current_serialno;   // +0x5c
    int             current_link;       // +0x60
    double          bittrack;           // +0x68
    double          samptrack;          // +0x70
};

#define OV_EINVAL -131
#define OPENED    2

static ogg_int64_t ov_pcm_tell(OggVorbis_File* vf)
{
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    return (vf->pcm_offset);
}

int main()
{
    int ok;
    static OggVorbis_File vf; vf.ready_state = 1; vf.pcm_offset = 0x100000005LL;
    ok = ov_pcm_tell(&vf) == -131; vf.ready_state = 4; ok = ok && ov_pcm_tell(&vf) == 0x100000005LL;
    std::printf(ok ? "00c8b9b0_TEST PASS\n" : "00c8b9b0_TEST FAIL\n");
    return ok ? 0 : 1;
}

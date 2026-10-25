// Behaviour test for 00c8a7e0: ov_clear -- clear block/dsp/stream, each link's info+comment, free the link tables, clear sync, zero the file, return 0
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
typedef struct { int dummy; } vorbis_block;
typedef struct { int dummy; } vorbis_dsp_state;
typedef struct { int dummy; } ogg_stream_state;
typedef struct { int dummy; } ogg_sync_state;
typedef struct vorbis_info { int version; int channels; long rate; long pad[5]; } vorbis_info;
typedef struct vorbis_comment { char** user_comments; int* comment_lengths; int comments; char* vendor; } vorbis_comment;
typedef struct OggVorbis_File {
    void* datasource; int seekable; ogg_sync_state oy; int links;
    ogg_int64_t* offsets; ogg_int64_t* dataoffsets; long* serialnos; ogg_int64_t* pcmlengths;
    vorbis_info* vi; vorbis_comment* vc;
    ogg_stream_state os; vorbis_dsp_state vd; vorbis_block vb;
} OggVorbis_File;

static int g_block, g_dsp, g_stream, g_info, g_comment, g_sync, g_frees;
static OggVorbis_File* g_vf;
int vorbis_block_clear(vorbis_block* vb) { if (vb == &g_vf->vb) ++g_block; return 0; }
void vorbis_dsp_clear(vorbis_dsp_state* v) { if (v == &g_vf->vd) ++g_dsp; }
int ogg_stream_clear(ogg_stream_state* os) { if (os == &g_vf->os) ++g_stream; return 0; }
void vorbis_info_clear(vorbis_info* vi) { vi->channels = -1; ++g_info; }
void vorbis_comment_clear(vorbis_comment* vc) { vc->comments = -1; ++g_comment; }
int ogg_sync_clear(ogg_sync_state* oy) { if (oy == &g_vf->oy) ++g_sync; return 0; }
static void test_free(void* p) { ++g_frees; std::free(p); }

int ov_clear(OggVorbis_File* vf)
{
    if (vf) {
        vorbis_block_clear(&vf->vb);
        vorbis_dsp_clear(&vf->vd);
        ogg_stream_clear(&vf->os);
        if (vf->vi && vf->links) {
            int i;
            for (i = 0; i < vf->links; i++) {
                vorbis_info_clear(vf->vi + i);
                vorbis_comment_clear(vf->vc + i);
            }
            test_free(vf->vi);
            test_free(vf->vc);
        }
        if (vf->dataoffsets) test_free(vf->dataoffsets);
        if (vf->pcmlengths) test_free(vf->pcmlengths);
        if (vf->serialnos) test_free(vf->serialnos);
        if (vf->offsets) test_free(vf->offsets);
        ogg_sync_clear(&vf->oy);
        std::memset(vf, 0, sizeof(*vf));
    }
    return 0;
}

int main()
{
    OggVorbis_File vf;
    std::memset(&vf, 0x11, sizeof(vf));
    g_vf = &vf;
    vf.links = 3;
    vf.vi = (vorbis_info*)std::calloc(3, sizeof(vorbis_info));
    vf.vc = (vorbis_comment*)std::calloc(3, sizeof(vorbis_comment));
    vf.offsets = (ogg_int64_t*)std::malloc(32);
    vf.dataoffsets = 0;
    vf.serialnos = (long*)std::malloc(16);
    vf.pcmlengths = (ogg_int64_t*)std::malloc(64);
    int ok = ov_clear(&vf) == 0 && g_block == 1 && g_dsp == 1 && g_stream == 1 && g_info == 3 && g_comment == 3
        && g_sync == 1 && g_frees == 5 && vf.links == 0 && vf.vi == 0 && vf.datasource == 0;
    OggVorbis_File empty;
    std::memset(&empty, 0, sizeof(empty));
    g_vf = &empty;
    ok = ok && ov_clear(&empty) == 0 && g_info == 3 && g_frees == 5 && g_sync == 2;
    ok = ok && ov_clear(0) == 0 && g_block == 2;
    if (!ok) { std::printf("00c8a7e0_TEST FAIL\n"); return 1; }
    std::printf("00c8a7e0_TEST PASS\n");
    return 0;
}
// Behaviour test for libvorbis ov_raw_seek (0x00C8AB70): the 1.0.1 vorbisfile.c body over mocked libogg/libvorbis calls --
// argument checks, then the scratch-stream scan that rebuilds pcm_offset (granulepos less the link start, plus the earlier
// links' lengths, less the accumulated half-block overlap of the packets before it).
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
#include <stdint.h>
typedef int64_t ogg_int64_t;
#endif
#define OV_EINVAL -131
#define OV_ENOSEEK -138
#define OV_EBADLINK -137
#define OPENED 2
#define STREAMSET 3
#define NULL0 0
struct ogg_packet { long blocksize; ogg_int64_t granulepos; };
struct ogg_page { int idx; };
struct ogg_stream_state { int serialno; ogg_packet q[8]; int n, head; };
struct vorbis_info { void* codec_setup; };
struct OggVorbis_File {
    int seekable; ogg_int64_t offset, end; int links; long* serialnos; ogg_int64_t* pcmlengths; vorbis_info* vi;
    ogg_int64_t pcm_offset; int ready_state; long current_serialno; int current_link; double bittrack, samptrack;
    ogg_stream_state os; int vd_restarts; int vf_os_packetouts;
};
// one page with three packets (blocksizes 256, 256, 1024; only the last carries a granulepos)
static ogg_packet g_page[3] = { { 256, -1 }, { 256, -1 }, { 1024, 600 } };
static int g_pages_left = 1, g_cleared = 0;
static void ogg_stream_reset_serialno(ogg_stream_state* os, int s) { os->serialno = s; os->n = os->head = 0; }
static void vorbis_synthesis_restart(OggVorbis_File* vf) { vf->vd_restarts++; }
static void _seek_helper(OggVorbis_File* vf, ogg_int64_t pos) { vf->offset = pos; }
static void ogg_stream_init(ogg_stream_state* os, int s) { std::memset(os, 0, sizeof(*os)); os->serialno = s; }
static void ogg_stream_reset(ogg_stream_state* os) { os->n = os->head = 0; }
static OggVorbis_File* g_vf;
static int ogg_stream_packetout(ogg_stream_state* os, ogg_packet* op)
{
    if (os == &g_vf->os) { g_vf->vf_os_packetouts++; return 1; }
    if (os->head >= os->n) return 0;
    *op = os->q[os->head++]; return 1;
}
static long vorbis_packet_blocksize(vorbis_info*, ogg_packet* op) { return op->blocksize; }
static ogg_int64_t _get_next_page(OggVorbis_File*, ogg_page* og, ogg_int64_t) { if (!g_pages_left) return -1; g_pages_left--; og->idx = 0; return 0; }
static ogg_int64_t ov_pcm_total(OggVorbis_File*, int) { return 12345; }
static int ogg_page_serialno(ogg_page*) { return 7; }
static void _decode_clear(OggVorbis_File* vf) { vf->ready_state = OPENED; }
static void ogg_stream_clear(ogg_stream_state*) { g_cleared++; }
static void ogg_stream_pagein(ogg_stream_state* os, ogg_page*) { if (os != &g_vf->os) { std::memcpy(os->q, g_page, sizeof(g_page)); os->n = 3; os->head = 0; } }
static int ogg_page_eos(ogg_page*) { return 0; }

static int ov_raw_seek(OggVorbis_File* vf, ogg_int64_t pos)
{
    ogg_stream_state work_os;
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (!vf->seekable) return (OV_ENOSEEK);
    if (pos < 0 || pos > vf->end) return (OV_EINVAL);
    vf->pcm_offset = -1;
    ogg_stream_reset_serialno(&vf->os, vf->current_serialno);
    vorbis_synthesis_restart(vf);
    _seek_helper(vf, pos);
    {
        ogg_page og; ogg_packet op;
        int lastblock = 0, accblock = 0, thisblock, eosflag = 0;
        ogg_stream_init(&work_os, vf->current_serialno);
        ogg_stream_reset(&work_os);
        while (1) {
            if (vf->ready_state >= STREAMSET) {
                int result = ogg_stream_packetout(&work_os, &op);
                if (result > 0) {
                    if (vf->vi[vf->current_link].codec_setup) {
                        thisblock = vorbis_packet_blocksize(vf->vi + vf->current_link, &op);
                        if (thisblock < 0) { ogg_stream_packetout(&vf->os, NULL0); thisblock = 0; }
                        else { if (eosflag) ogg_stream_packetout(&vf->os, NULL0); else if (lastblock) accblock += (lastblock + thisblock) >> 2; }
                        if (op.granulepos != -1) {
                            int i, link = vf->current_link;
                            ogg_int64_t granulepos = op.granulepos - vf->pcmlengths[link * 2];
                            if (granulepos < 0) granulepos = 0;
                            for (i = 0; i < link; i++) granulepos += vf->pcmlengths[i * 2 + 1];
                            vf->pcm_offset = granulepos - accblock;
                            break;
                        }
                        lastblock = thisblock;
                        continue;
                    } else ogg_stream_packetout(&vf->os, NULL0);
                }
            }
            if (!lastblock) {
                if (_get_next_page(vf, &og, -1) < 0) { vf->pcm_offset = ov_pcm_total(vf, -1); break; }
            } else { vf->pcm_offset = -1; break; }
            if (vf->ready_state >= STREAMSET)
                if (vf->current_serialno != ogg_page_serialno(&og)) { _decode_clear(vf); ogg_stream_clear(&work_os); }
            if (vf->ready_state < STREAMSET) {
                int link;
                vf->current_serialno = ogg_page_serialno(&og);
                for (link = 0; link < vf->links; link++) if (vf->serialnos[link] == vf->current_serialno) break;
                if (link == vf->links) goto seek_error;
                vf->current_link = link;
                ogg_stream_reset_serialno(&vf->os, vf->current_serialno);
                ogg_stream_reset_serialno(&work_os, vf->current_serialno);
                vf->ready_state = STREAMSET;
            }
            ogg_stream_pagein(&vf->os, &og);
            ogg_stream_pagein(&work_os, &og);
            eosflag = ogg_page_eos(&og);
        }
    }
    ogg_stream_clear(&work_os);
    vf->bittrack = 0.f; vf->samptrack = 0.f;
    return (0);
seek_error:
    vf->pcm_offset = -1;
    ogg_stream_clear(&work_os);
    _decode_clear(vf);
    return OV_EBADLINK;
}

int main()
{
    static OggVorbis_File vf; std::memset(&vf, 0, sizeof(vf)); g_vf = &vf;
    long serials[2] = { 5, 7 };
    ogg_int64_t pl[4] = { 0, 100, 10, 200 };
    int cs = 1; vorbis_info vi[2] = { { &cs }, { &cs } };
    vf.seekable = 1; vf.end = 1000; vf.links = 2; vf.serialnos = serials; vf.pcmlengths = pl; vf.vi = vi;
    vf.ready_state = STREAMSET; vf.current_serialno = 7; vf.current_link = 1; vf.bittrack = 3; vf.samptrack = 4;
    int bad = 0;
    if (ov_raw_seek(&vf, -1) != OV_EINVAL) bad = 1;
    if (ov_raw_seek(&vf, 1001) != OV_EINVAL) bad = 2;
    vf.seekable = 0; if (ov_raw_seek(&vf, 10) != OV_ENOSEEK) bad = 3; vf.seekable = 1;
    vf.ready_state = 1; if (ov_raw_seek(&vf, 10) != OV_EINVAL) bad = 4; vf.ready_state = STREAMSET;
    // granulepos 600 - link start 10 + link0 length 100 - overlap ((256+256)>>2 + (256+1024)>>2 = 448) = 242
    if (ov_raw_seek(&vf, 500) != 0 || vf.pcm_offset != 242 || vf.offset != 500 || vf.bittrack != 0 || vf.samptrack != 0) bad = 5;
    if (vf.vd_restarts != 1 || g_cleared != 1) bad = 6;
    // no pages at all: pcm_offset = ov_pcm_total(vf,-1)
    g_pages_left = 0;
    if (ov_raw_seek(&vf, 0) != 0 || vf.pcm_offset != 12345) bad = 7;
    if (bad) { std::printf("00c8ab70_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c8ab70_TEST PASS\n"); return 0;
}
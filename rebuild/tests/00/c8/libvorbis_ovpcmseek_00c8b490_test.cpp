// Behaviour test for libvorbis ov_pcm_seek (0x00C8B490): the 1.0.1 vorbisfile.c body over mocked calls -- after the page
// seek, leading packets are consumed track-only while their lapping window ends before pos (pcm_offset advanced by the
// half-block overlaps, then reset from a packet granulepos), and the remaining samples are decoded and discarded.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
#include <stdint.h>
typedef int64_t ogg_int64_t;
#endif
#define OV_HOLE -3
#define OV_EBADLINK -137
#define STREAMSET 3
#define NULL0 0
struct ogg_packet { long blocksize; ogg_int64_t granulepos; };
struct ogg_page { int x; };
struct ogg_stream_state { int head; };
struct vorbis_info { int x; };
struct OggVorbis_File {
    int links; long* serialnos; ogg_int64_t* pcmlengths; vorbis_info* vi; ogg_int64_t pcm_offset; int ready_state;
    long current_serialno; int current_link; double bittrack, samptrack; ogg_stream_state os;
    int tracked, blockins, avail, fetches, reads;
};
static ogg_packet g_q[3] = { { 256, -1 }, { 256, -1 }, { 2048, 4000 } };
static int ov_pcm_seek_page(OggVorbis_File* vf, ogg_int64_t) { vf->pcm_offset = 1000; return 0; }
static int _make_decode_ready(OggVorbis_File*) { return 0; }
static int ogg_stream_packetpeek(ogg_stream_state* os, ogg_packet* op) { if (os->head >= 3) return 0; *op = g_q[os->head]; return 1; }
static int ogg_stream_packetout(ogg_stream_state* os, ogg_packet*) { os->head++; return 1; }
static long vorbis_packet_blocksize(vorbis_info*, ogg_packet* op) { return op->blocksize; }
static long vorbis_info_blocksize(vorbis_info*, int) { return 2048; }
static void vorbis_synthesis_trackonly(OggVorbis_File* vf, ogg_packet*) { vf->tracked++; }
static void vorbis_synthesis_blockin(OggVorbis_File* vf) { vf->blockins++; }
static ogg_int64_t _get_next_page(OggVorbis_File*, ogg_page*, ogg_int64_t) { return -1; }
static int ogg_page_serialno(ogg_page*) { return 0; }
static void _decode_clear(OggVorbis_File*) { }
static void ogg_stream_reset_serialno(ogg_stream_state*, long) { }
static void ogg_stream_pagein(ogg_stream_state*, ogg_page*) { }
static long vorbis_synthesis_pcmout(OggVorbis_File* vf) { return vf->avail; }
static void vorbis_synthesis_read(OggVorbis_File* vf, long n) { vf->avail -= n; vf->reads++; }
static int _fetch_and_process_packet(OggVorbis_File* vf) { vf->fetches++; vf->avail += 400; return 1; }
static ogg_int64_t ov_pcm_total(OggVorbis_File*, int) { return 77; }

static int ov_pcm_seek(OggVorbis_File* vf, ogg_int64_t pos)
{
    int thisblock, lastblock = 0;
    int ret = ov_pcm_seek_page(vf, pos);
    if (ret < 0) return (ret);
    if ((ret = _make_decode_ready(vf))) return ret;
    while (1) {
        ogg_packet op; ogg_page og;
        int ret = ogg_stream_packetpeek(&vf->os, &op);
        if (ret > 0) {
            thisblock = vorbis_packet_blocksize(vf->vi + vf->current_link, &op);
            if (thisblock < 0) { ogg_stream_packetout(&vf->os, NULL0); continue; }
            if (lastblock) vf->pcm_offset += (lastblock + thisblock) >> 2;
            if (vf->pcm_offset + ((thisblock + vorbis_info_blocksize(vf->vi, 1)) >> 2) >= pos) break;
            ogg_stream_packetout(&vf->os, NULL0);
            vorbis_synthesis_trackonly(vf, &op);
            vorbis_synthesis_blockin(vf);
            if (op.granulepos > -1) {
                int i;
                vf->pcm_offset = op.granulepos - vf->pcmlengths[vf->current_link * 2];
                if (vf->pcm_offset < 0) vf->pcm_offset = 0;
                for (i = 0; i < vf->current_link; i++) vf->pcm_offset += vf->pcmlengths[i * 2 + 1];
            }
            lastblock = thisblock;
        } else {
            if (ret < 0 && ret != OV_HOLE) break;
            if (_get_next_page(vf, &og, -1) < 0) break;
            if (vf->current_serialno != ogg_page_serialno(&og)) _decode_clear(vf);
            if (vf->ready_state < STREAMSET) {
                int link;
                vf->current_serialno = ogg_page_serialno(&og);
                for (link = 0; link < vf->links; link++) if (vf->serialnos[link] == vf->current_serialno) break;
                if (link == vf->links) return (OV_EBADLINK);
                vf->current_link = link;
                ogg_stream_reset_serialno(&vf->os, vf->current_serialno);
                vf->ready_state = STREAMSET;
                ret = _make_decode_ready(vf);
                if (ret) return ret;
                lastblock = 0;
            }
            ogg_stream_pagein(&vf->os, &og);
        }
    }
    vf->bittrack = 0.f; vf->samptrack = 0.f;
    while (vf->pcm_offset < pos) {
        ogg_int64_t target = pos - vf->pcm_offset;
        long samples = vorbis_synthesis_pcmout(vf);
        if (samples > target) samples = (long)target;
        vorbis_synthesis_read(vf, samples);
        vf->pcm_offset += samples;
        if (samples < target)
            if (_fetch_and_process_packet(vf) <= 0) vf->pcm_offset = ov_pcm_total(vf, -1);
    }
    return 0;
}

int main()
{
    int bad = 0;
    {   // pos 1500: the first packet's window (1000 + (256+2048)/4 = 1576) already reaches pos -> nothing consumed track-only;
        // the 500 samples are then decoded and dropped (nothing buffered: two packet fetches of 400)
        OggVorbis_File vf; std::memset(&vf, 0, sizeof(vf)); ogg_int64_t pl[2] = { 0, 9000 }; vorbis_info vi; vf.vi = &vi; vf.pcmlengths = pl;
        vf.ready_state = 4; vf.bittrack = 5;
        if (ov_pcm_seek(&vf, 1500) != 0 || vf.pcm_offset != 1500 || vf.tracked != 0 || vf.bittrack != 0 || vf.fetches != 2) bad = 1;
    }
    {   // pos 4500: all three packets consumed track-only, the granule 4000 packet resets pcm_offset, then 500 samples
        // are decoded and dropped (300 buffered, one more packet adds 400, 200 taken)
        OggVorbis_File vf; std::memset(&vf, 0, sizeof(vf)); ogg_int64_t pl[2] = { 0, 9000 }; vorbis_info vi; vf.vi = &vi; vf.pcmlengths = pl;
        vf.ready_state = 4; vf.avail = 300;
        if (ov_pcm_seek(&vf, 4500) != 0 || vf.pcm_offset != 4500) bad = 2;
        if (vf.tracked != 3 || vf.blockins != 3 || vf.fetches != 1 || vf.reads != 2 || vf.avail != 200) bad = 3;
    }
    if (bad) { std::printf("00c8b490_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c8b490_TEST PASS\n"); return 0;
}
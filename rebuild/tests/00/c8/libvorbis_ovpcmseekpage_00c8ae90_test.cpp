// Behaviour test for libvorbis ov_pcm_seek_page (0x00C8AE90): the 1.0.1 vorbisfile.c body over a simulated four-page link
// -- argument checks, the link search, HB's bisection settling on the last granule-marked page before the target, then the
// packet peek that sets pcm_offset from that page's granulepos.
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
#define OV_EOF -2
#define OV_EREAD -128
#define OV_EFAULT -129
#define OV_EBADPACKET -136
#define OPENED 2
#define STREAMSET 3
#define CHUNKSIZE 8500
#define NULL0 0
struct Page { ogg_int64_t off; long len; ogg_int64_t gp; };
static Page g_pages[4] = { { 0, 10, -1 }, { 10, 30, 1000 }, { 40, 30, 2000 }, { 70, 30, 3000 } };
struct ogg_page { int idx; };
struct ogg_packet { ogg_int64_t granulepos; };
struct ogg_stream_state { long serialno; int peeked; };
struct OggVorbis_File {
    int seekable; ogg_int64_t offset; int links; ogg_int64_t* offsets; ogg_int64_t* pcmlengths; ogg_int64_t pcm_offset;
    int ready_state; long current_serialno; int current_link; double bittrack, samptrack; ogg_stream_state os; int restarts;
};
static void _seek_helper(OggVorbis_File* vf, ogg_int64_t pos) { vf->offset = pos; }
static ogg_int64_t _get_next_page(OggVorbis_File* vf, ogg_page* og, ogg_int64_t boundary)
{
    for (int i = 0; i < 4; i++)
        if (g_pages[i].off >= vf->offset) {
            if (boundary > 0 && g_pages[i].off >= vf->offset + boundary) return -1;
            og->idx = i; vf->offset = g_pages[i].off + g_pages[i].len; return g_pages[i].off;
        }
    return -1;
}
static ogg_int64_t _get_prev_page(OggVorbis_File*, ogg_page*) { return -1; }
static ogg_int64_t ogg_page_granulepos(ogg_page* og) { return g_pages[og->idx].gp; }
static int ogg_page_continued(ogg_page*) { return 0; }
static int ogg_page_serialno(ogg_page*) { return 9; }
static ogg_int64_t ov_pcm_total(OggVorbis_File* vf, int) { if (vf->ready_state < OPENED) return OV_EINVAL; return vf->pcmlengths[1]; }
static void _decode_clear(OggVorbis_File* vf) { vf->ready_state = OPENED; }
static void vorbis_synthesis_restart(OggVorbis_File* vf) { vf->restarts++; }
static void ogg_stream_reset_serialno(ogg_stream_state* os, long s) { os->serialno = s; os->peeked = 0; }
static void ogg_stream_pagein(ogg_stream_state*, ogg_page*) { }
static int ogg_stream_packetpeek(ogg_stream_state* os, ogg_packet* op) { op->granulepos = os->peeked == 0 ? -1 : 2000; return 1; }
static int ogg_stream_packetout(ogg_stream_state* os, ogg_packet*) { os->peeked++; return 1; }
static int ov_raw_seek(OggVorbis_File*, ogg_int64_t) { return 99; }

static int ov_pcm_seek_page(OggVorbis_File* vf, ogg_int64_t pos)
{
    int link = -1;
    ogg_int64_t result = 0;
    ogg_int64_t total = ov_pcm_total(vf, -1);
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (!vf->seekable) return (OV_ENOSEEK);
    if (pos < 0 || pos > total) return (OV_EINVAL);
    for (link = vf->links - 1; link >= 0; link--) { total -= vf->pcmlengths[link * 2 + 1]; if (pos >= total) break; }
    {
        ogg_int64_t end = vf->offsets[link + 1], begin = vf->offsets[link];
        ogg_int64_t begintime = vf->pcmlengths[link * 2], endtime = vf->pcmlengths[link * 2 + 1] + begintime;
        ogg_int64_t target = pos - total + begintime, best = begin;
        ogg_page og;
        while (begin < end) {
            ogg_int64_t bisect;
            if (end - begin < CHUNKSIZE) bisect = begin;
            else { bisect = begin + (target - begintime) * (end - begin) / (endtime - begintime) - CHUNKSIZE; if (bisect <= begin) bisect = begin + 1; }
            _seek_helper(vf, bisect);
            while (begin < end) {
                result = _get_next_page(vf, &og, end - vf->offset);
                if (result == OV_EREAD) goto seek_error;
                if (result < 0) {
                    if (bisect <= begin + 1) end = begin;
                    else { if (bisect == 0) goto seek_error; bisect -= CHUNKSIZE; if (bisect <= begin) bisect = begin + 1; _seek_helper(vf, bisect); }
                } else {
                    ogg_int64_t granulepos = ogg_page_granulepos(&og);
                    if (granulepos == -1) continue;
                    if (granulepos < target) {
                        best = result; begin = vf->offset; begintime = granulepos;
                        if (target - begintime > 44100) break;
                        bisect = begin;
                    } else {
                        if (bisect <= begin + 1) end = begin;
                        else {
                            if (end == vf->offset) { end = result; bisect -= CHUNKSIZE; if (bisect <= begin) bisect = begin + 1; _seek_helper(vf, bisect); }
                            else { end = result; endtime = granulepos; break; }
                        }
                    }
                }
            }
        }
        {
            ogg_page og; ogg_packet op;
            _seek_helper(vf, best);
            vf->pcm_offset = -1;
            if (_get_next_page(vf, &og, -1) < 0) return (OV_EOF);
            if (link != vf->current_link) { _decode_clear(vf); vf->current_link = link; vf->current_serialno = ogg_page_serialno(&og); vf->ready_state = STREAMSET; }
            else vorbis_synthesis_restart(vf);
            ogg_stream_reset_serialno(&vf->os, vf->current_serialno);
            ogg_stream_pagein(&vf->os, &og);
            while (1) {
                result = ogg_stream_packetpeek(&vf->os, &op);
                if (result == 0) {
                    _seek_helper(vf, best);
                    while (1) {
                        result = _get_prev_page(vf, &og);
                        if (result < 0) goto seek_error;
                        if (ogg_page_granulepos(&og) > -1 || !ogg_page_continued(&og)) return ov_raw_seek(vf, result);
                        vf->offset = result;
                    }
                }
                if (result < 0) { result = OV_EBADPACKET; goto seek_error; }
                if (op.granulepos != -1) {
                    vf->pcm_offset = op.granulepos - vf->pcmlengths[vf->current_link * 2];
                    if (vf->pcm_offset < 0) vf->pcm_offset = 0;
                    vf->pcm_offset += total;
                    break;
                } else result = ogg_stream_packetout(&vf->os, NULL0);
            }
        }
    }
    if (vf->pcm_offset > pos || pos > ov_pcm_total(vf, -1)) { result = OV_EFAULT; goto seek_error; }
    vf->bittrack = 0.f; vf->samptrack = 0.f;
    return (0);
seek_error:
    vf->pcm_offset = -1;
    _decode_clear(vf);
    return (int)result;
}

int main()
{
    OggVorbis_File vf; std::memset(&vf, 0, sizeof(vf));
    ogg_int64_t offs[2] = { 0, 100 }, pl[2] = { 0, 3000 };
    vf.seekable = 1; vf.links = 1; vf.offsets = offs; vf.pcmlengths = pl; vf.ready_state = 4; vf.current_link = 0; vf.bittrack = 1;
    int bad = 0;
    if (ov_pcm_seek_page(&vf, -5) != OV_EINVAL) bad = 1;
    if (ov_pcm_seek_page(&vf, 3001) != OV_EINVAL) bad = 2;
    vf.seekable = 0; if (ov_pcm_seek_page(&vf, 5) != OV_ENOSEEK) bad = 3; vf.seekable = 1;
    // target 2500: the last page with granulepos < 2500 is the one at offset 40 (gp 2000); its granule packet sets pcm_offset
    if (ov_pcm_seek_page(&vf, 2500) != 0 || vf.pcm_offset != 2000 || vf.restarts != 1 || vf.bittrack != 0) bad = 4;
    if (vf.os.peeked != 1) bad = 5;   // one leading packet without granulepos was dropped
    // target 1500: lands on the page at offset 10 (gp 1000), but the peeked packet says 2000 > pos -> OV_EFAULT
    if (ov_pcm_seek_page(&vf, 1500) != OV_EFAULT || vf.pcm_offset != -1 || vf.ready_state != OPENED) bad = 6;
    if (bad) { std::printf("00c8ae90_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c8ae90_TEST PASS\n"); return 0;
}
// Behaviour test for libvorbis _fetch_headers (0x00C8A0A0): with no page handed in, the first page is read
// (OV_EREAD / OV_ENOTVORBIS on failure); the stream is reset to the page's serial number, which is reported; three
// header packets are fed to the synthesis header parser across pages; a packet hole, a parser error or a missing page
// clears vi/vc and drops back to OPENED.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
#define CHUNKSIZE 8500
#define OV_EREAD -128
#define OV_ENOTVORBIS -132
#define OV_EBADHEADER -133
#define OPENED 2
#define STREAMSET 3
struct ogg_page { long serial; int packets; };
struct ogg_packet { int n; };
struct ogg_stream_state { long serialno; int pending; };
struct vorbis_info { int inits, clears; };
struct vorbis_comment { int inits, clears; };
struct OggVorbis_File {
    ogg_stream_state os; int ready_state;
    // mock source: a list of pages, the packet stream (result codes) and parser results
    ogg_page pages[4]; int npages, nextPage; bool readError;
    int packetResults[8]; int npacket; int headerinResults[4]; int nheaderin; int pageins;
};
static OggVorbis_File* g_vf;
static i64 _get_next_page(OggVorbis_File* vf, ogg_page* og, i64)
{
    if (vf->readError) return OV_EREAD;
    if (vf->nextPage >= vf->npages) return -1;
    *og = vf->pages[vf->nextPage++];
    return 0;
}
static long ogg_page_serialno(ogg_page* og) { return og->serial; }
static void ogg_stream_reset_serialno(ogg_stream_state* os, long s) { os->serialno = s; os->pending = 0; }
static void vorbis_info_init(vorbis_info* vi) { ++vi->inits; }
static void vorbis_comment_init(vorbis_comment* vc) { ++vc->inits; }
static void vorbis_info_clear(vorbis_info* vi) { ++vi->clears; }
static void vorbis_comment_clear(vorbis_comment* vc) { ++vc->clears; }
static void ogg_stream_pagein(ogg_stream_state* os, ogg_page* og) { os->pending += og->packets; ++g_vf->pageins; }
static int ogg_stream_packetout(ogg_stream_state* os, ogg_packet* op)
{
    if (os->pending == 0) return 0;
    --os->pending;
    op->n = g_vf->npacket;
    return g_vf->packetResults[g_vf->npacket++];
}
static int vorbis_synthesis_headerin(vorbis_info*, vorbis_comment*, ogg_packet*) { return g_vf->headerinResults[g_vf->nheaderin++]; }

static int _fetch_headers(OggVorbis_File* vf, vorbis_info* vi, vorbis_comment* vc, long* serialno, ogg_page* og_ptr)
{
    ogg_page og;
    ogg_packet op;
    int i, ret;
    if (!og_ptr) {
        i64 llret = _get_next_page(vf, &og, CHUNKSIZE);
        if (llret == OV_EREAD) return (OV_EREAD);
        if (llret < 0) return OV_ENOTVORBIS;
        og_ptr = &og;
    }
    ogg_stream_reset_serialno(&vf->os, ogg_page_serialno(og_ptr));
    if (serialno) *serialno = vf->os.serialno;
    vf->ready_state = STREAMSET;
    vorbis_info_init(vi);
    vorbis_comment_init(vc);
    i = 0;
    while (i < 3) {
        ogg_stream_pagein(&vf->os, og_ptr);
        while (i < 3) {
            int result = ogg_stream_packetout(&vf->os, &op);
            if (result == 0) break;
            if (result == -1) { ret = OV_EBADHEADER; goto bail_header; }
            if ((ret = vorbis_synthesis_headerin(vi, vc, &op))) goto bail_header;
            i++;
        }
        if (i < 3)
            if (_get_next_page(vf, og_ptr, CHUNKSIZE) < 0) { ret = OV_EBADHEADER; goto bail_header; }
    }
    return 0;
bail_header:
    vorbis_info_clear(vi);
    vorbis_comment_clear(vc);
    vf->ready_state = OPENED;
    return ret;
}

static OggVorbis_File make(int pagePackets0, int pagePackets1, int npages)
{
    OggVorbis_File f = OggVorbis_File();
    f.pages[0].serial = 77; f.pages[0].packets = pagePackets0;
    f.pages[1].serial = 77; f.pages[1].packets = pagePackets1;
    f.npages = npages;
    for (int i = 0; i < 8; ++i) f.packetResults[i] = 1;
    return f;
}

int main()
{
    int fails = 0;
    vorbis_info vi; vorbis_comment vc; long serial;
    // success over two pages: 1 packet then 2
    { OggVorbis_File f = make(1, 2, 2); g_vf = &f; vi = vorbis_info(); vc = vorbis_comment(); serial = 0;
      int r = _fetch_headers(&f, &vi, &vc, &serial, 0);
      if (r != 0 || serial != 77 || f.ready_state != STREAMSET || f.pageins != 2 || f.nheaderin != 3 || vi.inits != 1 || vi.clears != 0) ++fails; }
    // page handed in, serialno not wanted
    { OggVorbis_File f = make(3, 0, 0); g_vf = &f; vi = vorbis_info(); vc = vorbis_comment();
      ogg_page given = { 5, 3 };
      int r = _fetch_headers(&f, &vi, &vc, 0, &given);
      if (r != 0 || f.os.serialno != 5 || f.nextPage != 0 || f.nheaderin != 3) ++fails; }
    // read error / no first page
    { OggVorbis_File f = make(3, 0, 1); f.readError = true; g_vf = &f;
      if (_fetch_headers(&f, &vi, &vc, &serial, 0) != OV_EREAD) ++fails; }
    { OggVorbis_File f = make(3, 0, 0); g_vf = &f;
      if (_fetch_headers(&f, &vi, &vc, &serial, 0) != OV_ENOTVORBIS || f.ready_state != 0) ++fails; }
    // packet hole
    { OggVorbis_File f = make(3, 0, 1); f.packetResults[1] = -1; g_vf = &f; vi = vorbis_info(); vc = vorbis_comment();
      if (_fetch_headers(&f, &vi, &vc, &serial, 0) != OV_EBADHEADER || f.ready_state != OPENED || vi.clears != 1 || vc.clears != 1) ++fails; }
    // parser error code is returned
    { OggVorbis_File f = make(3, 0, 1); f.headerinResults[2] = -134; g_vf = &f; vi = vorbis_info(); vc = vorbis_comment();
      if (_fetch_headers(&f, &vi, &vc, &serial, 0) != -134 || f.ready_state != OPENED || vi.clears != 1) ++fails; }
    // missing second page
    { OggVorbis_File f = make(2, 0, 1); g_vf = &f; vi = vorbis_info(); vc = vorbis_comment();
      if (_fetch_headers(&f, &vi, &vc, &serial, 0) != OV_EBADHEADER || f.nheaderin != 2 || vc.clears != 1) ++fails; }
    std::printf(fails ? "00c8a0a0_TEST FAIL %d\n" : "00c8a0a0_TEST PASS\n", fails);
    return fails != 0;
}

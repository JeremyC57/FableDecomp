// Behaviour test for libvorbis vorbis_synthesis_headerin (0x00C8E580): the info.c header classifier over a small LSB-first
// bit reader (libogg's oggpack_read order): a null packet or an unknown type is OV_EBADHEADER, a wrong signature is
// OV_ENOTVORBIS, type 1 needs b_o_s and an uninitialised rate, type 3 needs the rate, type 5 needs the rate and the
// vendor string; each valid type is dispatched to its unpacker (stubs here that record the call).
#include <cstdio>
#include <cstring>

struct oggpack_buffer { const unsigned char* buf; long bytes; long bitpos; };
struct ogg_packet { unsigned char* packet; long bytes; long b_o_s; };
struct vorbis_info { long rate; };
struct vorbis_comment { char* vendor; };

static void oggpack_readinit(oggpack_buffer* b, unsigned char* buf, int bytes) { b->buf = buf; b->bytes = bytes; b->bitpos = 0; }
static long oggpack_read(oggpack_buffer* b, int bits)
{
    long v = 0;
    for (int i = 0; i < bits; ++i, ++b->bitpos) {
        if (b->bitpos / 8 >= b->bytes) return -1;
        if (b->buf[b->bitpos / 8] & (1 << (b->bitpos % 8))) v |= 1L << i;
    }
    return v;
}
static void _v_readstring(oggpack_buffer* o, char* buf, int bytes) { while (bytes--) *buf++ = (char)oggpack_read(o, 8); }

static int g_called = 0;
static int _vorbis_unpack_info(vorbis_info*, oggpack_buffer*) { g_called = 1; return 0; }
static int _vorbis_unpack_comment(vorbis_comment*, oggpack_buffer*) { g_called = 3; return 0; }
static int _vorbis_unpack_books(vorbis_info*, oggpack_buffer*) { g_called = 5; return 0; }

#define OV_ENOTVORBIS -132
#define OV_EBADHEADER -133

static int vorbis_synthesis_headerin(vorbis_info* vi, vorbis_comment* vc, ogg_packet* op)
{
    oggpack_buffer opb;
    if (op) {
        oggpack_readinit(&opb, op->packet, op->bytes);
        {
            char buffer[6];
            int packtype = oggpack_read(&opb, 8);
            std::memset(buffer, 0, 6);
            _v_readstring(&opb, buffer, 6);
            if (std::memcmp(buffer, "vorbis", 6)) return OV_ENOTVORBIS;
            switch (packtype) {
            case 0x01:
                if (!op->b_o_s) return OV_EBADHEADER;
                if (vi->rate != 0) return OV_EBADHEADER;
                return _vorbis_unpack_info(vi, &opb);
            case 0x03:
                if (vi->rate == 0) return OV_EBADHEADER;
                return _vorbis_unpack_comment(vc, &opb);
            case 0x05:
                if (vi->rate == 0 || vc->vendor == 0) return OV_EBADHEADER;
                return _vorbis_unpack_books(vi, &opb);
            default:
                return OV_EBADHEADER;
            }
        }
    }
    return OV_EBADHEADER;
}

static int Run(unsigned char type, const char* sig, long bos, long rate, char* vendor)
{
    unsigned char pkt[8]; pkt[0] = type; std::memcpy(pkt + 1, sig, 6); pkt[7] = 0;
    ogg_packet op; op.packet = pkt; op.bytes = 8; op.b_o_s = bos;
    vorbis_info vi; vi.rate = rate;
    vorbis_comment vc; vc.vendor = vendor;
    g_called = 0;
    return vorbis_synthesis_headerin(&vi, &vc, &op);
}

int main()
{
    int ok = 1;
    char vendor[] = "Xiph";
    ok &= vorbis_synthesis_headerin(0, 0, 0) == OV_EBADHEADER;
    ok &= Run(1, "vorbiX", 1, 0, 0) == OV_ENOTVORBIS && g_called == 0;
    ok &= Run(1, "vorbis", 0, 0, 0) == OV_EBADHEADER && g_called == 0;      // not the first packet
    ok &= Run(1, "vorbis", 1, 44100, 0) == OV_EBADHEADER;                    // info already read
    ok &= Run(1, "vorbis", 1, 0, 0) == 0 && g_called == 1;
    ok &= Run(3, "vorbis", 0, 0, 0) == OV_EBADHEADER && g_called == 0;      // no info yet
    ok &= Run(3, "vorbis", 0, 44100, 0) == 0 && g_called == 3;
    ok &= Run(5, "vorbis", 0, 44100, 0) == OV_EBADHEADER && g_called == 0;  // no comment header yet
    ok &= Run(5, "vorbis", 0, 44100, vendor) == 0 && g_called == 5;
    ok &= Run(7, "vorbis", 0, 44100, vendor) == OV_EBADHEADER && g_called == 0;
    if (!ok) { std::printf("00c8e580_TEST FAIL\n"); return 1; }
    std::printf("00c8e580_TEST PASS\n");
    return 0;
}
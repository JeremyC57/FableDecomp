// Behaviour test for 00c8d970: ogg_stream_reset (Xiph libogg framing.c)
#include <cstdio>
#include <cstring>
typedef __int64 ogg_int64_t;
struct ogg_stream_state {               // 0x168 bytes
    unsigned char* body_data;           // +0x000
    long           body_storage;        // +0x004
    long           body_fill;           // +0x008
    long           body_returned;       // +0x00c
    int*           lacing_vals;         // +0x010
    ogg_int64_t*   granule_vals;        // +0x014
    long           lacing_storage;      // +0x018
    long           lacing_fill;         // +0x01c
    long           lacing_packet;       // +0x020
    long           lacing_returned;     // +0x024
    unsigned char  header[282];         // +0x028
    int            header_fill;         // +0x144
    int            e_o_s;               // +0x148
    int            b_o_s;               // +0x14c
    long           serialno;            // +0x150
    long           pageno;              // +0x154
    ogg_int64_t    packetno;            // +0x158
    ogg_int64_t    granulepos;          // +0x160
};

static int ogg_stream_reset(ogg_stream_state* os)
{
    os->body_fill = 0;
    os->body_returned = 0;

    os->lacing_fill = 0;
    os->lacing_packet = 0;
    os->lacing_returned = 0;

    os->header_fill = 0;

    os->e_o_s = 0;
    os->b_o_s = 0;
    os->pageno = -1;
    os->packetno = 0;
    os->granulepos = 0;

    return (0);
}

int main()
{
    int ok;
    static ogg_stream_state os; os.body_fill = 5; os.lacing_fill = 3; os.serialno = 77; os.pageno = 9; os.packetno = 4; os.granulepos = 8;
    ok = ogg_stream_reset(&os) == 0 && os.body_fill == 0 && os.lacing_fill == 0 && os.serialno == 77 && os.pageno == -1 && os.packetno == 0 && os.granulepos == 0 && sizeof(ogg_stream_state) == 0x168;
    std::printf(ok ? "00c8d970_TEST PASS\n" : "00c8d970_TEST FAIL\n");
    return ok ? 0 : 1;
}

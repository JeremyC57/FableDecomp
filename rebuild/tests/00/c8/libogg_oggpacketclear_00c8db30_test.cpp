// Behaviour test for 00c8db30: ogg_packet_clear -- frees the packet data and zeroes the record
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct ogg_packet { unsigned char* packet; long bytes; long b_o_s; long e_o_s; __int64 granulepos; __int64 packetno; };
static void ogg_packet_clear(ogg_packet* op) { std::free(op->packet); std::memset(op, 0, sizeof(*op)); }
int main()
{
    ogg_packet op; op.packet = (unsigned char*)std::malloc(16); op.bytes = 16; op.b_o_s = 1; op.e_o_s = 1; op.granulepos = 5; op.packetno = 6;
    ogg_packet_clear(&op);
    int ok = op.packet == 0 && op.bytes == 0 && op.b_o_s == 0 && op.granulepos == 0 && op.packetno == 0 && sizeof(ogg_packet) == 0x20;
    std::printf(ok ? "00c8db30_TEST PASS\n" : "00c8db30_TEST FAIL\n");
    return ok ? 0 : 1;
}

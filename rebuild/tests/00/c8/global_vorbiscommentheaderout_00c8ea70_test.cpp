// Behaviour test for 0x00C8EA70 vorbis_commentheader_out (libvorbis info.c): copy of the logic with stand-in
// packer functions (writeinit / bytes / a pack_comment that writes a fixed byte pattern or fails).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
struct vorbis_comment { char** user_comments; int* comment_lengths; int comments; char* vendor; };
static int g_fail = 0;
struct oggpack_buffer {
    long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage;
    int _vorbis_pack_comment(vorbis_comment* vc)
    {
        int i;
        if (g_fail) return -1;
        for (i = 0; i < 7 + vc->comments; i++) buffer[endbyte++] = (unsigned char)(0x30 + i);
        return 0;
    }
};
struct ogg_packet { unsigned char* packet; long bytes; long b_o_s; long e_o_s; ogg_int64_t granulepos; ogg_int64_t packetno; };
static unsigned char g_store[64];
void oggpack_writeinit(oggpack_buffer* b) { memset(b, 0, sizeof(*b)); b->buffer = b->ptr = g_store; b->storage = 64; }
long oggpack_bytes(oggpack_buffer* b) { return b->endbyte + (b->endbit + 7) / 8; }
#define OV_EIMPL -130
int vorbis_commentheader_out(vorbis_comment* vc, ogg_packet* op)
{
    oggpack_buffer opb;
    oggpack_writeinit(&opb);
    if (opb._vorbis_pack_comment(vc)) return OV_EIMPL;
    op->packet = (unsigned char*)malloc(oggpack_bytes(&opb));
    memcpy(op->packet, opb.buffer, oggpack_bytes(&opb));
    op->bytes = oggpack_bytes(&opb);
    op->b_o_s = 0;
    op->e_o_s = 0;
    op->granulepos = 0;
    return 0;
}
int main()
{
    vorbis_comment vc; memset(&vc, 0, sizeof(vc)); vc.comments = 2;
    ogg_packet op; memset(&op, 0x77, sizeof(op));
    int ok = vorbis_commentheader_out(&vc, &op) == 0;
    ok &= op.bytes == 9 && op.packet != 0 && memcmp(op.packet, "012345678", 9) == 0 && op.packet != g_store;
    ok &= op.b_o_s == 0 && op.e_o_s == 0 && op.granulepos == 0;
    free(op.packet);
    g_fail = 1; ok &= vorbis_commentheader_out(&vc, &op) == -130;
    if (!ok) { std::printf("00c8ea70_TEST FAIL\n"); return 1; }
    std::printf("00c8ea70_TEST PASS\n");
    return 0;
}
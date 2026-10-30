// Behaviour test for libvorbis info.c _vorbis_pack_comment (0x00C8E790): type 3, "vorbis", the vendor string, each comment
// length-prefixed (a null comment as length 0), then the framing bit.
#include <cstdio>
#include <cstring>
struct pack { unsigned long v[64]; int bits[64]; int n; };
struct vorbis_comment { char** user_comments; int* comment_lengths; int comments; char* vendor; };
static void oggpack_write(pack* b, unsigned long value, int bits) { b->v[b->n] = value; b->bits[b->n++] = bits; }
static void _v_writestring(pack* o, const char* s, int bytes) { while (bytes--) oggpack_write(o, *s++, 8); }
static int _vorbis_pack_comment(pack* opb, vorbis_comment* vc)
{
    char temp[] = "Xiph.Org libVorbis I 20030909";
    int bytes = (int)std::strlen(temp);
    oggpack_write(opb, 0x03, 8);
    _v_writestring(opb, "vorbis", 6);
    oggpack_write(opb, bytes, 32);
    _v_writestring(opb, temp, bytes);
    oggpack_write(opb, vc->comments, 32);
    for (int i = 0; i < vc->comments; i++) {
        if (vc->user_comments[i]) {
            oggpack_write(opb, vc->comment_lengths[i], 32);
            _v_writestring(opb, vc->user_comments[i], vc->comment_lengths[i]);
        } else
            oggpack_write(opb, 0, 32);
    }
    oggpack_write(opb, 1, 1);
    return 0;
}
int main()
{
    static pack p; p.n = 0;
    char a[] = "A=b"; char* uc[2] = { a, 0 }; int len[2] = { 3, 0 };
    vorbis_comment vc = { uc, len, 2, 0 };
    if (_vorbis_pack_comment(&p, &vc) != 0) { std::printf("00c8e790_TEST FAIL\n"); return 1; }
    // 1 + 6 + 1 + 29 + 1 + (1 + 3) + 1 + 1 = 44 writes
    if (p.n != 44 || p.v[0] != 3 || p.v[1] != 'v' || p.v[7] != 29 || p.bits[7] != 32 || p.v[38] != 3 || p.v[42] != 0
        || p.bits[43] != 1 || p.v[43] != 1) { std::printf("00c8e790_TEST FAIL\n"); return 1; }
    std::printf("00c8e790_TEST PASS\n"); return 0;
}
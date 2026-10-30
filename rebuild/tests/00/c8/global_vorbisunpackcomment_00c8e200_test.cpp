// Behaviour test for libvorbis info.c _vorbis_unpack_comment (0x00C8E200): read vendor string and user comments.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct rd { const long* v; int n; };
static long oggpack_read(rd* b, int) { return b->v[b->n++]; }
static void _v_readstring(rd* o, char* buf, int bytes) { while (bytes--) *buf++ = (char)oggpack_read(o, 8); }
struct vc_t { char** user_comments; int* comment_lengths; int comments; char* vendor; int cleared; };
static void vorbis_comment_clear(vc_t* vc) { vc->cleared = 1; }
static int _vorbis_unpack_comment(vc_t* vc, rd* opb)
{
    int vendorlen = oggpack_read(opb, 32);
    if (vendorlen < 0) goto err_out;
    vc->vendor = (char*)std::calloc(vendorlen + 1, 1);
    _v_readstring(opb, vc->vendor, vendorlen);
    vc->comments = oggpack_read(opb, 32);
    if (vc->comments < 0) goto err_out;
    vc->user_comments = (char**)std::calloc(vc->comments + 1, sizeof(*vc->user_comments));
    vc->comment_lengths = (int*)std::calloc(vc->comments + 1, sizeof(*vc->comment_lengths));
    for (int i = 0; i < vc->comments; i++) {
        int len = oggpack_read(opb, 32);
        if (len < 0) goto err_out;
        vc->comment_lengths[i] = len; vc->user_comments[i] = (char*)std::calloc(len + 1, 1);
        _v_readstring(opb, vc->user_comments[i], len);
    }
    if (oggpack_read(opb, 1) != 1) goto err_out;
    return 0;
err_out:
    vorbis_comment_clear(vc); return -133;
}
int main()
{
    static const long data[] = { 2, 'X', 'O', 1, 3, 'a', '=', 'b', 1 };
    vc_t vc; std::memset(&vc, 0, sizeof(vc));
    rd r = { data, 0 };
    if (_vorbis_unpack_comment(&vc, &r) != 0 || std::strcmp(vc.vendor, "XO") != 0 || vc.comments != 1 || vc.comment_lengths[0] != 3
        || std::strcmp(vc.user_comments[0], "a=b") != 0) { std::printf("00c8e200_TEST FAIL\n"); return 1; }
    static const long neg[] = { -1 };
    vc_t v2; std::memset(&v2, 0, sizeof(v2)); rd r2 = { neg, 0 };
    if (_vorbis_unpack_comment(&v2, &r2) != -133 || !v2.cleared) { std::printf("00c8e200_TEST FAIL\n"); return 1; }
    std::printf("00c8e200_TEST PASS\n"); return 0;
}
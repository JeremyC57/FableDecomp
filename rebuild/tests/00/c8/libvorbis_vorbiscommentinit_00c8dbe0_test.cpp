// Behaviour test for 00c8dbe0: vorbis_comment_init (Xiph libvorbis info.c)
#include <cstdio>
#include <cstring>
struct vorbis_comment {                 // 0x10 bytes
    char** user_comments;               // +0x00
    int*   comment_lengths;             // +0x04
    int    comments;                    // +0x08
    char*  vendor;                      // +0x0c
};

static void vorbis_comment_init(vorbis_comment* vc)
{
    memset(vc, 0, sizeof(*vc));
}

int main()
{
    int ok;
    int x; vorbis_comment vc; vc.user_comments = (char**)&x; vc.comment_lengths = &x; vc.comments = 3; vc.vendor = (char*)&x;
    vorbis_comment_init(&vc); ok = vc.user_comments == 0 && vc.comment_lengths == 0 && vc.comments == 0 && vc.vendor == 0;
    std::printf(ok ? "00c8dbe0_TEST PASS\n" : "00c8dbe0_TEST FAIL\n");
    return ok ? 0 : 1;
}

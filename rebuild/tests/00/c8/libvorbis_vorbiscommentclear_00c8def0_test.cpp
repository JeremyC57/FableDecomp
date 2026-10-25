// Behaviour test for 0x00C8DEF0 vorbis_comment_clear (libvorbis info.c): copy of the logic over vorbis_comment_add.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct vorbis_comment { char** user_comments; int* comment_lengths; int comments; char* vendor; };
void vorbis_comment_add(vorbis_comment* vc, char* comment)
{
    vc->user_comments = (char**)realloc(vc->user_comments,
                            (vc->comments + 2) * sizeof(*vc->user_comments));
    vc->comment_lengths = (int*)realloc(vc->comment_lengths,
                            (vc->comments + 2) * sizeof(*vc->comment_lengths));
    vc->comment_lengths[vc->comments] = strlen(comment);
    vc->user_comments[vc->comments] = (char*)malloc(vc->comment_lengths[vc->comments] + 1);
    strcpy(vc->user_comments[vc->comments], comment);
    vc->comments++;
    vc->user_comments[vc->comments] = 0;
}
void vorbis_comment_clear(vorbis_comment* vc)
{
    if (vc) {
        long i;
        for (i = 0; i < vc->comments; i++)
            if (vc->user_comments[i]) free(vc->user_comments[i]);
        if (vc->user_comments) free(vc->user_comments);
        if (vc->comment_lengths) free(vc->comment_lengths);
        if (vc->vendor) free(vc->vendor);
    }
    memset(vc, 0, sizeof(*vc));
}
int main()
{
    vorbis_comment vc; memset(&vc, 0, sizeof(vc));
    char a[] = "A=1"; char b[] = "B=2";
    vorbis_comment_add(&vc, a); vorbis_comment_add(&vc, b);
    free(vc.user_comments[1]); vc.user_comments[1] = 0;            // a NULL slot is skipped
    vc.vendor = (char*)malloc(8);
    vorbis_comment_clear(&vc);
    int ok = vc.user_comments == 0 && vc.comment_lengths == 0 && vc.comments == 0 && vc.vendor == 0;
    vorbis_comment_clear(&vc);                                     // already empty: still fine
    ok &= vc.comments == 0;
    if (!ok) { std::printf("00c8def0_TEST FAIL\n"); return 1; }
    std::printf("00c8def0_TEST PASS\n");
    return 0;
}
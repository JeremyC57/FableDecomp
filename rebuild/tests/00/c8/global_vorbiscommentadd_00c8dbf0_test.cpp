// Behaviour test for 0x00C8DBF0 vorbis_comment_add (libvorbis info.c): copy of the logic.
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
int main()
{
    vorbis_comment vc; memset(&vc, 0, sizeof(vc));
    char a[] = "TITLE=Fable"; char b[] = "ARTIST=Russell Shaw";
    vorbis_comment_add(&vc, a);
    vorbis_comment_add(&vc, b);
    int ok = vc.comments == 2 && vc.comment_lengths[0] == 11 && vc.comment_lengths[1] == 19;
    ok &= strcmp(vc.user_comments[0], a) == 0 && strcmp(vc.user_comments[1], b) == 0 && vc.user_comments[2] == 0;
    ok &= vc.user_comments[0] != a;
    free(vc.user_comments[0]); free(vc.user_comments[1]); free(vc.user_comments); free(vc.comment_lengths);
    if (!ok) { std::printf("00c8dbf0_TEST FAIL\n"); return 1; }
    std::printf("00c8dbf0_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x00C8DC90 vorbis_comment_add_tag (libvorbis info.c): copy of the logic (alloca through
// _alloca / __builtin_alloca) over a copy of vorbis_comment_add.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
#include <malloc.h>
#define ALLOCA _alloca
#else
#define ALLOCA __builtin_alloca
#endif
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
void vorbis_comment_add_tag(vorbis_comment* vc, char* tag, char* contents)
{
    char* comment = (char*)ALLOCA(strlen(tag) + strlen(contents) + 2); /* +2 for = and \0 */
    strcpy(comment, tag);
    strcat(comment, "=");
    strcat(comment, contents);
    vorbis_comment_add(vc, comment);
}
int main()
{
    vorbis_comment vc; memset(&vc, 0, sizeof(vc));
    char t[] = "GENRE"; char c[] = "Soundtrack";
    vorbis_comment_add_tag(&vc, t, c);
    char e[] = ""; vorbis_comment_add_tag(&vc, t, e);
    int ok = vc.comments == 2 && strcmp(vc.user_comments[0], "GENRE=Soundtrack") == 0 && vc.comment_lengths[0] == 16;
    ok &= strcmp(vc.user_comments[1], "GENRE=") == 0 && vc.comment_lengths[1] == 6 && vc.user_comments[2] == 0;
    free(vc.user_comments[0]); free(vc.user_comments[1]); free(vc.user_comments); free(vc.comment_lengths);
    if (!ok) { std::printf("00c8dc90_TEST FAIL\n"); return 1; }
    std::printf("00c8dc90_TEST PASS\n");
    return 0;
}
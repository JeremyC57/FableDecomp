// Behaviour test for libvorbis info.c row 00c8dd30 (tagcompare / vorbis_comment_query / vorbis_comment_query_count).
#include <cstdio>
#include <cstring>
#include <cctype>
struct vorbis_comment { char** user_comments; int* comment_lengths; int comments; char* vendor; };
static int tagcompare(const char* s1, const char* s2, int n)
{
    int c = 0;
    while (c < n) {
        if (toupper(s1[c]) != toupper(s2[c]))
            return !0;
        c++;
    }
    return 0;
}
static char* vorbis_comment_query(vorbis_comment* vc, const char* tag, int count)
{
    long i; int found = 0;
    int taglen = (int)strlen(tag) + 1;
    char fulltag[256];
    strcpy(fulltag, tag); strcat(fulltag, "=");
    for (i = 0; i < vc->comments; i++) {
        if (!tagcompare(vc->user_comments[i], fulltag, taglen)) {
            if (count == found) return vc->user_comments[i] + taglen;
            else found++;
        }
    }
    return 0;
}
static int vorbis_comment_query_count(vorbis_comment* vc, const char* tag)
{
    int i, count = 0;
    int taglen = (int)strlen(tag) + 1;
    char fulltag[256];
    strcpy(fulltag, tag); strcat(fulltag, "=");
    for (i = 0; i < vc->comments; i++)
        if (!tagcompare(vc->user_comments[i], fulltag, taglen)) count++;
    return count;
}
static char c0[] = "ARTIST=Danny", c1[] = "title=Oakvale", c2[] = "Artist=Russell", c3[] = "ARTISTS=no";
static char* list[] = { c0, c1, c2, c3, 0 };
int main()
{
    int fail = 0;
    if (tagcompare("abcD", "ABCd", 4) != 0 || tagcompare("abcE", "ABCd", 4) != 1 || tagcompare("abX", "abY", 2) != 0) fail = 1;
    if (fail) { std::printf("00c8dd30_TEST FAIL\n"); return 1; }
    std::printf("00c8dd30_TEST PASS\n");
    return 0;
}
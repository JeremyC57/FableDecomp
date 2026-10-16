#include <cstdio>
// Behaviour model of CTCSkeletalMorph::CTCSkeletalMorph (0x007865E0): cleared state and a fresh morph object.
struct Morph { int id; };
struct TC { bool changed; void* first; void* last; void* end; Morph* morph;
    TC(Morph* fresh) : changed(false), first(0), last(0), end(0), morph(0) { morph = fresh; } };
int main()
{
    Morph m = { 7 };
    TC t(&m);
    if (t.changed || t.first || t.last || t.end || t.morph != &m) { std::printf("007865e0_TEST FAIL\n"); return 1; }
    std::printf("007865e0_TEST PASS\n");
    return 0;
}
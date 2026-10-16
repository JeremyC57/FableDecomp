#include <cstdio>
#include <cstring>
// C3DMeshFile(): the base adopts a counted root chunk (refcount 1 -> 2 in the base, temp released -> 1),
// the comment is the copyright string and the version is 100.
struct Block { long refs; };
struct File { void* root; Block* count; unsigned int Version; const char* FileComment;
    File(void* r, Block* b) : root(r), count(b) { ++count->refs; } };
int main() {
    int rootChunk = 0; Block b = { 1 };
    File f(&rootChunk, &b); --b.refs;            // the temporary's release
    f.FileComment = "Copyright Big Blue Box Studios Ltd."; f.Version = 100;
    if (b.refs != 1 || f.Version != 100 || std::strcmp(f.FileComment, "Copyright Big Blue Box Studios Ltd.") != 0 || f.root != &rootChunk) { std::printf("00a9ad80_TEST FAIL\n"); return 1; }
    std::printf("00a9ad80_TEST PASS\n"); return 0; }
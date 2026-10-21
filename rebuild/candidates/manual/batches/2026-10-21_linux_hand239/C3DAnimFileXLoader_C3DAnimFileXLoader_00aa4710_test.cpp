#include <cstdio>
#include <cstdlib>
// Behaviour copy of C3DAnimFileXLoader::C3DAnimFileXLoader (0x00AA4710): register XSEQ twice and XALO once,
// releasing each returned counted pointer before the prototype dies.
struct Count { long refs; void (*destroy)(void*); void* object; };
static int g_destroyed = 0, g_freed = 0, g_protoDtors = 0;
static void Destroy(void* p) { ++g_destroyed; }
struct Ptr {
    void* obj; Count* count;
    ~Ptr() { if (count) { --count->refs; if (count->refs == 0) { count->destroy(count->object); delete count; } } obj = 0; count = 0; }
};
struct Chunk {
    char type[5]; Chunk(const char* t) { for (int i = 0; i < 5; ++i) type[i] = t[i]; }
    virtual ~Chunk() { ++g_protoDtors; }
};
struct SeqChunk : Chunk {
    unsigned char* begin; unsigned char* end; unsigned char* cap;
    SeqChunk() : Chunk("XSEQ"), begin(0), end(0), cap(0) { begin = (unsigned char*)std::malloc(4); }
    ~SeqChunk() { if (begin) { std::free(begin); ++g_freed; } }
};
struct AloChunk : Chunk { unsigned long AllocSize; AloChunk() : Chunk("XALO") {} };
struct Loader {
    int obj, mv, help; char lastType[3][5];
    Ptr Add(const Chunk& c, int& counter, int slot) {
        ++counter; for (int i = 0; i < 5; ++i) lastType[slot][i] = c.type[i];
        Ptr p; p.obj = 0; p.count = new Count; p.count->refs = 1; p.count->destroy = Destroy; p.count->object = 0; return p;
    }
    Loader() : obj(0), mv(0), help(0) {
        { SeqChunk prototype; Ptr registered = Add(prototype, obj, 0); }
        { SeqChunk prototype; Ptr registered = Add(prototype, mv, 1); }
        { AloChunk prototype; Ptr registered = Add(prototype, help, 2); }
    }
};
int main() {
    Loader l;
    if (l.obj != 1 || l.mv != 1 || l.help != 1 || l.lastType[0][1] != 'S' || l.lastType[2][1] != 'A' || g_destroyed != 3
        || g_freed != 2 || g_protoDtors != 3) { std::printf("00aa4710_TEST FAIL\n"); return 1; }
    std::printf("00aa4710_TEST PASS\n"); return 0;
}

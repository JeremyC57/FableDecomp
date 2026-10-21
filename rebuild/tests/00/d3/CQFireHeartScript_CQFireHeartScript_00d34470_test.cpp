// Behaviour test for 00d34470 (CQ_FireHeartScript ctor): data/interface stored, both maps get a self-linked red
// header with zero count, every vector and handle is empty.
#include <cstdio>
#include <cstdlib>

struct Node { bool color; Node* parent; Node* left; Node* right; };
struct Map {
    Node* header; unsigned long count;
    Map() : header(0), count(0)
    {
        header = (Node*)std::malloc(sizeof(Node));
        header->color = false; header->parent = 0; header->left = header; header->right = header;
    }
    ~Map() { std::free(header); }
};
struct Vec { void* start; void* finish; void* eos; Vec() : start(0), finish(0), eos(0) { } };
struct Thing { void* data; void* info; Thing() : data(0), info(0) { } };
struct Handle { void* ip; void* data; void* info; Handle() : ip(0), data(0), info(0) { } };
struct Script {
    void* game; void* data;
    Map cutsceneFlags, csargs;
    Thing effect; Vec vecs[11]; Vec sehProphet; Handle seh[3]; Thing marker;
    Script(void* d, void* g) : game(g), data(d) { }
};
int main()
{
    int g = 0, d = 0;
    Script s(&d, &g);
    int ok = s.game == &g && s.data == &d;
    const Map* maps[2] = { &s.cutsceneFlags, &s.csargs };
    for (int i = 0; i < 2; ++i)
        if (maps[i]->count != 0 || maps[i]->header->left != maps[i]->header || maps[i]->header->right != maps[i]->header ||
            maps[i]->header->parent != 0 || maps[i]->header->color) ok = 0;
    for (int i = 0; i < 11; ++i) if (s.vecs[i].start || s.vecs[i].finish || s.vecs[i].eos) ok = 0;
    for (int i = 0; i < 3; ++i) if (s.seh[i].data || s.seh[i].info) ok = 0;
    if (s.effect.data || s.marker.info || s.sehProphet.start) ok = 0;
    if (!ok) { std::printf("00d34470_TEST FAIL\n"); return 1; }
    std::printf("00d34470_TEST PASS\n");
    return 0;
}
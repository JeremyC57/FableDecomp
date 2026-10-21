#include <cstdio>
#include <cstdlib>
// Behaviour copy of CCreatureDef::CCreatureDef (0x006765A0): zeroed containers, an empty map header, BattleCrySound = -1, four freshly built counted objects.
struct Node { unsigned char color; Node* parent; Node* left; Node* right; long key; long value; };
struct Map { Node* header; unsigned long count; Map() : header(0) { header = (Node*)std::malloc(sizeof(Node)); count = 0; header->color = 0; header->parent = 0; header->left = header; header->right = header; } ~Map() { std::free(header); } };
struct Vec { long* s; long* f; long* e; Vec() : s(0), f(0), e(0) {} };
struct Block { long refs; void* obj; };
template <class T> struct Counted { T* obj; Block* count; Counted() : obj(0), count(0) {} void Reset(T* o) { if (count && --count->refs == 0) { delete (T*)count->obj; delete count; } count = 0; obj = o; if (o) { count = new Block; count->refs = 1; count->obj = o; } } ~Counted() { if (count) { delete (T*)count->obj; delete count; } } };
struct Morphs { Vec v; bool applied; bool dirty; Morphs() : dirty(false) {} };
struct Stats { long manager; unsigned bits; Stats() : manager(0), bits(0) {} };
struct Def {
    long manager; unsigned bits; Counted<Morphs> wound; Counted<Morphs> expr; Counted<Stats> stats; Vec actions; Map phoneme; long battleCry; Map weights; float weight; float spreadMin;
    Def() : manager(0), bits(0), battleCry(-1) { weight = 0.0f; spreadMin = 0.0f; wound.Reset(new Morphs()); expr.Reset(new Morphs()); stats.Reset(new Stats()); }
};
int main()
{
    Def d;
    bool ok = d.manager == 0 && d.bits == 0 && d.actions.s == 0 && d.phoneme.count == 0 && d.phoneme.header->left == d.phoneme.header && d.phoneme.header->right == d.phoneme.header
        && d.phoneme.header->parent == 0 && d.phoneme.header->color == 0 && d.battleCry == -1 && d.weight == 0.0f && d.spreadMin == 0.0f
        && d.wound.obj && d.wound.count->refs == 1 && !d.wound.obj->dirty && d.expr.obj && d.stats.obj && d.stats.obj->manager == 0;
    std::printf(ok ? "006765a0_TEST PASS\n" : "006765a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
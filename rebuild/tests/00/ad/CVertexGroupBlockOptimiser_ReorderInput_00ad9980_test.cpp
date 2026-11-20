// Behaviour test for 0x00AD9980 CVertexGroupBlockOptimiser::ReorderInput (links the REAL row object, including its same-TU IsSubsetOf,
// the noinline STLport const find, CArray<bool>, vector<CVertexGroupBlock> _M_insert_overflow / operator= and the CArray<unsigned long> copy).
// Fixtures: blocks are real STLport layouts (Groups vector +0, GroupToIndexMap {header, count, compare} +0xC, nodes {color, parent,
// left, right, key, value}); each map is a right-leaning chain of real tree nodes. The three externals the object leaves undefined
// source header (so identity survives the copy), destroy counts elements. Runs check the exact output order and contents.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma comment(linker, "/alternatename:@CallRow@12=?ReorderInput@CVertexGroupBlockOptimiser@@QAEXABVCVertexGroupBlockList@@@Z")
#pragma comment(linker, "/alternatename:??0?$map@KJU?$less@K@std@@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@QAE@ABV01@@Z=@MapCopy@12")
#pragma comment(linker, "/alternatename:??4?$_Rb_tree@KU?$pair@$$CBKJ@std@@U?$_Select1st@U?$pair@$$CBKJ@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@QAEAAV01@ABV01@@Z=@TreeAssign@12")

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct Node { long color; Node* parent; Node* left; Node* right; unsigned long key; long value; };
struct Map { Node* header; unsigned long count; unsigned long cmp; };
struct Vec { unsigned long* start; unsigned long* finish; unsigned long* eos; };
struct Block { Vec groups; Map map; };
struct BVec { Block* start; Block* finish; Block* eos; };
struct Opt { BVec input; unsigned char rest[0x58 - 12]; };
typedef char SizeCheck[sizeof(Block) == 0x18 ? 1 : -1];

extern "C" void __fastcall CallRow(Opt* self, void*, const BVec* solution);
static int g_mapCopies = 0, g_treeAssigns = 0, g_destroyed = 0;
extern "C" Map* __fastcall MapCopy(Map* self, void*, const Map* other) { ++g_mapCopies; *self = *other; return self; }
extern "C" Map* __fastcall TreeAssign(Map* self, void*, const Map* other) { ++g_treeAssigns; *self = *other; return self; }
class CVertexGroupBlock;
namespace std {                                    // the object's __fastcall std::__destroy_aux<CVertexGroupBlock*> (an explicit
struct __false_type;                               // specialization gives the exact decorated name; /alternatename does not bind ??$ names)
template<class T> void __fastcall __destroy_aux(T first, T last, const __false_type&);
template<> void __fastcall __destroy_aux<CVertexGroupBlock*>(CVertexGroupBlock* first, CVertexGroupBlock* last, const __false_type&)
{ g_destroyed += (int)((Block*)last - (Block*)first); }
}

static Node g_nodes[256]; static int g_nn = 0;
static void MakeBlock(Block* b, const unsigned long* keys, int n)   // keys ascending
{
    b->groups.start = (unsigned long*)malloc(sizeof(unsigned long) * (n ? n : 1));
    memcpy(b->groups.start, keys, sizeof(unsigned long) * n);
    b->groups.finish = b->groups.eos = b->groups.start + n;
    Node* h = &g_nodes[g_nn++]; memset(h, 0, sizeof *h);
    Node* prev = h;
    for (int i = 0; i < n; ++i) {
        Node* x = &g_nodes[g_nn++]; memset(x, 0, sizeof *x);
        x->key = keys[i]; x->value = i; x->parent = prev;
        if (prev == h) h->parent = x; else prev->right = x;
        prev = x;
    }
    h->left = n ? h->parent : h; h->right = n ? prev : h;
    b->map.header = h; b->map.count = n; b->map.cmp = 0;
}
static void MakeList(BVec* v, Block* blocks, int n)
{
    v->start = (Block*)malloc(sizeof(Block) * (n ? n : 1)); memcpy(v->start, blocks, sizeof(Block) * n);
    v->finish = v->eos = v->start + n;
}

static void Run(const unsigned long* const* inKeys, const int* inN, int ni, const unsigned long* const* solKeys, const int* solN, int ns,
                const int* expect, int ne)
{
    ++g_run; g_nn = 0;
    Block in[16], sol[16];
    for (int i = 0; i < ni; ++i) MakeBlock(&in[i], inKeys[i], inN[i]);
    for (int i = 0; i < ns; ++i) MakeBlock(&sol[i], solKeys[i], solN[i]);
    Opt opt; memset(&opt, 0x5a, sizeof opt); MakeList(&opt.input, in, ni);
    Opt keep = opt;
    BVec solution; MakeList(&solution, sol, ns);
    BVec solKeep = solution;
    g_mapCopies = g_treeAssigns = g_destroyed = 0;
    CallRow(&opt, 0, &solution);
    CHECK(memcmp(&solution, &solKeep, sizeof solution) == 0);
    CHECK(memcmp(opt.rest, keep.rest, sizeof opt.rest) == 0);
    CHECK(opt.input.finish - opt.input.start == ne);
    for (int k = 0; k < ne && k < (int)(opt.input.finish - opt.input.start); ++k) {
        const Block& got = opt.input.start[k]; const Block& want = in[expect[k]];
        CHECK(got.map.header == want.map.header);
        CHECK(got.groups.finish - got.groups.start == inN[expect[k]]);
        CHECK(memcmp(got.groups.start, inKeys[expect[k]], sizeof(unsigned long) * inN[expect[k]]) == 0);   // the input buffers are reused by operator=
    }
    CHECK(g_mapCopies >= ne);
}

int main()
{
    static const unsigned long a[] = {1, 2}, b[] = {3}, c[] = {2, 5, 9}, d[] = {1, 2, 3}, e[] = {7}, f[] = {2};
    static const unsigned long s1[] = {1, 2, 3}, s2[] = {2, 5, 7, 9}, s3[] = {1, 2, 3, 5, 7, 9, 11};
    // 1: input {a b c d e f}; solution {s1 s2}: s1 takes a, b, d, f (input order); s2 takes c, e (f already taken)
    { const unsigned long* ik[] = {a, b, c, d, e, f}; int in[] = {2, 1, 3, 3, 1, 1};
      const unsigned long* sk[] = {s1, s2}; int sn[] = {3, 4}; int ex[] = {0, 1, 3, 5, 2, 4};
      Run(ik, in, 6, sk, sn, 2, ex, 6); }
    // 2: solution order reversed: s2 takes c, e, f first, then s1 takes a, b, d
    { const unsigned long* ik[] = {a, b, c, d, e, f}; int in[] = {2, 1, 3, 3, 1, 1};
      const unsigned long* sk[] = {s2, s1}; int sn[] = {4, 3}; int ex[] = {2, 4, 5, 0, 1, 3};
      Run(ik, in, 6, sk, sn, 2, ex, 6); }
    // 3: a block bigger than every solution block is dropped; a superset solution takes everything left
    { const unsigned long* ik[] = {s3, b, e}; int in[] = {7, 1, 1};
      const unsigned long* sk[] = {s1, s2}; int sn[] = {3, 4}; int ex[] = {1, 2};
      Run(ik, in, 3, sk, sn, 2, ex, 2); }
    // 4: empty solution: the input list becomes empty
    { const unsigned long* ik[] = {a, b}; int in[] = {2, 1};
      const unsigned long* sk[] = {s1}; int sn[] = {3};
      Run(ik, in, 2, sk, sn, 0, 0, 0); }
    // 5: empty input
    { const unsigned long* ik[] = {a}; int in[] = {2};
      const unsigned long* sk[] = {s1, s3}; int sn[] = {3, 7};
      Run(ik, in, 0, sk, sn, 2, 0, 0); }
    // 6: equal sets count as subsets; many blocks force the insert-overflow growth path several times
    { const unsigned long* ik[] = {s1, a, s1, b, f, d, s1, a}; int in[] = {3, 2, 3, 1, 1, 3, 3, 2};
      const unsigned long* sk[] = {s3}; int sn[] = {7}; int ex[] = {0, 1, 2, 3, 4, 5, 6, 7};
      Run(ik, in, 8, sk, sn, 1, ex, 8); }
    if (!g_fail) printf("00ad9980_TEST PASS\n");
    return g_fail;
}
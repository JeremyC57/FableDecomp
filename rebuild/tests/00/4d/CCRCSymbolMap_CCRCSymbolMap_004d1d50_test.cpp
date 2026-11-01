// Behaviour test for 0x004D1D50 CCRCSymbolMap::~CCRCSymbolMap (implicit): the NamesUsed set is cleared and its header freed,
// then the SLONGMap entry block is freed, then ~CBaseClassNonCopyable runs.
#include <cstdio>
#include <cstdlib>
static const void* g_log[16]; static int g_n = 0; static int g_erased = 0; static int g_baseAt = -1;
static void Free(void* p) { if (p) { g_log[g_n++] = p; std::free(p); } }
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; unsigned long value; };
static void Erase(NodeBase* x) { while (x) { Erase(x->right); NodeBase* l = x->left; std::free(x); ++g_erased; x = l; } }
struct Set {
    NodeBase* header; unsigned count; unsigned char compare;
    ~Set() { if (count != 0) { Erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } Free(header); }
};
struct Pair { unsigned long first; long second; };
struct CVectorMap { Pair* start; Pair* finish; Pair* eos; unsigned char compare; bool Dirty; ~CVectorMap() { if (start != 0) Free(start); } };
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { g_baseAt = g_n; } };
class CCRCSymbolMap : public CBaseClassNonCopyable { public: CVectorMap SLONGMap; Set NamesUsed; bool CheckForDuplicateSymbols; };
int main()
{
    CCRCSymbolMap* m = new CCRCSymbolMap;
    m->NamesUsed.header = (NodeBase*)std::malloc(sizeof(NodeBase));
    NodeBase* n = (NodeBase*)std::malloc(sizeof(NodeBase)); n->left = n->right = 0;
    m->NamesUsed.header->parent = n; m->NamesUsed.count = 1;
    m->SLONGMap.start = (Pair*)std::malloc(4 * sizeof(Pair));
    const void* header = m->NamesUsed.header; const void* block = m->SLONGMap.start;
    delete m;
    if (g_erased != 1 || g_n != 2 || g_log[0] != header || g_log[1] != block || g_baseAt != 2) {
        std::printf("004d1d50_TEST FAIL\n"); return 1;
    }
    std::printf("004d1d50_TEST PASS\n");
    return 0;
}
// Behaviour test for retail 0x00ad7400: the candidate source below is embedded verbatim (from src/00ad7400.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// QuadricOptimiserInternals::COptimisedVertex::~COptimisedVertex() -- destroy the three STLport tree members in reverse
//   declaration order: PairContractions, Groups, then FaceList's index set; each clears its nodes, resets the header
//   links and count, and frees the header node
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD7400, 197 bytes.
// Leaf: ??1COptimisedVertex
// Identity: the scalar deleting destructor at 0x00AD79C0 calls it, the COptimisedVertex vector helpers around it destroy
//   elements with it, and its three trees sit exactly where the PDB COptimisedVertex members land in the retail layout
//   (FaceList +0x84, Groups +0x9c, PairContractions +0xa8; PDB +0x84/+0xa0/+0xb0 with 16-byte trees, retail trees are
//   12 bytes). The manifest name "CWound::Kill" is propagated.
// Callees: each tree's recursive node erase -- set<ulong> 0x004CF810, map<ulong,float> 0x00AD14E0,
//   map<ulong,CContractionReference> 0x00AD5240 (labels propagated) -- and the CRT free import 0x00BFEA14.
// Layout: PDB (Ego_r) names; the members before FaceList (Position .. Cost) are opaque here. Local model: no engine/ header.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
namespace QuadricOptimiserInternals {
struct CContractionReference { void* _opaque[2]; };   // contents not needed here (size unproven)
template<class V> struct CRbTreeNode { long color; CRbTreeNode* parent; CRbTreeNode* left; CRbTreeNode* right; V value; };
template<class V> struct CRbTree {                      // STLport _Rb_tree: header node, node count, (empty) compare
    CRbTreeNode<V>* header;
    unsigned long node_count;
    long compare;
    void Erase(CRbTreeNode<V>* root);                   // per-instance recursive node erase
    ~CRbTree() {
        if (node_count) {
            Erase(header->parent);
            header->left = header; header->parent = 0; header->right = header;
            node_count = 0;
        }
        if (header) free(header);
    }
};
template<class K, class V> struct CPair { K first; V second; };
struct CIndexList { CRbTree<unsigned long> Indices; };   // set<unsigned long>
struct COptimisedVertex {
    unsigned char Position_to_Cost[0x84];               // +0x00..+0x83 (Position, Normal, TextureCoord, PositionError, Cost)
    CIndexList FaceList;                                // +0x84
    bool Exists;                                        // +0x90
    unsigned long UniqueIndex;                          // +0x94
    unsigned long OutputIndex;                          // +0x98
    CRbTree<CPair<const unsigned long, float> > Groups;                          // +0x9c
    CRbTree<CPair<const unsigned long, CContractionReference> > PairContractions;  // +0xa8
    ~COptimisedVertex();
};
COptimisedVertex::~COptimisedVertex() {}
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
using namespace QuadricOptimiserInternals;
static int g_seq[16], g_n; static void* g_freed[8]; static int g_nf, g_armed;
extern "C" void __cdecl free(void* p) { if (g_armed) { g_freed[g_nf++] = p; g_seq[g_n++] = 100 + g_nf; } }
template<class V> void CRbTree<V>::Erase(CRbTreeNode<V>* root) { g_seq[g_n++] = sizeof(V); (void)root; }   /* records which tree (by value size) */
template<class V> static void setup(CRbTree<V>& t, CRbTreeNode<V>* hdr, CRbTreeNode<V>* root, unsigned long count) {
    t.header = hdr; t.node_count = count; hdr->parent = root; hdr->left = root ? root : hdr; hdr->right = root ? root : hdr; }
int main() {
    int ok = offsetof(COptimisedVertex, FaceList) == 0x84 && offsetof(COptimisedVertex, Groups) == 0x9c && offsetof(COptimisedVertex, PairContractions) == 0xa8;
    static unsigned char buf[sizeof(COptimisedVertex)]; memset(buf, 0, sizeof buf);
    COptimisedVertex* v = (COptimisedVertex*)buf;
    typedef CRbTreeNode<CPair<const unsigned long, float> > N2; typedef CRbTreeNode<CPair<const unsigned long, CContractionReference> > N3;
    static CRbTreeNode<unsigned long> h1, r1; static double raw[3][8]; memset(raw, 0, sizeof raw);   /* const keys: no default ctor */
    N2& h2 = *(N2*)raw[0]; N3& h3 = *(N3*)raw[1]; N3& r3 = *(N3*)raw[2];
    setup(v->FaceList.Indices, &h1, &r1, 3); setup(v->Groups, &h2, (CRbTreeNode<CPair<const unsigned long, float> >*)0, 0); setup(v->PairContractions, &h3, &r3, 1);
    g_armed = 1; v->~COptimisedVertex(); g_armed = 0;
    /* PairContractions (value 12 bytes) erased + freed, Groups (empty) only freed, FaceList (value 4) erased + freed */
    ok = ok && g_n == 5 && g_seq[0] == (int)sizeof(CPair<const unsigned long, CContractionReference>) && g_seq[1] == 101 && g_seq[2] == 102
            && g_seq[3] == 4 && g_seq[4] == 103;
    ok = ok && g_freed[0] == &h3 && g_freed[1] == &h2 && g_freed[2] == &h1;
    ok = ok && h3.left == &h3 && h3.right == &h3 && h3.parent == 0 && v->PairContractions.node_count == 0 && h1.left == &h1 && v->FaceList.Indices.node_count == 0;
    printf(ok ? "00ad7400_TEST PASS\n" : "00ad7400_TEST FAIL\n"); return ok ? 0 : 1;
}
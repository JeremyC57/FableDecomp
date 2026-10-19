// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test uses a local Tree::clear; propagated CWound::Kill name contradicted by its own comment)
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
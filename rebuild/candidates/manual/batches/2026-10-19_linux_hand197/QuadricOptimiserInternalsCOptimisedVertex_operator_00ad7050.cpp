// QuadricOptimiserInternals::COptimisedVertex::operator=(const COptimisedVertex&) -- the compiler-generated member-wise assignment of an optimiser vertex (positions, quadric, cost, face set, flags, group and pair-contraction maps)
// Leaf: ??4COptimisedVertex
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD7050, 196 bytes (+ int3 padding; `ret 4`, returns `this`).
// Label correction: catalogued as the COptimisedVertex constructor; the callees are the trees' assignment operators, so this is
//   operator= (its sibling 0x00AD77F0, calling the trees' copy constructors, is the copy constructor).
// Callees: set<unsigned long>::operator= 0x00AD3720 (CIndexList::Indices; landed as CRetailTree_00ad3230::operator=),
//   map<unsigned long, float>::operator= 0x00AD3E30 (Groups), map<unsigned long, CContractionReference>::operator= 0x00AD6140
//   (PairContractions).
// Layout: Ego_r.pdb COptimisedVertex with STLport 12-byte trees (0xb8 bytes): Position +0, Normal +0xc, TextureCoord +0x18,
//   PositionError +0x28 (CQuadric, eleven doubles), Cost +0x80, FaceList +0x84, Exists +0x90, UniqueIndex +0x94, OutputIndex
//   +0x98, Groups +0x9c, PairContractions +0xa8.
// Shape: nothing is user-written: the three C3DVector assignments use the shared-destination form, the quadric is a 22-dword
//   `rep movsd`, the trees call their own operator=. A one-line assignment in the TU makes VC emit it.
struct C3DVector {                                 // engine C3DVector: user copy ctor, implicit assignment
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
namespace std {
template <class T> struct less { };
template <class T> struct allocator { };
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class K, class C = less<K>, class A = allocator<K> > struct set {
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    set(const set& other);                         // 0x00AD3A10 for set<unsigned long>
    set& operator=(const set& other);              // 0x00AD3720 for set<unsigned long> (landed placeholder CRetailTree_00ad3230)
};
template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > struct map {
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    map(const map& other);
    map& operator=(const map& other);              // 0x00AD3E30 (Groups) / 0x00AD6140 (PairContractions)
};
}
namespace QuadricOptimiserInternals {
struct CQuadric {                                  // eleven doubles, 0x58 bytes
    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight;
};
struct CIndexList {                                // 12 bytes (STLport set)
    std::set<unsigned long> Indices;
};
struct CContractionReference { unsigned long Index; long Count; };
struct COptimisedVertex {                          // retail 0xb8 bytes (Ego_r.pdb members, STLport 12-byte trees)
    C3DVector     Position;                        // +0x00
    C3DVector     Normal;                          // +0x0c
    C3DVector     TextureCoord;                    // +0x18
    CQuadric      PositionError;                   // +0x28 (8-aligned)
    float         Cost;                            // +0x80
    CIndexList    FaceList;                        // +0x84
    bool          Exists;                          // +0x90
    unsigned long UniqueIndex;                     // +0x94
    unsigned long OutputIndex;                     // +0x98
    std::map<unsigned long, float> Groups;         // +0x9c
    std::map<unsigned long, CContractionReference> PairContractions;   // +0xa8
};
void AssignVertex(COptimisedVertex& to, const COptimisedVertex& from) { to = from; }
}

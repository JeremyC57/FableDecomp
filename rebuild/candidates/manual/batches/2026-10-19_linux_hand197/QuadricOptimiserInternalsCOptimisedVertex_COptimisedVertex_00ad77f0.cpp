// QuadricOptimiserInternals::COptimisedVertex::COptimisedVertex(const COptimisedVertex&) -- the compiler-generated member-wise copy constructor of an optimiser vertex
// Leaf: ??0COptimisedVertex
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD77F0, 187 bytes (`ret 4`, returns `this`).
// Callees: set<unsigned long>::set(const set&) 0x00AD3A10 (CIndexList::Indices), map<unsigned long, float>::map(const map&)
//   0x00AD4210 (Groups), map<unsigned long, CContractionReference>::map(const map&) 0x00AD5920 (PairContractions).
// Layout: as the assignment operator 0x00AD7050 (Ego_r.pdb COptimisedVertex with STLport 12-byte trees, 0xb8 bytes).
// Shape: nothing is user-written: the C3DVector members copy dword by dword, the quadric is a 22-dword `rep movsd`, the trees
//   are copy-constructed in place. A placement copy in the TU makes VC emit it.
#include <new>
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
};
template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > struct map {
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    map(const map& other);                         // 0x00AD4210 (Groups) / 0x00AD5920 (PairContractions)
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
void CopyVertex(void* to, const COptimisedVertex& from) { new (to) COptimisedVertex(from); }
}

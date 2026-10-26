#include <cstdio>
#include <cstring>
#include <new>
// Behaviour test for 00ad7050 COptimisedVertex assignment: every plain member (vectors, quadric, cost, flags, indices) is copied and each
// tree member goes through its own operator= exactly once.
static int g_setCopies = 0, g_setAssigns = 0, g_mapCopies = 0, g_mapAssigns = 0;
class C3DVector { public:                                 // engine C3DVector: user copy ctor, implicit assignment
    float X, Y, Z;
    C3DVector() { }
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
    set() : _M_header(0), _M_node_count(0) { }
    set(const set& other) : _M_header(other._M_header), _M_node_count(other._M_node_count + 100) { ++g_setCopies; }
    set& operator=(const set& other) { _M_header = other._M_header; _M_node_count = other._M_node_count + 100; ++g_setAssigns; return *this; }
};
template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > struct map {
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    map() : _M_header(0), _M_node_count(0) { }
    map(const map& other) : _M_header(other._M_header), _M_node_count(other._M_node_count + 200) { ++g_mapCopies; }
    map& operator=(const map& other) { _M_header = other._M_header; _M_node_count = other._M_node_count + 200; ++g_mapAssigns; return *this; }
};
}
namespace QuadricOptimiserInternals {
class CQuadric { public:                                  // eleven doubles, 0x58 bytes
    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight;
};
class CIndexList { public:                                // 12 bytes (STLport set)
    std::set<unsigned long> Indices;
};
class CContractionReference { public: unsigned long Index; long Count; };
class COptimisedVertex { public:                          // retail 0xb8 bytes (Ego_r.pdb members, STLport 12-byte trees)
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

using namespace QuadricOptimiserInternals;
int main()
{
    static COptimisedVertex src;
    std::memset(&src, 0, sizeof(src));
    src.Position.X = 1; src.Position.Y = 2; src.Position.Z = 3; src.Normal.X = 4; src.Normal.Y = 5; src.Normal.Z = 6;
    src.TextureCoord.X = 7; src.TextureCoord.Y = 8; src.TextureCoord.Z = 9;
    src.PositionError.AA = 10.5; src.PositionError.DD = -2.25; src.PositionError.Weight = 3.0;
    src.Cost = 11.0f; src.Exists = true; src.UniqueIndex = 12; src.OutputIndex = 13;
    src.FaceList.Indices._M_node_count = 1; src.Groups._M_node_count = 2; src.PairContractions._M_node_count = 3;
    static COptimisedVertex dst;
    std::memset(&dst, 0, sizeof(dst));
    AssignVertex(dst, src);
    bool ok = g_setAssigns == 1 && g_mapAssigns == 2 && g_setCopies == 0 && g_mapCopies == 0;
    ok = ok && dst.Position.X == 1 && dst.Position.Z == 3 && dst.Normal.Y == 5 && dst.TextureCoord.Z == 9;
    ok = ok && dst.PositionError.AA == 10.5 && dst.PositionError.DD == -2.25 && dst.PositionError.Weight == 3.0;
    ok = ok && dst.Cost == 11.0f && dst.Exists && dst.UniqueIndex == 12 && dst.OutputIndex == 13;
    ok = ok && dst.FaceList.Indices._M_node_count == 101 && dst.Groups._M_node_count == 202 && dst.PairContractions._M_node_count == 203;
    if (!ok) { std::printf("00ad7050_TEST FAIL\n"); return 1; }
    std::printf("00ad7050_TEST PASS\n");
    return 0;
}
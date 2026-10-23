// COptimisedPrimitive::DetectFoldOver(CSubContraction*, C3DVector*) -- true when moving the sub-contraction's vertex pair to the position would flip any live face touching either vertex (the position crosses the plane through the face's opposite edge, perpendicular to the face)
// Grade: DIFFER(1048v1041) -- set merge, rb-tree walk, inline set dtor, both guarded fast Normalises and the cross products match; left: VC threads the not-found -1 of the first inline vertex lookup straight into the second (retail keeps or eax,-1 / cmp eax,-1), and x87 operand order in the side-plane scaling and the final dot products
// Leaf: DetectFoldOver
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ACD0E0, 1022 bytes (+ int3 padding), `ret 8`.
// Callees: the merged face-index set of the two vertices 0x00ACA380 (thiscall on the first vertex's set, result by hidden
//   pointer; forge autoname "QuadricOptimiser_DetectFoldOver_BuildTree" is low trust), STLport set<ulong> _M_erase 0x004CF810
//   (the landed CTwoLongSets dtor model 0x00AD4A70), free 0x00BFEA14. Globals: epsilon 0x0129BA3C (squared), 3.0f 0x0122DED4,
//   0.5f 0x0122F59C, the reciprocal square root mantissa table 0x013CE558.
// Caller: the landed DetectFoldOver(CVertexPairContraction*) 0x00ACD900.
// Layout: retail COptimisedPrimitive Vertices first +0x10 (0xb8-byte vertices: Position +0, face set +0x84), Faces first +0x1c
//   (0x48-byte faces: VertexIndices +0, Exists +0x3c), as in the landed FindFaceTextureTangents 0x00AC7E10.
extern "C" void free(void* p);                     // 0x00BFEA14
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern const float g_fTangentEpsilon;               // 0x0129BA3C

namespace QuadricOptimiserInternals {
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline void SafeNormalise()
    {
        if (X * X + Y * Y + Z * Z > g_fTangentEpsilon * g_fTangentEpsilon) {
            float lengthSquared = X * X;
            lengthSquared += Y * Y;
            lengthSquared += Z * Z;
            unsigned long bits = *(unsigned long*)&lengthSquared;
            unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                     (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
            float r = *(float*)&estimate;
            r = (3.0f - r * r * lengthSquared) * r;
            r *= 0.5f;
            X *= r;
            Y *= r;
            Z *= r;
        }
    }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
inline float DotProduct(const C3DVector& a, const C3DVector& b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

struct CSubContraction {
    unsigned long Index1;
    unsigned long Index2;
};

struct CRbNode {
    char          Color;                            // +0x00
    CRbNode*      Parent;                           // +0x04
    CRbNode*      Left;                             // +0x08
    CRbNode*      Right;                            // +0x0C
    unsigned long Value;                            // +0x10
};
struct CLess {};
struct CFaceSet {                                   // STLport set<unsigned long>
    CRbNode*     _M_header;                         // +0x00
    unsigned int _M_node_count;                     // +0x04
    CLess        _M_key_compare;                    // +0x08
    void _M_erase(CRbNode* x);                      // 0x004CF810
    CFaceSet MergedWith(const CFaceSet& other) const;   // 0x00ACA380
    __forceinline ~CFaceSet()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->Parent);
            _M_header->Left = _M_header;
            _M_header->Parent = 0;
            _M_header->Right = _M_header;
            _M_node_count = 0;
        }
        free(_M_header);
    }
};
__forceinline CRbNode* NextNode(CRbNode* node)
{
    if (node->Right != 0) {
        node = node->Right;
        while (node->Left != 0)
            node = node->Left;
    } else {
        CRbNode* parent = node->Parent;
        while (node == parent->Right) {
            node = parent;
            parent = parent->Parent;
        }
        if (node->Right != parent)
            node = parent;
    }
    return node;
}

struct COptimisedVertex {                           // retail 0xb8 bytes
    C3DVector     Position;                         // +0x00
    unsigned char _pad[0x84 - 0x0c];
    CFaceSet      Faces;                            // +0x84
    unsigned char _rest[0xb8 - 0x90];
};
struct COptimisedFace {                             // 0x48 bytes
    unsigned long VertexIndices[3];                 // +0x00
    unsigned char _pad[0x3c - 0x0c];
    bool          Exists;                           // +0x3c
    unsigned char _rest[0x48 - 0x3d];
    unsigned long FindVertex(unsigned long vertex) const
    {
        for (unsigned long i = 0; i < 3; ++i)
            if (VertexIndices[i] == vertex)
                return i;
        return (unsigned long)-1;
    }
};
template <class T> struct CRetailVector {           // retail std::vector (12 bytes)
    T* m_first;
    T* m_last;
    T* m_end;
    T& operator[](unsigned long n) { return m_first[n]; }
};
}

using namespace QuadricOptimiserInternals;

struct COptimisedPrimitive {
    unsigned char                   _pad[0x10];
    CRetailVector<COptimisedVertex> Vertices;       // +0x10
    CRetailVector<COptimisedFace>   Faces;          // +0x1c
    bool DetectFoldOver(CSubContraction* subContraction, C3DVector* position);
};

bool COptimisedPrimitive::DetectFoldOver(CSubContraction* subContraction, C3DVector* position)
{
    CFaceSet faces = Vertices[subContraction->Index1].Faces.MergedWith(Vertices[subContraction->Index2].Faces);
    for (CRbNode* node = faces._M_header->Left; node != faces._M_header; node = NextNode(node)) {
        COptimisedFace& face = Faces[node->Value];
        if (!face.Exists)
            continue;
        unsigned long corner = face.FindVertex(subContraction->Index1);
        if (corner == (unsigned long)-1)
            corner = face.FindVertex(subContraction->Index2);
        C3DVector& p0 = Vertices[face.VertexIndices[0]].Position;
        C3DVector& p1 = Vertices[face.VertexIndices[1]].Position;
        C3DVector& p2 = Vertices[face.VertexIndices[2]].Position;
        C3DVector normal = CrossProduct(p2 - p0, p1 - p0);
        normal.SafeNormalise();
        C3DVector& edgeStart = Vertices[face.VertexIndices[(corner + 1) % 3]].Position;
        C3DVector& edgeEnd = Vertices[face.VertexIndices[(corner + 2) % 3]].Position;
        C3DVector side = CrossProduct(normal, edgeEnd - edgeStart);
        side.SafeNormalise();
        if (DotProduct(side, *position) > DotProduct(side, edgeStart))
            return true;
    }
    return false;
}

// QuadricOptimiserInternals::COptimisedPrimitive::ComputeVertexCost(COptimisedVertex& vertex) -- texture-distortion cost of a vertex from the spread of its faces' tangents in the vertex's tangent plane
// Leaf: ?ComputeVertexCost@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCOptimisedVertex@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC8C80, 1597 bytes, thiscall `ret 4`, 0x90-byte frame, no EH frame.
// Identity: callers 0x00ACB83F (COptimisedPrimitive::Initialise, once per 0xB8-byte vertex) and 0x00ACD098
//   (PerformSubContraction, when a contraction is unsplit) pass one COptimisedVertex&; the body writes only Cost (+0x80)
//   from Parameters_TexturePenalty (+0x70) / Parameters_TextureWeight (+0x6C). Ego_r.pdb and FableWin.pdb both list
//   ?ComputeVertexCost@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCOptimisedVertex@2@@Z, the only private
//   void (COptimisedVertex&) member of the class. The catalog name (forge autoname) is replaced by it.
// Callees: GetFaceNormal 0x00AC7CB0 (?GetFaceNormal@...@ABE?AVC3DVector@@ABVCOptimisedFace@2@@Z), MakeOrthogonal 0x00AC7200
//   (?MakeOrthogonal@...@ABE?AVC3DVector@@ABV3@0@Z, a length-preserving projection onto the plane of `normal`; catalog
//   placeholder Vector_ProjectOntoPlane), vector<C3DVector>::reserve 0x00920FF0 and _M_insert_overflow 0x0066E370,
//   set<unsigned long> iterator operator++(int) 0x00AD0A70 (landed placeholder CUnprovenValue_00ad0a70), _free.
// Opt-ins: FABLE_RETAIL_STLP_VECTOR_RESERVE (a visible noinline STLport 4.6 reserve body, so VC7.1 keeps the local
//   vector's _M_finish in a register across the push_back loop) and FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN (retail's
//   Normalise / IsNonZero sum order X*X+Z*Z+Y*Y, 0x00AC8DCA..0x00AC8E01); see hdr/README.md.
// Shape: GetFaceNormal and MakeOrthogonal carry their real bodies (noinline): VC7.1 reads a same-TU callee's register
//   clobbers, and retail keeps edx live across MakeOrthogonal (whose body uses eax/ecx only). The tangent lengths are
//   spelled in-row as sqrt(Y*Y+Z*Z+X*X) (retail loads X, Z, Y; an inline GetMagnitude canonicalises the other way). A
//   separate `start` iterator copied into the loop iterator keeps `this` in edi and avoids a store-back; a `faces` base
//   pointer local matches retail's one Faces load. The `normals` vector is filled but never read (retail does the same).
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
#include "stlport/fable_map.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#define COptimisedPrimitive COptimisedPrimitiveFields
#include "engine/COptimisedPrimitive.h"
#undef COptimisedPrimitive

// STLport 4.6 _tree.h `_Self operator++(int)`: retail calls the out-of-line set<unsigned long> iterator copy 0x00AD0A70.
template<> std::_Rb_tree_iterator<unsigned long, std::_Const_traits<unsigned long> >
std::_Rb_tree_iterator<unsigned long, std::_Const_traits<unsigned long> >::operator++(int);

inline float __fastcall GFFloatAbs(float f)
{
    *(unsigned long*)&f &= 0x7fffffff;
    return f;
}

namespace QuadricOptimiserInternals {

class CQuadric {
public:
    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight;
};

class CIndexList {
public:
    std::set<unsigned long> Indices;
};

class COptimisedFace {
public:
    unsigned long VertexIndices[3];         // +0x00
    unsigned long UniqueVertexIndices[3];   // +0x0c
    long          Edges[3];                 // +0x18
    C3DVector     UTangent;                 // +0x24
    C3DVector     VTangent;                 // +0x30
    bool          Exists;                   // +0x3c
    unsigned long OutputIndex;              // +0x40
    long          Segment;                  // +0x44
};

class COptimisedVertex {
public:
    C3DVector     Position;
    C3DVector     Normal;
    C3DVector     TextureCoord;
    CQuadric      PositionError;
    float         Cost;
    CIndexList    FaceList;
    bool          Exists;
    unsigned long UniqueIndex;
    unsigned long OutputIndex;
    unsigned char Groups[0x0C];
    unsigned char PairContractions[0x0C];
};

class COptimisedPrimitive : public COptimisedPrimitiveFields {
    C3DVector GetFaceNormal(const COptimisedFace& face) const;                          // 0x00AC7CB0
    C3DVector MakeOrthogonal(const C3DVector& vector, const C3DVector& normal) const;    // 0x00AC7200
    void ComputeVertexCost(COptimisedVertex& vertex);
};

__declspec(noinline) C3DVector COptimisedPrimitive::GetFaceNormal(const COptimisedFace& face) const
{
    const COptimisedVertex& vertex0 = reinterpret_cast<const CArray<COptimisedVertex>&>(Vertices)[face.VertexIndices[0]];
    const COptimisedVertex& vertex1 = reinterpret_cast<const CArray<COptimisedVertex>&>(Vertices)[face.VertexIndices[1]];
    const COptimisedVertex& vertex2 = reinterpret_cast<const CArray<COptimisedVertex>&>(Vertices)[face.VertexIndices[2]];
    C3DVector edge1 = vertex1.Position - vertex0.Position;
    C3DVector edge2 = vertex2.Position - vertex0.Position;
    C3DVector normal = edge1 ^ edge2;
    if (normal.IsNonZero())
        normal.Normalise();
    return normal;
}

__declspec(noinline) C3DVector COptimisedPrimitive::MakeOrthogonal(const C3DVector& vector, const C3DVector& normal) const
{
    float magnitude = vector.GetMagnitude();
    if (GFFloatAbs(magnitude) > DELTA_NOTIONAL_ZERO) {
        C3DVector result = vector - normal * vector.Dot(normal);
        float squared = result.GetSquaredMagnitude();
        if (squared > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            result *= magnitude / (float)sqrt(squared);
        return result;
    }
    return C3DVector(vector.X, vector.Y, vector.Z);
}

void COptimisedPrimitive::ComputeVertexCost(COptimisedVertex& vertex)
{
    float distortion = 0.0f;
    if (vertex.FaceList.Indices.size() != 0) {
        C3DVector normal(0.0f, 0.0f, 0.0f);
        CArray<C3DVector> normals;
        normals.reserve(vertex.FaceList.Indices.size());
        for (std::set<unsigned long>::iterator it = vertex.FaceList.Indices.begin(); it != vertex.FaceList.Indices.end(); ++it) {
            C3DVector face_normal = GetFaceNormal(reinterpret_cast<CArray<COptimisedFace>&>(Faces)[*it]);
            normal += face_normal;
            normals.push_back(face_normal);
        }
        if (normal.IsNonZero())
            normal.Normalise();

        {
        std::set<unsigned long>::iterator end = vertex.FaceList.Indices.end();
        std::set<unsigned long>::iterator start = vertex.FaceList.Indices.begin();
        const COptimisedFace* faces = &reinterpret_cast<CArray<COptimisedFace>&>(Faces)[0];
        const COptimisedFace& first = faces[*start];
        C3DVector u = MakeOrthogonal(first.UTangent, normal);
        C3DVector v = MakeOrthogonal(first.VTangent, normal);
        C3DVector u_min = u;
        C3DVector u_max = u;
        C3DVector v_min = v;
        C3DVector v_max = v;
        float u_length = (float)sqrt(u.Y * u.Y + u.Z * u.Z + u.X * u.X);
        float v_length = (float)sqrt(v.Y * v.Y + v.Z * v.Z + v.X * v.X);
        start++;
        for (std::set<unsigned long>::iterator face_it = start; face_it != end; ++face_it) {
            const COptimisedFace& face = faces[*face_it];
            u = MakeOrthogonal(face.UTangent, normal);
            v = MakeOrthogonal(face.VTangent, normal);
            u_min.LimitUpper(u);
            u_max.LimitLower(u);
            v_min.LimitUpper(v);
            v_max.LimitLower(v);
            float length = (float)sqrt(u.Y * u.Y + u.Z * u.Z + u.X * u.X);
            if (u_length < length) u_length = length;
            length = (float)sqrt(v.Y * v.Y + v.Z * v.Z + v.X * v.X);
            if (v_length < length) v_length = length;
        }
        C3DVector u_range = u_max - u_min;
        C3DVector v_range = v_max - v_min;
        if (GFFloatAbs(u_length) > DELTA_NOTIONAL_ZERO) {
            float ratio = (float)sqrt(u_range.Y * u_range.Y + u_range.Z * u_range.Z + u_range.X * u_range.X) / u_length;
            if (distortion < ratio) distortion = ratio;
        }
        if (GFFloatAbs(v_length) > DELTA_NOTIONAL_ZERO) {
            float ratio = (float)sqrt(v_range.Y * v_range.Y + v_range.Z * v_range.Z + v_range.X * v_range.X) / v_length;
            if (distortion < ratio) distortion = ratio;
        }
        }
    }
    float squared = distortion * distortion;
    if (squared > 1.0f)
        vertex.Cost = Parameters_TexturePenalty;
    vertex.Cost = squared * Parameters_TextureWeight;
}

}
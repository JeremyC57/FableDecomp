// QuadricOptimiserInternals::COptimisedPrimitive::PerformSubContraction(const CSubContraction& sub, const CVertexPairContraction& contraction) -- merge vertex Index2 into vertex Index1
// Grade: DIFFER(1282v1264) -- 139 masked diff lines (best m1 of 16 logged grades): same calls and blocks; vertex1/vertex2 swap registers (retail ebx=vertex1, edi=vertex2), the map loop keeps the key in ebp from node+0x10 before the refcount increment where ours reloads entry.first, and the normal renormalise needs the locals form (header Normalise loads Z,Y,X). Next idea: iterate the multimap with a separate `current` iterator / `(*pair).first` read before the copy, and declaration orders that put vertex2 first.
// Leaf: ?PerformSubContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXABVCSubContraction@2@ABVCVertexPairContraction@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ACCBF0, 1264 bytes, thiscall `ret 8`. Ego_r.pdb ?PerformSubContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXABVCSubContraction@2@ABVCVertexPairContraction@2@@Z.
// Headers: FABLE_RETAIL_STLP_TREE_ERASE / FABLE_RETAIL_STLP_TREE_POSTINC (stlport/fable_map.h overlay, chunk 778) and the visible STLport
//   map::operator[] (FABLE_RETAIL_STLP_MAP). The local CIndexList / COptimisedFace / COptimisedVertex models are the landed siblings' bodies
//   (same TU: RemapIndex, IsDegenerate, HasPairContraction noinline, RemovePairContraction / Kill inline).
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_C3DVECTOR
#include "stlport/fable_map.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#define COptimisedPrimitive COptimisedPrimitiveFields
#include "engine/COptimisedPrimitive.h"
#undef COptimisedPrimitive

namespace QuadricOptimiserInternals {

class COptimisedPrimitive;

class CQuadric {
public:
    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight;
    void operator+=(const CQuadric& other);
    void operator*=(double scale);
};

__declspec(noinline) void CQuadric::operator+=(const CQuadric& other)
{
    AA += other.AA; AB += other.AB; AC += other.AC; AD += other.AD;
    BB += other.BB; BC += other.BC; BD += other.BD;
    CC += other.CC; CD += other.CD;
    DD += other.DD;
    Weight += other.Weight;
}

__declspec(noinline) void CQuadric::operator*=(double scale)
{
    AA *= scale; AB *= scale; AC *= scale; AD *= scale;
    BB *= scale; BC *= scale; BD *= scale;
    CC *= scale; CD *= scale;
    DD *= scale;
    Weight *= scale;
}

class CIndexList {
public:
    typedef std::set<unsigned long>::iterator Iterator;
    std::set<unsigned long> Indices;
    Iterator Begin() const { return Indices.begin(); }
    Iterator End() const { return Indices.end(); }
    void AddIndex(unsigned long index) { Indices.insert(index); }
    void Clear() { Indices.clear(); }
    CIndexList GetUnion(const CIndexList& other) const;          // 0x00ACA2B0
};

class COptimisedFace {
public:
    unsigned long VertexIndices[3];
    unsigned long UniqueVertexIndices[3];
    long          Edges[3];
    C3DVector     UTangent;
    C3DVector     VTangent;
    bool          Exists;
    unsigned long OutputIndex;
    long          Segment;
    void RemapIndex(unsigned long from, unsigned long to);
    bool IsDegenerate() const;
};

__declspec(noinline) void COptimisedFace::RemapIndex(unsigned long from, unsigned long to)
{
    if (VertexIndices[0] == from) VertexIndices[0] = to;
    if (VertexIndices[1] == from) VertexIndices[1] = to;
    if (VertexIndices[2] == from) VertexIndices[2] = to;
}

__declspec(noinline) bool COptimisedFace::IsDegenerate() const
{
    if (VertexIndices[0] == VertexIndices[1] || VertexIndices[1] == VertexIndices[2] || VertexIndices[2] == VertexIndices[0])
        return true;
    return false;
}

class CSubContraction {
public:
    unsigned long Index1;
    unsigned long Index2;
};

class CVertexPairContraction;
typedef std::multimap<float, CCountedPointer<CVertexPairContraction> > CContractionList;

class CVertexPairContraction {
public:
    bool                        Exists;
    bool                        Unsplit;
    CArray<CSubContraction>     SubContractions;
    float                       Weight2;
    float                       Cost;
    C3DVector                   ResultingPosition;
    CContractionList::iterator  ListIterator;
    void RemapIndex(unsigned long from, unsigned long to);       // 0x00AC7940
};

class CContractionReference {
public:
    CCountedPointer<CVertexPairContraction> Contraction;
    unsigned long                           SubIndex;
    CContractionReference& operator=(const CContractionReference& other);   // 0x00AD1FC0
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
    std::map<unsigned long, float> Groups;
    std::map<unsigned long, CContractionReference> PairContractions;
    bool HasPairContraction(unsigned long index) const;
    void RemovePairContraction(unsigned long index) { PairContractions.erase(PairContractions.find(index)); }
    void InterpolateGroups(const COptimisedVertex& other, float weight);     // 0x00AC8720
    void UpdatePairContractions(unsigned long index, COptimisedPrimitive& primitive);   // 0x00ACA450
    void Kill() { Exists = false; FaceList.Clear(); PairContractions.clear(); }
};

__declspec(noinline) bool COptimisedVertex::HasPairContraction(unsigned long index) const
{
    return const_cast<std::map<unsigned long, CContractionReference>&>(PairContractions).find(index) !=
           const_cast<std::map<unsigned long, CContractionReference>&>(PairContractions).end();
}

class COptimisedPrimitive : public COptimisedPrimitiveFields {
    void ComputeVertexCost(COptimisedVertex& vertex);            // 0x00AC8C80
    void PerformSubContraction(const CSubContraction& sub, const CVertexPairContraction& contraction);
};

void COptimisedPrimitive::PerformSubContraction(const CSubContraction& sub, const CVertexPairContraction& contraction)
{
    CIndexList::Iterator it;
    unsigned long index1 = sub.Index1;
    unsigned long index2 = sub.Index2;
    if (index1 == index2)
        return;
    COptimisedVertex& vertex1 = reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[index1];
    COptimisedVertex& vertex2 = reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[index2];
    if (!vertex1.Exists)
        return;
    if (!vertex2.Exists)
        return;
    if (!vertex1.HasPairContraction(index2))
        return;
    if (!vertex2.HasPairContraction(index1))
        return;
    it = vertex2.FaceList.Begin();
    while (it != vertex2.FaceList.End()) {
        COptimisedFace& face = reinterpret_cast<CArray<COptimisedFace>&>(Faces)[*it];
        if (face.Exists) {
            face.RemapIndex(index2, index1);
            if (face.IsDegenerate())
                face.Exists = false;
        }
        ++it;
    }
    CIndexList faces = vertex1.FaceList.GetUnion(vertex2.FaceList);
    vertex1.FaceList.Clear();
    for (it = faces.Begin(); it != faces.End(); ++it) {
        if (reinterpret_cast<CArray<COptimisedFace>&>(Faces)[*it].Exists)
            vertex1.FaceList.AddIndex(*it);
    }
    vertex1.RemovePairContraction(index2);
    vertex2.RemovePairContraction(index1);
    for (std::map<unsigned long, CContractionReference>::iterator pair = vertex2.PairContractions.begin(); pair != vertex2.PairContractions.end(); ) {
        std::pair<const unsigned long, CContractionReference> entry = *pair++;
        unsigned long index = entry.first;
        COptimisedVertex& other = reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[index];
        CContractionReference& reference = vertex1.PairContractions[index];
        if (reference.Contraction.IsNull()) {
            entry.second.Contraction->RemapIndex(index2, index1);
            reference = entry.second;
        }
        else if (entry.second.Contraction->Exists) {
            entry.second.Contraction->Exists = false;
            reinterpret_cast<CContractionList&>(VertexPairContractions).erase(entry.second.Contraction->ListIterator);
        }
        other.UpdatePairContractions(index, *this);
    }
    vertex1.Position = contraction.ResultingPosition;
    float weight1 = 1.0f - contraction.Weight2;
    float weight2 = contraction.Weight2;
    vertex1.TextureCoord = vertex1.TextureCoord * weight1 + vertex2.TextureCoord * weight2;
    vertex1.Normal = vertex1.Normal * weight1 + vertex2.Normal * weight2;
    if (vertex1.Normal.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
        { C3DVector& v = vertex1.Normal; float x=v.X,y=v.Y,z=v.Z; float m=x*x+y*y+z*z; float factor = GFFastOneOverSqrt(m); v.X *= factor; v.Y *= factor; v.Z *= factor; }
    vertex1.PositionError += vertex2.PositionError;
    vertex1.PositionError *= 0.5;
    vertex1.InterpolateGroups(vertex2, contraction.Weight2);
    if (contraction.Unsplit)
        ComputeVertexCost(vertex1);
    else
        vertex1.Cost = (vertex1.Cost > vertex2.Cost) ? vertex1.Cost : vertex2.Cost;
    vertex2.Kill();
}

}

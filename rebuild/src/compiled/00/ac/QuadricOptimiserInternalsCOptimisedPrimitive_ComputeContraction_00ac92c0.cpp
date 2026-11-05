// QuadricOptimiserInternals::COptimisedPrimitive::ComputeContraction(CVertexPairContraction& contraction) -- compute a vertex-pair contraction's resulting position, interpolation weight and cost
// Leaf: ?ComputeContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCVertexPairContraction@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC92C0, 1,045 bytes, thiscall `ret 4`, 0xD4-byte frame, no EH frame.
// Identity: Ego_r.pdb ?ComputeContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCVertexPairContraction@2@@Z
//   (private); retail reloc binding of the landed caller COptimisedPrimitive::UpdateContractions 0x00AC96E0 (call at
//   0x00AC9715) names this address. Catalog "_global::COptimisedPrimitive_ContractVertexPair" is descriptive.
// Callees: ComputeSelfContraction 0x00AC7EB0 (Unsplit path), CQuadric::Clear 0x00AC6CE0, CQuadric::operator+= 0x00AC6E80,
//   ComputeOptimalVertexPlacement 0x00AC7750 (C3DVector by hidden pointer), set<unsigned long> copy 0x00AD3A10 (the inline
//   CUniqueVertex copy: Exists byte + two CIndexList sets), GetRemainingVertices 0x00AC8840 (CIndexList by value; the
//   temporary's ~set inline: _M_erase 0x004CF810 + free 0x00BFEA14), CVertexPairContraction::ContainsAllVertices 0x00AC7A90,
//   ComputeSubContractionCost 0x00AC82A0, ~CUniqueVertex 0x00AD4A70 (landed as CTwoLongSets::~CTwoLongSets; PDB ??1CUniqueVertex).
//   Constants 0x0122DEDC 0.0f, 0x0122DED8 1.0f; Weight2's 0.5f is an immediate.
// Layout (retail, STLport 12-byte containers): COptimisedPrimitive UniqueVertices +0x04 (0x1C-byte CUniqueVertex), Vertices
//   +0x10 (0xB8-byte COptimisedVertex: Position +0, PositionError +0x28, UniqueIndex +0x94), Parameters.HighPenalty +0x7C
//   (shared engine/COptimisedPrimitive.h under an alias, as the landed UpdateCost 0x00AC8A60); CVertexPairContraction
//   Unsplit +1, SubContractions +4, Weight2 +0x10, Cost +0x14, ResultingPosition +0x18 (landed UpdateContractions model).
// Shape: both vertex indices and both UniqueIndex values are read before the Unsplit test. The declaration order
//   vertex1, unique1, vertex2, unique2 is what gives retail's register split (this in edi, unique1 in ebp, vertex2's
//   offset in ebx); the order vertex1, vertex2, unique1, unique2 swaps them (45 lines). The cost loop's max uses
//   `!(cost > sub_cost)` (retail `test ah,41h / je`; `cost <= sub_cost` emits `jp`).
// Same-TU bodies (retail keeps them in this TU): CQuadric::Clear / operator+= follow their retail code (landed 0x00AC6CE0 /
//   0x00AC6E80) and stay out of line; with them visible VC7.1 keeps vertex1's offset in edx across both calls and hoists the
//   quadric loop's count, as retail. ComputeSubContractionCost follows retail 0x00AC82A0 (quadric copy, EvaluateError * 0.5,
//   GetGroupDifference * GroupWeight, the larger vertex Cost, HighPenalty when the normals face apart) with EvaluateError's
//   landed body (0x00AC6D50) and a GetGroupDifference stand-in (0x00AC7FF0); declaration-only, VC7.1 treats the cost call as
//   writing the contraction -- it re-indexes SubContractions each pass and keeps the second flag in bl (frame 0xD8).
#define FABLE_RETAIL_STLP_SET_COPY
#include "stlport/fable_map.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#define COptimisedPrimitive COptimisedPrimitiveFields
#include "engine/COptimisedPrimitive.h"
#undef COptimisedPrimitive

namespace QuadricOptimiserInternals {

class CQuadric {
public:
    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight;
    void Clear();
    void operator+=(const CQuadric& other);
    double EvaluateError(const C3DVector& position) const;
};

__declspec(noinline) void CQuadric::Clear()
{
    AA = 0.0; AB = 0.0; AC = 0.0; AD = 0.0; BB = 0.0; BC = 0.0; BD = 0.0; CC = 0.0; CD = 0.0; DD = 0.0; Weight = 0.0;
}

__declspec(noinline) void CQuadric::operator+=(const CQuadric& other)
{
    AA += other.AA; AB += other.AB; AC += other.AC; AD += other.AD;
    BB += other.BB; BC += other.BC; BD += other.BD;
    CC += other.CC; CD += other.CD;
    DD += other.DD;
    Weight += other.Weight;
}

__declspec(noinline) double CQuadric::EvaluateError(const C3DVector& v) const
{
    float x = v.X, y = v.Y, z = v.Z;
    return (z * z) * CC + (y * z * 2.0f) * BC + (y * y) * BB + (x * z * 2.0f) * AC + (x * y * 2.0f) * AB
         + (z * 2.0f) * CD + (y * 2.0f) * BD + (x * 2.0f) * AD + (x * x) * AA + DD;
}

class CIndexList {                                      // Ego_r CIndexList: one STLport set (12 bytes)
public:
    std::set<unsigned long> Indices;                    // +0x00
};

class CUniqueVertex {
public:
    bool       Exists;                                  // +0x00
    CIndexList VertexIndices;                           // +0x04
    CIndexList FaceList;                                // +0x10 (sizeof 0x1C)
    ~CUniqueVertex();                                   // 0x00AD4A70, out of line
};

class CSubContraction {
public:
    unsigned long Vertex1;
    unsigned long Vertex2;
};

class CVertexPairContraction {
public:
    bool                    Exists;
    bool                    Unsplit;
    CArray<CSubContraction> SubContractions;
    float                   Weight2;
    float                   Cost;
    C3DVector               ResultingPosition;
    void*                   ListIterator;               // +0x24 multimap<float, CCountedPointer<...> >::iterator
    bool ContainsAllVertices(const CIndexList& vertices);
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
    float GetGroupDifference(const COptimisedVertex& other) const;   // 0x00AC7FF0
};

// Stand-in for the retail group-map walk (landed 0x00AC7FF0); it writes no memory, like the original.
__declspec(noinline) float COptimisedVertex::GetGroupDifference(const COptimisedVertex& other) const
{
    return (float)fabs(Cost - other.Cost);
}

class COptimisedPrimitive : public COptimisedPrimitiveFields {
    void ComputeSelfContraction(CVertexPairContraction& contraction);
    C3DVector ComputeOptimalVertexPlacement(const CQuadric& quadric, const C3DVector& position1, const C3DVector& position2);
    float ComputeSubContractionCost(const CSubContraction& sub, const C3DVector& position);
    CIndexList GetRemainingVertices(const CIndexList& indices) const;
    void ComputeContraction(CVertexPairContraction& contraction);
};

__declspec(noinline) float COptimisedPrimitive::ComputeSubContractionCost(const CSubContraction& sub, const C3DVector& position)
{
    COptimisedVertex& vertex1 = reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[sub.Vertex1];
    COptimisedVertex& vertex2 = reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[sub.Vertex2];
    CQuadric quadric = vertex1.PositionError;
    quadric += vertex2.PositionError;
    float cost = (float)(quadric.EvaluateError(position) * 0.5);
    cost += vertex1.GetGroupDifference(vertex2) * Parameters_GroupWeight;
    cost += (vertex1.Cost > vertex2.Cost) ? vertex1.Cost : vertex2.Cost;
    if (vertex2.Normal.Dot(vertex1.Normal) < 0.0f)
        cost += Parameters_HighPenalty;
    return cost;
}

void COptimisedPrimitive::ComputeContraction(CVertexPairContraction& contraction)
{
    unsigned long vertex1 = contraction.SubContractions[0].Vertex1;
    unsigned long unique1 = reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[vertex1].UniqueIndex;
    unsigned long vertex2 = contraction.SubContractions[0].Vertex2;
    unsigned long unique2 = reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[vertex2].UniqueIndex;
    if (contraction.Unsplit) {
        ComputeSelfContraction(contraction);
        return;
    }
    CQuadric quadric;
    quadric.Clear();
    for (unsigned long i = 0; i < contraction.SubContractions.size(); ++i) {
        quadric += reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[contraction.SubContractions[i].Vertex1].PositionError;
        quadric += reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[contraction.SubContractions[i].Vertex2].PositionError;
    }
    C3DVector position1 = reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[vertex1].Position;
    C3DVector position2 = reinterpret_cast<CArray<COptimisedVertex>&>(Vertices)[vertex2].Position;
    C3DVector position = ComputeOptimalVertexPlacement(quadric, position1, position2);
    CUniqueVertex unique_vertex1 = reinterpret_cast<CArray<CUniqueVertex>&>(UniqueVertices)[unique1];
    CUniqueVertex unique_vertex2 = reinterpret_cast<CArray<CUniqueVertex>&>(UniqueVertices)[unique2];
    bool fixed1 = false;
    if (!contraction.ContainsAllVertices(GetRemainingVertices(unique_vertex1.VertexIndices))) {
        fixed1 = true;
        position = position1;
    }
    bool fixed2 = false;
    if (!contraction.ContainsAllVertices(GetRemainingVertices(unique_vertex2.VertexIndices))) {
        fixed2 = true;
        position = position2;
    }
    contraction.ResultingPosition = position;
    float distance1 = (position - position1).GetMagnitude();
    float distance2 = (position - position2).GetMagnitude();
    float total = distance1 + distance2;
    if (total > 0.0f)
        contraction.Weight2 = distance1 / total;
    else
        contraction.Weight2 = 0.5f;
    float cost = 0.0f;
    for (unsigned long j = 0; j < contraction.SubContractions.size(); ++j) {
        float sub_cost = ComputeSubContractionCost(contraction.SubContractions[j], position);
        if (!(cost > sub_cost))
            cost = sub_cost;
    }
    if (fixed1) {
        if (fixed2)
            cost += Parameters_HighPenalty;
    }
    else if (fixed2) {
        contraction.Weight2 = 1.0f - contraction.Weight2;
        for (unsigned long k = 0; k < contraction.SubContractions.size(); ++k) {
            CSubContraction& sub = contraction.SubContractions[k];
            unsigned long temp = sub.Vertex1;
            sub.Vertex1 = sub.Vertex2;
            sub.Vertex2 = temp;
        }
    }
    contraction.Cost = cost;
}

}
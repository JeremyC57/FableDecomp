// QuadricOptimiserInternals::COptimisedPrimitive::ComputeSubContractionCost(const CSubContraction&, const C3DVector&) -- cost of moving a vertex pair to a position: half the summed quadric error there, plus the weighted smoothing-group difference, plus the dearer of the two vertices' own costs, plus the high penalty when their normals face apart
// Grade: DIFFER(192v192) -- prologue scheduling: retail push ebx ; push ebp then the this spill [esp+8]; ours spills this between the pushes and interleaves the imuls; PDB names fixed (namespaced, private non-const AAE, EvaluateError const, operator+= void, Index1/Index2), shared CArray/C3DVector; vertex binding/copy/tail forms, Parameters struct, /G5-/G7 flags all leave it
// Leaf: ?ComputeSubContractionCost@COptimisedPrimitive@QuadricOptimiserInternals@@AAEMABVCSubContraction@2@ABVC3DVector@@@Z
// Flags: none
#include "engine/C3DVector.h"
#include "engine/CArray.h"
namespace QuadricOptimiserInternals {
class CQuadric {
public:
    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight;
    void operator+=(const CQuadric& other);                  // 0x00AC6E80
    double EvaluateError(const C3DVector& position) const;   // 0x00AC6D50
};
class COptimisedVertex {
public:
    C3DVector     Position;                        // +0x00
    C3DVector     Normal;                          // +0x0c
    C3DVector     TextureCoord;                    // +0x18
    unsigned char _pad24[4];
    CQuadric      PositionError;                   // +0x28
    float         Cost;                            // +0x80
    unsigned char _rest[0xb8 - 0x84];              // FaceList, Exists, UniqueIndex, OutputIndex, Groups, PairContractions
    float GetGroupDifference(const COptimisedVertex& other) const;   // 0x00AC7FF0
};
class CSubContraction {
public:
    unsigned long Index1;                          // +0x00
    unsigned long Index2;                          // +0x04
};
class COptimisedPrimitive {
public:
    long                     MaterialIndex;        // +0x00
    unsigned char            UniqueVertices[0xc];  // +0x04
    CArray<COptimisedVertex> Vertices;             // +0x10
    unsigned char            _pad1c[0x4c];
    float                    Parameters_GroupWeight;            // +0x68
    float                    Parameters_TextureWeight;          // +0x6c
    float                    Parameters_TexturePenalty;         // +0x70
    float                    Parameters_UnsplitTextureWeight;   // +0x74
    float                    Parameters_UnsplitNormalWeight;    // +0x78
    float                    Parameters_HighPenalty;            // +0x7c
private:
    float ComputeSubContractionCost(const CSubContraction& sub, const C3DVector& position);
};
}
using namespace QuadricOptimiserInternals;

float COptimisedPrimitive::ComputeSubContractionCost(const CSubContraction& sub, const C3DVector& position)
{
    COptimisedPrimitive* self = this;
    COptimisedVertex& v1 = self->Vertices[sub.Index1];
    COptimisedVertex& v2 = self->Vertices[sub.Index2];
    CQuadric error = v1.PositionError;
    error += v2.PositionError;
    float cost = (float)(error.EvaluateError(position) * 0.5);
    cost = v1.GetGroupDifference(v2) * self->Parameters_GroupWeight + cost;
    cost += (v1.Cost > v2.Cost) ? v1.Cost : v2.Cost;
    if (v1.Normal.Dot(v2.Normal) < 0.0f)
        cost += self->Parameters_HighPenalty;
    return cost;
}

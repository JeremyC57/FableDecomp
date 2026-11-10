// QuadricOptimiserInternals::COptimisedPrimitive::ComputeSubContractionCost(const CSubContraction&,const C3DVector&) -- evaluate vertex-pair contraction cost
// Leaf: ?ComputeSubContractionCost@COptimisedPrimitive@QuadricOptimiserInternals@@AAEMABVCSubContraction@2@ABVC3DVector@@@Z
// Retail: 0x00AC82A0,192 bytes,thiscall
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_SUBCOST
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"
namespace QuadricOptimiserInternals {
void CQuadric::operator+=(const CQuadric& other){AA+=other.AA;AB+=other.AB;AC+=other.AC;AD+=other.AD;BB+=other.BB;BC+=other.BC;BD+=other.BD;CC+=other.CC;CD+=other.CD;DD+=other.DD;Weight+=other.Weight;}

float COptimisedPrimitive::ComputeSubContractionCost(const CSubContraction& sub_contraction,const C3DVector& position){
 CQuadric quadric;
 float cost;
 const COptimisedVertex& vertex1=Vertices[sub_contraction.Index1];
 const COptimisedVertex& vertex2=Vertices[sub_contraction.Index2];
 quadric=vertex1.PositionError;
 quadric+=vertex2.PositionError;
 cost=(float)(quadric.EvaluateError(position)*0.5);
 cost=vertex1.GetGroupDifference(vertex2)*Parameters.GroupWeight+cost;
 cost+=(vertex1.Cost>vertex2.Cost)?vertex1.Cost:vertex2.Cost;
 if(vertex1.Normal.Dot(vertex2.Normal)<0.0f)cost+=Parameters.HighPenalty;
 return cost;
}
}
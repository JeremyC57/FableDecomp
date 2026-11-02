// QuadricOptimiserInternals::COptimisedPrimitive::ComputeContraction(QuadricOptimiserInternals::CVertexPairContraction &) -- select a vertex pair placement, weight and contraction cost.
// Leaf: ?ComputeContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCVertexPairContraction@2@@Z
// Grade: DIFFER(1045v1045) -- first-vertex offsets kept in registers vs reloaded; unique-vertex/flag slots and the cost loop allocation
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00ac92c0/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 00ac92c0 probes in probe/00ac92c0.log
// Flags: /Gr
#define FABLE_RETAIL_CARRAY_GEOMETRY
#define FABLE_RETAIL_VECTOR_GEOMETRY
#define FABLE_RETAIL_VECTOR_COPY_INITIALISE
#define FABLE_RETAIL_COPTIMISEDPRIMITIVE
#include "engine/COptimisedPrimitive.h"
#include "engine/QuadricOptimiserBodies.h"
namespace QuadricOptimiserInternals {
void COptimisedPrimitive::ComputeContraction(CVertexPairContraction& contraction){
 const CSubContraction& sub_contraction=contraction.SubContractions[0];
 unsigned long unique_index_1=GetVertex(sub_contraction.Index1).UniqueIndex;
 unsigned long unique_index_2=GetVertex(sub_contraction.Index2).UniqueIndex;
 if(contraction.Unsplit){ComputeSelfContraction(contraction);return;}
 CQuadric quadric;quadric.Clear();
 for(unsigned long c_sub=0;c_sub<contraction.SubContractions.size();++c_sub){
  const CSubContraction& sub_contraction2=contraction.SubContractions[c_sub];
  const COptimisedVertex& vertex1=GetVertex(sub_contraction2.Index1);
  quadric+=vertex1.PositionError;
  const COptimisedVertex& vertex2=GetVertex(sub_contraction2.Index2);
  quadric+=vertex2.PositionError;
 }
 C3DVector position1=GetVertex(sub_contraction.Index1).Position;
 C3DVector position2=GetVertex(sub_contraction.Index2).Position;
 C3DVector position=ComputeOptimalVertexPlacement(quadric,position1,position2);
 CUniqueVertex unique_vertex_1=UniqueVertices[unique_index_1];
 CUniqueVertex unique_vertex_2=UniqueVertices[unique_index_2];
 bool locked_1=false;
 if(!contraction.ContainsAllVertices(GetRemainingVertices(unique_vertex_1.VertexIndices))){locked_1=true;position=position1;}
 bool locked_2=false;
 if(!contraction.ContainsAllVertices(GetRemainingVertices(unique_vertex_2.VertexIndices))){locked_2=true;position=position2;}
 contraction.ResultingPosition=position;
 float dist1=(position-position1).GetMagnitude();
 float dist2=(position-position2).GetMagnitude();
 float distsum=dist1+dist2;
 if(distsum>0.0f)contraction.Weight2=dist1/distsum;else contraction.Weight2=0.5f;
 float cost=0.0f;
 for(unsigned long c_sub=0;c_sub<contraction.SubContractions.size();++c_sub){
  const CSubContraction& sub_contraction=contraction.SubContractions[c_sub];
  float sub_cost=ComputeSubContractionCost(sub_contraction,position);
  if(!(cost>sub_cost))cost=sub_cost;
 }
 if(locked_1&&locked_2){cost+=Parameters.HighPenalty;}
 else if(!locked_1&&locked_2){
  contraction.Weight2=1.0f-contraction.Weight2;
  for(unsigned long c_sub=0;c_sub<contraction.SubContractions.size();++c_sub){
   CSubContraction& sub_contraction=contraction.SubContractions[c_sub];
   std::swap(sub_contraction.Index1,sub_contraction.Index2);
  }
 }
 contraction.Cost=cost;
}
}

// QuadricOptimiserInternals::COptimisedPrimitive::ComputeVertexCost(QuadricOptimiserInternals::COptimisedVertex &) -- compute the candidate contraction cost for a vertex.
// Leaf: ?ComputeVertexCost@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCOptimisedVertex@2@@Z
// Grade: DIFFER(1594v1597) -- register roles: retail keeps this in edi and vertex in ebp (push ebp/edi before the empty test, ebx/esi after); this build keeps this in ebx
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00ac8c80/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = retry11a probe p01; 00ac8c80 probes in probe/00ac8c80.log
// Flags: /Gr
#define FABLE_RETAIL_QUADRIC_VERTEX_COST_RETRY
#define FABLE_GEOMETRY_VECTOR_BODIES
#define FABLE_RETAIL_CARRAY_GEOMETRY
#define FABLE_RETAIL_VECTOR_GEOMETRY
#define FABLE_RETAIL_COPTIMISEDPRIMITIVE
#include "engine/COptimisedPrimitive.h"
namespace QuadricOptimiserInternals {
void COptimisedPrimitive::ComputeVertexCost(COptimisedVertex& vertex){
 float cost=0.0f;
 if(!vertex.FaceList.Empty()){
  const CIndexList& face_indices=vertex.FaceList;
  C3DVector vertex_normal(0.0f,0.0f,0.0f);
  CArray<C3DVector> face_normals;
  face_normals.reserve(face_indices.GetSize());
  CIndexList::TConstIterator itr=face_indices.Begin();
  for(;itr!=face_indices.End();++itr){
   unsigned long face_index=*itr;
   C3DVector face_normal=GetFaceNormal(Faces[face_index]);
   vertex_normal+=face_normal;
   face_normals.push_back(face_normal);
  }
  if(vertex_normal.IsNonZero())vertex_normal.Normalise();
  itr=face_indices.Begin();
  unsigned long face_index=*itr;
  const COptimisedFace& first_face=Faces[face_index];
  C3DVector u_tangent=MakeOrthogonal(first_face.UTangent,vertex_normal);
  C3DVector v_tangent=MakeOrthogonal(first_face.VTangent,vertex_normal);
  C3DVector min_u_tangent=u_tangent,max_u_tangent=u_tangent;
  C3DVector min_v_tangent=v_tangent,max_v_tangent=v_tangent;
  float u_magnitude=u_tangent.GetMagnitude();
  float v_magnitude=v_tangent.GetMagnitude();
  itr++;
  for(;itr!=face_indices.End();++itr){
   const COptimisedFace& face=Faces[*itr];
   u_tangent=MakeOrthogonal(face.UTangent,vertex_normal);
   v_tangent=MakeOrthogonal(face.VTangent,vertex_normal);
   min_u_tangent.LimitUpper(u_tangent);max_u_tangent.LimitLower(u_tangent);
   min_v_tangent.LimitUpper(v_tangent);max_v_tangent.LimitLower(v_tangent);
   if(u_magnitude<u_tangent.GetMagnitude())u_magnitude=u_tangent.GetMagnitude();
   if(v_magnitude<v_tangent.GetMagnitude())v_magnitude=v_tangent.GetMagnitude();
  }
  C3DVector delta_u_tangent=max_u_tangent-min_u_tangent;
  C3DVector delta_v_tangent=max_v_tangent-min_v_tangent;
  if(GFFloatAbs(u_magnitude)>DELTA_NOTIONAL_ZERO){if(cost<delta_u_tangent.GetMagnitude()/u_magnitude)cost=delta_u_tangent.GetMagnitude()/u_magnitude;}
  if(GFFloatAbs(v_magnitude)>DELTA_NOTIONAL_ZERO){if(cost<delta_v_tangent.GetMagnitude()/v_magnitude)cost=delta_v_tangent.GetMagnitude()/v_magnitude;}
 }
 cost*=cost;
 if(cost>1.0f)vertex.Cost=Parameters.TexturePenalty;
 vertex.Cost=cost*Parameters.TextureWeight;
}
}

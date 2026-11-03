// CParticleProgram::CalculateAveragePatchSize(const CParticleSimulation&) -- root mean squared perimeter edge length
// Leaf: ?CalculateAveragePatchSize@CParticleProgram@@AAEXABVCParticleSimulation@@@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPARTICLEPROGRAM
#include "engine/CParticleProgram.h"
#include <math.h>
#pragma intrinsic(sqrt)
void CParticleProgram::CalculateAveragePatchSize(const CParticleSimulation& sim)
{
 float total_squared_length=0.0f;
 unsigned long total_sides=0;
 unsigned long no_tris=RenderTris.size();
 for(unsigned long c_tri=0;c_tri<no_tris;++c_tri) {
  const C3DTriangle2& tri=RenderTris[c_tri];
  C3DVector a=sim.GetParticlePosition(tri.GetVertexIndex(0));
  C3DVector b=sim.GetParticlePosition(tri.GetVertexIndex(1));
  C3DVector c=sim.GetParticlePosition(tri.GetVertexIndex(2));
  total_squared_length+=(b-a).GetSquaredMagnitude();
  total_squared_length+=(c-b).GetSquaredMagnitude();
  total_squared_length+=(a-c).GetSquaredMagnitude();
  total_sides+=3;
 }
 unsigned long no_quads=RenderQuads.size();
 for(unsigned long c_quad=0;c_quad<no_quads;++c_quad) {
  const C3DQuad2& quad=RenderQuads[c_quad];
  C3DVector a=sim.GetParticlePosition(quad.GetVertexIndex(0));
  C3DVector b=sim.GetParticlePosition(quad.GetVertexIndex(1));
  C3DVector c=sim.GetParticlePosition(quad.GetVertexIndex(2));
  C3DVector d=sim.GetParticlePosition(quad.GetVertexIndex(3));
  total_squared_length+=(b-a).GetSquaredMagnitude();
  total_squared_length+=(c-b).GetSquaredMagnitude();
  total_squared_length+=(d-c).GetSquaredMagnitude();
  total_squared_length+=(a-d).GetSquaredMagnitude();
  total_sides+=4;
 }
 if(total_sides)AveragePatchSize=(float)sqrt(total_squared_length/total_sides);
}
// CParticleProgram::OptimiseSpringOrder(CArray<CParticleDistanceConstraint>&) const -- order springs by breadth-first distance from fixed particles.
// Leaf: ?OptimiseSpringOrder@CParticleProgram@@QBEXAAV?$CArray@VCParticleDistanceConstraint@@@@@Z
// Flags: /Gr
// Retail: Fable.exe 0x00AE2E20, RELOCATION_MATCH (chunk 849 rebase of the held Codex pass 12 source onto rebuild/include).
// Callees: CArray<set<unsigned long> >(n) (row-local explicit specialization, same-TU noinline body), vector::_M_insert_overflow
//   0x00464ACE (the CArray<unsigned long>::push_back fast path, FABLE_RETAIL_CARRAY_PUSHBACK_FASTPATH), set insert_unique, std::sort
//   (FABLE_RETAIL_STLP_SORT2, /Gr fastcall), DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Shape: Codex body unchanged except `set<unsigned long>::iterator` for its const_iterator spelling; the noinline CArray ctor body
//   and the push_back fast path were the levers (26-line register residue without the ctor body).
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_COPY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_RETAIL_STLP_SORT2
#define FABLE_RETAIL_CARRAY_PUSHBACK_FASTPATH
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CParticleProgram.h"
#include "engine/CParticleDistanceConstraint.h"
extern const float DELTA_NOTIONAL_ZERO;
template<> __declspec(noinline) CArray<std::set<unsigned long> >::CArray(unsigned int n) : std::vector<std::set<unsigned long> >(n) { }
void CParticleProgram::OptimiseSpringOrder(CArray<CParticleDistanceConstraint>&springs)const
{
 unsigned long no_particles=InitialSimulation.Size;
 CArray<std::set<unsigned long> >vertex_connections(no_particles);
 for(unsigned long c_spring=0;c_spring<springs.size();++c_spring){const CParticleDistanceConstraint&spring=springs[c_spring];vertex_connections[spring.Index1].insert(spring.Index2);vertex_connections[spring.Index2].insert(spring.Index1);}
 CArray<long>egde_distances(no_particles,-1);
 CArray<unsigned long>particle_list;
 for(unsigned long c_particle=0;c_particle<no_particles;++c_particle){if(InitialSimulation.SimulationAlphas[c_particle]<DELTA_NOTIONAL_ZERO){egde_distances[c_particle]=0;particle_list.push_back(c_particle);}}
 long current_dist=1;
 while(!particle_list.empty()){
  CArray<unsigned long>next_particle_list;
  for(unsigned long c_particle=0;c_particle<particle_list.size();++c_particle){unsigned long here=particle_list[c_particle];const std::set<unsigned long>&connection_list=vertex_connections[here];for(std::set<unsigned long>::iterator itr=connection_list.begin();itr!=connection_list.end();++itr){unsigned long there=*itr;if(egde_distances[there]==-1){egde_distances[there]=current_dist;next_particle_list.push_back(there);}}}
  particle_list.swap(next_particle_list);
  ++current_dist;
 }
 CArray<std::pair<long,unsigned long> >sorted_order;
 for(unsigned long c_spring=0;c_spring<springs.size();++c_spring){const CParticleDistanceConstraint&spring=springs[c_spring];long dist=egde_distances[spring.Index1]+egde_distances[spring.Index2];sorted_order.push_back(std::pair<long,unsigned long>(dist,c_spring));}
 std::sort(sorted_order.begin(),sorted_order.end());
 CArray<CParticleDistanceConstraint>springs_copy(springs);
 for(unsigned long c_spring=0;c_spring<springs.size();++c_spring)springs[c_spring]=springs_copy[sorted_order[c_spring].second];
}

#include <stdio.h>
#include <stdlib.h>
#include <new>
static int events[12],used,typeSeen;static unsigned long flagsSeen;
static void mark(int e){events[used++]=e;}
enum EEnginePrimitiveType {ENGINE_PRIMITIVE_PARTICLE_DECAL_GROUP=30,ENGINE_PRIMITIVE_PARTICLE_GROUP=24};
enum EEnginePersistentPrimitiveFlags {EPF_DISABLE_OCCLUSION_TESTING=16384};
class CIVCountedPointeeBase {public:virtual ~CIVCountedPointeeBase(){mark(4);}};
class CEngineInternalPrimitiveBase:public CIVCountedPointeeBase {public:CEngineInternalPrimitiveBase(long t,unsigned long f){typeSeen=t;flagsSeen=f;}virtual ~CEngineInternalPrimitiveBase(){}};
class Member {public:int id;void* p;Member():id(0),p(0){}~Member(){mark(id);if(p){free(p);mark(id+4);}}};
class CEngineInternalPrimitiveParticleGroup:public CEngineInternalPrimitiveBase {protected:
 CEngineInternalPrimitiveParticleGroup(EEnginePrimitiveType,unsigned long);
 Member Primitives,PrimitiveIDToIterator,FreePrimitiveIDs;
public:
 void setup(int mask){Member* m[3]={&FreePrimitiveIDs,&PrimitiveIDToIterator,&Primitives};for(int i=0;i<3;i++){m[i]->id=i+1;if(mask&(1<<i))m[i]->p=malloc(12);}}
 static CEngineInternalPrimitiveParticleGroup* construct(void* mem,unsigned long f){return ::new(mem) CEngineInternalPrimitiveParticleGroup(ENGINE_PRIMITIVE_PARTICLE_GROUP,f);}
 static void operator delete(void* p){mark(8);free(p);}
};
class CEngineInternalPrimitiveParticleDecalGroup:public CEngineInternalPrimitiveParticleGroup {public:CEngineInternalPrimitiveParticleDecalGroup();};
CEngineInternalPrimitiveParticleDecalGroup::CEngineInternalPrimitiveParticleDecalGroup()
    : CEngineInternalPrimitiveParticleGroup(ENGINE_PRIMITIVE_PARTICLE_DECAL_GROUP, EPF_DISABLE_OCCLUSION_TESTING) { }

CEngineInternalPrimitiveParticleGroup::CEngineInternalPrimitiveParticleGroup(EEnginePrimitiveType type, unsigned long flags)
    : CEngineInternalPrimitiveBase(type, flags)
{
}

typedef void* (__fastcall *DeletingDtor)(void*,void*,unsigned);
int main(){for(int child=0;child<2;++child)for(unsigned long flags=0;flags<32;++flags)for(int m=0;m<8;++m){
 void* raw=malloc(sizeof(CEngineInternalPrimitiveParticleDecalGroup));CEngineInternalPrimitiveParticleGroup* group=child?(CEngineInternalPrimitiveParticleGroup*)::new(raw) CEngineInternalPrimitiveParticleDecalGroup:CEngineInternalPrimitiveParticleGroup::construct(raw,flags);
 if(typeSeen!=(child?30:24)||flagsSeen!=(child?16384ul:flags))return 1;group->setup(m);used=0;DeletingDtor fn=**(DeletingDtor**)group;void* ptr=fn(group,0,flags);if(ptr!=raw)return 2;int n=0;for(int i=1;i<=3;++i){if(events[n++]!=i)return 3;if((m&(1<<(i-1)))&&events[n++]!=i+4)return 4;}if(events[n++]!=4)return 5;
 if(flags&1){if(events[n++]!=8)return 6;}else free(raw);if(used!=n)return 7;}puts("00bba1e0_TEST PASS");return 0;}
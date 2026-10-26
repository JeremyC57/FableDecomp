#include "rebuild_abi.h"
#include <cstdio>
class CEngineInternalPrimitiveBase; class CEngineSubPrimitiveChain;
class CEnginePrimitiveManagerParticleDecalGroup { public:
    int RenderPrimitive(CEngineInternalPrimitiveBase *primitive, int layer, CEngineSubPrimitiveChain *chain);
};
int CEnginePrimitiveManagerParticleDecalGroup::RenderPrimitive(CEngineInternalPrimitiveBase *primitive, int layer, CEngineSubPrimitiveChain *chain){ return 0; }
int main(){
    CEnginePrimitiveManagerParticleDecalGroup g;
    if(g.RenderPrimitive(0,0,0)!=0){std::printf("BAD\n");return 1;}
    std::printf("RENDPRIM_OK\n"); return 0;
}
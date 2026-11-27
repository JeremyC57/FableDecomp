// 00c40d10 CCodaEnvironment::Commit
// Leaf: ?Commit@CCodaEnvironment@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_CCODACONTEXT_PROCESS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CCODA3DSOURCE_COMMIT
#define FABLE_RETAIL_CCODAVECTOR_ENV_SCALE_PASS29
#define private public
#include "engine/CCodaVector.h"
#undef private
#include "engine/CSalOpenALReverb.h"

void CCodaEnvironment::Commit()
{
    if (m_ffCommit) {
        CCodaVector v;
        v=m_Params.vReflectionsPan;
        float fMagnitude=v.Magnitude();
        if (fMagnitude>1.0f) {
            v.Scale(1.0f/fMagnitude);
            m_Params.vReflectionsPan=v;
        }
        v=m_Params.vReverbPan;
        fMagnitude=v.Magnitude();
        if (fMagnitude>1.0f) {
            v.Scale(1.0f/fMagnitude);
            m_Params.vReverbPan=v;
        }
        m_pReverb->Update(this);
        m_ffCommit=0;
    }
}

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
CCodaObjectBase::~CCodaObjectBase(){}
CCodaObject::~CCodaObject(){}
CCodaEnvironment::~CCodaEnvironment(){}
CSalReverb::~CSalReverb(){}
int CSalReverb::Create(CSalPlatform*){return 0;}
void CSalReverb::Release(){}
bool CSalReverb::IsSoftware()const{return false;}
void CSalReverb::AddSwInput(const int*){}
extern "C" void CodaFree(void* p){free(p);}
struct TestReverb : CSalReverb {
    unsigned calls, flags;
    const CCodaEnvironment* seen;
    CENVIRONMENT params;
    void Update(const CCodaEnvironment* owner) {
        ++calls;seen=owner;flags=owner->GetCommit();params=owner->GetParams();
    }
    void Process(){assert(false);}
};
static void check(const CVECTOR& before,const CVECTOR& after,bool update) {
    const double length=sqrt(before.x*before.x+before.y*before.y+before.z*before.z);
    if (!update || length<=1) {assert(memcmp(&before,&after,sizeof before)==0);return;}
    assert(fabs(after.x-before.x/length)<1.0/65536 && fabs(after.y-before.y/length)<1.0/65536 && fabs(after.z-before.z/length)<1.0/65536);
}
int main(){
    CVECTOR points[]={{0,0,0},{0.5f,0,0},{0,-1,0},{-2,0,0},{0,0,4},{1,-2,2}};
    unsigned masks[]={0,1,0x80000000U,~0U};
    for(unsigned m=0;m<4;++m)for(unsigned i=0;i<6;++i)for(unsigned j=0;j<6;++j){
        CCodaEnvironment env; TestReverb sink;
        memset(&env.m_Params,0,sizeof env.m_Params); env.m_Params.iLevel=123;
        env.m_Params.vReflectionsPan=points[i];env.m_Params.vReverbPan=points[j];
        env.m_pReverb=&sink;env.m_ffCommit=masks[m];sink.calls=0;env.Commit();
        assert(env.m_ffCommit==0 && env.m_Params.iLevel==123 && sink.calls==(m?1U:0U));
        check(points[i],env.m_Params.vReflectionsPan,m!=0);check(points[j],env.m_Params.vReverbPan,m!=0);
        if(m){assert(sink.seen==&env && sink.flags==masks[m] && memcmp(&sink.params,&env.m_Params,sizeof sink.params)==0);}
        env.Commit();assert(sink.calls==(m?1U:0U));
    }
    puts("00c40d10_TEST PASS");
}
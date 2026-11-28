// Leaf: ?GetCurrentFadeValue@CPSCUpdateNormal@NParticleEngine@@IBEMXZ
// Retail-asm: existing GFFloatToLongNear expands fld and fistp at 00B0A84C / 00B0A850 and 00B0A8D0 / 00B0A8D4.
#define FABLE_RETAIL_CPSCUPDATENORMAL_NO_CONTAINERS
#define FABLE_RETAIL_PARTICLE_FADE_VALUE_PASS30
#include "engine/CPSCUpdateNormal.h"
#include "engine/NParticleEngine_GFSmoothFadeValue.h"

namespace NParticleEngine {
float CPSCUpdateNormal::GetCurrentFadeValue() const
{
    if (!UseSystemLifeSecs)
        return 1.0f;
    float age = 0.0f;
    if (SystemLifeSecs > 0.0f) {
        age = (NParticleEngine::GetOneOverTickRate() * LifeTimer) / SystemLifeSecs;
        if (age < 0.0f)
            age = 0.0f;
        else if (age > 1.0f)
            age = 1.0f;
    }
    float fadeIn = (float)FadeInEndInteger * 0.001f;
    float fadeOut = (float)FadeOutBeginInteger * 0.001f;
    float result = 1.0f;
    if (age < fadeIn)
        result = GFSmoothFadeValue(age / fadeIn);
    if (age > fadeOut)
        result *= GFSmoothFadeValue((1.0f - age) / (1.0f - fadeOut));
    return result;
}
}

#include <math.h>
#include <stdio.h>
float GFastCosTable[1025];
namespace NParticleEngine {
float FABLE_FASTCALL GetOneOverTickRate(){return 1.0f;}
class FadeAccess : public CPSCUpdateNormal {
public:
    using CPSCUpdateNormal::GetCurrentFadeValue;
    void configure(bool enabled,unsigned in,unsigned out,float seconds,long frame){
        UseSystemLifeSecs=enabled; FadeInEndInteger=in; FadeOutBeginInteger=out; SystemLifeSecs=seconds; LifeTimer=frame;
    }
};
}
int main() {
    for(int j=0;j<=1024;++j)GFastCosTable[j]=(float)cos(j*6.283185307179586/1024.0);
    static unsigned long storage[(sizeof(NParticleEngine::FadeAccess)+3)/4];
    NParticleEngine::FadeAccess* u=reinterpret_cast<NParticleEngine::FadeAccess*>(storage);
    const unsigned ins[]={0,200,600,1023},outs[]={0,300,700,1023}; const float life[]={-1,0,10};
    for(int on=0;on<2;++on) for(int i=0;i<4;++i) for(int o=0;o<4;++o) for(int n=0;n<3;++n) for(int frame=-2;frame<=14;++frame) {
        u->configure(on!=0,ins[i],outs[o],life[n],frame);float result=u->GetCurrentFadeValue();
        double age=life[n]>0?frame/(double)life[n]:0;if(age<0)age=0;if(age>1)age=1;
        double in=ins[i]/1000.0,out=outs[o]/1000.0,expect=1.0;
        if(on && age<in)expect=0.5-0.5*cos(3.141592653589793*age/in);
        if(on && age>out)expect*=0.5-0.5*cos(3.141592653589793*(1.0-age)/(1.0-out));
        if(fabs(result-expect)>0.00002)return 1;
    }
    puts("00b0a750_TEST PASS");return 0;
}

#include <stdio.h>
#include <stdlib.h>
struct CRGBFloatColour {
    float R,G,B,A;
    CRGBFloatColour(float r=0,float g=0,float b=0,float a=0):R(r),G(g),B(b),A(a){}
    CRGBFloatColour operator*(float f) const {return CRGBFloatColour(R*f,G*f,B*f,A*f);}
    CRGBFloatColour operator+(const CRGBFloatColour& c)const{return CRGBFloatColour(R+c.R,G+c.G,B+c.B,A+c.A);}
};
struct CIDrawEnvironmentTextureSet { int id; };
namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {
public:
    float TimeOfDay;
    CRGBFloatColour Colours[19];
    float Scalers[30];
    CIDrawEnvironmentTextureSet TextureSets[3];
    void Blend(CBlendedEnvironmentTheme&,const CBlendedEnvironmentTheme&,float) const;
    void BlendTextureSets(const CIDrawEnvironmentTextureSet&,const CIDrawEnvironmentTextureSet&,CIDrawEnvironmentTextureSet&,float)const;
};
}
using NEnvironmentInternal::CBlendedEnvironmentTheme;
static const CBlendedEnvironmentTheme* origin;
static const CBlendedEnvironmentTheme* target;
static CBlendedEnvironmentTheme* output;
static int calls;
static float fraction;
void CBlendedEnvironmentTheme::BlendTextureSets(const CIDrawEnvironmentTextureSet& a,const CIDrawEnvironmentTextureSet& b,CIDrawEnvironmentTextureSet& c,float f)const {
    if(calls>=3 || this!=origin || &a!=&origin->TextureSets[calls] || &b!=&target->TextureSets[calls] || &c!=&output->TextureSets[calls] || f!=fraction)exit(2);
    c.id=1000+a.id*4-b.id;
    ++calls;
}
void NEnvironmentInternal::CBlendedEnvironmentTheme::Blend(CBlendedEnvironmentTheme& result, const CBlendedEnvironmentTheme& theme, float interp) const
{
    float blend0 = (1.0f - interp);
    float blend1 = interp;
    result.TimeOfDay = TimeOfDay * blend0 + theme.TimeOfDay * blend1;
    for (long n = 0; n < 19; ++n)
        result.Colours[n] = Colours[n] * blend0 + theme.Colours[n] * blend1;
    for (long n = 0; n < 30; ++n)
        result.Scalers[n] = Scalers[n] * blend0 + theme.Scalers[n] * blend1;
    for (long n = 0; n < 3; ++n)
        BlendTextureSets(TextureSets[n], theme.TextureSets[n], result.TextureSets[n], blend1);
}
static float value(int k,int n){return ((k*13+n*7)%47-23)*0.25f;}
static void fill(CBlendedEnvironmentTheme& t,int k){
    t.TimeOfDay=value(k,0);
    for(int n=0;n<19;++n)t.Colours[n]=CRGBFloatColour(value(k,n+1),value(k,n+20),value(k,n+39),value(k,n+58));
    for(int n=0;n<30;++n)t.Scalers[n]=value(k,n+77);
    for(int n=0;n<3;++n)t.TextureSets[n].id=k*3+n;
}
static float ref(float x,float y){return x+(y-x)*fraction;}
int main(){
    for(int k=0;k<32;++k) for(int f=-4;f<=12;++f) for(int mode=0;mode<5;++mode){
        CBlendedEnvironmentTheme a,b,c;
        fill(a,k);fill(b,k+7);fill(c,k+17);
        origin=&a;target=(mode>=3)?&a:&b;output=(mode==0||mode==3)?&c:(mode==2?&b:&a);
        const CBlendedEnvironmentTheme x=*origin,y=*target;
        calls=0;fraction=f/8.0f;origin->Blend(*output,*target,fraction);
        if(calls!=3||output->TimeOfDay!=ref(x.TimeOfDay,y.TimeOfDay))return 3;
        for(int n=0;n<19;++n){ const CRGBFloatColour& v=output->Colours[n];const CRGBFloatColour& l=x.Colours[n];const CRGBFloatColour& r=y.Colours[n];
            if(v.R!=ref(l.R,r.R)||v.G!=ref(l.G,r.G)||v.B!=ref(l.B,r.B)||v.A!=ref(l.A,r.A))return 4;}
        for(int n=0;n<30;++n)if(output->Scalers[n]!=ref(x.Scalers[n],y.Scalers[n]))return 5;
        for(int n=0;n<3;++n)if(output->TextureSets[n].id!=1000+x.TextureSets[n].id*4-y.TextureSets[n].id)return 6;
    }
    puts("00887b70_TEST PASS");return 0;
}
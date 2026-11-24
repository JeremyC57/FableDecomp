#include <stdio.h>
#include <string.h>
#include <math.h>
struct C2DVector {
    float X,Y; const C2DVector& operator+=(const C2DVector& other){X+=other.X; Y+=other.Y; return *this;}
    C2DVector ToC2DVector()const{return *this;}
};
struct CEngineEnvironment {
    bool flag;float speed;C2DVector wind;mutable int samples;
    const bool PeekEnvironmentalEffectsAlwaysUpdate()const{return flag;}
    C2DVector GetClampedScaledNextWindVector(float scale,float low,float high)const{
        ++samples;float v=speed*scale;if(v<low)v=low;if(v>high)v=high;
        C2DVector result={wind.X*v,wind.Y*v};return result;
    }
};
namespace NEngineInternal {CEngineEnvironment* GPEngineEnvironment;}
struct CEngineWaterSettings {
    float IceBumpMapPCFrameOffset[2],WaterBumpMapPCFrameOffset[2],SeaBumpMapPCFrameOffset[2];
    float IceBumpMapPCFrameOffsetSpeed,WaterBumpMapPCFrameOffsetSpeed,SeaBumpMapPCFrameOffsetSpeed;
    float SwellTrigIncrimentHistory[2],SwellOscilationSpeed,EdgeWakeOffsetHistory[2],EdgeWakeSpeed;
    float BumpMapSpeed0,BumpMapSpeed1,FlowSpeedFactor,LakeMinimumFlowSpeed,LakeMaximumFlowSpeed;
    C2DVector BumpmapGenerationShiftHistory[3][2][2];
    void KeepHistoryInRange(float*,float)const;
    void KeepHistoryInRange(C2DVector*,float)const;
    void WorldFrameUpdate(bool);
};
void CEngineWaterSettings::KeepHistoryInRange(float* history,float range) const
{
    if (history[0] > range) {
        history[0] -= range;
        history[1] -= range;
    } else if (history[0] < -range) {
        history[0] += range;
        history[1] += range;
    }
}
void CEngineWaterSettings::KeepHistoryInRange(C2DVector* history, float range) const
{
    if (history[0].X > range) {
        history[0].X -= range;
        history[1].X -= range;
    } else if (-range > history[0].X) {
        history[0].X += range;
        history[1].X += range;
    }
    if (history[0].Y > range) {
        history[0].Y -= range;
        history[1].Y -= range;
    } else if (-range > history[0].Y) {
        history[0].Y += range;
        history[1].Y += range;
    }
}

void CEngineWaterSettings::WorldFrameUpdate(bool time_for_full_update)
{
    if (!time_for_full_update && !NEngineInternal::GPEngineEnvironment->PeekEnvironmentalEffectsAlwaysUpdate())
        return;
    IceBumpMapPCFrameOffset[1] = IceBumpMapPCFrameOffset[0];
    WaterBumpMapPCFrameOffset[1] = WaterBumpMapPCFrameOffset[0];
    SeaBumpMapPCFrameOffset[1] = SeaBumpMapPCFrameOffset[0];
    SwellTrigIncrimentHistory[1] = SwellTrigIncrimentHistory[0];
    IceBumpMapPCFrameOffset[0] += IceBumpMapPCFrameOffsetSpeed;
    WaterBumpMapPCFrameOffset[0] += WaterBumpMapPCFrameOffsetSpeed;
    SeaBumpMapPCFrameOffset[0] += SeaBumpMapPCFrameOffsetSpeed;
    SwellTrigIncrimentHistory[0] += SwellOscilationSpeed;
    BumpmapGenerationShiftHistory[1][0][1] = BumpmapGenerationShiftHistory[1][0][0];
    BumpmapGenerationShiftHistory[1][1][1] = BumpmapGenerationShiftHistory[1][1][0];
    BumpmapGenerationShiftHistory[0][0][1] = BumpmapGenerationShiftHistory[0][0][0];
    BumpmapGenerationShiftHistory[0][1][1] = BumpmapGenerationShiftHistory[0][1][0];

    C2DVector wind_dir_0 = NEngineInternal::GPEngineEnvironment->GetClampedScaledNextWindVector(BumpMapSpeed0 * FlowSpeedFactor, LakeMinimumFlowSpeed, LakeMaximumFlowSpeed).ToC2DVector();
    BumpmapGenerationShiftHistory[0][0][0] += wind_dir_0;
    BumpmapGenerationShiftHistory[1][0][0] += wind_dir_0;
    C2DVector wind_dir_1 = NEngineInternal::GPEngineEnvironment->GetClampedScaledNextWindVector(BumpMapSpeed1 * FlowSpeedFactor, LakeMinimumFlowSpeed, LakeMaximumFlowSpeed).ToC2DVector();
    BumpmapGenerationShiftHistory[1][1][0] += wind_dir_1;
    BumpmapGenerationShiftHistory[0][1][0] += wind_dir_1;
    EdgeWakeOffsetHistory[1] = EdgeWakeOffsetHistory[0];
    EdgeWakeOffsetHistory[0] += EdgeWakeSpeed;
    KeepHistoryInRange(SwellTrigIncrimentHistory,6.2831855f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[1][0],1.0f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[1][1],1.0f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[0][0],1.0f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[0][1],1.0f);
    KeepHistoryInRange(EdgeWakeOffsetHistory,1.0f);
}

static float adjustment(float n,float r){return n>r?-r:n< -r?r:0.0f;}
static bool close_number(float a,float b){return fabs((double)a-b)<0.000002;}
int main(){
    CEngineEnvironment environment;NEngineInternal::GPEngineEnvironment=&environment;
    float speeds[]={-2,0,0.25f,0.5f,1.5f,4};
    float starts[]={-2,-1,-0.5f,0,0.5f,1,2};
    float swell[]={-7,-6.2831855f,0,6.2831855f,7};
    for(int full=0;full<2;++full)for(int always=0;always<2;++always)for(int w=0;w<6;++w)
    for(int pos=0;pos<7;++pos)for(int oscill=0;oscill<5;++oscill){
        environment.flag=always!=0;environment.samples=0;environment.wind.X=1;environment.wind.Y=-0.5f;environment.speed=speeds[w];
        CEngineWaterSettings water;memset(&water,0,sizeof(water));
        const float initial=starts[pos];
        water.IceBumpMapPCFrameOffset[0]=1;water.WaterBumpMapPCFrameOffset[0]=2;water.SeaBumpMapPCFrameOffset[0]=3;
        water.IceBumpMapPCFrameOffsetSpeed=0.5f;water.WaterBumpMapPCFrameOffsetSpeed=-0.25f;water.SeaBumpMapPCFrameOffsetSpeed=1;
        water.SwellTrigIncrimentHistory[0]=swell[oscill];water.EdgeWakeOffsetHistory[0]=initial;
        water.EdgeWakeSpeed=pos%2?0:0.25f;water.SwellOscilationSpeed=oscill%2?0:-0.25f;
        water.BumpMapSpeed0=1;water.BumpMapSpeed1=2;water.FlowSpeedFactor=1;water.LakeMinimumFlowSpeed=0.25f;water.LakeMaximumFlowSpeed=1.5f;
        for(int t=0;t<3;++t)for(int p=0;p<2;++p)for(int j=0;j<2;++j){
            C2DVector& v=water.BumpmapGenerationShiftHistory[t][p][j];v.X=initial+t*0.25f+j;v.Y= -initial+p*0.25f+j;
        }
        CEngineWaterSettings before=water;
        water.WorldFrameUpdate(full!=0);
        if(!full && !always){if(memcmp(&water,&before,sizeof(water)) || environment.samples!=0)return 1;continue;}
        if(environment.samples!=2 || water.IceBumpMapPCFrameOffset[0]!=1.5f || water.WaterBumpMapPCFrameOffset[0]!=1.75f ||
           water.SeaBumpMapPCFrameOffset[0]!=4 || water.IceBumpMapPCFrameOffset[1]!=1 || water.WaterBumpMapPCFrameOffset[1]!=2 ||water.SeaBumpMapPCFrameOffset[1]!=3)return 2;
        float cursor=swell[oscill]+before.SwellOscilationSpeed,wrap=adjustment(cursor,6.2831855f);
        if(!close_number(water.SwellTrigIncrimentHistory[0],cursor+wrap) || !close_number(water.SwellTrigIncrimentHistory[1],swell[oscill]+wrap))return 3;
        cursor=initial+before.EdgeWakeSpeed;wrap=adjustment(cursor,1);
        if(water.EdgeWakeOffsetHistory[0]!=cursor+wrap || water.EdgeWakeOffsetHistory[1]!=initial+wrap)return 4;
        for(int t=0;t<3;++t)for(int layer=0;layer<2;++layer){
            const C2DVector* old=before.BumpmapGenerationShiftHistory[t][layer];
            const C2DVector* now=water.BumpmapGenerationShiftHistory[t][layer];
            if(t==2){if(memcmp(now,old,2*sizeof(C2DVector)))return 5;continue;}
            float rate=speeds[w]*(layer+1);if(rate<0.25f)rate=0.25f;if(rate>1.5f)rate=1.5f;
            float xx=old[0].X+rate,yy=old[0].Y-rate*0.5f;
            const float dx=adjustment(xx,1),dy=adjustment(yy,1);
            if(now[0].X!=xx+dx || now[0].Y!=yy+dy || now[1].X!=old[0].X+dx || now[1].Y!=old[0].Y+dy)return 6;
        }
    }
    puts("00be95f0_TEST PASS");return 0;
}
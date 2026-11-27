// 00b62440 WorldFrameUpdate and KeepHistoryInRange native fixture.
#include <stdio.h>
#include <stdlib.h>
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#include "engine/C2DVector.h"
namespace Fixture {
void require(bool v) { if (!v) abort(); }
struct CIDrawEnvironmentParams {
    struct Channel { float Scalers[2]; } ThemeChannels[1];
};
struct World {
    CIDrawEnvironmentParams params;
    int mapReads, envReads, paramReads;
    World* PeekGameWorld() { ++mapReads; return this; }
    World* DrawGetEnvironment() { ++envReads; return this; }
    const CIDrawEnvironmentParams& PeekInterpolatedParams() { ++paramReads; return params; }
};
struct Weather {
    C3DVector wind;
    float speed, observed[2]; int queries; float expectedMin, expectedMax;
    C3DVector GetClampedScaledCurrentWindVector(float scale,float min,float max) {
        require(queries<2 && expectedMin==min && expectedMax==max);
        observed[queries++]=scale;
        float amount=speed*scale;
        return C3DVector(wind*(amount<min?min:(amount>max?max:amount)));
    }
};
namespace NEngineInternal { World* GPEngineWorldMap; Weather* GPEngineEnvironment; }
struct CEngineSkyRenderer {
    C2DVector CloudVectorHistory[2][2];
    float CloudSpeedMultiplier, CloudMinSpeed, CloudMaxSpeed;
    void KeepHistoryInRange(C2DVector*,const float) const;
    void WorldFrameUpdate(bool);
};
void CEngineSkyRenderer::KeepHistoryInRange(C2DVector* history, const float range) const
{
    if (history[0].X > range) {
        history[0].X -= range;
        history[1].X -= range;
    } else if (history[0].X < -range) {
        history[0].X += range;
        history[1].X += range;
    }
    if (history[0].Y > range) {
        history[0].Y -= range;
        history[1].Y -= range;
    } else if (history[0].Y < -range) {
        history[0].Y += range;
        history[1].Y += range;
    }
}

void CEngineSkyRenderer::WorldFrameUpdate(bool time_for_full_update)
{
    if (time_for_full_update) {
        const CIDrawEnvironmentParams& params = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
        float speed = CloudSpeedMultiplier * params.ThemeChannels[0].Scalers[1];
        C3DVector wind_vel = NEngineInternal::GPEngineEnvironment->GetClampedScaledCurrentWindVector(speed, CloudMinSpeed, CloudMaxSpeed);
        CloudVectorHistory[1][1] = CloudVectorHistory[1][0];
        CloudVectorHistory[1][0] += C2DVector(wind_vel);
        speed = CloudSpeedMultiplier * params.ThemeChannels[0].Scalers[0];
        wind_vel = NEngineInternal::GPEngineEnvironment->GetClampedScaledCurrentWindVector(speed, CloudMinSpeed, CloudMaxSpeed);
        CloudVectorHistory[0][1] = CloudVectorHistory[0][0];
        CloudVectorHistory[0][0] += C2DVector(wind_vel);
        KeepHistoryInRange(CloudVectorHistory[1], 1.0f);
        KeepHistoryInRange(CloudVectorHistory[0], 1.0f);
    }
}

static float correction(float current) {
    return (current>1.0f)?-1.0f:((current< -1.0f)?1.0f:0.0f);
}
static void run(bool update,int x,int y,int ws,int mode,int flip) {
    World world; Weather weather; CEngineSkyRenderer renderer;
    world.mapReads=world.envReads=world.paramReads=0; weather.queries=0;
    NEngineInternal::GPEngineWorldMap=&world; NEngineInternal::GPEngineEnvironment=&weather;
    world.params.ThemeChannels[0].Scalers[0]=float(mode-2);
    world.params.ThemeChannels[0].Scalers[1]=float(3-mode);
    renderer.CloudSpeedMultiplier=(flip&1)?0.5f:1.0f;
    renderer.CloudMinSpeed=weather.expectedMin=-0.5f;
    renderer.CloudMaxSpeed=weather.expectedMax=1.0f;
    weather.speed=0.5f*float(ws); weather.wind=C3DVector(2,-1,4);
    float before[2][2][2]; int layer,time,axis;
    for(layer=0;layer<2;++layer) for(time=0;time<2;++time) {
        float prevX=float(x+layer*(flip-1)) * 0.5f+time*8.0f;
        float prevY=float(y-layer*(flip-1)) * 0.5f-time*8.0f;
        before[layer][time][0]=renderer.CloudVectorHistory[layer][time].X=prevX;
        before[layer][time][1]=renderer.CloudVectorHistory[layer][time].Y=prevY;
    }
    renderer.WorldFrameUpdate(update);
    int expected=update?1:0;
    require(world.mapReads==expected && world.envReads==expected && world.paramReads==expected && weather.queries==2*expected);
    for(layer=0;layer<2;++layer) {
        float scale=renderer.CloudSpeedMultiplier*world.params.ThemeChannels[0].Scalers[layer];
        if(update) require(weather.observed[1-layer]==scale);
        float magnitude=weather.speed*scale;
        if(magnitude < -0.5f) magnitude=-0.5f;
        if(magnitude > 1.0f) magnitude=1.0f;
        for(axis=0;axis<2;++axis) {
            float old=before[layer][0][axis], cur=old+(axis?-1.0f:2.0f)*magnitude;
            for(time=0;time<2;++time) {
                float want=update?((time?old:cur)+correction(cur)):before[layer][time][axis];
                const C2DVector& p=renderer.CloudVectorHistory[layer][time];
                require((axis?p.Y:p.X)==want);
            }
        }
    }
}
}
int main() {
    for(int update=0;update<2;++update) for(int x=-4;x<=4;++x) for(int y=-4;y<=4;++y)
        for(int wind=-2;wind<=2;++wind) for(int mode=0;mode<5;++mode) for(int flip=0;flip<3;++flip)
            Fixture::run(update!=0,x,y,wind,mode,flip);
    puts("00b62440_TEST PASS"); return 0;
}
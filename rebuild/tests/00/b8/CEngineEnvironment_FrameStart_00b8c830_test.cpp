// 00b8c830 CEngineEnvironment::FrameStart
// Leaf: ?FrameStart@CEngineEnvironment@@UAEXABVCEngineComponentParams@@@Z
// Flags: (none)
#define FABLE_RETAIL_CENGINEENVIRONMENT
#define FABLE_RETAIL_CENGINEENVIRONMENT_WIND
#define FABLE_RETAIL_ENVIRONMENT_FRAMESTART_PASS30
#define FABLE_RETAIL_CENGINECOMPONENTPARAMS_INTERPOLATIONINFO
#define FABLE_RETAIL_CENGINEINTERPOLATIONINFO_NESTED_SET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#include "engine/CEngineEnvironment.h"
#include "engine/CEngineComponentRenderParams.h"
#pragma intrinsic(sqrt)

void CEngineEnvironment::FrameStart(const CEngineComponentParams& params)
{
    BulletTimeInterpolation = params.InterpolationInfo.BulletTimeCurrent.WFInterpolate;
    PausedInterpolation = params.InterpolationInfo.PausedCurrent.WFInterpolate;
    NonPausedInterpolation = params.InterpolationInfo.Current.WFInterpolate;
    CurrentInterpolation = BulletTimeInterpolation;
    CurrentRenderFrame = params.FrameID;

    for (long n = 0; n < 63; ++n) {
        const C3DVector& skew_next = WindSkewHistory[n];
        const C3DVector& skew_last = WindSkewHistory[n + 1];
        InterpolatedWindSkewHistory[n] = skew_last + (skew_next - skew_last) * CurrentInterpolation;
    }
    CurrentWindSpeed = PreviousWindSpeed + (NextWindSpeed - PreviousWindSpeed) * CurrentInterpolation;
    CurrentWindVector = PreviousWindVector + (NextWindVector - PreviousWindVector) * CurrentInterpolation;
    if (CurrentWindVector.GetAccurateMagnitude() < 0.1f) {
        CurrentWindVector = PreviousWindVector;
    }
    CurrentWindVector.AccurateNormalise();
}

#include <stdio.h>
#include <stdlib.h>
#define CHECK(expression) do { if (!(expression)) { fprintf(stderr, "line %d failed %s\n", __LINE__, #expression); abort(); } } while (0)
#include <string.h>

// The base services below belong to the test environment. Only FrameStart's normalization service is called.
CEngineComponent::CEngineComponent() {}
CEngineComponent::~CEngineComponent() {}
void CEngineComponent::FrameStart(const CEngineComponentParams&) {}
void CEngineComponent::FrameEnd(const CEngineComponentParams&) {}
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&) { return false; }
void CEngineComponent::Render(const CEngineComponentRenderParams&) {}
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::WorldFrameUpdate(bool) {}
void CEngineComponent::PausedWorldFrameUpdate() {}
CCharString CEngineComponent::GetConsoleEnableFunctionName() { abort(); }
void CEngineComponent::OnReleaseDefaultPoolResources() {}
bool CEngineComponent::OnRestoreDefaultPoolResources() { return true; }
CEngineInterpolationInfo::CEngineInterpolationInfo() {}
CEngineEnvironment::CEngineEnvironment() {}
unsigned long CEngineEnvironment::PeekSceneFilterFlags() const { return 0; }

static C3DVector received;
static unsigned calls;
C3DVector& C3DVector::AccurateNormalise() {
    received = *this;
    ++calls;
    X = -17; Y = 23; Z = 33;
    return *this;
}
static void putWord(CEngineEnvironment& object, unsigned offset, float value) {
    memcpy((unsigned char*)&object + offset, &value, sizeof value);
}
static float readWord(const CEngineEnvironment& object, unsigned offset) {
    float value;
    memcpy(&value, (const unsigned char*)&object + offset, sizeof value);
    return value;
}
int main() {
    const float factors[] = {-1, 0, 0.25f, 0.5f, 1, 1.5f, 2};
    const float cases[][6] = {{0,0,0,0,0,0},{1,0,0,-1,0,0},
                              {0,0.0625f,0,0.0625f,0,0},{0,0.125f,0,0,0.0625f,0},
                              {-3,2,1,2,4,-3},{0.1f,0,0,0.1f,0,0}};
    for (unsigned ti=0; ti!=sizeof factors/sizeof *factors; ++ti)
        for (unsigned scenario=0; scenario!=sizeof cases/sizeof *cases; ++scenario)
            for (int seed=0; seed!=4; ++seed) {
                CEngineEnvironment world;
                CEngineComponentParams params;
                const float t = factors[ti];
                params.InterpolationInfo.BulletTimeCurrent.WFInterpolate=t;
                params.InterpolationInfo.PausedCurrent.WFInterpolate=(float)(seed+7);
                params.InterpolationInfo.Current.WFInterpolate=(float)(13-seed);
                params.FrameID=0x100U+scenario+seed;
                const float* next=&cases[scenario][3];
                const float* previous=&cases[scenario][0];
                for (int axis=0; axis!=3; ++axis) {
                    putWord(world,0x628+4*axis,next[axis]);
                    putWord(world,0x634+4*axis,previous[axis]);
                }
                putWord(world,0x644,8.0f);putWord(world,0x648,-4.0f);
                for (int n=0; n!=64; ++n)
                    for(int k=0;k!=3;++k) {
                        putWord(world,0x1c+12*n+4*k,(float)((n*7+seed*3+k*5)%33-16));
                        putWord(world,0x31c+12*n+4*k,97.0f);
                    }
                calls=0;
                CEngineComponent& dispatch=world;
                dispatch.FrameStart(params);
                CHECK(calls==1);
                unsigned id;memcpy(&id,(const unsigned char*)&world+0x64c,sizeof id);
                CHECK(id==params.FrameID);
                CHECK(readWord(world,0xc)==t && readWord(world,0x10)==t);
                CHECK(readWord(world,0x14)==seed+7 && readWord(world,0x18)==13-seed);
                CHECK(readWord(world,0x640)==-4.0f+t*12.0f);
                for (int i=0; i!=64; ++i)
                    for (int k=0;k!=3;++k) {
                        const float here=(float)((i*7+seed*3+k*5)%33-16);
                        const float after=(float)(((i+1)*7+seed*3+k*5)%33-16);
                        CHECK(readWord(world,0x1c+12*i+4*k)==here);
                        CHECK(readWord(world,0x31c+12*i+4*k)==(i==63?97.0f:after+t*(here-after)));
                    }
                float v[3];
                for(int axis=0;axis!=3;++axis) v[axis]=previous[axis]+t*(next[axis]-previous[axis]);
                const float mag=(float)sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
                const float* expect=mag<0.1f ? previous:v;
                CHECK(received.X==expect[0] && received.Y==expect[1] && received.Z==expect[2]);
                CHECK(readWord(world,0x61c)==-17 && readWord(world,0x620)==23 && readWord(world,0x624)==33);
            }
    puts("00b8c830_TEST PASS");
    return 0;
}
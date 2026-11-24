#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
// Original source locals and member expression sequence recovered from FableWin02dc4700 and Ego_d.
// Leaf: ?WorldFrameUpdate@CEngineEnvironment@@UAEX_N@Z
// Flags: (none)
#define FABLE_RETAIL_CENGINEENVIRONMENT
#define FABLE_RETAIL_CENGINEENVIRONMENT_WIND
#define FABLE_RETAIL_CENGINEENVIRONMENT_WORLDFRAMEUPDATE_PASS26
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#include "engine/CEngineEnvironment.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#pragma intrinsic(sqrt)
namespace NEngineInternal {extern CEngineWorldMap* GPEngineWorldMap;}
void CEngineEnvironment::WorldFrameUpdate(bool time_for_full_update)
{
    if(time_for_full_update || EnvironmentalEffectsAlwaysUpdate) {
        const CIDrawEnvironment* penvironment=NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment();
        C3DVector wind_dir=penvironment->PeekInterpolatedParams().ThemeWindDirection;
        if(wind_dir.GetAccurateMagnitude()<0.1f)
            wind_dir=PreviousWindVector;
        wind_dir.AccurateNormalise();
        float theme_wind_speed=penvironment->PeekInterpolatedParams().ThemeWindSpeed;
        const float wind_speed=theme_wind_speed<0.4f?theme_wind_speed:0.4f;
        PreviousWindVector=NextWindVector;
        NextWindVector=wind_dir;
        PreviousWindSpeed=NextWindSpeed;
        NextWindSpeed=wind_speed;
        C3DVector velocity=WindSkewHistory[0]-WindSkewHistory[1];
        C3DVector skew_vector=WindSkewHistory[0]*0.75f+velocity*0.9f;
        skew_vector+=wind_dir*wind_speed;
        for(long c1=63;c1>0;--c1)
            WindSkewHistory[c1]=WindSkewHistory[c1-1];
        WindSkewHistory[0]=skew_vector;
    }
}
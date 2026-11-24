// CGameCameraManager::UpdateEarthquake() -- accumulate decaying quake displacements and clamp the final camera displacement.
// Leaf: ?UpdateEarthquake@CGameCameraManager@@AAEXXZ
// Flags: (none)
// Evidence: both PDB names; original FableWin locals, scopes and source lines1524..1569.
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_CGAMECAMERAMANAGER_EARTHQUAKE_PASS26
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#include "fable_string.h"
#include "engine/CGameCameraManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/GFExp.h"
#include "engine/GFSin.h"
#include "engine/NRumble.h"
extern const float DELTA_NOTIONAL_ZERO;
void CGameCameraManager::UpdateEarthquake()
{
    C3DVector pos=Camera.PeekWorldPos();
    long frame=GetWorld().GetFrame();
    long const_fps=CMainGameComponent::GetConstantFPS();
    const float MAX_DISPLACEMENT=1.0f;
    C3DVector offset_vector(0.0f,0.0f,0.0f);
    for(std::vector<CQuake>::iterator it=EarthQuakes.begin();it!=EarthQuakes.end();) {
        CQuake& quake=*it;
        long frames=frame-quake.Frame;
        float seconds=(float)frames/const_fps;
        if(seconds>quake.Duration)it=EarthQuakes.erase(it);
        else {
            ++it;
            float distance_attenuator=1.0f;
            if(!quake.Global)distance_attenuator=NRumble::GetRumbleDistanceAttenuationAtPos(quake.Position,pos);
            float amplitude=quake.Amplitude*distance_attenuator;
            float decay_param=3.0f/quake.Duration;
            float displacement=(float)(amplitude*GFExp(-seconds*decay_param)*GFSin(seconds*quake.Frequency+quake.Phase*0.25f));
            offset_vector+=quake.Axis*displacement;
        }
    }
    float mag=offset_vector.GetAccurateMagnitude();
    if(mag>DELTA_NOTIONAL_ZERO) {
        float disp=mag<MAX_DISPLACEMENT?mag:MAX_DISPLACEMENT;
        Camera.MoveTo(pos+offset_vector*disp/mag);
    }
}
// 0075ddf0 CTCCameraPointScriptedSpline::UpdateSplines
// Leaf: ?UpdateSplines@CTCCameraPointScriptedSpline@@AAEXXZ
// Flags: /O2 /Gr
#define FABLE_RETAIL_SCRIPTED_SPLINE_UPDATE_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
#define FABLE_RETAIL_STLP_LIST
#include "engine/CTCCameraPointScriptedSpline.h"
void CTCCameraPointScriptedSpline::UpdateSplines()
{
    if (KeyCameras.size() > 1) {
        CArray<C3DVector> pts, look, scalar_pts, durs, anim_speed;
        float t = 0.0f;
        float step = 1.0f / (KeyCameras.size() - 1);
        if (PTimingSpline.IsNull()) {
            PTimingSpline.Reset(new CCardinalSpline(durs));
        } else {
            long num = PTimingSpline->GetControlPointCount();
            for (long i = 0; i != num; ++i)
                PTimingSpline->DeleteControlPoint(0);
        }

        long count = 0;
        for (std::vector<CKeyCamera>::iterator it = KeyCameras.begin(); it != KeyCameras.end(); ++it) {
            CKeyCamera& cam = *it;
            pts.push_back(cam.GetPosition());
            look.push_back(cam.GetLookDirection());
            scalar_pts.push_back(C3DVector(cam.GetShuttleSpeed(), cam.GetFOV(), cam.GetRoll()));
            anim_speed.push_back(C3DVector(cam.GetAnimationSpeed(), 0.0f, 0.0f));
            PTimingSpline->AppendControlPoint(C3DVector(t, cam.GetDuration(), 0.0f));
            t += step;
        }
        PTimingSpline->SetTension(Tension);
        PPositionSpline.Reset(new CCardinalSpline(pts));
        PViewDirectionSpline.Reset(new CCardinalSpline(look));
        PScalarSpline.Reset(new CCardinalSpline(scalar_pts));
        PAnimationSpeedSpline.Reset(new CCardinalSpline(anim_speed));
        EstimateKeyIndex();
        InitialiseSpline(0, -1);
    }
}
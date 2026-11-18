// CEngine::ConvertWorldPosToScreenPos(const C3DVector&, C2DVector&) const -- project a world position into screen coordinates
// Leaf: ?ConvertWorldPosToScreenPos@CEngine@@UBE_NABVC3DVector@@AAVC2DVector@@@Z
// Retail: 0x00B242D0, 293 bytes, virtual const thiscall.
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE
#define FABLE_RETAIL_CENGINECAMERA_VIEWTOSCREEN_TWO_ARGS
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#include "engine/C2DVector.h"
#include "engine/CEngine.h"
#include "engine/CEngineCamera.h"

namespace NEngineInternal { extern CEngineCamera* GPCamera; }

bool CEngine::ConvertWorldPosToScreenPos(const C3DVector& wpos, C2DVector& mpos) const
{
    if (IsActive()) {
        C3DVector from;
        // Assign the input so the view transform reads the preserved coordinates.
        from = wpos;
        C3DVector to;
        NEngineInternal::GPCamera->TransformWorldToView(from, to);
        from = to;
        if (from.Z > 0.0f) {
            NEngineInternal::GPCamera->TransformViewToScreen(from, to);
            mpos.X = to.X;
            mpos.Y = to.Y;
            return true;
        }
    }
    return false;
}

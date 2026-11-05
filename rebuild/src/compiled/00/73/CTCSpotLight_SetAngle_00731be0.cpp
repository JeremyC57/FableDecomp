// Leaf: ?CTCSpotLight_SetAngle@@YIXPAVCTCSpotLight@@M@Z
#include "engine/CTCSpotLight.h"

void __fastcall CTCSpotLight_SetAngle(CTCSpotLight* self, float a)
{
    float* p = (float*)self->PLightInfo_Data;
    self->Overridden = true;
    *p = a;
}

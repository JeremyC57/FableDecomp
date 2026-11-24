#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVETEXT_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVETEXT_H
#ifdef FABLE_RETAIL_PROJECT_TEXT_PASS27
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "fable_string.h"
class CFontBank;
// Ego_r original record and retail loads agree on Pos +0x48 through Font +0x64.
class CEngineInternalPrimitiveText : public CEngineInternalPrimitiveBase {
public:
    C3DVector Pos;
    float Scale;
    CRGBColour Colour;
    bool ScaleIn3D;
    CWideString Text;
    CFontBank* Font;
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveText)==104);
#endif
#endif

#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVETEXT_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVETEXT_H
// Original complete class, both PDBs; native Font/Mask at30/34 after retail4byte wide string.
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "fable_string.h"
class CFontBank;class CTexture;
class CEngineSubPrimitiveText:public CEngineSubPrimitiveBase {
public:
    C3DVector Pos;
    float Scale;
    CRGBColour Colour;
    CWideString Text;
    const CFontBank* Font;
    const CTexture* Mask;
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveText)==56);
#endif

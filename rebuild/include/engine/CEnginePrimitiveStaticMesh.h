#ifndef FABLE_ENGINE_CENGINEPRIMITIVESTATICMESH_H
#define FABLE_ENGINE_CENGINEPRIMITIVESTATICMESH_H
#ifdef FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_PASS22
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CRGBColour.h"
class CEnginePrimitiveStaticMesh:public CEnginePrimitive {
public:
    CEnginePrimitiveStaticMesh(unsigned __int64,unsigned long,const C3DVector&,const CRightHandedSet&,float,bool,unsigned char,CRGBColour,float,EEngineInterpolationSet,const CEnginePrimitiveThemeChannelDesc&,bool);
    unsigned long MeshIndex;
    C3DVector WorldPos;
    float Size,FadeStart;
    CRightHandedSet RightHandedSet;
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
    CRGBColour TintColour;
    unsigned char Alpha;
    unsigned char Lit:1;
    bool BulletTimeAffectsCloth;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveStaticMesh)==76);
#endif
#endif
// Retail opt-in (Codex pass22): Original Ego_r size76 and complete fields; FableWin uses8-byte CIVCountedPointer and size80. Native0077BD70 ctor177.

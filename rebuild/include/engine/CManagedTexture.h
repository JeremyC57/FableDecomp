#ifndef FABLE_ENGINE_CMANAGEDTEXTURE_H
#define FABLE_ENGINE_CMANAGEDTEXTURE_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CBaseClass.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CManagedTextureData.h"
class CManagedTexture : public CBaseClass {
public:
    void TouchTexture();
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
    unsigned long GetUnusedFrames() {return Data->GetUnusedFrames();}
    bool IsValid() const {return !Data.IsNull();}
    unsigned long Sizeof() const;
#endif
private:
    CIVCountedPointer<CManagedTextureData> Data;
};
FABLE_STATIC_ASSERT(sizeof(CManagedTexture)==8);
#endif
#endif

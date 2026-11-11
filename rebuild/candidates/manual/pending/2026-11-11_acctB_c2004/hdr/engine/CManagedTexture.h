#ifndef FABLE_ENGINE_CMANAGEDTEXTURE_H
#define FABLE_ENGINE_CMANAGEDTEXTURE_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CBaseClass.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CManagedTextureData.h"
class CManagedTexture : public CBaseClass {
public:
    void TouchTexture();
#ifdef FABLE_RETAIL_CMANAGEDTEXTURE_GETTEXTURE
    // Retail opt-in (chunk 2004, row 0x009FED30): Ego_r.pdb ?GetTexture@CManagedTexture@@QAE?AV?$CCountedPointer@VCTexture@@@@XZ and
    // ?GetTexture@CManagedTextureData@@QAE?AV?$CCountedPointer@VCTexture@@@@XZ, inline in retail (0x009FEDB9: Data +4, the data's
    // Texture counted pointer +0x20 copied in place with `inc dword ptr [info]`), as GetVolumeTexture in the landed 0x009FD9B0.
    CCountedPointer<CTexture> GetTexture() { return Data->GetTexture(); }
#endif
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

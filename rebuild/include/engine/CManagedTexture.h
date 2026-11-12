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
#ifdef FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE
    // Chunk Q28 (row 0x00A69B70 CTextureManager::AllocateTexture): the manager resets Data in place (`mov esi,[esp+14h] ; mov
    // ecx,[esi+4]` ... `mov [esi+4],ebx`), and no PDB accessor of CManagedTexture sets it: CTextureManager is taken to be a friend
    // (inference from those bytes). No layout change.
    friend class CTextureManager;
#endif
private:
    CIVCountedPointer<CManagedTextureData> Data;
};
FABLE_STATIC_ASSERT(sizeof(CManagedTexture)==8);
#endif
#endif

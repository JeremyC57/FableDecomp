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
#ifdef FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
    // Retail opt-in (wf12, row 0x009FF220): Ego_r.pdb ??4CManagedTexture@@QAEABV0@ABV0@@Z (0x00A69B20) and ?Release@CManagedTexture@@QAE_NXZ
    // (0x00A69AD0), both called out of line. No layout change.
    const CManagedTexture& operator=(const CManagedTexture&);  // 0x00A69B20
    bool Release();                                             // 0x00A69AD0
#endif
private:
    CIVCountedPointer<CManagedTextureData> Data;
};
FABLE_STATIC_ASSERT(sizeof(CManagedTexture)==8);
#endif
#endif

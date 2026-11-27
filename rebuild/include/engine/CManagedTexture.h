#ifndef FABLE_ENGINE_CMANAGEDTEXTURE_H
#define FABLE_ENGINE_CMANAGEDTEXTURE_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CBaseClass.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CManagedTextureData.h"
class CManagedTexture : public CBaseClass {
public:
#ifdef FABLE_RETAIL_PASS22_RESOURCE_ORIGINAL_SPECIAL_MEMBERS
    CManagedTexture();
    virtual ~CManagedTexture();
#ifndef FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
    const CManagedTexture& operator=(const CManagedTexture&);
#endif
private:
    CManagedTexture(const CIVCountedPointer<CManagedTextureData>&);
public:
#endif
#ifdef FABLE_RETAIL_TEXTURE_MIP_REDUCE_PASS29
    CManagedTexture();
    bool Release();
    virtual ~CManagedTexture();
    const CManagedTexture& operator=(const CManagedTexture& src);
#endif
    void TouchTexture();
#ifdef FABLE_RETAIL_CMANAGEDTEXTURE_GETTEXTURE
    // Retail opt-in (chunk 2004, row 0x009FED30): Ego_r.pdb ?GetTexture@CManagedTexture@@QAE?AV?$CCountedPointer@VCTexture@@@@XZ and
    // ?GetTexture@CManagedTextureData@@QAE?AV?$CCountedPointer@VCTexture@@@@XZ, inline in retail (0x009FEDB9: Data +4, the data's
    // Texture counted pointer +0x20 copied in place with `inc dword ptr [info]`), as GetVolumeTexture in the landed 0x009FD9B0.
    CCountedPointer<CTexture> GetTexture() { return Data->GetTexture(); }
#endif
#ifdef FABLE_RETAIL_CMANAGEDTEXTURE_GETVOLUMETEXTURE
    // Re-port opt-in (audit h3c2b, 2026-11-20, row 0x009FD9B0 CGraphicDataBank::LoadVolumeGraphic): FableWin.pdb 0x03124C00
    // ?GetVolumeTexture@CManagedTexture@@QAE?AV?$CCountedPointer@VCVolumeTexture@@@@XZ and 0x03124C50
    // ?GetVolumeTexture@CManagedTextureData@@QAE?AV?$CCountedPointer@VCVolumeTexture@@@@XZ, inline in retail (the data's VolumeTexture
    // counted pointer +0x28 copied in place), the twins of GetTexture above. No layout change.
    CCountedPointer<CVolumeTexture> GetVolumeTexture() { return Data->GetVolumeTexture(); }
#endif
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
#ifdef FABLE_RETAIL_CMANAGEDTEXTURE_LOCK
    // agentC27 (row 0x009FDE80): FableWin.pdb ?Lock@CManagedTexture@@QAEXXZ / ?Unlock@CManagedTexture@@QAEXXZ (inline: the data's
    // Locked byte +0x49 set / cleared when Data is set) and the out-of-line ??1CManagedTexture@@UAE@XZ (retail 0x00A69E20).
    void Lock() { if (!Data.IsNull()) Data->SetLocked(true); }
    void Unlock() { if (!Data.IsNull()) Data->SetLocked(false); }
    virtual ~CManagedTexture();                                 // 0x00A69E20
#endif
private:
    CIVCountedPointer<CManagedTextureData> Data;
};
FABLE_STATIC_ASSERT_AT(36, sizeof(CManagedTexture)==8);
#endif
#endif

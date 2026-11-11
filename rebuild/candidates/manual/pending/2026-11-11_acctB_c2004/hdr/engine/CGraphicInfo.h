#ifndef FABLE_ENGINE_CGRAPHICINFO_H
#define FABLE_ENGINE_CGRAPHICINFO_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfoFrame.h"
#ifdef FABLE_RETAIL_CGRAPHICINFO_LOADFRAME
#include "engine/CCountedPointer.h"
class CManagedTexture;class CTexture;
#endif
class CGraphicInfo {
public:
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long index) const {return Frames+index;}
#ifdef FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
    CGraphicInfo(unsigned long index,CGraphicDataBank* bank,const CGraphicDataBank::CGraphicHeader& header);
    bool IsBump() const {return Bank->GetEntryDataType(Index)==2;}
private:
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
private:
    friend class CGraphicInfoFrame;
#endif
    CGraphicDataBank* GetBank() const {return Bank;}
#ifdef FABLE_RETAIL_CGRAPHICINFO_LOADFRAME
    // Retail opt-in (chunk 2004, row 0x009FED30 CGraphicDataBank::LoadGraphicFrame): Ego_r.pdb
    // ?InitialiseFramesAsSurfaceSections@CGraphicInfo@@AAEXAAVCManagedTexture@@CAAV?$CCountedPointer@VCTexture@@@@_N2@Z (private; the
    // `signed char` second parameter is the PDB's; out of line in retail at 0x009FE780, called by the bank with ecx = the graphic info,
    // 0x009FF190) -- the bank reads the info's header in place (Width +0x00 .. FrameDataSize +0x14, CompressedMipmapSizes +0x18), so
    // it is a friend (the original access grant is not in the PDB name sets). Declaration only, no layout change.
    friend class CGraphicDataBank;
    void InitialiseFramesAsSurfaceSections(CManagedTexture& managed_texture,signed char mipmap_level,CCountedPointer<CTexture>& texture,
                                           bool alpha,bool boolean_alpha);   // 0x009FE780
#endif
private:
#if defined(FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT) || defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE)
    CGraphicDataBank::CGraphicHeader Header;
#else
    unsigned char Header[28]; // complete CGraphicDataBank::CGraphicHeader storage
#endif
    CGraphicDataBank::ETextureType TextureType;
    CGraphicDataBank* Bank;
    long Index;
    CGraphicInfoFrame* Frames;
};
FABLE_STATIC_ASSERT(sizeof(CGraphicInfo)==44);
#endif
#endif

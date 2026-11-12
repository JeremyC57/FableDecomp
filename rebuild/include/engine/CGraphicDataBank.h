#ifndef FABLE_ENGINE_CGRAPHICDATABANK_H
#define FABLE_ENGINE_CGRAPHICDATABANK_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CBankFileAsync.h"
#include "engine/CResourceBank.h"
#include "engine/CArray.h"
#include "engine/CTexture.h"
#if defined(FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT) || defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE) || defined(FABLE_RETAIL_CGRAPHICINFO_SPRITE)
#include "engine/CPixelFormat.h"
#endif
class CGraphicInfo;class CGraphicInfoFrame;class CGraphicFrame;class CVolumeTexture;class CTextureManager;
class CGraphicDataBank : public CBankFileAsync, public CResourceBank {
public:
    class CGraphicHeader;
    enum ETextureType {TEXTURE_TYPE_OPAQUE=0,TEXTURE_TYPE_ALPHA=1,TEXTURE_TYPE_BOOLEAN_ALPHA=2,TEXTURE_TYPE_INTERPOLATED_ALPHA=3,TEXTURE_TYPE_BUMP=4,TEXTURE_TYPE_BUMP_ALPHA=5,TEXTURE_TYPE_UNCOMPRESSED_OPAQUE=6,TEXTURE_TYPE_UNCOMPRESSED_ALPHA=7,TEXTURE_TYPE_UNCOMPRESSED_BUMP=8,TEXTURE_TYPE_UNCOMPRESSED_BUMP_ALPHA=9,TEXTURE_TYPE_SIGNED=10,TEXTURE_TYPE_COUNT=11};
public:
    virtual ~CGraphicDataBank();
    virtual unsigned long GetNoFramesInGraphic(unsigned long) const;
    virtual unsigned long GetGraphicFrameWidth(unsigned long,unsigned long) const;
    virtual unsigned long GetGraphicFrameHeight(unsigned long,unsigned long) const;
    virtual unsigned long GetGraphicMaxWidth(unsigned long) const;
    virtual unsigned long GetGraphicMaxHeight(unsigned long) const;
    virtual bool IsUsingSurfaces() const;
    virtual void Close();
protected:
    virtual unsigned long GetDataFormatVersion();
    virtual void NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount>&);
    virtual void NotifyBankOpened();
    virtual void NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char>&);
    virtual void NotifyEntryAdded(unsigned long,unsigned long,const CArray<char>&);
    virtual void NotifyEntryRemoved(unsigned long);
    virtual void PrepareDataForInclusion(unsigned long,const CArray<CCharString>&,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream&,CDataOutputStream&);
public:
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
    unsigned long GetReduceSizeFrameDelay() const {return ReduceSizeFrameDelay;}
    unsigned long GetTexturePointersValidForFrames() const {return TexturePointersValidForFrames;}
    friend class CGraphicInfoFrame;
#endif
    const CGraphicInfo* PeekGraphic(unsigned long) const;
    const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long,unsigned long) const;
    const CTexture* GetBlankTexture(ETextureType) const; // native009fd150
private:
#ifdef FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
    friend class CGraphicInfo;
    ETextureType GetTextureType(const CGraphicHeader& header,bool is_bump) const;
#endif
    CArray<CGraphicInfo> GraphicList;
    CArray<CGraphicInfoFrame> GraphicFrameBuffer;
    bool Initialised;
    unsigned char StateBlock[100]; // CGraphicBankStateBlock
    long TextureSizeWarningMaxWidth,TextureSizeWarningMaxHeight;
    unsigned long UnloadFrameDelay,ReduceSizeFrameDelay,TexturePointersValidForFrames;
    long MinAvailablePreloadMemory,AvailableMemory;
    bool FreezeGraphics;
    CCountedPointer<CTextureManager> TextureManager;
    unsigned char PixelFormats[44]; // CPixelFormat[11]
    CTexture BlankTextures[11];
};
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBank)==0x30c);
#if defined(FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT) || defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE) || defined(FABLE_RETAIL_CGRAPHICINFO_SPRITE)
// Layout: original PDB nested class,28 bytes; native009fd700 copies7 words.
class CGraphicDataBank::CGraphicHeader {
public:
 unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;
 CPixelFormat PixelFormat;
 unsigned char TransparencyType,MipmapLevels,IsFlatAnimatedGraphic:1;
 unsigned long FrameDataSize,CompressedMipmapSizes[1];
 CGraphicHeader():Width(0),Height(0),Depth(0),FrameWidth(0),FrameHeight(0),FrameCount(1),PixelFormat(),TransparencyType(0),MipmapLevels(0),IsFlatAnimatedGraphic(false),FrameDataSize(0) {
  for(unsigned long c_mipmap=0;c_mipmap<1;++c_mipmap) CompressedMipmapSizes[c_mipmap]=0;
 }
};
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBank::CGraphicHeader)==28);
#endif

#else
// New header (chunk 767, row 0x00B2AD80 CEngineSurfaceCompositionManager::ChangeSceneForTextureBank). Retail RTTI
// .?AVCGraphicDataBank@@ : CBankFileAsync (+0x0), CResourceBank (+0x164) (ghidra_out/rtti_bases.tsv); sizeof 0x30C (the surface
// manager's three banks sit at +0x8C / +0x398 / +0x6A4, engine/CEngineSurfaceCompositionManager.h). The bases and members are not
// modelled yet (one opaque block); only the members rows call are declared. Ego_r.pdb / FableWin.pdb ?ChangeScene@CGraphicDataBank@@QAEXXZ,
// retail 0x009FD0C0 (landed).
#include "rebuild_abi.h"
#if defined(FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE) && !defined(FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE)
// Retail opt-in (chunk 1021, row 0x00BEC680 CEngineInternalPrimitiveMeshBase::CollectStats): the RTTI base chain's CBankFile part
// (CGraphicDataBank : CBankFileAsync : CBankFile at +0, as engine/CMeshDataBank.h's FABLE_RETAIL_CMESHDATABANK_BASES models the mesh
// bank), so CBankFile's members apply to a texture bank at offset 0 (CollectStats calls ?GetEntryDataSize@CBankFile@@QBEKK@Z
// 0x009CC240 with the bank pointer unchanged, 0x00BEC76C). CBankFileAsync (0x110..0x164) and the rest stay opaque. Same size.
#include "engine/CBankFile.h"
class CGraphicDataBank : public CBankFile {
public:
    void ChangeScene();                                      // 0x009FD0C0
private:
    unsigned char Unmodeled_0x110[0x30c - 0x110];            // CBankFileAsync +0x110.., CResourceBank +0x164, own members (not modelled)
};
#else
class CGraphicDataBank {
public:
    void ChangeScene();                                      // 0x009FD0C0
#ifdef FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
    // Retail opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): the two graphic-size virtuals the row
    // calls through the bank's vtable, FableWin.pdb ?GetGraphicMaxWidth@CGraphicDataBank@@UBEKK@Z / ?GetGraphicMaxHeight@..@@UBEKK@Z at
    // slots 23 (+0x5C, 0x00A014D0) and 24 (+0x60, 0x00A01540) of ??_7CGraphicDataBank@@6B@ (rebuild/manifest/rtti-vtables.tsv). The
    // bases' 23 earlier slots are not modelled (placeholders keep the numbering); the vptr takes the first dword of the opaque block.
    virtual void _vslot00(); virtual void _vslot01(); virtual void _vslot02(); virtual void _vslot03(); virtual void _vslot04();
    virtual void _vslot05(); virtual void _vslot06(); virtual void _vslot07(); virtual void _vslot08(); virtual void _vslot09();
    virtual void _vslot10(); virtual void _vslot11(); virtual void _vslot12(); virtual void _vslot13(); virtual void _vslot14();
    virtual void _vslot15(); virtual void _vslot16(); virtual void _vslot17(); virtual void _vslot18(); virtual void _vslot19();
    virtual void _vslot20();
#ifdef FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE
    // Retail opt-in (Codex 2026-11-09): FableWin03117420/03117450,
    // native006512b9/006512dd dispatch through21/22 with graphic and frame0.
    virtual unsigned long GetGraphicFrameWidth(unsigned long,unsigned long) const;
    virtual unsigned long GetGraphicFrameHeight(unsigned long,unsigned long) const;
#else
    virtual void _vslot21(); virtual void _vslot22();
#endif
    virtual unsigned long GetGraphicMaxWidth(unsigned long graphic) const;    // slot 23 (+0x5C) 0x00A014D0
    virtual unsigned long GetGraphicMaxHeight(unsigned long graphic) const;   // slot 24 (+0x60) 0x00A01540
private:
    unsigned char Unmodeled_0x004[0x308];                    // CBankFileAsync +0x4.., CResourceBank +0x164, own members (not modelled)
#else
private:
    unsigned char Unmodeled_0x000[0x30c];                    // CBankFileAsync +0x0, CResourceBank +0x164, own members (not modelled)
#endif
};
#endif
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBank) == 0x30c);
#endif
#endif // FABLE_ENGINE_CGRAPHICDATABANK_H

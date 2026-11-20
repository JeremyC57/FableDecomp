// CManagedTextureData::CreateVolumeTexture(ulong,ulong,ulong,long,const CPixelFormat&) -- round dimensions, replace volume object, initialise it and record allocation result.
// Leaf: ?CreateVolumeTexture@CManagedTextureData@@QAE_NKKKJABVCPixelFormat@@@Z
// Native00A699D0 length169;9FA620 valid dimensions;99A2D0 base ctor;A6AE60 counted reset;A68370 volume initialise;BFEA1A scalar new16.
// Re-port (audit h3c2b, 2026-11-20; widened shared-header policy): the three local class models (CTexture, CVolumeTexture and the
//   CManagedTextureData wrapper over the renamed donor header, with its reinterpret_cast Volume() helper) are gone. The class is the
//   retail engine/CManagedTextureData.h (FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS: VolumeTexture is the CCountedPointer<CVolumeTexture>
//   member; new FABLE_RETAIL_CMANAGEDTEXTUREDATA_CREATEVOLUMETEXTURE declares this member), CTexture is engine/CTexture.h
//   (FABLE_RETAIL_CTEXTURE_NEXTVALIDDIMENSIONS) and CVolumeTexture is engine/CVolumeTexture.h (new FABLE_RETAIL_CVOLUMETEXTURE_INITIALISE:
//   the RTTI base CBaseClass, the inline default constructor, Initialise, Ego_d.pdb's EVolumePool enumerators -- the earlier text
//   had a made-up VolumePoolManaged).
//   Text in Ego_d.pdb's spelling (lines 93..128): parameters width / height / depth / mipmap_levels / format and ONE local,
//   rounded_dimensions (the earlier x / y / size / volume locals are not in Ego_d); the rounded extents are assigned back to the
//   width / height parameters (lines 96 / 97, 6 bytes each), Width / Height stored from them (101 / 102, 11 bytes each), then
//   `Allocated = VolumeTexture->Initialise(..)` (125) and `return Allocated;` (127, 6 bytes).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CMANAGEDTEXTUREDATA_CREATEVOLUMETEXTURE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_NEXTVALIDDIMENSIONS
#define FABLE_RETAIL_CVOLUMETEXTURE_INITIALISE
#include "engine/C2DExtentsI.h"
#include "engine/C3DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CVolumeTexture.h"
#include "engine/CManagedTextureData.h"
bool CManagedTextureData::CreateVolumeTexture(unsigned long width, unsigned long height, unsigned long depth, long mipmap_levels, const CPixelFormat& format)
{
    C2DExtentsI rounded_dimensions = CTexture::GetNextValidDimensions(C2DExtentsI(width, height), format);
    width = rounded_dimensions.X;
    height = rounded_dimensions.Y;
    VolumeTexture.Reset(new CVolumeTexture);
    Width = (unsigned short)width;
    Height = (unsigned short)height;
    Allocated = VolumeTexture->Initialise(C3DExtentsI(width, height, depth), 1, format, 0, VOLUME_POOL_MANAGED, false);
    return Allocated;
}
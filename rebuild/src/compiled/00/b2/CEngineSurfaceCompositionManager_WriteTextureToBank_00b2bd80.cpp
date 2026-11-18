// CEngineSurfaceCompositionManager::WriteTextureToBank(CTexture& texture, CGraphicDataBank& bank_file, unsigned long bank_index, bool is_alpha) -- size the bank entry for every mip level, copy each level's read-locked surface bytes into the entry through a zeroed staging array, then build the graphic header (dimensions, pixel format, transparency, mip count, top-level size), save it to a memory stream and write it as the entry's info
// Leaf: ?WriteTextureToBank@CEngineSurfaceCompositionManager@@AAEXAAVCTexture@@AAVCGraphicDataBank@@K_N@Z
// Retail: 0x00B2BD80, 641 bytes (ret 10h).
// Names: Ego_d.pdb parameters / locals (texture, bank_file, bank_index, is_alpha; no_levels, data_size, c_level, top_surface_size,
//        current_offset, surface, surface_size, surface_data, locked_surface, dimensions, header, outstream, instream) in Ego_d's scopes.
// FITTED (chosen by grading, Ego_d gives names / scopes only): `data_size` declared before `no_levels` (retail zeroes ebp / edi before the
//        GetNoLevels call); `dimensions` built in one constructor from two GetDimensions(0) calls (retail calls it twice, .Y first);
//        `TransparencyType = is_alpha ? 1 : 0` (retail `setne`; the transparency enumerators are not known).
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CTEXTURE_GETNOLEVELS
#define FABLE_RETAIL_CTEXTURE_GETLEVELBYTELENGTH
#define FABLE_RETAIL_CTEXTURE_FORMATQUERIES
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_READONLYLOCK
#define FABLE_RETAIL_CSURFACE_LOADGRAPHICFRAME
#define FABLE_RETAIL_CBANKFILE_ENTRYWRITE
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICHEADER_SAVE
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include <string.h>
#include "engine/C2DExtentsI.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CEngineSurfaceCompositionManager.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
#include "engine/CArray.h"
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CMemoryDataInputStream.h"

void CEngineSurfaceCompositionManager::WriteTextureToBank(CTexture& texture, CGraphicDataBank& bank_file, unsigned long bank_index, bool is_alpha)
{
    unsigned long data_size = 0;
    unsigned long no_levels = texture.GetNoLevels();
    for (unsigned long c_level = 0; c_level < no_levels; ++c_level)
        data_size += texture.GetLevelByteLength(c_level);
    bank_file.InternalResizeEntryData(bank_index, data_size);

    unsigned long top_surface_size = texture.GetLevelByteLength(0);
    unsigned long current_offset = 0;
    for (unsigned long c_level = 0; c_level < no_levels; ++c_level)
    {
        CSurface surface = texture.GetSurfaceLevel(c_level);
        unsigned long surface_size = surface.GetByteLength();
        CArray<unsigned char> surface_data(surface_size);
        CSurfaceLockedRect locked_surface = surface.ReadOnlyLock(0);
        memcpy(surface_data.GetAsCArray(), locked_surface.PMem, surface_size);
        surface.Unlock();
        bank_file.InternalWriteEntryData(bank_index, current_offset, surface_data.GetAsCArray(), surface_size);
        current_offset += surface_size;
    }

    C2DExtentsI dimensions(texture.GetDimensions(0).X, texture.GetDimensions(0).Y);
    CGraphicDataBank::CGraphicHeader header;
    header.Width = (unsigned short)dimensions.X;
    header.Height = (unsigned short)dimensions.Y;
    header.Depth = 1;
    header.FrameWidth = (unsigned short)dimensions.X;
    header.FrameHeight = (unsigned short)dimensions.Y;
    header.FrameCount = 1;
    header.PixelFormat = texture.GetPixelFormat();
    header.TransparencyType = is_alpha ? 1 : 0;
    header.MipmapLevels = (unsigned char)no_levels;
    header.IsFlatAnimatedGraphic = false;
    header.FrameDataSize = top_surface_size;

    CMemoryDataOutputStream outstream;
    header.Save(outstream);
    CMemoryDataInputStream instream(outstream.PeekData(), outstream.GetLength());
    bank_file.UpdateEntryInfoOnly(bank_index, instream, outstream.GetLength());
}
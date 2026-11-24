// 009fe780 CGraphicInfo::InitialiseFramesAsSurfaceSections
// Leaf: ?InitialiseFramesAsSurfaceSections@CGraphicInfo@@AAEXAAVCManagedTexture@@CAAV?$CCountedPointer@VCTexture@@@@_N2@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CGRAPHICDATABANK_SURFACECOMPOSITION
#define FABLE_RETAIL_CGRAPHICINFO_FRAMESECTIONS_PASS27
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#include "engine/CGraphicInfo.h"

void CGraphicInfo::InitialiseFramesAsSurfaceSections(CManagedTexture& managed_texture,
    signed char mipmap_level, CCountedPointer<CTexture>& texture, bool is_alpha, bool is_boolean_alpha)
{
    long tlx = 0;
    long tly = 0;
    long frame_width = Header.FrameWidth;
    long frame_height = Header.FrameHeight;
    long width = Header.Width;
    long height = Header.Height;
    for (unsigned long c_frame = 0; c_frame < Header.FrameCount; ++c_frame) {
        if (tlx + frame_width > width) {
            tlx -= width;
            tly += frame_height;
        }
        CGraphicFrame frame;
        frame.InitialiseAsSurfaceSection(texture, (float)tlx / width, (float)tly / height,
            frame_width, frame_height, is_alpha, is_boolean_alpha);
        CGraphicInfoFrame& info_frame = Frames[c_frame];
        info_frame.InitialiseFrame(frame, managed_texture, mipmap_level);
        tlx += frame_width;
    }
}
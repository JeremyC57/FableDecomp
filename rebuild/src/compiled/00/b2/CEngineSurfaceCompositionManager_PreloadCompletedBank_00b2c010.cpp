// 00b2c010 CEngineSurfaceCompositionManager::PreloadCompletedBank
// Flags: none
#define FABLE_RETAIL_STLP_RB_GLOBAL_INCREMENT_SIZE_BODY_PASS27
#define FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY
// Leaf: ?PreloadCompletedBank@CEngineSurfaceCompositionManager@@AAEXXZ
#define FABLE_RETAIL_PRELOAD_COMPLETED_BANK_PASS29
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINESURFACECOMPOSITION_GETCOMPOSED_PASS26
#define FABLE_RETAIL_CBANKFILE_GETSIZE_PASS26
#define FABLE_RETAIL_CCOMPOSITESURFACEINFO_MORPH_PASS26
#define FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_GETFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_TOUCHFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_CALCCOLOURDIFFERENCE
#define FABLE_RETAIL_CRGBCOLOUR
#include "fable_string.h"
#include "engine/CEngineSurfaceCompositionManager.h"
#include "engine/CCompositeSurfaceInfo.h"
#include "engine/CGraphicInfo.h"
#include "engine/C2DExtentsI.h"
#include "engine/CResourceList.h"
#include "engine/GFSqr.h"
#include "engine/CCompositeSurfaceCache.h"
#include "engine/GFGetTime.h"
void CEngineSurfaceCompositionManager::PreloadCompletedBank()
{
    long most_recent = 0x7fffffff;
    std::set<unsigned long> preload_indices;
    if (false) {} else for (std::list<CCountedPointer<CCompositeSurfaceCache> >::iterator itr = CachedSurfaces.begin(); itr != CachedSurfaces.end(); ++itr) {
        CCompositeSurfaceCache& surface = **itr;
        unsigned long index = FindIndexInCompletedBank(surface.GetCurrentMorph());
        if (index && surface.LastUsedFrame) {
            long frames_since_used = CurrentFrame - surface.LastUsedFrame;
            most_recent = most_recent < frames_since_used ? most_recent : frames_since_used;
        }
    }
    if (false) {} else for (std::list<CCountedPointer<CCompositeSurfaceCache> >::iterator itr = CachedSurfaces.begin(); itr != CachedSurfaces.end(); ++itr) {
        CCompositeSurfaceCache& surface = **itr;
        unsigned long index = FindIndexInCompletedBank(surface.GetCurrentMorph());
        if (index && surface.LastUsedFrame) {
            long frames_since_used = CurrentFrame - surface.LastUsedFrame;
            if (frames_since_used <= most_recent + 100)
                preload_indices.insert(index);
        }
    }
    double start_t = GFGetTime();
    bool loading = true;
    long preload_count = 0;
    while (loading) {
        loading = false;
        preload_count = 0;
        if (false) {} else for (std::set<unsigned long>::iterator itr = preload_indices.begin(); itr != preload_indices.end(); ++itr) {
            CGraphicFrame frame;
            const CGraphicInfoFrame* p_graphic_info_frame = CompletedBankFile.PeekGraphicInfoFrame(*itr,0);
            if (p_graphic_info_frame->GetFrame(frame,0,0))
                ++preload_count;
            else
                loading = true;
        }
        double delta_t = GFGetTime() - start_t;
        if (delta_t >= MaxPreloadTime)
            break;
        CompletedBankFile.FrameEnd();
        CompletedBankFile.ProcessLoadingQueue(true, MaxPreloadTime + start_t);
    }
    if (preload_count) {
        double delta_t = GFGetTime() - start_t;
    }
}
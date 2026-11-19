// NUISystem::CMorphingSprite::GetDataFromDef() -- records every state's graphic index in GraphicIndices and starts loading (and forces) each state graphic's first frame
// Leaf: ?GetDataFromDef@CMorphingSprite@NUISystem@@QAEXXZ
// Grade: RELOCATION_MATCH (727 bytes). The same-TU hinted insert_unique body it calls grades RELOCATION_MATCH at its own address
//   0x005537F0 from this source; the _M_insert / insert_unique(v) bodies this TU sees are bound by retail to framed size-TU COMDATs
//   (0x004313E9 / 0x00431363, `push ebp ; mov ebp,esp`) from another object, so this TU's copies are not retail's text there.
// Retail: Fable.exe (TLC, VC7.1) @ 0x005542F0, 727 bytes, thiscall, no frame pointer.
// Identity: Ego_r.pdb release\ui_morphing_sprite.obj #5 (0x005CD490, 680 bytes) and FableWin.pdb debug\ui_morphing_sprite.obj #11
//   (0x022A8CB0, 418 bytes) ?GetDataFromDef@CMorphingSprite@NUISystem@@QAEXXZ; FableWin.pdb locals pdefinition, count (block),
//   map_entry, pinfo, frame (FableWin.symbols.txt S_BPREL32 records).
// Callees: CComponent::LoadAndReturnDef (slot 108, +0x1B0), _Rb_tree<..>::insert_unique(iterator, const value_type&) 0x005537F0,
//   NUISystem::CManager::GetInstance 0x0041E5F2 (GetGraphicsBank inline), CGraphicInfoFrame::BeginLoading 0x009FE620, the inline
//   GetFrame chain (CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90, CFrameData::BeginLoading 0x009FDE80, new CFrameData
//   0x009FE380, InternalGetFrameForcedLoading 0x009FF6B0, CGraphicFrameData::Uninitialise 0x009FCBA0), operator new 0x00BFEA1A /
//   delete 0x00BFE9BC.
// Shape: FableWin's statement order and names on the shared headers, with the opt-in set of the landed NUISystem::CIconText::AddIcon
//   0x00542780 (the same PeekGraphicInfoFrame / BeginLoading / GetFrame chain). The map opt-ins of the landed 0x0061B610 hinted insert
//   (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY + FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY: the _tree.c bodies visible, as in every
//   retail TU that includes <map>) let VC7.1 see that insert_unique only reads map_entry: the pair's stores then drop their memory
//   edges and fall where retail has them (map_entry.second after the argument pushes, .first last before the call; c2 DAG dump +
//   sim.py, NOTES.md). Without them: 727v727, 3 instructions.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_CTEXTURE
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CUIStateDef.h"
#include "engine/NUISystem_CMorphingSprite.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CGraphicInfo.h"

namespace NUISystem {
void CMorphingSprite::GetDataFromDef()
{
    CDefPointer<const CUIDef> pdefinition = LoadAndReturnDef();
    for (unsigned long count = 0; count < pdefinition->States.size(); ++count) {
        std::pair<const unsigned long, unsigned long> map_entry(count, pdefinition->States[count].GraphicIndex);
        GraphicIndices.insert(GraphicIndices.begin(), map_entry);
        const CGraphicInfoFrame* pinfo = CManager::GetInstance()->GetGraphicsBank()->PeekGraphicInfoFrame(pdefinition->States[count].GraphicIndex, 0);
        pinfo->BeginLoading(0, 0x10);
        CGraphicFrame frame;
        pinfo->GetFrame(frame, 0, 1);
    }
}
}
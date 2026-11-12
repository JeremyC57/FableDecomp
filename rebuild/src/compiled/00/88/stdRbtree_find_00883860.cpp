// std::_Rb_tree<CThing*, pair<CThing* const, NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::CThingGraphicByPassFlagState>, _Select1st<..>, less<CThing*>, allocator<..> >::find<CThing*>(CThing* const&) -- STLport 4.6 member-template find: lower-bound walk from the root, end() unless the found key is not greater; iterator returned through the hidden pointer
// Leaf: ??$find@PAVCThing@@@?$_Rb_tree@PAVCThing@@U?$pair@QAVCThing@@VCThingGraphicByPassFlagState@CScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@U?$_Select1st@U?$pair@QAVCThing@@VCThingGraphicByPassFlagState@CScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@@3@U?$less@PAVCThing@@@3@V?$allocator@U?$pair@QAVCThing@@VCThingGraphicByPassFlagState@CScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@QAVCThing@@VCThingGraphicByPassFlagState@CScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@U?$_Nonconst_traits@U?$pair@QAVCThing@@VCThingGraphicByPassFlagState@CScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@@2@@1@ABQAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00883860, 71 bytes.
// Naming (agentNF31, 2026-11-12): was the placeholder _Rb_tree<CUnprovenKey_00883860, ..>::find over a 345-line local STLport model.
//   Descriptive-by-library: retail is STLport 4.6.1 while Ego_r.pdb / FableWin.pdb are Dinkumware builds, so no PDB carries this
//   STLport spelling. The key/value are proven by (1) FableWin.pdb _fablewin_api 0x01817856 / 0x02A5B6F0 Dinkumware
//   _Tree<_Tmap_traits<CThing*, CThingGraphicByPassFlagState, ..> >::find (the find of map<CThing*, CThingGraphicByPassFlagState>);
//   (2) Ego_r.pdb 0x008C43B0 map<CThing*, CThingGraphicByPassFlagState>::operator[] (retail 0x00884C70, called by the landed
//   ScreenFilterSetFlags 0x00881A40 on the same map); (3) that landed caller binds exactly this symbol at 0x00881ABB
//   (RELOCATION_MATCH). Same form as the landed 0x00553520 (STLport member-template find, explicit instantiation of
//   stlport/fable_map.h under FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE); headers and opt-ins are 0x00881A40's.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_BYPASSCOLORFILTER
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_SCREENFILTERENTRY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_FIND
#define FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"

typedef std::pair<CThing* const, NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry::CThingGraphicByPassFlagState> CByPassValue;
typedef std::_Rb_tree<CThing*, CByPassValue, std::_Select1st<CByPassValue>, std::less<CThing*>, std::allocator<CByPassValue> > CByPassTree;
template CByPassTree::iterator CByPassTree::find<CThing*>(CThing* const&);

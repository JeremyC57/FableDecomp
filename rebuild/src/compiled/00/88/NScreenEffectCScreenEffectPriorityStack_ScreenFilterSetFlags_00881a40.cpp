// NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags(const ThingGraphicArgPassers& things, void* handle) -- when the
// handle is a live screen-filter entry (found in ScreenFilterKeys), first restore and forget every listed thing's saved colour-filter
// bypass state (the entry's reset map), then for every listed live thing with a graphic appearance save its current bypass state into
// the reset map and apply the requested bypass flag.
// Leaf: ?ScreenFilterSetFlags@CScreenEffectPriorityStack@NScreenEffect@@QAEXABV?$vector@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@@std@@@std@@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00881A40, 581 bytes, __fastcall `ret 8`. Identity: Ego_r.pdb + FableWin.pdb (same mangled name).
// Callees: set<CStackEntry*>::find 0x008839E0 (tree const find, out of line); map<CThing*,CThingGraphicByPassFlagState>::find
// 0x00883860 (STLport member-template _Rb_tree::find<CThing*>, see FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE below) and
// operator[] 0x00884C70; _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E, free 0x00BFEA14 (erase(iterator) expanded);
// CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10;
// NGraphicAppearance::CTCGraphicAppearance::SetByPassColorFilter 0x004BFB00.
// Shape: CThingGraphicByPassFlagState::Reset (PDB, inline) is the loop-1 restore; loop 2 tests the thing through one reference
// (two GetPItem calls on the same element address) and re-indexes for the graphic; GetByPassColorFilter (PDB, inline) is the setg.
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
// FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE (__declspec(noinline) on the STLport tree find, overlay fable_map.h) is a
// compiler-steering opt-in, justified by two retail facts at the loop-1 lookup:
//  (a) retail calls 0x00883860 out of line: 0x00881AB4 lea eax,[esp+14h] / 0x00881AB8 push eax / 0x00881AB9 mov ecx,ebx /
//      0x00881ABB call 0x00883860 (fully inline STLport find, the default spelling, expands the walk: 609 bytes, 32 hunks);
//  (b) retail's compiler saw the find BODY: after the call it reloads the result node into edi and adds 14h in place --
//      0x00881AC0 8B7C2410 mov edi,[esp+10h] / 0x00881AC4 3B3B cmp edi,[ebx] / 0x00881AC8 83C714 add edi,14h. With the find
//      only declared (FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL, an opaque call) VC7.1 emits 8B442410 mov eax,[esp+10h] /
//      3B03 cmp eax,[ebx] / 8D7814 lea edi,[eax+14h] at the same addresses (581 bytes, 2 hunks). Body visible + noinline is the
//      only spelling graded that gives both (a) and (b) (ATTEMPTS.log: x_inl / x_decl / x_def, repair 3).
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE
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
namespace NScreenEffect {
void CScreenEffectPriorityStack::ScreenFilterSetFlags(const ThingGraphicArgPassers& things, void* handle)
{
    // The found iterator lives in a block of its own: retail keeps the key local at [esp+10h] and the hidden find result at
    // [esp+0Ch] ("lea ecx,[esp+10h]; push ecx; mov ecx,esi" at 0x00881A54, then "mov edx,[esp+0Ch]; cmp edx,[esi]" at 0x00881A64
    // against end() through esi = lea [ecx+0A0h] at 0x00881A49). Spelled inline (no named local) the compiler loads the header
    // instead (mov esi,[ecx+0A0h]; cmp [esp+0Ch],esi: 4 hunks); a function-scope local gives 22 hunks.
    {std::set<CStackEntry*>::iterator found = ScreenFilterKeys.find((CStackEntry*)handle);
    if (found == ScreenFilterKeys.end())
        return;}
    for (int i = 0; i < (int)things.size(); ++i) {
        std::map<CThing*, CScreenFilterEntry::CThingGraphicByPassFlagState>::iterator it =
            ((CScreenFilterEntry*)handle)->reset.find(static_cast<CThing*>(things[i].thing.GetPItem()));
        if (it != ((CScreenFilterEntry*)handle)->reset.end()) {
            (*it).second.Reset();
            ((CScreenFilterEntry*)handle)->reset.erase(it);
        }
    }
    for (int j = 0; j < (int)things.size(); ++j) {
        const CIntelligentPointer<CThing>& thing = things[j].thing;
        if (thing.GetPItem() != 0 && !static_cast<CThing*>(thing.GetPItem())->IsDead()) {
            NGraphicAppearance::CTCGraphicAppearance* graphic = static_cast<CThing*>(things[j].thing.GetPItem())->GetTCGraphicAppearance();
            if (graphic) {
                CScreenFilterEntry::CThingGraphicByPassFlagState state;
                state.thing = things[j].thing;
                state.bypassReset = graphic->GetByPassColorFilter();
                ((CScreenFilterEntry*)handle)->reset[static_cast<CThing*>(state.thing.GetPItem())] = state;
                graphic->SetByPassColorFilter(things[j].bypassSet);
            }
        }
    }
}
}
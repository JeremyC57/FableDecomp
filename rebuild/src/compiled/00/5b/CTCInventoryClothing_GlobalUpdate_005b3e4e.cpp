// CTCInventoryClothing::GlobalUpdate(bool paused) -- base update, then keep the mannequin playing DEFAULT (or EMBARRASSED_TWIRL when it wears nothing covering) and advance its animation one frame
// Leaf: ?GlobalUpdate@CTCInventoryClothing@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B3E4E, 543 bytes, thiscall `leave ; ret 4`, frame 0x58. Vtable 0x0124356C slot 26;
//   Ego_r.pdb / FableWin.pdb ?GlobalUpdate@CTCInventoryClothing@@UAEX_N@Z.
// Callees (PDB spellings): NInventory::CTCInventoryBase::GlobalUpdate 0x005BDD39 (qualified), CDefClassBase::GetSubDef<CAppearanceDef>
//   0x0042B0A2, CAnimationSet::GetAnim 0x005DC340 (returns a reference), CTCHeroAttachableAppearanceModifiers::
//   GetAppearanceModifierListWithCoversBodyAreaFlags 0x00706B50 / UploadChangesToGraphics 0x00707930 (called directly; Ego_r.pdb QAEXXZ), the list<long>
//   destructor 0x0042AB73, CTCAnimationComplex::GetAnimation 0x0070B460, CAnimationEntry::GetAnimateThingFlags 0x005DC390 /
//   GetTransitionTime 0x005DC1D0, CAnimInit::Init 0x0070C050, CAnimateThing::SetMainAnim 0x0070D580 / Update 0x0070E8B0;
//   inline: IsInventoryOpen (+0x58), GetPBaseDef, the GetTC expansions (keys 0x5E / 0x5A), CAnimFilterNoGroup temporaries
//   (vtables 0x0122FFDC / 0x0122FFCC), CAnimSet::GetAnimIndex / GetFrame, CMainGameComponent::GetConstantFPS (0x01375550).
// Shape (the levers, each measured): the same-TU GetSubDef body keeps `appearance` uncaptured, so retail's `&appearance->Animation`
//   is computed once in ebx for both GetAnim calls; `anim` starts at DEFAULT and only the empty-list test re-points it (no else:
//   anim shares DEFAULT's slot, retail's `mov eax,[ebp-14h] ... mov [ebp-14h],eax`); CAnimInit built by its argument
//   constructor (the flags call before the Frame zeroing); the CAnimateThingUpdate a named local (its own frame words, 0x58).
// Graded with FABLE_HDR_OVERLAY=$A/hdr (hdr/README.md: CAnimInit / CAnimSet / CAnimateThing / CAnimFilterNoGroup / CAnimationEntry /
//   CTCHeroAttachableAppearanceModifiers / NInventory_CTCInventoryBase / CTCInventoryClothing opt-ins, new CAnimateThingUpdate /
//   CAnimFilterBase / CAnimationSet / CAppearanceDef / CTCAnimationComplex headers).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ISOPEN
#define FABLE_RETAIL_CTCINVENTORYBASE_GLOBALUPDATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_GLOBALUPDATE
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_COVERS
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_UPLOADCHANGES
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMSET_FRAME
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryClothing.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimateThing.h"
#include "engine/CDefinitionManager.h"
#include "fable_game.h"

// Same-TU body of the def lookup (a header template in the original; out of line in retail, GetSubDef<CAppearanceDef> 0x0042B0A2).
// Visible here, VC7.1 sees that it does not keep the out pointer, so the CDefPointer stays uncaptured and the Animation address
// is computed once for both GetAnim calls (retail `add ebx,34h` 0x005B3EC0, `mov ecx,ebx` 0x005B3EFF).
#pragma auto_inline(off)
template <class T>
bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

void CTCInventoryClothing::GlobalUpdate(bool paused)
{
    NInventory::CTCInventoryBase::GlobalUpdate(paused);
    if (IsInventoryOpen() && PMannequin) {
        CDefPointer<const CAppearanceDef> appearance;
        PMannequin->GetPBaseDef()->GetSubDef(&appearance);
        const CAnimationEntry& defaultAnim = appearance->Animation.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
        const CAnimationEntry& twirlAnim = appearance->Animation.GetAnim("EMBARRASSED_TWIRL", 0, CAnimFilterNoGroup());
        const CAnimationEntry* anim = &defaultAnim;
        CTCHeroAttachableAppearanceModifiers* modifiers;
        if (PMannequin->GetTC(&modifiers)) {
            if (modifiers->GetAppearanceModifierListWithCoversBodyAreaFlags(4, APPEARANCE_CLOTHING).empty())
                anim = &twirlAnim;
            modifiers->UploadChangesToGraphics();
        }
        CTCAnimationComplex* complex = 0;
        PMannequin->GetTC(&complex);
        CAnimateThing& animator = complex->GetAnimation();
        if (animator.GetMainAnim().GetAnimIndex() != anim->BankIndex) {
            CAnimInit init(6, 0, anim->BankIndex, ANIM_UPDATE_FROM_SPEED, true, anim->GetAnimateThingFlags(), 100);
            init.Frame = animator.GetMainAnim().GetFrame();
            animator.SetMainAnim(init, anim->GetTransitionTime(), 0);
        }
        CAnimateThingUpdate update(1.0f / CMainGameComponent::GetConstantFPS(), 0.0f);
        animator.Update(update);
    }
}
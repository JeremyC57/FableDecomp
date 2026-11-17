// CCreatureActionBase::FinishAction() -- base action finish: drop this action from the creature's haste component, fade out the action's single anim set, mark the action finished, re-equip the item removed for it (now, or after the cutscene for the player), and kill carried things and the creature itself when they carry the die-with-action interface 0xFD
// Leaf: ?FinishAction@CCreatureActionBase@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00694430, 656 bytes, plain `ret`; virtual slot 4 of CCreatureActionBase.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, PItemRemoved +0x2c), CVectorMap::LowerBound 0x0040F020
//   (inline lookups 0x8C CTCHaste, 0x46 CTCCarrying), the haste set erase-by-key 0x0058119B and recalculation 0x00800020
//   (manifest labels CBankFile::CompareDataOffset::operator() / CAnimation_RecalculateScale are propagated),
//   GetSingleActionAnimSet 0x00693250 (__fastcall), CAnimSet::SetAsFinished 0x00668A50, this's virtual slots 9 (+0x24, the
//   fade-out frame count) and 37 (+0x94, whether to re-equip PItemRemoved), CCreatureAction_UnsheatheItemFromInventory ctor
//   0x006A0150 and its folded destructor 0x005CC091, CThingCreatureBase::SetCurrentAction 0x006644F0, the creature's
//   virtual slot 88 (+0x160: CThingPlayerCreature returns true, CThingAICreature false), CThingCreatureBase::
//   SetPItemToUnsheatheAfterCutscene 0x00662450 (sets +0x150; manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is
//   wrong), CTCCarrying::GetAllItems 0x006A2220 (manifest "COptimisedPrimitive::RemapGroups" is propagated) / RemoveThing
//   0x006A19B0, free 0x00BFEA14 (the item vector's buffer), CThing::Kill(bool) 0x004C9B80.
// Globals: CThing::GetPCurrentThingManager() 0x013B8A1C (NonScriptedEntitesPauseMode +0x54, AllEntitesPauseMode +0x55).
// Layout: Ego_r.pdb CCreatureActionBase agrees with retail below +0x84 (PCreature +8, PItemRemoved +0x2c, AddedActionAnim
//   +0x60, Finished +0x61, PAnimEntry +0x74) but its vectors/map are 16 bytes (retail 12, sizeof 0xa8) and the generated
//   header carries the intelligent pointers as opaque bytes, so `this` is modelled locally. CThing: interface bitset +0x20
//   (0xFD = bit 29 of +0x3c), map +0x44, dead bit +0x91; CAnimSet UpdateMode +0, Finished bit 1 of +0x51.
// Shape: the anim entry is a named local read after the alive test (retail keeps it in esi across GetPItem); the pause test
//   reads the thing manager through a STATIC creature accessor (`PCreature.GetPItem()->PeekPCurrentThingManager()`): the call is
//   still made for its side effects and the global load follows it (a non-static inline hoists the global above the call);
//   the haste and carrying components use the out-parameter GetInterface; the carried-thing vector is released with free.
// NF66 (round 7): ported onto engine/CCreatureActionBase.h (FULL_MEMBERS), CThingCreatureBase.h, CThing.h, CTCHaste.h (UNSET_INLINE,
//   as the same-TU 0x00694020), CAnimSet.h, CTCCarrying.h and CCreatureAction_UnsheatheItemFromInventory.h; slot 9 is
//   GetAnimationTransitionOutTime and slot 37 IsToReplaceItemIfRemoved (header order), the creature's slot 88 is
//   IsToUnsheatheItemsAfterCutscenes. The anim-set helper 0x00693250 is file-local in retail (Ego_r anonymous-namespace
//   ?GetPActionAnimSet@?A0x..@@YIPAVCAnimSet@@AAVCThingCreatureBase@@J@Z; the namespace hash is per TU and cannot be reproduced).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETPITEMTOUNSHEATHE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_UNSET_INLINE
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCCARRYING_GETALLCARRIEDTHINGS
#define FABLE_RETAIL_CANIMSET_SETASFINISHED_DECL
#define FABLE_RETAIL_CANIMSET_SETUPDATEMODE
#define FABLE_RETAIL_C3DVECTOR
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingManager.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"
#include "engine/CTCHaste.h"
#include "engine/CAnimSet.h"
#include "engine/CAnimationEntry.h"
#include "engine/CTCCarrying.h"

namespace {
CAnimSet* __fastcall GetPActionAnimSet(CThingCreatureBase& creature, long anim);   // 0x00693250 (file-local in retail)
}

void CCreatureActionBase::FinishAction()
{
    if (PCreature.Get() && !PCreature.Get()->IsDead()) {
        CTCHaste* haste;
        if (PCreature.Get()->GetTC(TCI_HASTE, &haste))
            haste->UnsetActionSpeedMultiplier(this);
    }
    if (PCreature.Get() && !PCreature.Get()->IsDead()) {
        CAnimationEntry* entry = PAnimEntry.Get();
        if (entry && entry->BankIndex && AddedActionAnim) {
            CAnimSet* animSet = GetPActionAnimSet(*PCreature.Get(), entry->BankIndex);
            if (animSet) {
                animSet->SetUpdateMode(ANIM_UPDATE_FROM_SPEED);
                long frames = GetAnimationTransitionOutTime();
                if (frames > 0)
                    animSet->SetAsFinished(frames);
                else
                    animSet->SetAsFinished();
            }
        }
    }
    Finished = true;
    if (IsToReplaceItemIfRemoved()) {
        if (PItemRemoved.Get() && !PItemRemoved.Get()->IsDead()) {
            if (!PCreature.Get()->GetPCurrentThingManager()->IsNonScriptedEntitesPauseMode() &&
                !PCreature.Get()->GetPCurrentThingManager()->IsAllEntitesPauseMode()) {
                PCreature.Get()->SetCurrentAction(CCreatureAction_UnsheatheItemFromInventory(*PCreature.Get(), *PItemRemoved.Get(), 0, 150));
            }
            else if (PCreature.Get()->IsToUnsheatheItemsAfterCutscenes()) {
                PCreature.Get()->SetPItemToUnsheatheAfterCutscene(PItemRemoved.Get());
            }
        }
    }
    if (PCreature.Get() && !PCreature.Get()->IsDead()) {
        CTCCarrying* carrying;
        if (PCreature.Get()->GetTC(TCI_CARRYING, &carrying)) {
            std::vector<CThing*> items;
            carrying->GetAllCarriedThings(items);
            for (std::vector<CThing*>::iterator it = items.begin(); it != items.end(); ++it) {
                if ((*it)->HasTC(TCI_DESTROY_THING_ON_ACTION_FINISH)) {
                    carrying->RemoveThing(**it);
                    (*it)->Kill(true);
                }
            }
        }
        if (PCreature.Get()->HasTC(TCI_DESTROY_THING_ON_ACTION_FINISH))
            PCreature.Get()->Kill(true);
    }
}

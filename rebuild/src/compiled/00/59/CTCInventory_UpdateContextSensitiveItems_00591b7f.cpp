// CTCInventory::UpdateContextSensitiveItems() -- every 8th frame (or when flagged) drop exhausted preferred items, count the carried items' opinion expressions, rank the context-sensitive candidates and fade each new winner into its button slot
// Leaf: ?UpdateContextSensitiveItems@CTCInventory@@QAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00591B7F, 735 bytes, plain `ret` (size-optimised tc_inventory TU, ebp frame 0x30).
// Flags evidence: the TU is /Gr -- its own STLport instantiations are __fastcall (the landed std::sort row 0x00594ED4 `// Flags: /Gr`,
//   called here with first / last in ecx / edx, 0x00591D35).
// Identity: Ego_r.pdb / Ego_d.pdb / FableWin.pdb ?UpdateContextSensitiveItems@CTCInventory@@QAEXXZ (the shared header's COMPLETE_CLASS
//   declaration; caller = landed GlobalUpdate 0x00591E8C). Locals exactly as Ego_d.pdb (debug length 1966): things_size, things,
//   override_found, context_things; { it { thing_def_index } }; { i }; { i { pcontext_def, pthing_base_def } }; { i };
//   { i { thing_def_index, slot_index, button_index } }; { slot_index }.
// Shape: Ego_d.pdb's C13 line table (ref/00591b7f/egod_lines.tsv, tc_inventory.cpp 929..1056; a line that calls a header inline
//   carries the callee's expanded bytes there -- line 962 GetDef = 497 -- so only call-free lines compare): the two vector
//   constructors 15 / 12 (947, 950) around GetAllItems (948), the zero loop (953 / 955), then `long things_size = things.size();`
//   (958, 11 bytes) -- AFTER the zero loop: declared before it VC7.1 keeps the size in edi (159 instructions off); the two def
//   pointers at 961 / 963 (14 bytes each: the ENullDef constructor) with GetDef between them; `override_found` initialised after the
//   sort (989, 16 bytes = four byte stores, retail 0x00591D42).
// Callees: CTCBase::PeekWorld 0x00686A80, CWorld::GetFrame 0x0049D870 (static), NInventory::CTCInventoryBase::
//   GetNumberOfItemsOfTypeInInventory 0x005BDF08 / GetAllItems 0x005BF482, vector<CContextSensitiveItem>::erase 0x0059310D,
//   CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C, CDefClassBase::
//   GetSubDef<CContextSensitiveItemDef> 0x005933C8, pair<long, CDefPointer<..> >::pair 0x00592962 / ~pair 0x005930F3, vector<pair>::
//   push_back 0x00594E15 / ~vector 0x00594BE8, std::sort<pair*, CContextSensitivePriorityCompare> 0x00594ED4,
//   GetContextSensitivePreferredItemButtonIndex 0x00581BFE (static), GetQuickAccessIndexFromButtonIndex 0x005819F2 (Ego_r donor order
//   between ConstructItemDescription and GetPlayerMode; landed under the descriptive name ValidateIndex: NAME_FIXES),
//   CContextSensitiveItemDef::IsValidOverride 0x00850A80, CFadeInFadeOutBase::Open 0x00434840, free 0x00BFEA14.
//   Data: ContextSensitivePreferredItems 0x013B8B40 (static vector).
// Inline-budget / visibility: header and TU devices, no flag beyond /Gr --
//   (1) the vector members keep their natural header bodies (no *_OUTOFLINE opt-in): VC7.1 itself leaves erase, push_back, the pair
//       constructor / destructor and the candidate vector's destructor out of line (retail's calls), and with the bodies visible it
//       knows the candidate vector's address does not escape -- its _M_finish stays in edi across the sort (0x00591D2A / 0x00591D3D)
//       and the last loop walks a pointer with a count-down ([ebp-8] / [ebp-10h], 0x00591D54). With declaration-only members the loop
//       re-reads _M_start every pass (probes probe/1b7f b1 v b3).
//   (2) FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE (the landed 0x0066BC10 device) and FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY:
//       both lookups are this TU's COMDATs / visible bodies. Without GetDef's body the thing def's release is the address-temp form
//       (`add eax,4 ; dec [eax] ; cmp [eax],ebx`, 741 v 735) and its null store is scheduled before the argument push (0x00591C2F).
//   (3) CContextSensitivePriorityCompare derives from std::binary_function (Ego_d.pdb class record 0x46FD8: one LF_BCLASS): the
//       temporary `CContextSensitivePriorityCompare()` then needs no zero store (retail pushes the uninitialised word, 0x00591D2D;
//       a base-less empty class is value-initialised with `xor eax,eax ; lea edi ; stosb`).
// Overlays (today's headers + NEW opt-ins): engine/CContextSensitiveItemDef.h FABLE_RETAIL_CCONTEXTSENSITIVEITEMDEF_CLASS (the typed
//   sub-def: CSubDefClassBase base, EContextSensitiveType, IsValidOverride, StatGetTypeName); engine/CTCInventory.h
//   FABLE_RETAIL_CTCINVENTORY_CONTEXTSENSITIVEITEM (the nested CContextSensitiveItem record, the EContextSensitiveOpinionExpression enum).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CBASE_ORIGINAL_MEMBERS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_IMPLICIT_GENERATED_SPECIALS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_CONTEXTSENSITIVEITEM
#define FABLE_RETAIL_CCONTEXTSENSITIVEITEMDEF_CLASS
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CRGBColour.h"
#include "engine/CDefPointer.h"
#include "engine/CThingBaseDef.h"
#include "engine/CContextSensitiveItemDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/CTCInventory.h"
#include "engine/CWorld.h"

class CContextSensitivePriorityCompare : public std::binary_function<std::pair<long, CDefPointer<const CContextSensitiveItemDef> >,
                                                                       std::pair<long, CDefPointer<const CContextSensitiveItemDef> >, bool> {
public:
    bool operator()(const std::pair<long, CDefPointer<const CContextSensitiveItemDef> >& a,
                    const std::pair<long, CDefPointer<const CContextSensitiveItemDef> >& b) const;   // 0x005928A8
};

void CTCInventory::UpdateContextSensitiveItems()
{
    if (!ContextSensitiveUpdate && PeekWorld().GetFrame() % 8 != 0)
        return;
    for (std::vector<CContextSensitiveItem>::iterator it = ContextSensitivePreferredItems.begin(); it != ContextSensitivePreferredItems.end(); )
    {
        long thing_def_index = it->ThingDefIndex;
        if (GetNumberOfItemsOfTypeInInventory(thing_def_index) == 0)
            it = ContextSensitivePreferredItems.erase(it);
        else
            ++it;
    }
    std::vector<std::pair<long, long> > things;
    GetAllItems(things);
    std::vector<std::pair<long, CDefPointer<const CContextSensitiveItemDef> > > context_things;
    for (long i = 0; i < CONTEXT_SENSITIVE_OPINION_EXPRESSION_LAST; i++)
        NumberOfCSOpinionExpressions[i] = 0;
    long things_size = things.size();
    for (long i = 0; i < things_size; i++)
    {
        CDefPointer<const CThingBaseDef> pthing_base_def;
        GetDefinitionManager().GetDef(things[i].first, &pthing_base_def);
        CDefPointer<const CContextSensitiveItemDef> pcontext_def;
        if (pthing_base_def->GetSubDef(&pcontext_def))
        {
            context_things.push_back(std::pair<long, CDefPointer<const CContextSensitiveItemDef> >(things[i].first, pcontext_def));
            if (pcontext_def->Type == CONTEXT_SENSITIVE_OPINION_EXPRESSION && pcontext_def->ExtraData1 >= 0 &&
                pcontext_def->ExtraData1 < CONTEXT_SENSITIVE_OPINION_EXPRESSION_LAST)
                NumberOfCSOpinionExpressions[pcontext_def->ExtraData1]++;
        }
    }
    for (long i = 0; i < ContextSensitivePreferredItems.size(); i++)
        context_things.push_back(std::pair<long, CDefPointer<const CContextSensitiveItemDef> >(ContextSensitivePreferredItems[i].ThingDefIndex,
                                                                                         CDefPointer<const CContextSensitiveItemDef>()));
    std::sort(context_things.begin(), context_things.end(), CContextSensitivePriorityCompare());
    bool override_found[4] = { false, false, false, false };
    for (long i = 0; i < context_things.size(); i++)
    {
        long thing_def_index = context_things[i].first;
        long slot_index;
        long button_index = GetContextSensitivePreferredItemButtonIndex(thing_def_index);
        if (button_index != -1)
            slot_index = GetQuickAccessIndexFromButtonIndex(button_index);
        else
            slot_index = context_things[i].second->SlotIndex;
        if (slot_index >= 0)
        {
            if (context_things[i].second.IsNull() || context_things[i].second->IsValidOverride(GetParentThing(), thing_def_index, this))
            {
                if (!ContextSensitiveFadeTimes[slot_index].Opening && ContextSensitiveItems[slot_index] != thing_def_index &&
                    !override_found[slot_index])
                {
                    ContextSensitiveItems[slot_index] = thing_def_index;
                    ContextSensitiveFadeTimes[slot_index].Open(1.0f, 0.0f, CRGBColour(0, 0, 0, 255));
                }
                override_found[slot_index] = true;
            }
        }
    }
    for (long slot_index = 0; slot_index < ContextSensitiveItems.size(); slot_index++)
        if (!override_found[slot_index])
            ContextSensitiveItems[slot_index] = 0;
    ContextSensitiveUpdate = false;
}
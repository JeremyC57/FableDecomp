// CTCInventoryTrade::UpdateBuyActionsList() -- send every buy list's owner component a type-3 action naming the list's key component, then clear the buy-list map
// Leaf: UpdateBuyActionsList@CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059DE04, 114 bytes, plain `ret` (thiscall, no arguments).
// Name: FableWin.pdb `?UpdateBuyActionsList@CTCInventoryTrade@@AAEXXZ` (private); the map is ItemBuyLists (retail +0x200,
// map<NUISystem::CComponent*, NUISystem::CComponent*>, as the landed ctor 0x0059EB0C lays it out); the Sell twin
// 0x0059DE76 walks ItemSellLists (+0x20C). Callers: CTCInventoryTrade::SellItemPC 0x005AF55E, BuyItem 0x005B00B4,
// BuyItemPC 0x005B0F6A.
// Callees: NUISystem::CAction::CAction() @ 0x0042BED4 (landed) and its copy ctor @ 0x0042CD84 (the by-value argument
// built in its slot), list<CActionParam*>::push_back(const value_type&) @ 0x0042B6B8 (the manifest's "Std_List_PushFront":
// it inserts before end()), the value component's virtual slot 62 (+0xF8, a CAction by value; descriptive name as in the
// landed RefreshEntriesAmountsAndAvailabilities 0x0059DB82), the CAction's list destructor @ 0x0042ABCA, the STLport
// tree successor @ 0x004292C0 (`__cdecl`), map::clear() @ 0x0042ADE5 (the landed "CTCCarryable::OnKill").
// Label: the catalog "CTCCarryable_Cleanup" is propagated and wrong.
// Body: `for (it = ItemBuyLists.begin(); it != ItemBuyLists.end(); ++it) { NUISystem::CAction action;
// action.ActionType = 3; action.UIActionParams.push_back(it->first); it->second->SetAction(action); }
// ItemBuyLists.clear();` -- push_back's reference binds to the node's key itself (`lea eax,[esi+10h]`), so in this
// build the parameter list's element type is the map's key type: a list<CActionParam*> (both PDBs, later builds) with
// a CComponent* key converts through a frame temporary (`mov [ebp-4],eax ; lea eax,[ebp-4]`, 120 v 114), with or
// without the PDB's CAction::AddParam(CActionParam*) inline. CComponent is a CActionParam (FableWin.pdb
// GFDebugPDynamicCast<CActionParam*, CComponent*>), so the list is modelled as list<CComponent*>.
// Shape: the map in edi, the node in esi, the action at [ebp-0Ch] (list at [ebp-8]), the target component spilled to
// [ebp-4] across the argument copy and its vtable in ebx. Frame-pointer size TU: `optimize("y", off)` +
// `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
// NF51 (round 7): the UI classes and containers come from the shared headers (were local NUISystem::CActionParam / CAction /
// CComponent models and local STLport list / map / pair / _Rb_tree stand-ins). The 8-byte object is NUISystem::CDeletion
// {Method, AssociatedParents} (engine/NUISystem_CDeletion.h), not CAction: retail CAction() is 0x0042BE50 (Ego_r.pdb / FableWin.pdb
// name ??0CAction@NUISystem@@QAE@XZ there), and CComponent's virtual slot 62 (+0xF8) is SetDeletion(NUISystem::CDeletion) by value
// (engine/NUISystem_CComponent.h). So 0x0042BED4 / 0x0042CD84 are ??0CDeletion@NUISystem@@QAE@XZ / ??0CDeletion@NUISystem@@QAE@ABV01@@Z
// (FABLE_RETAIL_CDELETION, out of line), Method 3 = UI_DELETE_THIS_INSTANCE_PER_PARENT. The list push_back 0x0042B6B8, ~list
// 0x0042ABCA and map clear 0x0042ADE5 are STLport's bodies kept out of line (FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE /
// _LIST_DTOR_OUTOFLINE / _TREE_CLEAR_OUTOFLINE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CDELETION
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"

typedef std::map<NUISystem::CComponent*, NUISystem::CComponent*> CComponentMap;

class CTCInventoryTrade {
public:
    unsigned char _pad000[0x200];                    // CTCInventoryBase (0x130) and the trade state (landed ctor 0x0059EB0C)
    CComponentMap ItemBuyLists;                      // +0x200
    CComponentMap ItemSellLists;                     // +0x20c
    void UpdateBuyActionsList();
};

void CTCInventoryTrade::UpdateBuyActionsList()
{
    for (CComponentMap::iterator it = ItemBuyLists.begin(); it != ItemBuyLists.end(); ++it) {
        NUISystem::CDeletion deletion;
        deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE_PER_PARENT;
        deletion.AssociatedParents.push_back(it->first);
        it->second->SetDeletion(deletion);
    }
    ItemBuyLists.clear();
}
// CTCInventory::UnAssignThingPC(unsigned long, NUISystem::CComponent*) -- empties a quick-access slot: hides the container's assignment icon and puts a fresh empty item component in the container
// Leaf: UnAssignThingPC
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584F15, 206 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 8`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x2), the container's CComponent slot 3 (+0x0C, find
//          child by name, CCharString by value), the {unsigned long, list<CComponent*>} action-parameter pair's ctor
//          @ 0x0042BED4 and copy ctor @ 0x0042CD84 (by-value argument), the icon's slots 62 (+0xF8, takes the pair), 14
//          (+0x38, GetColour by hidden pointer) and 13 (+0x34, SetColour), list<CComponent*> dtor @ 0x0042ABCA,
//          NUISystem::CManager::GetInstance() @ 0x0041E5F2, NUISystem::CManager::CreateComponent(const CCharString&,
//          bool) @ 0x0041DB1D, CCountedPointer<NUISystem::CComponent>(CComponent*) @ 0x00429C15 / dtor @ 0x004291DE,
//          ~CCharString @ 0x0099EAE0, the container's slot 60 (+0xF0, takes the counted pointer; name unproven).
// Shape: `QuickAccessItems[slot] = 0` first (`and [eax+ecx*4],0`); the icon gets the pair {1, empty list} by value
// (built in the argument area through the copy ctor) and its colour's alpha cleared through a CRGBColour local in
// the dead `slot` word; the pair dies after the colour calls. The manager is a named local fetched before the
// implicit "PC_EXPRESSION_ITEM_INSIDE_CONTAINER" conversion temporary (re-addressed with `lea`, in the same dead
// word), which dies once the counted pointer holds the new component.
// Layout: retail CTCInventory QuickAccessItems vector<long> at +0x148 (PDB +0x168).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)

// NF51 (round 7): the string, colour, counted pointer, UI component, deletion and manager come from the shared headers (were local
// CCharString / CRGBColour / CComponentList / SActionParamPair / NUISystem::CComponent / NUISystem::CManager models). The 8-byte
// "pair<unsigned long, list<CComponent*> >" is NUISystem::CDeletion {Method, AssociatedParents} (engine/NUISystem_CDeletion.h): it
// goes to CComponent slot 62 (+0xF8) SetDeletion(NUISystem::CDeletion) by value (engine/NUISystem_CComponent.h), so 0x0042BED4 /
// 0x0042CD84 are ??0CDeletion@NUISystem@@QAE@XZ / ??0CDeletion@NUISystem@@QAE@ABV01@@Z (FABLE_RETAIL_CDELETION) and Method 1 =
// UI_DELETE_THIS_INSTANCE; slot 3 is FindChild(CCharString), slots 13 / 14 SetColour / GetColour, slot 60
// AddChildDynamically(CCountedPointer<CComponent>&) (was a made-up SetContents_slot60); ~list 0x0042ABCA is STLport's body kept
// out of line (FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_LIST
// NF241 (2026-11-22, the list<CComponent*> family pass): the out-of-line list destructor call binds _List_base<CComponent*>'s
//   destructor 0x0042ABCA (was FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE, binding list's own name there; retail 0x0042B6D2 is list's
//   size-optimised `~list() {}`, a 5-byte `jmp 0x0042ABCA`). Same bytes.
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CMANAGER
#include "fable_string.h"
#include "engine/CRGBColour.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CManager.h"

struct CLongVector {                                 // retail std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long& operator[](unsigned long n) { return _M_start[n]; }
};

class CTCInventory { public:
    void*         __vftable;                         // +0x000
    unsigned char _pad_0x004[0x144];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)
    void UnAssignThingPC(unsigned long slot, NUISystem::CComponent* container);
};

void CTCInventory::UnAssignThingPC(unsigned long slot, NUISystem::CComponent* container)
{
    QuickAccessItems[slot] = 0;
    NUISystem::CComponent* icon = container->FindChild(CCharString("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER", -1));
    if (icon) {
        NUISystem::CDeletion deletion;
        deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
        icon->SetDeletion(deletion);
        CRGBColour colour = icon->GetColour();
        colour.A = 0;
        icon->SetColour(colour);
    }
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> item(manager->CreateComponent("PC_EXPRESSION_ITEM_INSIDE_CONTAINER", false));
    container->AddChildDynamically(item);
}
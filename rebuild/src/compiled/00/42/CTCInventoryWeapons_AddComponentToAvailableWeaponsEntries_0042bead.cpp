// CTCInventoryWeapons::AddComponentToAvailableWeaponsEntries(NUISystem::CComponent* entry) -- AvailableWeaponsEntries.push_back(entry)
// Leaf: ?AddComponentToAvailableWeaponsEntries@CTCInventoryWeapons@@QAEXPAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042BEAD, 19 bytes, thiscall, `ret 4`: the ADDRESS of the stack argument is forwarded to
// list<NUISystem::CComponent*>::push_back(const value_type&) 0x0042B6B8 on the member at this+0x198.
// Identity (NF44, round 6): Ego_r.pdb and FableWin.pdb both name
//   ?AddComponentToAvailableWeaponsEntries@CTCInventoryWeapons@@QAEXPAVCComponent@NUISystem@@@Z (ghidra_out/egor_pdb_names.tsv,
//   fablewin_editor_symbols.tsv); engine/CTCInventoryWeapons.h lays AvailableWeaponsEntries (list<CComponent*>) at +0x198 and declares
//   the method; the landed caller NUISystem::CManager::Action 0x0041E6D3 (call at 0x00424C0B) already binds that spelling at 0x0042BEAD.
//   Was the placeholder T::Add(int) forwarding to a local Sub::Add_0042b6b8(int*) (catalog "Forward_Std_List_PushFront_member0x198").
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"

void CTCInventoryWeapons::AddComponentToAvailableWeaponsEntries(NUISystem::CComponent* entry)
{
    AvailableWeaponsEntries.push_back(entry);
}

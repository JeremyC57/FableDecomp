// CTCInventoryQuests::OnItemSelected(long item, bool& handled) -- the quest inventory consumes every item selection: report it handled and do nothing else
// Leaf: ?OnItemSelected@CTCInventoryQuests@@UAEXJAA_N@Z
// Retail: Fable.exe @ 0x00624D90, 10 bytes, thiscall `ret 8`.
// Identity (proven): CTCInventoryQuests vtable 0x012585B4 slot 33 (rtti-vtables.tsv); every inventory class overrides slot
// 33 (CTCInventoryExperience 0x0057A253, CTCInventoryTrade 0x005B3065, CTCInventoryMap 0x00619F30, ... all landed as
// OnItemSelected), and Ego_r.pdb names ?OnItemSelected@CTCInventoryQuests@@UAEXJAA_N@Z (long, bool&) -- the store of 1
// through the second argument is `handled = true`.
// Label correction: the catalog's "CTCInventoryQuests::vfunc_33" is a placeholder.
// Callees: none.
// Layout: no member is touched; CTCInventoryQuests is modelled as a polymorphic partial view (no engine header).
class CTCInventoryQuests {
public:
    virtual void OnItemSelected(long item, bool& handled);
};

void CTCInventoryQuests::OnItemSelected(long item, bool& handled)
{
    handled = true;
}
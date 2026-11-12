## 0x005AD571 CTCInventoryTrade::ConstructTradeDescription -- RELOCATION_MATCH (4 grades)
- What made it match: the outline's body on the shared headers (first grade 4399v4414, slots only); `combatDef` declared then
  assigned from GetPHeroCombatDef() (restores the `inc ; inc ; dec` at 0x005AE35A, 4414v4414); ONE reused `CText* line` local for
  every text line (per-line locals each took a frame slot and shifted the temporary area by 4) -> RELOCATION_MATCH. Strict regrade OK, vt PASS.
- Pragmas: optimize("y",off) + optimize("s",on); no flags. Same-TU GetDef / GetSubDef template bodies under auto_inline(off) (as 0x005C90FC).
- Header opt-ins (hdr/README.md): CTCINVENTORYTRADE_TRADEDESCRIPTION, CTCSHOP_STOCKINFO, CTCINVENTORYITEM_ISCLOTHING,
  NGAMETEXT_CONSTRUCTSTRING_VECTOR, CTCINVENTORYEXPERIENCE_INTERFACETYPE, CWIDESTRING_ASSIGNCHAR (whole-file fable_string.h overlay +
  unchanged fable_definition_table.h copy so CDefString.h reaches it -- put the opt-in into the repo fable_string.h at landing).
- Names: params itemDefIndex/count/buying/special are descriptive (no PDB has them). 0x007726B0 is CTCShop::GetQuantity const
  (Ego_r ?GetQuantity@CTCShop@@QBEJJ@Z 0x0077F780, then GetMaxStock 0x0077F7F0 / GetDaysTillRestock 0x0077F860 at the retail 0x70
  spacing 0x007726B0/0x00772720/0x00772790; FableWin same name) -- the landed GetStockQuantity is wrong (NAME_FIXES.tsv).
- Repair (verifier findings, all applied): NAME_FIXES.tsv now also hands back 0x005BEAD5 GetBoughtPrice, 0x005D99D0 GetInventoryType,
  0x005BC4C4 GetBuildingThing (the vland RELOC_NAME warnings) and 0x0042B312 GetDef<CUIMiscThingsDef> (FableWin mangled name); source
  comment fixed (movzx ax,al at 0x005AE4EA) and caller list adds NUISystem::CManager::Action 0x0041E6D3 (xref.py: 0x00425095/0x004256FF/
  0x00425CFF). Comments only: still RELOCATION_MATCH, strict regrade OK, vt PASS.

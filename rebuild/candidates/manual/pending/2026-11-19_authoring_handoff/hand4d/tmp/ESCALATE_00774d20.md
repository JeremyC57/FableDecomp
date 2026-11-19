# ESCALATE 0x00774D20 CTCShop::BuyItem(long) -- DIFFER(1149v1149), ndiff 34
Source: w24/hand4d/differ/00774d20.cpp (== src/00774d20.cpp). Identity proven (Ego_r 0x00782AB0 / FableWin 0x0261E960; caller 0x005A3436).
## Residue (retail v ours), one register-colouring permutation from 0x00774D7B to the end
- zero constant: retail `xor esi,esi` (0x00774D7B) used for every CDefPointer null init AND `push esi` as GetFreeSaleItemPlaces(false)
  (0x00774E6C); ours `xor edi,edi`, then `push 0` (edi was reassigned to pshop_def at 0x00774E49).
- def: retail caches the parameter in edi at 0x00774E07 (push edi for GetPDefFromGlobalIndex and again for AddSaleItem 0x00774E97);
  ours esi.
- pshop_def.Data: retail ebp (0x00774E49, reloaded 0x00774EAE / 0x00774FD4; ebp = &Stock earlier); ours edi.
- places begin: retail esi (0x00774E7D), ours ebp.
- MinGoldForDonation: retail `fild [ebp+0B0h]` at 0x00775052 then `mov ebx,[ebp+0B0h]` at 0x0077508A; ours loads it into ebx once at
  0x00775052 (spill + fild).
- Same length (1149) and every other instruction equal.
## Levels tried (22 grades)
- Port onto shared headers, Ego_d locals/scopes, retail CDefPointer construction order: 1154v1149.
- Same-TU callee bodies: GetFreeSaleItemPlaces real body (grades RELOCATION_MATCH at 0x00774B40 too) -> 1150 (fixed the places begin
  reload); GetCurrentBasicPrice / GetCurrentBuyPrice / SellItem (matches at 0x00774730) / AddSaleItem bodies: no effect; FindDefaultStockItem
  noinline stub: no effect.
- Accessor text: `ParentThing->GetDefGlobalIndex()` (as the landed twin GetCurrentBasicPrice) instead of GetParentThing(): fixed this ebx /
  &Stock ebp (0x00774D27/32) -> 1149v1149 ndiff 34. GETSUBDEF_BODY v NOINLINE: no effect.
- STL visibility: map find NOINLINE / DECL, operator[] DECL_ONLY (flips this/Stock but makes &def escape -- rejected), no improvement.
- FableWin spellings IS_THING_ALIVE(phero), GFPower: no effect. Include order (CTCShop/CShopItem first / last): no effect.
## Best guess
A numbering tie between the zero-constant CSE and `def`. Candidates not yet tried: the CDefPointer locals' scopes (an inner block for
pdef / pshop_def / def_class_base / stock_item_def is NOT in Ego_d, which lists them at function scope), bool argument spelling, the
GetFreeSaleItemPlaces declaration (bool vs long), and whether retail's GetFreeSaleItemPlaces was visible to BuyItem at all (with it
declared only, the places-begin reload returns but `push edi` appears: probe p4/a).

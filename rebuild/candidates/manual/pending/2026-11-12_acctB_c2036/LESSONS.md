# Chunk 2036 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (c2036, 0x005841F9) A CDefPointer filled through a hidden return pointer (`CTCInventoryItem::GetTattooCardDef(i)`) is
  address-taken: `card->F(); card->G();` reloads `[ebp-0Ch]` after every call. Retail's `mov edi,[ebp-0Ch] ; test edi,edi ;
  mov ecx,edi` x3 is a named reference taken after the null test: `else if (!card.IsNull()) { const CTattooDef& t = *card; ... }`.
  A pointer local (`const T* t = card.Get();`, in the condition or after it) gives the same instructions but one more c2
  candidate in the block (the forwarded load and the copy), which cost `text` its register (priority 28 v 27).
- (c2036) An inline accessor returning a reference (`GetDefinitionManager().PeekHeroStatsDef()`) leaves an inline-return
  candidate in the caller's block (c2prio `local`, prio 24) even when the bytes are identical to a direct member read; that one
  extra K lowered every live-through candidate's priority by 1 and flipped a 28/28 colouring tie. When a row's colouring is one
  point off, compare accessor v direct member read with c2prio before anything else (free).
- (c2036) c2 colouring ties go to the larger key: a parameter (`text`, key 2) loses every tie against a value defined in a block
  (key in the hundreds); it needs a strictly higher priority.
- (c2036) One local re-assigned in three repeated blocks (`value = ...` x3) is ONE frame-packer node (weight 9) even though c2
  colours its three webs separately; it is listed first among the 4-byte nodes and takes the first free slot (here the dead
  `text` home). Three named locals are three weight-3 nodes that reach w3 after the blocks' CCharString label temporaries, which
  then take that slot first -- retail's layout. Repeated statement blocks with their own named results is the Lionhead shape.
- (c2036) bin/bk.py (agent2036): diff two c2prio `--blocks` runs for one candidate, block by block (K and contribution) -- shows
  in seconds which block's K changed.
- Extent check of an unaligned start: the previous function's `ret N` immediately before, the only reference a vtable slot, and the
  next function starting right after `leave ; ret 0Ch` (0x005841F9 .. 0x005849EA).

Review at landing:
- 0x005841F9 uses two new nested overlay opt-ins (hdr/README.md): CTCInventory.h FABLE_RETAIL_CTCINVENTORY_ITEMDESCRIPTION and
  CTattooDef.h FABLE_RETAIL_CTATTOODEF_TYPEBALANCED (declarations only).
- The row reads `GetDefinitionManager().PHeroStatsDef` (descriptive member name of engine/CGameDefinitionManager.h) instead of the
  FableWin accessor PeekHeroStatsDef(): measured, the accessor's return temporary breaks retail's register colouring.
- Catalog label 0x005841F9 CTCShop::BuildItemDescription is wrong (CTCInventory::GetItemDescription); landed 0x0057FC46 grades a
  stand-in class CTattooDef_Methods (NAME_FIXES.tsv); 0x01231728 is bound as `_s_newLine` (a static-array stand-in in
  0x00497A60) but is the pooled literal L"\n".
- 0x005F6E40 not attempted; its extent 1746 is right (the 2113-byte reservation estimate includes unrelated accessor COMDATs).

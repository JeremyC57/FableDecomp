# agentC35 NOTES
## Row 1 0x0058578B CTCInventory::OpenInventory -- working log
- b1 (EVIDENCED): count loop is `if (def->GetSubDef(&augmentation)) continue;` not `if (!...) { ... }`: retail has a separate destructor copy of
  augmentation + def on the true path (0x00586890..0x005868B6, `call [eax+4]` at 0x0058689E, tail shared at 0x00586AB6). Fixes 520 v 521 calls. 8121 bytes.
- b2 (EVIDENCED): `if (has_quests) {reverse loop} else {text}` (arms swapped): gives retail's block layout (join code 0x00585E62 right after the text
  arm, loop out of line at 0x00585ED9 ending `jmp 0x585E62`). 8128 bytes. Tools: tools/calls.py (call-target sequence diff by address), tools/mh.py
  (mnemonic histogram), tools/ls.py (our listing by source line), tools/ga.sh.
- Second layout difference (hair/appearance else-chain placed late in ours) is a consequence of constants in edi (`xor edi,edi` restore block after
  the suit loop), not structure.
- LENS (diagnostic, tools/lens.sh <base> [prio]): wf what-if forcing ROOT `top_level` priority (natural 73) to 200 (window: below experience_entry 245 which
  takes esi first, above clothing_entry 158 whose colouring triggers the split of top_level). Result on b2: first region 0x0058591E..0x00585E0A is
  retail's (top_level edi unsplit, this ebx, gold memory, keys immediates), hunks 410 -> 197, structural hunks start at 0x00585E0A. prio 246/300 = mirrored
  (top_level esi). So retail = top_level coloured as an unsplit root after one esi entry. Natural lever not yet found; the lens is used to find
  remaining REAL source differences (structural hunks in address order).
- b3 `CList* top_level` (push_back through a temp): no (8149).
- b4 (EVIDENCED by 0x00585E0A `mov ebx,[ebp+5Ch]` before the AddChild call): text arm names the CText* BEFORE `description->AddChild(text)`.
- b5: `hero_weapons->GetAllItems(items)` without the C cast (+ #include CTCInventoryWeapons.h): byte-neutral, cleaner. Natural grade 8125.
- LENS on b5 (tools/lin2.py linear compare): retail == ours instruction-for-instruction from 0x0058578B to 0x0058680C (hero PeekTC weapons) except frame
  offsets; after that the count loop differs by allocation only (retail: def.ptr in ebx, i*8 in esi, zero NOT in a register; ours: const 0 in ebx).
- WINDOW (measured by what-if): top_level root prio must be in (158, 245): 157 and 100 = split (natural result), 200 = retail, 246+ = mirrored.
  Natural: top_level 74 = +852 (53 refs) - 778 (290 live-through blocks of 357). clothing_entry / weapons_entry 158, experience_entry 245.
  tools/kh.py: in top_level's live-through blocks K is made by `const 0` (123 blocks; 108 of its block refs have cost 0), locals, temps.
  OPEN: the natural source fact that gives top_level +85 (or the entries -85). Regime 1 (w = 1, globals are candidates).
- b6 (EVIDENCED, 4 sites 0x00586AE4 / 6CB1 / 6E82 / 704F: retail loads the viewport vtable AFTER the FindChild call): named `CComponent* x_desc =
  entry->FindChild(..)` before `viewport->AddFakeChild(x_desc)` in the weapons / items / clothing / style sections. Natural 8140, hunks 347; top_level
  prio 90, entries 163. LENS b6: equal to retail 0x0058578B..0x0058680C, 0x00586AD4..0x00586BE9 and 0x00586CB1..0x00587661 (826 instructions);
  remaining under the lens: count loop 0x0058680C..0x00586ACE, layout hunk 0x00586BE9, tail 0x0058765E (`this` reload register).
- COUNT LOOP DSE FACT (EVIDENCED): retail's destructors of augmentation [ebp+74h], suit [ebp+3Ch], appearance [ebp+0Ch] have NO `Object = 0` store
  (0x00586A6F..0x00586AA4, also the continue path 0x00586890); the three hair objects (returned by value) keep it (0x00586A31 / 6A50 / 6A6B).
  Measured rule (probes e1 / e2): c2 drops the null store of a default-constructed CDefPointer only when the object is the LAST one destroyed in its
  scope. So retail has def > augmentation > suit > appearance in strictly nested scopes, with suit and appearance constructed back to back before
  IsWeapon (0x005868C1). e2 = bare nested braces (FITTED spelling of that fact; natural spelling not found). e2 natural 8131, lens 8120.
- Anniversary PDB locals of CTCInventory::OpenInventory (weak third source, PowerPC): ptc_stats, things (the pair vector), pTCHeroStats, psuit_def,
  pappearance_modifier_def, paugmentation_def, pthing_base_def, suit_part_it, ptattoo_def, phair_card_def, phair_appearance_modifier_def, db
  (CDefPointer<const CDefClassBase>), weapon_title_value, title_value (CWideString). tmp/anniv_openinv.txt.
- SPLIT RULE (measured, corrects the "window (158,245)" line above, which held only for b2..b5): the split of top_level is triggered by the first
  candidate that takes EDI while interfering with it (neighbours left with ebx only are split), not by an entry taking esi. On b6 / e2 the order is
  experience_entry 245, clothing / weapons 163, items 145, style_entry 121 (all esi), i*8 temp 113, temp 112, then `style_count` 109 -> edi -> split
  pass [.. top_level ..]. Retail keeps style_count in MEMORY (0x00587082 `mov [ebp+0],eax`, tested through eax) and top_level in edi, so retail's
  order has top_level before style_count: NEED top_level (89 on e2) > style_count (109), i.e. +21 for top_level or -21 for style_count.
  Probe f1 (wrong code: releases without Delete, 25 fewer blocks) had top_level 135 > 121 and came out unsplit in edi naturally (hunks 202).
- Helper texts tried for that +21 (tools/fwsize output in tmp/fwsize_e2.txt):
  f3 / f4 CDefPointer::Release through the self-deleting pointee Release (FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE +
    FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT / _RELEASE_DELETES; FableWin Release 14i/33b v ours 27i/65b): byte-identical to e2, priorities
    identical (named locals inside an expanded inline are copy-forwarded and do not change K).
  f5 / f6 GetTC / PeekTC as HasTC + forced-inline Components.Find (FableWin 27i/70b v the EXPANDED text 43i/120b; tried as an overlay opt-in, REMOVED
    again): 8116 bytes, top_level 99, but under the lens the nine GetTC sites then DIFFER (`push 4 ; pop eax ; push eax`): the EXPANDED text is the
    right one for retail's bytes. Overlay hdr/ is unchanged from C33's.
  h1 named FindChild results in the five back actions (address-taken by push_back(const T&)): no priority change (not candidates).
  b6's four x_desc locals (register candidates) moved top_level +16 and the entries +5: only REGISTER-candidate locals move K.
- Remaining under the lens (what a successor must still close once top_level is natural): count loop 0x0058680C..0x00586ACE: retail has const 0 in
  esi only for the seven count / i stores (0x00586824), i*8 in esi, def.Object in ebx from 0x0058687A to the suit loop (`part` iterator ebx
  0x005868F6), `xor ebx,ebx` only in the hair branch (0x0058699C), i in memory; ours gives ebx to const 0 for the whole loop (def root prio -106).
  Then the layout hunk 0x00586BE9 (follows from it) and the tail 0x0058765E (`this` reload: ours loads [this] into a second register).
- NOT TRIED: a natural spelling for the nested scopes (inline helper chain? the Anniversary frame shares pthing_base_def / psuit_def / suit_part_it
  at one offset, i.e. disjoint lifetimes there); lowering style_count (retail computes hair [ebp+1Ch] + tattoo [ebp+40h] + facial [ebp+10h] with both
  operands loaded, 0x00587073); Anniversary local names in the source (things, pthing_base_def, ...); row 2.

## Row 2: 0x006096E0 CTCInventoryMap::ConstructLogbookListPC -- NOT WORKED by C35 (context spent on row 1). State = C33's notes (prev_C33/NOTES.md,
"Row 2" section): draft SRC_FAIL on today's headers (seven missing opt-ins), extent confirmed 12,832. differ/006096e0.cpp = the draft unchanged.

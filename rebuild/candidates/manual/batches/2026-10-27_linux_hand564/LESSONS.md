# Chunk 564 lessons (GOTCHAS one-liners)

- A "~Class" catalog row whose first vptr store names another class is that other class's dtor: 4 of 12 rows here (0x00798620 is CTCForcePushed, not CTCTurncoat; 0x007A73F0 CTCBattleCharge, not CTCActionUseBed; 0x00750850 CTCChest, not CModeOutOfActionBase; 0x0072C430 IsAbilityAvailable, not "GetAbilityAvailability").
- CThing::PeekTC/GetTC retail bit test is the NON-const bitset reference (`test ch,ch` / `test ch,1`); spell it `const_cast<std::bitset<274>&>(UsingComponents)[type]` inside a const PeekTC. A plain `!HasTC(type)` (const operator[] -> _Unchecked_test) gives the `shr/not/and 1` arms.
- The bitset reference form in a `return !...` arm leaves a dead `add reg,30h` (the _M_wp pointer) after the folded load; the const form does not.
- PeekTC's map lookup matches retail only as ONE ternary store: `*out = (it == end || key < it->first ? end : it)->second;` (an if-then-reassign leaves `mov ecx,eax`).
- A `&&` chain that guards a whole switch keeps one shared epilogue; early `return false` per test splits it and moves `push edi`.
- A virtual call lands on the right slot only when the whole base vtable is declared in order: CTCBase has 28 retail slots (editor slots absent), CTCTargetingBase GetPTargetedThing is slot 33 (+0x84).
- CTCSpecialAbilities::FadeOrKillEffect is a STATIC __fastcall member (`SIX`), not a free function: the reference overload takes CIntelligentPointer<CThing>&.
- C3DVector and CIntelligentPointer are 1-aligned in retail objects (pack(1)): PTarget lands at +0x6D in CTCDivineWrath, POpenParticle at +0x31 in CTCChest.
- Relocation masking hides the case->arm mapping of a switch: check each jump-table entry by hand (objdump -r on the object plus the `$L` offsets) before calling a switch matched.
- exp.py-style ndiff over a function with a jump table counts ~30 junk entries (the table bytes disassembled as code); compare the arms, not the total.

## Review at landing
- 0x0072C430 IsAbilityAvailable: DIFFER(584v584), register only (case arms load ParentThing into ecx/edx/eax instead of esi). Test: differ/0072c430.test.cpp (g++).
- 0x0077FDE0: re-split the catalog row at 0x0077FEB0 (the 2-argument Transfer<EHeroCentreDoorType>, 32 bytes), then land differ/0077fde0.cpp (208 bytes, byte-exact with relocations masked).
- vland RELOC_NAME warnings on 0x0072ACF0 callees (0x0079DCB0, 0x007CC9B0, 0x007CBEE0) are propagated catalog labels; the PDB spellings used here are in NAME_FIXES.tsv / NOTES.md.

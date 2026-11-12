# OUTLINE -- 0x0045A159 CInventoryDef::operator=(const CInventoryDef&)  (6481 bytes, ends 0x0045BAAA)

## Frame facts (python3 $S/w17/xdis.py 0045a159 0x1961; $W/dis_all.txt)
- __thiscall, `ret 4` (0x0045BAA7), returns `this` in eax (`mov eax,ebx` 0x0045BAA4).
- NO frame pointer: ebp is a general register holding `other`. => no optimize("y",off).
- No SEH, no /GS cookie, no locals, no stack frame (esp only moves for pushes).
- Prologue (exact order): 0x0045A159 push ebx ; push ebp ; mov ebp,[esp+0Ch] (other) ; push esi ; push edi ;
  push ebp (arg of base op=) ; mov ebx,ecx (this) ; call 0x00431ED0.
- Register map: ebx = this, ebp = &other, eax = the only copy scratch (dwords via eax, the bool via al),
  esi/edi = movs source/dest (lea each time), ecx = this-member for the vector op= calls (push eax=&other.member).
- Epilogue (interleaved with the last member): mov eax,[ebp+A30h] ; pop edi ; pop esi ; mov [ebx+A30h],eax ; pop ebp ;
  mov eax,ebx ; pop ebx ; ret 4.
- Single straight-line block: no branches, no jump tables, no cold blocks.
- Only scheduling deviation from source order: 0x0045A2A7..0x0045A2B9 the ClothingDoll (+0xD4) load comes first, then
  lea esi/edi for DollCentreOffset (+0xD8), then the ClothingDoll store -- compiler scheduling, write in member order.
- Copy idioms (=> #pragma optimize("s", on), as siblings 0x0045BD42 / 0x0045BE2B):
  * 4-byte member (float/long/ulong/CDefString/CRGBColour/enum): `mov eax,[ebp+o] ; mov [ebx+o],eax`
  * C2DVector (8 bytes, packed): two such dword pairs (X then Y)
  * bool: `mov al,[ebp+58h] ; mov [ebx+58h],al`
  * CEngineGraphic (14 bytes, pack 1): `lea esi,[ebp+o] ; lea edi,[ebx+o] ; movsd ; movsd ; movsd ; movsw`
  * C3DVector (12 bytes): `lea esi ; lea edi ; movsd x3`
  * STLport vector: `lea eax,[ebp+o] ; push eax ; lea ecx,[ebx+o] ; call <vector op=>`

## Source shape
```cpp
CInventoryDef& CInventoryDef::operator=(const CInventoryDef& other)
{
    CParentDefClassBase::operator=(other);        // 0x0045A164 call 0x00431ED0 (??4CParentDefClassBase@@QAEAAV0@ABV0@@Z)
    <391 member assignments, PDB declaration order -- table below>
    return *this;
}
```
(or: leave operator= implicit and let `Copy` (`*this = *static_cast<const CInventoryDef*>(other)`) make the compiler emit
the COMDAT, the CInventoryCategoryDef 0x0045BD42 idiom -- see DOSSIER "Approach".)

## Block list (retail order == Ego_r.pdb member order; address = the first store/lea of that member)
Full table, one line per member: $W/members_body.txt (statement + retail address + offset + idiom).
Declarations for the class (type, name, retail offset): $W/members_decl.txt. Machine map: $W/member_map.tsv.
Key anchor points:
- 0x0045A169 +0x3C InitialCategoryMovementRepeatDelay .. 0x0045A193 +0x58 TrackSelectedItems (bool)
- 0x0045A199..0x0045A1CA four std::vector<long> (+0x5C,+0x68,+0x74,+0x80) -> call 0x00454886
- 0x0045A1CF.. C2DVectors +0x8C..+0xC4, GenericFont CDefString +0xCC, BackGroundColour +0xD0, ClothingDoll +0xD4,
  DollCentreOffset C3DVector +0xD8 (movs), DollBoundingSphereRadius +0xE4 ...
- 0x0045A412 +0x154 StatLevelsTexts std::vector<unsigned long> -> call 0x0045BC09
- 0x0045A424 +0x160 StatLevelsColours std::vector<CRGBColour> -> call 0x0045BAAA
- 0x0045A5C8 +0x1F0 InventoryIdentifier (EInventoryIdentifier, dword)
- 0x0045AF29 +0x55C ExperienceItemLabelStat, +0x568 ExperienceItemLabelAbility (vector<long>, call 0x00454886)
- 0x0045B5B8 +0x7D4 ExpSpendItemLabelStat, +0x7E0 ExpSpendItemLabelAbility (vector<long>, call 0x00454886)
- 0x0045B8A2.. CEngineGraphic runs (ExpSpendGreen +0x92C ...), unaligned C2DVectors after them (e.g. +0x9FE
  MainDescriptionBoxTLPos), last member 0x0045BA95 +0xA30 MainQuickAccessItemFlashTime (float). sizeof = 0xA34.
Calls total: 1 base + 8 x 0x00454886 + 1 x 0x0045BC09 + 1 x 0x0045BAAA. 474 dword pairs, 38 movs groups (36 CEngineGraphic,
2 C3DVector), 1 byte copy -- verified: the expected stream from the PDB layout equals the retail stream (523 events) up to the
one scheduling swap.

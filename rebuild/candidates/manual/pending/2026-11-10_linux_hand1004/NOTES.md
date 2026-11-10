# Chunk 1004 notes (CTCInventoryWeapons / CTCInventoryClothing leftovers)

Grade environment: `FABLE_HDR_OVERLAY=$A/hdr` (see hdr/README.md; regenerate with `python3 $A/apply_hdr.py`). Final gall.sh: 47 MATCH +
2 RELOCATION_MATCH; tall.sh (g++) and tall.py (VC7.1) 49/49 PASS; relchk.py on both RELOCATION_MATCH rows shows only the
anonymous-namespace hash of the TU's file-scope strings and the compiler's `__real@00000000` (0x0122DEDC = 0.0f) as "CHECK".

## Rows
- 0x005C90FC ConstructWeaponDescriptionContainer -- RELOCATION_MATCH. Built on the landed 0x005C6B26 description body. Levers: each
  text line created with `if (IsLiveGUIMode()) X.Reset(Create("PC_...")); else X.Reset(Create("UI_..."));` (VC7.1 cross-jumps the two
  arms into retail's `je ; push PC ; jmp ; push UI ; call`; a ternary inside the call does not); the text pointer named before
  AddChild; `text = CWideString::NullString` after each line; heavy-weapon colouring walks HeavyWeapons.StatThresholds (new
  CHeroAbilityStatPackage header) and scales the weight line's colour B/G by 0.0f.
- 0x005CACB9 RefreshWeaponDescriptions -- RELOCATION_MATCH. Levers: viewport cast written as a nested `if` (a ternary adds a null arm),
  named `description` local in the loop, visible STLport map operator[] / erase bodies (FABLE_RETAIL_STLP_MAP, _TREE_ERASE*), CDeletion
  bodies (Method store before the copy), same-TU GetSubDef/GetDef bodies under auto_inline(off).
- 0x005CA68E SetupActiveWeaponsOnCarrier -- DIFFER(1538v1535), 3 grades. Fully decoded (hero/IsDying gates, multi-strike / multi-arrow
  GetTC, the UnsheatheItemFromInventory action on a 0xB8 local with SetCurrentAction / PerformAction fallback, passive-slot handling with
  IS_THING_DEAD 0x005CBDBD, the carried-things loop with operator!= 0x005CBDD2 and IsProjectile, the final ApplyEffectOnWeapon pair).
  Every instruction matches except register choice: c2prio.py shows `this` (priority -72) coloured before the TCI_WEAPON constant 0x48
  (-76); retail has the constant in ebx across both in-hand arms. Spellings tried under c2prio.py (free) without moving the priorities:
  one shared `const CTCWeapon*` for all three lookups, declaring it at the top, the FableWin inline accessors
  GetActiveMeleeWeapon / GetActiveRangedWeapon (transparent once inlined; the opt-in was withdrawn again). 7 grades of budget remain.
- 0x005C71AA ConstructWeaponDescriptionNoDamageNorWeight -- parked, DIFFER(680v680) 4 lines (scheduling of the NONE block); over the
  10-grade budget (17 probes + 1 re-verify this session, 8 in chunk 1002b). c2prio.py: the nested-else spelling that fixes the NONE block
  needs const 0 to out-rank i#26 for the copy loop's count to split into ecx; only a diagnostic extra use of 0 does that.
- 0x005B3E4E CTCInventoryClothing::GlobalUpdate -- not re-attempted (0 grades); differ/ is the prior local model (538v543). Blockers:
  no shared CTCAnimationComplex / CAppearanceDef / CAnimationSet / CAnimFilterBase / CAnimateThingUpdate headers, and the shared
  CAnimateThing / CAnimSet keep CurrentAnim / Anims private, so the row's `[animThing+4] -> movsx word` main-anim read and its
  `[+1Ch]/[+20h]` frame-info copy need inline accessors whose FableWin names were not found in the time left (FableWin lists only the
  Dinkumware list helpers for CAnimateThing::CDelayedAnimation and CAnimSet::CIsType / CHasTypeFlags). Retail frame decoded: filters +
  strings at [ebp-4..-10h], defaultAnim -14h, this-spill / GetTC key / returned list -18h, twirlAnim / complex / frame-hi -1Ch, frame-lo
  -20h, CAnimateThingUpdate -2Ch, CAnimInit -58h (0x58 total).
- 0x005B9641 DrawPreparePrimitivesForRendering -- split, main body not built. SPLITS.tsv: main 0x5AF (`add ebp,74h ; leave ; ret 4`
  at 0x005B9BE9), then 47 contiguous leaf helpers 0x005B9BF0..0x005B9D8C (412 bytes, the row's tail). split_oracle.py re-cut all 48
  oracle lines into chunk1004_oracle.tsv (its `N` was '1001'; corrected to '1004' in my copy).

## The 47 helpers (CUnproven placeholders)
Search for their owners: a whole-image scan of Fable.exe for rel32 call/jmp targets and absolute dwords equal to any of the 47 starts
finds nothing (dead COMDATs kept by the linker); the Ego_r.pdb run after DrawPreparePrimitivesForRendering is a Dinkumware rebuild
(`_Tree`, `_Tmap_traits`, `$$Q` rvalue refs), so it names different instantiations. Two byte-identical `less` bodies (0x005B9D05,
0x005B9D1E) must be two instantiations, so even the obvious shapes cannot be pinned to one template. Each is authored as a descriptive
`CUnproven<Kind>_<addr>` class whose shape is the bytes (iterator ctor / ==, empty ctor / dtor, first / second word, copy ctor,
operator[] on 4 / 12 / 24-byte elements, size() over 12 / 20 / 24-byte elements, tree node +0x10 / list node +8 value, pair ctors,
list ++, signed less). All 47 MATCH (one probe: `==` needed `other.Node == Node` for retail's operand order). Generator:
$A/wip/hlp/gen.py.

## Name fixes found (NAME_FIXES.tsv)
NullString 0x013BCA24, WEAPON_DESCRIPTION_STRING 0x013B8BAC, the STLport map<unsigned long, CUIState>::operator[] 0x0042D5B1,
GetTrainableStatLevel 0x006D6520, the UnsheatheItemFromInventory ctor 0x006A0150 (references, not pointers) / dtor 0x005CC091 /
PerformAction 0x0069F9E0 (vtable 0x0125CCBC slot 3), IsProjectile 0x005DAA70 (bound as IsMeleeWeapon), GetThingInCarrySlot 0x006A11F0.

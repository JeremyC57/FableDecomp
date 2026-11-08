# Hand chunk 880 (CTCInventory .. CTCInventory): 1 of 1 byte-exact

## What matched (1)
- CTCInventory (1): 0x0058AF10 `OpenPCOptionsMenu` (13,048 bytes = 12,980 bytes of code + the 17-entry anti-aliasing switch
  table 0x0058E1C4..0x0058E207), RELOCATION_MATCH under q.py and vland (behaviour PASS under g++ and VC). Closes the open
  menus, marks the options menu open (CurrentOpenMenu = 4), reuses PC_OPTIONS_LIST if it is still a menu component, else builds
  it: frame pictures / descriptions / subtitles (descriptions also become fake children of the description viewport), the
  back / close clickables observed for mouse movement, the in-game menu's PC save / load entries, the accept / back /
  restore-defaults actions of the gameplay, audio and video pages (plus an OBSERVE action on the accept button that is built and
  never attached, as in retail), the redefine-keys page (restore button hidden, edit boxes set on select and deactivated on
  accept / back, the redefiner list's accept button), the left / right arrow actions of every slider found (paired by position
  with a fixed 10 gameplay / 3 audio / 10 video page list), the resolution entries "<w>X<h>X<bpp>" and anti-aliasing entries
  "<n>X" / TEXT_GUI_MENU_OFF; then (always) observes the scroll buttons, greys out the arrows and label of a detail slider with
  no range (Min.X == Max.X) or a text slider with no entries (MaxChild == 0), appends " " + TEXT_GUI_MENU_REFRESH_RATE_HZ to
  every refresh rate and sets state 5.
  Shape lessons that made it match (17 grades):
  1. `list` must not be address-exposed: with the plain inline STLport `vector::push_back` body (which VC7.1 declines here, so
     0x00486024 is still called) the `const T&` temporary shares list's home [ebp+10h] and c2 keeps list in edi / ebx; the
     out-of-line declaration opt-in (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE) exposed list (memory only, frame 0xE8/0xF0).
  2. The switch table starts at 0 only with a separate `case SURFACE_MULTISAMPLE_NONE:` body (same text as default).
  3. `if (!p.IsNull()) { CText* t = static_cast<CText*>(p.Get()); ... }` (store after the branch) in both text-slider loops.
  4. Named locals for the CRGBColour argument, the CRedefinerList and each quit picture (`CComponent* picture = *it;`).
  5. The widget loop: slider iterator declared before the `for`, page iterator in it, `++entry, ++page`, nested null tests of
     named page / whole / slider locals (lazy loads, and the non-null slider needs no `neg/sbb` in AddParam).
  6. The GetName() == "PC_OPTIONS_LIST" compare expands CCharString::operator==(const char*) in place: the existing shared-header
     opt-in FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (plain C++ 13019v13048; /d2inlS10..15 over-inline everything else,
     /d2inlT200 changes nothing), recorded on line 2 as `// Inline-budget:`.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x00556570 is ?SetAcceptButton@CRedefinerList@NUISystem@@QAEXPAVCComponent@2@@Z (catalog `_global::SetMember_dword0x18c_00556570`).
- 0x00406880 is ?GetDisplayModes@CUserProfileManager@@QBE... (catalog `NCreatureMode::CModeBlock::GetAnimFilter`, an ICF-shared
  `lea eax,[ecx+80h] ; ret`: check label_trust before renaming the shared body).
- 0x00592495 is pair<CCharString, CCharString>::pair<const char*, const char*> (landed as pair<const CCharString, CCharString>; NAME_FIXES.tsv).
- 0x0059326D / 0x00593ADE / 0x00593B03 are list<pair<CCharString,CCharString> > ctor / insert / dtor, 0x00593288 / 0x00593B1A /
  0x005932A3 list<pair<CComponent*,CComponent*> > ctor / insert / dtor (catalog StdList_* / ListNode_Create; LABELS.tsv).
- 0x004804DE ?OpenInGameMenuPC@CTCInGameMenu@@QAEXPAVCComponent@NUISystem@@0@Z is not in the catalog yet.

vland RELOC_NAME warnings (all label-only, bytes proven): 0x0042ABAF / 0x0042ABCA (the header calls the `_List_base` ctor / dtor,
the catalog says list::list / ~list, as every list<CComponent*> row), 0x00556570, 0x0059326D, 0x00592495, 0x00593288, 0x00593B1A,
0x00406880, 0x005932A3, 0x00593B03.

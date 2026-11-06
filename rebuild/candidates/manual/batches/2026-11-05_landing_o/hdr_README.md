# chunk 788 header overlay

Grade with `export FABLE_HDR_OVERLAY=$S/w22/agent788/hdr`. All six rows grade RELOCATION_MATCH with this overlay (gall.sh, vland).
Overlay use confirmed: rows 0x00E693F0 / 0x00EDB190 call `CCharString::Contains`, which exists only in the overlay copy of
fable_string.h (the repo copy has no such member), so they cannot compile against the repo header.

## New headers (no landed includer; default path is the only path)
- `engine/CQ_HobbeCaveScript.h` -- NScript::CQ_HobbeCaveScript : CLevelScriptBase<CQ_SunnyvaleMasterData>, sizeof 0x78. Ego_r.pdb
  layout - 0x14 (bools +0x48..+0x50, CTimer BanditComment +0x54, ... WaitForJamesReleased +0x74); slots 1-3 RegisterMain 0x00D8DC00 /
  Main 0x00D8DD10 / Init 0x00D8D640 of vtable 0x012D5540 (stored by the "Q_HobbeCave" Alloc 0x00D99DC0); the four thread methods and
  the fifteen nested entity classes (forward-declared). Checked against Main's reads (+0x49, +0x69, +0x6a) and the landed
  WatchForTermination 0x00D8E990 (+0x4b, +0x4c, +0x70).
- `engine/CQ_HeroSoulsGuildmasterScript.h` -- NScript::CQ_HeroSoulsGuildmasterScript, sizeof 0x64: PlayedBriarCS +0x48 ..
  GiveWarningCutscene +0x4c, GuardInWoodsCount +0x50, DistanceReached +0x54, `std::map<CCharString,bool> cs_flags` +0x58
  (stlport/fable_map.h; 0xC bytes); PlayCutscene(CCharString) 0x00D872D0; vtable 0x012D4804 (ctor 0x00D84B50, Alloc 0x00D87A80);
  seven nested entity classes forward-declared.
- `engine/CV_ChapelOfEvilScript.h` -- NScript::CV_ChapelOfEvilScript, sizeof 0x90 (landed Alloc 0x00E6E230 news 0x90; ctor 0x00E68F70
  builds TortureSound +0x88). Ego_r.pdb layout - 0x14 (DonationTotal +0x48 .. TortureType +0x8c, `enum ChapelOfEvilTortureType {}`,
  enumerators not recovered); full Ego_r method set with retail addresses where known; vtable 0x012E8080; three nested entity classes.
  Checked against Main 0x00E693F0 (+0x6e, +0x59, +0x58, +0x5a, +0x56, +0x60).
- `engine/CV_TempleOfLightScript.h` -- NScript::CV_TempleOfLightScript, sizeof 0x74 (landed Alloc 0x00EDEB40 news 0x74). Ego_r.pdb
  layout - 0x14 (DonationTotal +0x48 .. QuestActivated +0x72); method set (WatchForLeaving from FableWin.pdb); vtable 0x012F3C68; two
  nested entity classes. Checked against Main 0x00EDB190 (+0x72, +0x5a, +0x59, +0x5b, +0x57, +0x64).

All four new headers carry the `#ifndef FABLE_RETAIL_NSCRIPT ... #error` guard of the chunk 787 headers (never reached: every includer
defines FABLE_RETAIL_NSCRIPT first).

## Opt-in edits to existing headers (default path byte-identical to the repo copy)
- `engine/CQ_AmbushTradersScript.h` -- copy of the repo header plus, at the end of the class, `#ifdef FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN`:
  the eleven thread methods Main 0x00F024C0 spawns (Ego_r.pdb ?<Name>@CQ_AmbushTradersScript@NScript@@QAEXXZ, addresses from Main's
  member pointers) and the ten nested CAT_* classes (forward declarations). No layout change. Landed includers (hdr/INCLUDERS_CQ_AmbushTradersScript.txt):
  0x00CF9610 OnPersist, 0x00CF9A50 Init -- both RELOCATION_MATCH + PASS under the overlay (tmp/recheck.log).
- `fable_string.h` -- copy of the repo header plus `#ifdef FABLE_RETAIL_CCHARSTRING_CONTAINS`: `__forceinline bool
  CCharString::Contains(const CCharString&) const { return strstr(GetAsCharArray(), other.GetAsCharArray()) != 0; }`. Evidence:
  FableWin.pdb ?Contains@CCharString@@QBE_NABV1@@Z (0x018D7370); retail expands it in place in both village Mains (0x00E695B8..0x00E695E8,
  0x00EDB2EC..0x00EDB31C: the pattern's text from one null test with `mov ecx,offset NullChar`, then this string's, strstr 0x00BFEB3A,
  `setne`). __forceinline because the body is expanded in a large function, like the StartsWith / TEXT_EQ opt-ins next to it.
- `fable_definition_table.h` -- unchanged copy. Needed only so that includers reaching fable_string.h through engine/CDefString.h ->
  "fable_definition_table.h" pick up the overlay fable_string.h: fable_string.h is `#pragma once`, so a second physical copy (the repo
  one, found first from fable_definition_table.h's own directory) redefines every class (C2011). Drop it at landing once the
  fable_string.h opt-in is in the repo.

Sample landed includers of fable_string.h / fable_definition_table.h also rechecked under the final overlay (tmp/recheck.log, all
RELOCATION_MATCH + PASS): 0x00E92A50, 0x00E8A2B0, 0x009D4A40, 0x00EA0090, 0x00E6B750, 0x00E6E050.

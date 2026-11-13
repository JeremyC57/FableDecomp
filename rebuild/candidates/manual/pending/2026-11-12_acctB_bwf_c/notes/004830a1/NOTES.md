## 0x004830A1 CTCInGameMenu::OpenInGameMenu -- session 1 (2026-11-12)
- Grades 1-25: first full draft compiles (overlay hdr/: CTCInGameMenu OPENMENU, CUserProfileManager SAVESLOTS, CPlayerManager JOYSTICKDEVICE). All 374 direct calls match (cd: FORCEINLINE_EQ + TEXT_EQ). Frame 0x230 matches; biased frame needs `optimize("s", on)` ALONE (y-off gives mov ebp,esp).
- Best v16 (7919v7896, 48 normalized hunks, all register-allocation): loadMenuEntry as inline cast helped (71->48). Named sprite local in P4 / volatile probes / long i / text.Get() / shared CMenuEntry* local did not help.
- Grades 26-60 (session 1 end): best stays v16 = differ/004830a1.cpp, DIFFER(7919v7896), 48 normalized hunks (tools/nd.py: ebp slots,
  relocations and call targets masked; tools/g.sh grades + diffs). Every remaining hunk is register allocation (no missing/extra code,
  all 374 direct calls match by count, frame 0x230 matches, block layout matches).
  Residue map (retail v ours): retail edi = saveList only 0x0048385F..0x004838C6, then savePicture.Data (if-block), then loadList
  0x0048393E..0x00483A75, then `this` from 0x00483A7B through the loop and post-loop (reloaded from [ebp+48h] after branches);
  ours keeps saveList in edi through InitialiseOffsets, so `this` only wins edi inside the loop body, and the loop-head text vtable,
  saveEntry, the menuEntry.Data CSE and the load-confirm title/list locals take edi where retail keeps this / manager.
  P5: retail computes FileNames.size() once after IsDebugProfile and does `jne; dec edx`; no spelling found
  (ternary = duplicated arms; `size-(dbg?0:1)` / `+(dbg?0:-1)` = sbb; named bool+`if(!dbg)--n` = `cmp al,bl` and global flip).
- Tried without gain: named CSprite local for savePicture (any form flips the WHOLE allocation: edi=0 constant becomes ebx from the
  prologue), IsNull/operator* spellings, `long i`, text.Get()/(*text), one shared CMenuEntry* local, inlining saveEntry (worse),
  inlining load-confirm locals (47/46/45 hunks but different residue, not better in substance), volatile probes (worse), unused
  `valid` as expression, reusing the save-entry FindChild local, const manager/menu/area (no change), /GS (no change), /Gr (51).
- FableWin COMDATs after OpenInGameMenu (0x01AA64D9): CActionParamState/SLONG/EBOOL ctors, CAction(), CInputManager::GetJoystick,
  CTCFishing::IsActive, vector<CCountedPointer<CJoystick>>::operator[], CCountedPointer<CJoystick>::operator*, CDeletion ctors -- all
  used by the draft. fwsize: CCountedPointer<CComponent>::operator* is NOT in FableWin, so `&*loadEntry` (no-null-check base
  conversion at 0x004849CF `lea edx,[ecx+18h]`) is probably the wrong spelling (try a reference / `*loadMenuEntry` form).
- Best next idea: attack the P4 edi split (retail kills the saveList web after AddNonScrollingChild(savePicture)): look for a source
  form where the later saveList uses are separate (e.g. the save/load list setup in a block or helper scope, or the back/select/title
  adds reading through saveSubList/loadSubList-derived CList* locals), then re-check the loop -- `this` should win edi once P4/P5 match.
- Headers: hdr/ overlay (README.md); defines in the source (no Flags line needed beyond /GS-neutral). Names: see NAME_FIXES.tsv.
- v48 (= v16 bytes) is the differ source: self-param through a named CMenuEntry* local instead of &*loadEntry (FableWin has no CCountedPointer<CComponent>::operator*).

## 0x004830A1 CTCInGameMenu::OpenInGameMenu -- session 2 (2026-11-12): RELOCATION_MATCH (grades 61-76)
- What made it match (from v48/v16, 48 normalized hunks): (1) the loop-head `text->SetValue(L"INVALID FILE DESCRIPTION STRING", true, true)`
  with the IMPLICIT wchar_t* -> CWideString conversion (not `CWideString(L"...")`): retail builds the temporary first and pushes its
  address; this one fact fixed the whole global edi allocation (48 -> 6 hunks: the P4 saveList split and the loop `this` web followed);
  (2) P5 save-slot count as `if (IsDebugProfile()) saveList->InitialiseOffsets(size()); else saveList->InitialiseOffsets(size() - 1);`
  (tail-merged to `size; test al; jne; dec`); (3) `bool hasSignature = false;` declared before `CWideString signaturePath;`;
  (4) one `CMenuEntry* loadMenuEntry` local for the three load-entry calls (AddUIActionParam(self) / AddChild(picture) /
  AddActionOnSelected) -- retail's spilled CSE temp at [ebp+70h].
- Headers: hdr/ overlay (README.md: CTCInGameMenu OPENMENU, CUserProfileManager SAVESLOTS, CPlayerManager JOYSTICKDEVICE); flags /GS
  with `#pragma optimize("s", on)` alone. Strict-header regrade RELOCATION_MATCH. Test test/004830a1_test.cpp (vt.py PASS).
- Names: manifest label OpenInGameMenuPC is wrong -> OpenInGameMenu (NAME_FIXES.tsv). Made-up local CTCFishing model (no shared header).

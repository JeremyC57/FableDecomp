# Hand chunk 77 (CSpellContainerList .. CSubtitleRenderer): 34 of 40 byte-exact

All 34 sources in `src/` grade MATCH or RELOCATION_MATCH (`grades.txt`). Their tests in `test/` print PASS under g++ (`tall.sh`) and under the lane's VC7.1 cl.exe (`tall.py`). `payload.json` and `oracle.tsv` come from `finish.py`. Six rows are parked in `differ/`. No row was skipped.

## What matched (34)

- **"CSpellContainerList::AddChild" (3 rows, all mislabelled two-call forwarders):**
  - 00ae3330 is `reserve(n)` on two vectors of 16-byte elements, at +0xFC and +0x108.
  - 00bf7b90 is `LoadHeader(stream)` on two embedded CLandscapeBackgroundTreeNode members, at +0x4 and +0xEC.
  - 00c219f0 runs two input-event stages. It then returns `m_state == 6 ? 0 : -1` as a long (`xor eax,eax ; sete ; dec eax`).
- **CSplinePosParamData / CSplineRandomOffsetData (2 rows, PDB names, retail 12-byte CArray):**
  - 00b21a00 is the constructor: `SplinePosParam.resize(count, 0)`. **Lesson:** the control-point count must be a NAMED local. Passed straight in, the zero temporary in the dead argument slot is scheduled before the member re-read and coloured in eax, where retail hoists `xor edx,edx`.
  - 00b21160 Update inlines CCardinalSpline's memberwise assignment: an out-of-line CArray `operator=`, then Tension and EndPointsConnected. The final OffsetSplinePositions call is a sibling-call `jmp`.
- **CStartupDefinitionManager (1 row):** 00433693 is the constructor: `CDefinitionManager(file, StartupErrorCallback)` plus the vtable store. The pass-through `push dword ptr [esp+0Ch]` needs `#pragma optimize("s", on)`. The engine header is the flattened PDB view, so the polymorphic base is modelled locally.
- **NPlayerGui::CStatBar / CStatBase (3 rows):**
  - 00645f2e DrawChangeText scales the colour's alpha by the game UI's global alpha. **Lesson:** retail stores +2, +1, A, then +0. That is a CRGBColour(r, g, b, a) constructor with BODY assignments R, G, B, A. An init list stores in declaration order (B, G, R, A).
  - 0064dae0 AddChangeText. **Lesson:** each arm keeps only `push flag ; add eax,anchor`, with `push 0` in the false arm. That is an if/else of two whole `push_front(CChangeText(...))` statements, whose identical tails the /Os optimiser cross-jumps. A ternary anchor argument gives 73 v 75.
  - Both bodies are frame-pointer /Os code: `optimize("y",off)` + `("s",on)`.
  - 0064cc11 is the CStatBase constructor (PDB names, retail 12-byte vectors). Under `optimize("s")` every float zero is `fldz ; fstp` and the integer zeros share ecx.
- **CStaticBlock (1 row):** 00a8e710 SaveBinary writes, in source order, two ULongs, Bool, UChar, Bool and Long. VC7.1 widens each byte argument with `xor reg,reg ; mov reg8`.
- **CStatueMasterChest (1 row):** 00ed4830 Main is the quest-entity shape of the landed CHH_CellarDoor. With the statue at position 3, only the abort test runs (tail `jmp`). Otherwise it calls `if (!IsAborted()) PQuest->RemoveThing(Thing, false, true)`, which is CGameScriptInterface slot 108.
- **CSteeringAgent (1 row):** 00a1a1a0 returns `BestDistLeft > BestNextDistLeft || BestDistRight > BestNextDistRight` as a long (`mov eax,1` / `xor eax,eax`). Retail lists are one word, so the PDB offsets sit 0x18 lower.
- **CStopWatch (7 rows, exact PDB layout):** 0062f740..0062f900 are the constructor, GetTicks, Start, StartZero, Stop, Reset and GetElapsedSeconds.
  - The members are m_fTimerPeriod, m_nStartTick, m_nPrevElapsedTicks and m_bIsRunning.
  - The QueryPerformanceCounter/Frequency imports are declared `extern "C" __declspec(dllimport) __stdcall`.
  - GetTicks is an inline static (`QPC ? ticks : 0`). Its two returns are tail-duplicated in Start/StartZero/Reset and joined with `jmp` in Stop and GetElapsedSeconds.
  - All seven matched on the first grade.
- **CStreamingFontDataBank (2 rows):**
  - 00a5eea0 AdjustUpdateEntries counts the existing entries whose `EntrySetUpdate(i, true)` fails. It uses `engine/CBankFile.h` (Size +0x4) because the class's own header flattens the base.
  - 00a5f9d0 GetLastModifiedTime is the landed 0x009CB710 source: `CDateAndTime(1,1,1,1,1,0,0)` through the hidden pointer.
- **"CStringAndCage" (1 row):** 00c1e050 is a 4-argument UI display-element constructor: base 0x00C1EAA0 (owner, rect), then the second and third arguments as init-list members. It is the sibling of the landed 0x00C1E110.
- **CStringParser (6 rows, retail 0x28-byte layout):** the comment list is one word, ErrorCallback is at +0x18 and CursorPos at +0x1C. The class is modelled locally because the 0x30-byte PDB header disagrees.
  - 00403c20 is `Init(const CCharString&, ...)`: `TempCharString = text; Init(text.GetString(), ...)`. The characters are read from the argument through the null-case-first accessor.
  - 00414e30 is the `(char*, callback, const CCharString&, long)` constructor: inline comment-list constructor, then Init.
  - 0046381b is the copy constructor. **Lesson:** every scalar goes through eax and CCursorPos is copied with `movsd` ×3, which needs `optimize("s")`. The speed build copies with rotating registers.
  - 009ba4f0 NextItemExists and 009ba8c0 PeekNextItem save CursorPos in registers and restore it only on success.
- **"CStringParser::PeekNextItem" 00c345b0 (mislabelled):** a change-detecting assignment of a 16-byte record. It returns 0 if the record is equal (the `__fastcall` equality test 0x00C20370), else copies it memberwise and returns 1. **Lesson:** the callee's result is tested with `test al,al`, so it must be declared `bool`; `int` gives `test eax,eax`.
- **CSubtitleRenderer (6 rows):** the retail layout has DisplayFlags at +0x7C and the cutscene bools at +0xDC.. (VC6 deque and vectors). The class is modelled locally with PDB names.
  - 0048db46 IsWaitingForClickPast and 0048de44 IsCutsceneSkippable. **Lesson:** the shared `mov al,1` after the last `je` comes from NESTED ifs (`if (InCutscene) { ...return false...; } return true;` / `if (DisplayingText) { if (..) return true; ... } return false;`). Flat early returns end in `sete al`/`setne al`. The hoisted `xor eax,eax` zero register in `cmp [mem],al` and the `cmp byte ptr [..],0` tests both mean `optimize("s")`.
  - 0048dbc3 GetBlackBarHeight is `(VY(h) - VX(w) * 0.5625f) * 0.5f`, with the first result in a named float local.
  - 0048f1f4 Draw2DText picks layer 0x43 or 0x40, minus 1 when asked.
  - 004908da SetCutsceneActionMode uses an implicit `""` CCharString conversion homed in the dead flag slot. The mode store comes after the call in the true arm and before it in the false arm.
  - 00492f69 is the 9-argument SetText: a named empty CWideString local built before any push.
  - The last four are frame-pointer /Os code: `y`-off + `s`.

## Parked for Fable 5.1 (6)

- 00b1b5e0 CSprite::GetHeight DIFFER(8v34): retail `sub esp,0Ch ; mov [esp+8],float ; mov [esp+4],&result ; fld [esp+8] ; mov edx,[esp+4] ; fistp dword ptr [edx]` vs the lane's `fld [ecx+48h] ; jmp __ftol2` | tried: `(long)m_height` | next: lane gap: inlined inline-asm rounding helper (asm_bake, same as 006472b2)
- 00a5f290 CStreamingFontDataBank::CStreamingFontDataBank DIFFER(89v89): `mov ecx,edi` (Filename as `this`) comes between the 2nd and 3rd Fonts zero stores in retail, before all three in the lane | tried: init-list CArray constructor, body-assigned CArray constructor, declared `~CArray` | next: lane gap: member-constructor `this` setup scheduled before register-zero stores (the 24i gap)
- 009b9940 CStringParser::SetStringPos DIFFER(62v62): retail moves `this` to esi and keeps the quote flag in cl (end pointer in edi), the lane keeps `this` in ecx with the flag in bl (end in esi); loop-head padding is `jmp ; lea ecx,[ecx]` in retail and `lea esp,[esp]` in the lane | tried: pointer loop `p != String + pos`, index loop, `text` local, declaration order flag/pointer before the store | next: lane gap: register allocation / alignment padding
- 009ba390 CStringParser::ReadLineAsString DIFFER(76v71): retail reads `[ecx+4]` / `[ecx+20h]` AFTER the call to SkipPastNewline (`this` kept in ecx across the call) | tried: start pointer local + `String + StringPos - start` length | next: lane gap: whole-program (/GL) callee register knowledge
- 009ba4a0 CStringParser::ReadAsStringUntilWhitespace DIFFER(81v80): retail `mov eax,[esi+4] ; lea edi,[eax+ecx] ; cmp byte ptr [edi],0`, lane `mov edi,[esi+4] ; mov al,[edi+ecx] ; add edi,ecx ; test al,al`; the length is built in edx (retail) vs eax (lane) | tried: early empty return, reading path as the `if` body, `optimize("s")` (57 bytes, merges returns), `&String[pos]` form | next: lane gap: the lane folds the dereference into the address computation
- 0048dc3c CSubtitleRenderer::GetTextOffsetFromEmbeddedTagGraphic DIFFER(79v79): retail places the thumb-click return (`add eax,6A4h ; jmp`) right after its `||` pair (`je A ; cmp ; jne next ; A:`); the lane jumps (`je ; je`) to a copy below the default return | tried: else-if returns (+`optimize("s")`, which fixes the shared `ret 4`), a nested `!=` guard, a result-pointer local, a ternary chain | next: lane gap: block placement

## Skipped

None. Every row is a single function. The trailing bytes of each row are int3 padding only.

## Label corrections

- 00ae3330 `CSpellContainerList::AddChild`: `reserve(n)` on two vectors of 16-byte elements (+0xFC, +0x108) of a render-module object (caller 0x00AC3E80). The callee 0x00AE80F0 "Container_Reserve" is `vector<T16>::reserve`.
- 00bf7b90 `CSpellContainerList::AddChild`: a landscape-background node pair's LoadHeader (two `CLandscapeBackgroundTreeNode::LoadHeader` 0x00BF7400 calls; callers 0x00BF8398 / 0x00BF8731).
- 00c219f0 `CSpellContainerList::AddChild`: an input-event processor (two stages 0x00C208E0 / 0x00C21780, then `state == 6 ? 0 : -1`). Its caller 0x00C10D2A loops while the result is >= 0.
- 00c1e050 `CStringAndCage::CStringAndCage`: a 4-argument UI display-element constructor (base 0x00C1EAA0, vtable 0x012B33A4, caller 0x00C0E7B5 `new` 0x28).
- 00c345b0 `CStringParser::PeekNextItem`: the change-detecting assignment of a 16-byte UI record at +0x38 of a control (caller 0x00C20E50 ORs 6 into +0xA8 on change).
- 00b1b5e0 `CSprite::GetHeight`: reads a float at +0x48 (the PDB CSprite Height is an unsigned long at +0x19C). The owner is unknown, and there are no direct callers.
- 00414e30 `CStringParser::CStringParser::ConstructorInitialize`: the `CStringParser(char*, void (*)(CCharString*), const CCharString&, long)` constructor.
- Callees:
  - 0x00438010 (`_Cons_val<...>`) is `CGameUI::Draw2DText`, which forwards to `CDraw2DText::AddText` 0x0064B6DA through +0x238.
  - 0x0099EAE0 (`_Cons_val<...>`) is `CCharString::~CCharString`.
  - 0x00463871 (`CEngineSceneGrid_InitFromCell`) is CStringParser's comment-list copy constructor.
  - 0x00C20370 (landed as `CTCScriptedControl::IsActive`) is a `__fastcall` bool equality test of two 16-byte records.
  - 0x0075B8D0 (`Std_Vector_Assign`) is `CArray<C3DVector>::operator=`.
  - 0x00F35B30 (`CCreatureAction_TrollWhackGroundBase::Initialise`) is the quest-entity abort test.
  - 0x009E1E40 / 0x009E1E10 (GFAYToVY / GFAXToVX, manifest `__fastcall`) take their float on the stack, callee-popped.
  - 0x00993E50 (`WriteCBYTE`) is the 1-byte writer; 0x00993EA0 / 0x00993EB0 are the 4-byte ULong / Long writers.

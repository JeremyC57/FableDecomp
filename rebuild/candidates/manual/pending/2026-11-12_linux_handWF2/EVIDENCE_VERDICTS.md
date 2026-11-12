# Workflow wf2 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 00868ce0 -- VERIFIED, author attempts 1

- What made it match: Moving off the local stand-in models onto the shared engine headers fixed the old frame residue (0x1C v 0x18) by itself. The one remaining residue was argument load order, fixed by reading the def index into a local (`long index = *it;`) before the CDefPointer out local is declared. Leaf ?SetWeapon@CHitParameters@@QAEXAAVCThing@@@Z, no compiler flags, no made-up names. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf2/00868ce0/00868ce0.cpp; test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf2/00868ce0/test_00868ce0.cpp; payload.json and oracle.tsv written by addmatch.py in the same directory.

## 00e2eac0 -- VERIFIED, author attempts 2

- What made it match: Three things. (a) The loop is the generated form of Main 0x00E2B090: `for (;;) { CHECK; if (dead) { CHECK; flag; break; } if (near) { CHECK; flag; break; } NewScriptFrame(); } CHECK;` with one check at the loop head, not a pre-loop check plus a check after NewScriptFrame. VC7.1 inverts the loop itself. This form is byte-exact, zero-store order included, under the diagnostic /d2inlT86 to /d2inlT100, so the earlier "Info-first" residue came from the two-check spelling. (b) At the default threshold that form is 293v363 because the dead-arm, post-loop and scope-end destructor sites stay calls. A 16-variant probe (each of the four checks single or double wrapped) showed that the retail split comes only when the dead arm's check is double wrapped; double-wrapping the head or post-loop check as well changes nothing, and double-wrapping the near check breaks it. (c) The folded `?:` statement with a class temporary still supplies the flag word; any position works. SpringAmbush 0x00E2F170 opens with the same loop and shows the split this predicts (dead-arm check and return expanded, near-arm and post-loop checks calling 0x004AA840).

## 005590c0 -- VERIFIED, author attempts 1

- What made it match: Shared-header port (CScrollBarOutside, CManager, CUIDef, CDefPointer default Release) plus four source-level facts read off the frame: (1) the 12 unused frame bytes are a `C2DBoxI box` local (only TLY stored; TLX/BRX dead, which leaves the first GetUIScale/GetSize pair with no arithmetic); (2) each bottom-right coordinate is `(PScrollBar->GetSize() * CManager::GetInstance()->GetUIScale().<axis>).<axis> * zoom + position` via C2DVector::operator*(float), whose by-value scalar explains the integer-move copy of the scale's Y before the second GetSize call; (3) the tests read `box.GetBRPos().Y` / `box.GetTLPos().Y` (the C2DCoordI temporary is the esp+8 fild slot); (4) control flow is `if (under) ... else if (above) ...` followed by one ObserveEvent(UI_EVENT_LEFT_MOUSE_RELEASE), which VC7.1 tail-duplicates into retail's two call+epilogue copies (an explicit early return gets cross-jumped instead).

## 00541fd0 -- VERIFIED, author attempts 1

- What made it match: Replacing the draft's local models and wrong identities (Icon / CGraphicBank::GetGraphic / SetText / Colour / g_DefStringTable) with the shared headers, and writing the font lookup as one full expression: `PFont = GFGetSystemManager()->GetFontManager()->GetFont(def->Font.GetString());` through the inline CDefString::GetString and CSystemManager::GetFontManager. The rest is `CDefPointer<const CUIDef> def = LoadAndReturnDef();`, plain struct copies for Window and PositionOffset, `SetValue(def->TextValue)`, and a switch on LEFT / CENTER / RIGHT OR-ing 0x08 / 0x10 / 0x20 into FlagsSetTwo. No pragma, no flags, no made-up names. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf2/00541fd0/00541fd0.cpp; test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf2/00541fd0/00541fd0_test.cpp; payload.json and oracle.tsv written by addmatch.py in the same directory.

## 008c2ad0 -- VERIFIED, author attempts 1

- What made it match: The source is unchanged (effect index declared inside a block, no body opt-ins); only NAME_FIXES.tsv was repaired.

## 005f11a9 -- VERIFIED, author attempts 1

- What made it match: Ported onto the shared headers with the body of the landed sibling 0x005F1320 (flag/break loop, icon block, find/erase tail), plus this head:

    EHeroAbility current = AssignedAbilities[slot];
    EHeroAbility saved = AssignedAbilitiesSave[slot];
    EHeroAbility removed = AssignedAbilities[slot];
    EHeroAbility ability = removed;
    if (current != saved) AssignedAbilities[slot] = saved; else { both = 0; }

Three facts drive it:
- Two reads of the slot before the branch make VC7.1 keep the value and re-read each `begin` for the stores; one read makes it keep `&AssignedAbilities[slot]` in a register (the old 358-byte residue).
- Both reads must come before the store to the address-taken key local, or the second read comes from memory again.
- `ability = removed` (register local assigned from the address-taken key local) leaves retail's `mov edx,ecx`; `removed = ability` coalesces and loses it.

Local names are descriptive. Files are in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf2/005f11a9: 005f11a9.cpp, 005f11a9_test.cpp, payload.json, oracle.tsv, NOTES.md, hdr/README.md. The differ/005f11a9.cpp there is still the old parked draft, superseded.

## 006978d0 -- VERIFIED, author attempts 1

- What made it match: Unchanged from author: shared headers, by-value up/view copies blended memberwise, parenthesised GFGetInterpolatedValue assignments. The repair fixed the test's SetFOV model to match retail's FOVFlags behaviour.

## 00eb3f40 -- VERIFIED, author attempts 1

- What made it match: Port onto engine/CV_MurderTwistFinalScript.h, CIsThingAlive.h and TRetVoid.h with the PDB callee names, plus two devices. (a) Loop A's body is an if/else on `trust` with identical arms: VC7.1 merges the arms after register allocation, leaving the dead `mov bl,al` at 0x00EB3FF4 and the doubled terminate check. (b) One statement `if (trust && false && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) { }` after the trust line: the front end allocates a conditional-destruction flag word for the CCharString temporary, the optimiser deletes the statement, and only `mov dword ptr [esp+0Ch],0` remains; the register assignment and late `push ebx` then fall out. Caveat: the bytes prove a flag word and a trust value live into loop A (also in twin 0x00EB55D0), but not the wording of those script lines; the string and callee in (b) are my choice, borrowed from DoWatchingForTrouble.

## 008d5300 -- VERIFIED, author attempts 1

- What made it match: The port onto the shared headers was enough; the old 379v383 "register tie" (&PCreature hoisted into ebp) was a model error. The fixes: `PCreature->` written per access instead of a cached creature pointer (GetTC<CTCCarrying>, GetPos, GetPlayerNumber); `PAmmo = CThingObject::Create(ammo, PCreature->GetPos(), PCreature->GetPlayerNumber(), 0, 0, "")` with the real six-argument static __fastcall signature, replacing the draft's 2-arg Create plus an invented 4-arg position query; PAmmo typed CIntelligentPointer<CThingObject>; `pweapon->PeekBaseDef().GetSubDef(&weapon_def)` with CDefPointer<const CWeaponDef>; `long ammo = def->Ammo; long slot = def->MissileLoadCarrySlot; if (ammo > 0 && slot > 0)`; RemoveThing / PutThingInCarrySlot taking CThing&; both IS_THING_ALIVE forms, the CIntelligentPointer (T*) ctor and operator=, and GetSubDef<CWeaponDef> kept out of line through existing opt-ins.

## 009adc90 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The source is unchanged from the author's match. The repair only corrected the NAME_FIXES and NOTES spelling.
- OPEN at hand-back: The overlay header $W/hdr/engine/CDefinitionManager.h declares the CInstantiatedDefInfo constructor as (CDefString, CDefString, unsigned long, CDefClassBase* pdef). FableWin.pdb has the real spelling, ??0CInstantiatedDefInfo@CDefinitionManager@@QAE@VCDefString@@0KABV?$CDefPointer@VCDefClassBase@@@@@Z at FableWin 0x03061650 (in ghidra_out/fablewin_pdb_names.tsv, _fablewin_api.tsv and native_gameplay_targets.tsv), so the 4th parameter is const CDefPointer<CDefClassBase>&, not CDefClassBase*. The signature disagrees with the PDB and nothing justifies the difference with retail bytes.
- OPEN at hand-back: NOTES.md says the CInstantiatedDefInfo ctor 'is not in any PDB list (fully inline)', and DOSSIER.md line 75 says 'no out-of-line ctor in PDB lists'. Both are false: FableWin.pdb lists ??0CInstantiatedDefInfo@CDefinitionManager@@QAE@VCDefString@@0KABV?$CDefPointer@VCDefClassBase@@@@@Z (0x03061650), and also ??0CInstantiatedDefInfo@CDefinitionManager@@QAE@XZ (0x0305D840). The author should either respell the ctor (and pdef, possibly as a CDefPointer local whose destructor is the final release) to match the PDB, or explain with the 0x009ADD38-0x009ADD4C bytes (single `inc [esi+4]`, no null test) why the PDB parameter type cannot reproduce retail.
- OPEN at hand-back: Minor: the test's model copy is not the shipped body. It adds a test-only `if (DropRecordReference) pdef->Release();` before the final Release, so step 5 tests a modified function rather than the row's code. The delete path can be exercised without editing the body, for example with a record ctor stand-in that skips the AddRef.


# Chunk 574 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Several string tests sharing one action: write one `else if` arm per test (0x00DFC320).**
  - `if (GetDefName() == "X" || GetName() == "A" || ...)` puts the GetDefName temporary inside a `||`. VC7.1 then adds
    conditional-destruction flag bits (838v777).
  - `if (def) {X} else if (A || B || ...) {X}` is the right size, but VC7.1 keeps the cross-jumped X at the second arm and
    places it at the loop end.
  - Six arms `if (def) {X} else if (A) {X} else if (B) {X} ...` keep the first arm's X in place, as retail does.
  - Try the per-test arm form whenever retail's merged block sits right after the first test.
- **A `test al,al ; setne bl` loop bool on a bool-returning call is a double negation (0x00EAEA20).**
  - Retail: `while (!(!Game->IsLevelLoaded("OakBay")))`, a wait-until(!loaded).
  - The plain `while (IsLevelLoaded(..))` gives `mov bl,al` and a different loop layout.
  - `while (!(s != "NULL"))` likewise gives `neg/sbb/inc` (0x00D37D00).
- **Entity-script `Main` rows (rotated waits, a mission loop, member calls) match on the first probe (0x00D37D00 /
  0x00D384A0).**
  - Model: CEntityScriptBase `{vptr, Game, Me}` with `IsActiveThreadTerminating() const` 0x00F35B30, plus the __fastcall
    IsDistanceBetweenThings* helpers.
  - `PadRadius * 1.5` is a double product (`fmul qword [__real@3ff8...]`) narrowed to the float argument.
  - A trailing `if (IsActiveThreadTerminating()) return;` compiles to a bare call.
- **Reusing a CScriptThing local in a loop (0x00E56D10).** The form
  `thing = Game->GetThingWithScriptName(name);` matches on the first probe when:
  - `CScriptThing::operator=` and `CCountedPointer::operator=` are `__forceinline` (the latter reads other.Info, then
    other.Data, before the self-compare);
  - the dtor is `__forceinline virtual ~CScriptThing() { }`, which stores the vtable inline;
  - `virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }` is defined inline (slot 75).
  - Two passes ("boy", then "girl") whose start block equals the loop block are merged by VC7.1 itself.
- **Persist rows (0x00E690F0) match on the first probe from the landed OnPersist model.** Use one `Transfer(name, member, 0)`
  per member in retail's order. The default stores scheduled before the `lea/push` in a few calls come out by themselves.
- **Quest vtables name the rows quickly.** Slot 1 RegisterMain, 2 Main, 3 Init and 4 OnPersist of the vtable whose slot
  holds the row; entity vtables have slot 1 Main.
  - xref the row address: it is usually referenced only from its vtable slot.
  - The landed Alloc that stores the vtable names the class.
- **FireHeart's `std::vector<bool>` is byte-based in retail (0x00D37D00).** `cmp byte [StartingPattern._M_start + PadNumber],
  0`; every member offset fits a 12-byte vector (as the landed SetupPuzzleBlocks 0x00D36140 has it). The owner rules list
  vector<bool> as 0x14, so check per class before assuming either size.

Review at landing:
- No row needed more than 4 grades. 0x00DFC320 used 3 grades plus 5 compile-only probes (`bin/pq.py`, a copy of the chunk-573
  probe sweep).
- vland RELOC_NAME warnings for 0x00EAEA20 come from wrong catalog labels:
  - 0x00EB1F80 is the landed CMurderTwistSheriffGuard::Alloc, but its catalog row says `_gapscan::sub_00eb1f80`;
  - 0x00CB8930 is labelled CTCVillage::OnInitialActivate but is CScriptBase::PostAddScriptedEntities.
- **The landed 0x00EB6A00 is misnamed.** It sits as `CV_SwordInTheStoneScript::Init` (global struct, invented members), but it
  is `NScript::CV_MurderTwistFinalScript::Init`: slot 3 of vtable 0x012EF978, and its members are SheriffState /
  AssassinState / TaskDeclined / PreemptiveAttackInProgress / Guards / seh_Guards (NAME_FIXES.tsv).
- **NAME_FIXES_QUEUE.tsv row 522 has the wrong spelling for 0x0099EFB0.** It gives the non-const-return operator=; both PDBs
  give `??4CCharString@@QAEABV0@ABV0@@Z`, as queue rows 529/571 do.
- **CScriptThing vtable 0x01238C8C has 35 invented data-symbol spellings bound** (`??_7CEvent_<addr>@@6B@`, ScriptMethods,
  CScriptThingRuntime, ...). The correct spelling is `??_7CScriptThing@@6B@`.
- **Local models next to flattened headers.** The rows model CScriptThing / CGameScriptInterface locally even though
  engine/CScriptThing.h and engine/CGameScriptInterface.h exist (flattened, no vtables), as the chunk 570-573 rows do. vland
  did not warn.

# Chunk 508 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A single-test entity wait loop whose test builds a CCharString temporary (`MsgIsUsedBy("SCRIPT_NAME_HERO")`), with the
  action after the test, is `do { if (Me.MsgIsUsedBy(..)) { if (term) return; act; return; } frame; } while (!term);`.
  `while (!call())` gives neg/sbb/inc, and `for (;;)` duplicates the temporary's construction (0x00DD8FD0).
- A ternary over two string literals: `ID == 0 ? "_WEAK" : "_TOUGH"` gives retail's `mov eax,A ; je`. The `ID != 0` form
  swaps the operands (0x00EB2070).
- Inline helpers that call through g_pGameScriptInterface (CTimer::operator=(long), slot 89 SetTimer) must be declared after
  CGameScriptInterface. Declared before it, the TU fails to compile (SRC_FAIL, 0x00E323B0).
- CWideString::operator=(const wchar_t*) needs `// Flags: /Zc:wchar_t` to get the PDB spelling ??4CWideString@@QAEABV0@PB_W@Z (0x00E68DC0).
- `if (!Flag && Me.MsgIsKilledBy(""))` gives retail's conditional temporary: `xor ebx,ebx`, a stored result byte, and
  `test bl,1` before the dtor (0x00EC8230).
- CDefString has twin conversions with byte-identical bodies: non-const operator CCharString() is 0x00415D70, const is
  0x00415DD0. Access through the non-const `CScriptDef* pGlobalScriptDef` picks 0x00415D70; declare both operators so each
  call site binds its own twin (0x00EC8230, 0x008E7040, 0x0071C7C0).
- CScriptDef members after a vector sit 4 bytes lower again in retail, because STLport vectors are 12 bytes against
  Dinkumware's 16. KilledSomeoneForMushroom is Ego_r +0x7c8 -> retail +0x768: 0x5c below for the base, plus 4 for
  SickChildsSisterAttentionAnims (0x00EC8230).
- A wait on `IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.0f)` (__fastcall 0x00CBE2FF): the 5.0f is pushed before the
  GetHero vcall, and edx takes GetHero's result (0x00EE3FC0).
- `SetPredicate(CIsThingAlive(Me))` with an inline ctor and a declared virtual `~CIsThingAlive()` (0x00E241B0) gives retail's
  single vtable store, the out-of-line CScriptThing copy 0x004ABE90, and the direct dtor call (0x00EE3FC0).
- A trailing `if (IsActiveThreadTerminating()) return;` at the end of a void level-script method compiles to a tail
  `jmp 0x00CB7940` (0x00EE29D0). In an entity Main it compiles to a call whose result is unused (0x00EDC390).
- An unexplained entry-time `mov dword ptr [esp+N],0` into a temporary's slot is the home-slot init of a conditional-temporary
  flag. Rows with a `new CSpawnedFunc("Name", ...)` get it for free (0x00EEE800, 0x00EFC260). In 0x00EE82D0 there is no
  visible conditional temporary, and the row stays parked.

Review at landing:
- 0x00EE82D0 CCS_OakValeRevisitedScript::Main is parked DIFFER(172v181). The only residue is the entry zero store plus a
  loop-align nop. Three grades were used: the plain form, a `true &&` guard, and a dead `|| false &&` that hit C1001.
- vland RELOC_CONFLICT on ??BCDefString@@QBE?AVCCharString@@XZ is resolved here by evidence: 0x00EC8230 now binds the
  non-const ??BCDefString@@QAE?AVCCharString@@XZ -> 0x00415D70, as the landed 0x008E7040 does with struct spelling. Confirm
  the twin split. Ego_r keeps only the const spelling.
- The landed 0x00E76540 "CEM_Rhodri::Init" is really CEM_Sheriff::Init (vtable 0x012E98B4, stored by CEM_Sheriff::Alloc
  0x00E76470).
- The landed 0x00E1FEB0 "CV_AmbushScamScript::ResetRandomSpeechTime" uses a timer at +0x84, but CV_AmbushScamScript's
  RandomSpeechTimer is at +0x7c. The class attribution is suspect.
- The landed 0x00E5FC70 is named descriptively GetHenchmanGoldCost. The PDB name is CBodyGuard::GetGoldCost (RELOC_NAME in
  vland).
- Local base models (CGameScriptInterface, CScriptThing, CScriptBase / CLevelScriptBase, CEntityScriptBase, CSpawnedFuncBase)
  sit next to existing engine headers: 27 SHARED_HEADER_AVAILABLE warnings.
- The catalog labels of the CSpawnedFunc SuspendableProcess slots 0x00EEE7F0 and 0x00EFC250 (RansomVictimChiefsHouse) were
  propagated across identical bodies.

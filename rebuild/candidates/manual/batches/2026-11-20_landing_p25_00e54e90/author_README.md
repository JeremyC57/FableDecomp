# 00e54e90 (p25B, Account B, 2026-11-20): NScript::CV_BookCollectingScript::CBS_Teacher::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00E54E90 | CBS_Teacher::Main() | 3534 | RELOCATION_MATCH (no flags) | PASS (8 targeted + 20,000 random runs) | 46 live: 45 killed, 1 same code; dead line reported separately |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`, no warnings (no new relocation or binding).
- Purity: plain C++. No asm, no pragma, no flag.
- Line 2 `// Inline-budget:` names the header-contract inline devices, each with its ablation grade (below).
- One `// Inferred:` fitted statement, admitted under the coordinator's ruling of 2026-11-20 for 0x00E72630, which extends it to this row (same probe,
  same constraints).

## What the parked diagnosis missed
The parked draft (chunk 845, DIFFER 3534v3534) had two residues: retail's entry store `mov dword ptr [esp+34h],0` (0x00E54E9C) and the
exit-block (pad) order. They have separate causes:
1. **The exit order comes from the plain `return;`.** Rewritten in the generated text, every exit becomes `return _Fail_Val.Return();` with
   `TRetVoid _Fail_Val;`, the WF11 lesson. With that and the fitted operand the row matches. With the fitted operand but plain returns
   (ablation/e54_f1.cpp) it is DIFFER(3534v3534), 66 lines, all in the exit order.
2. **The entry store is a dead conditional-temporary flag**, as in 0x00E72630. Without the fitted operand: DIFFER(3534v3534), 37 lines (the store
   and the moved loop pad).

## The fitted statement
The probe the ruling asks for (ablation/e54_p1.cpp): a conditional temporary inside a `__forceinline` body that Main expands mid-function
(the first talk test) puts its flag init at the entry: `mov dword ptr [esp+30h],0` 0x00E54EAB, before the first call.

The bodies Main expands (c2 trace):
- the CCountedPointer ctor / dtors;
- the CScriptGameResourceObjectBase ctor / dtor;
- the movie ctor / dtor (expanded and called);
- ~CScriptThing (the row-local contract body);
- the pause ctor / dtor;
- vector<CCharString>::operator[] / begin;
- TRetVoid::Return.

Their expanded retail bytes equal ours, and none takes a class temporary in a conditional operand. The list is clean, and the evidence matches
0x00E72630's.

**Fitted statement.** The dead post-loop check after the endless completion loop (Ego_d line 751, 26 bytes = a terminate check with no live
local) gets one more operand: `if (IsActiveThreadTerminating() || Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) return _Fail_Val.Return();`.
- It adds no line Ego_d lacks. It uses only calls and literals this function already uses (both talk tests).
- `&&` also matches (ablation/e54_and.cpp). There is no other dead position in this function.
- **The bytes prove that a conditional temporary existed in dead code, but they constrain neither its content nor its place.**
- Ego_d's prolog line 603 is 36 bytes (the flag-free value; 46-47 with a flag), so its later revision had no such temporary.

## Ladder and per-device ablation (ablation/, grades.txt)

| device removed | grade without it |
|---|---|
| the fitted `|| Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")` operand (`// Inferred:`) | DIFFER(3534v3534), 37 lines |
| FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR | DIFFER(3473v3534) |
| row-local `inline CScriptThing::~CScriptThing() { }` (as the landed 0x00E7C010) | DIFFER(3486v3534) |
| `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` (out-of-line declaration) | DIFFER(3600v3534) |
| `TRetVoid` returns -> plain `return;` (text, not a device) | DIFFER(3534v3534), 66 lines |

No flag and no pragma are used. The row matches at default flags, so the flag step of the ladder had nothing to replace.

## Source shape
- The draft's statement order: its bytes already matched apart from the two residues.
- Written in the generated text of the landed Mains:
  - the seh macro and each speak-and-wait macro on one line;
  - each terminate check on its statement's line;
  - movie and pause on one line.
- Names from Ego_d.pdb's locals:
  - function scope `_Fail_Val`;
  - blocks {speech_movie, PauseMyEntitiesPlease, {Given}} and {speech_movie, PauseMyEntitiesPlease};
  - `bool Given=AskForBook(..)` matches as the draft's `if (!AskForBook(..))` did (g1 / g2);
  - seh_me is the class member at +0x34. Its virtual calls (GetScriptThing, Speak, IsPerformingScriptTask) go through its vptr.
- Ego_d is a later revision of this script. Its talk-arm line table does not fit retail's order, so it fixes names and scopes here, not the order.
- No other `// Inferred:` lines.

## Headers (hdr/, hdr.diff: one NEW file)
NEW engine/CV_BookCollectingScript.h, carried unchanged from chunk 845's pending overlay. Checked against two sources:
- Ego_r.pdb (struct_layouts_egor.tsv): CBS_Teacher 68 bytes, BooksAccepted +40 = +0x28, seh_me +0x34. CV_BookCollectingScript 228 bytes:
  DoneIntro 144 - 0x14 - 4 (the vector size difference) = +0x78.
- The retail bytes of this row: GossipState +0x7C, HasInformation +0x80, BooksDonated +0x84, LastBookRequested +0x8C, BookDef +0x94,
  BooksAccepted +0x28, seh_me +0x34.
- The test runs the row against those offsets.

NEWLY FLAGGED (owner policy: listed, not a blocker). These landed rows define CV_BookCollectingScript / CBS_Teacher locally:
0x00E54310 0x00E543B0 0x00E54440 0x00E54450 0x00E54570 0x00E54580 0x00E54620 0x00E54630 0x00E546A0 0x00E546D0 0x00E54830 0x00E54880
0x00E54990 0x00E55C60 0x00E55CE0 0x00E566D0 0x00E566F0 0x00E569D0 0x00E56D10 0x00E57000 0x00E57020 0x00E57530 0x00E57A50 0x00E57AA0.

## Test (test/00e54e90.cpp)
Links the real row object. These run as compiled:
- the row body;
- the expanded and called movie ctor / dtor COMDATs;
- the expanded ~CScriptThing of the GetScriptThing temporaries;
- the pause expansion;
- seh_me's virtual calls.

Every external is bound by /alternatename to a __fastcall stub, and every vtable slot to a trap. ParentClass and PMasterData are raw buffers at
the header offsets.

What the inputs script:
- terminate positions and StartScriptingEntity retries;
- talk results and the time of day (844 / 845 / 846 / 1200 / 1614 / 1615 / 1616);
- speaker health and IsPerformingScriptTask;
- book possession and hand-over;
- the hero-dolls flag and donated books, changed per frame;
- the initial gossip stage, books, last request, BooksAccepted, DoneIntro, HasInformation, LadyGrey and a held seh_me.

What the test checks:
- each run, event by event, against an independent model, including the evaluation and destruction order of every string temporary and the callee
  destruction of AddGossip's / AskForBook's by-value strings;
- every argument;
- the final GossipState / BooksDonated / LastBookRequested / HasInformation / DoneIntro;
- no other store, and seh_me's held state;
- every string, thing and handle released.

What the 20,000 random runs reach:

| path | runs |
|---|---|
| quest-card intro | 2,570 |
| completion | 12,502 |
| hand-over given | 1,047 |
| hand-over refused | 947 |
| wrong time | 920 |
| rumour removal | 3,190 |
| seh_me release | 12,982 |

## Mutants (mutants/)
**Live code.** 46 one-decision mutants (mutants/m54.tsv, m54.log): 45 killed.
- SAME_CODE (1): the objective's last `""` -> `"x"` changes only a relocation target (mut2 compares bytes). The test's Lit check would catch it.

**Dead line** (the fitted operand, mutants/m54dead.tsv, m54dead.log; the behaviour test cannot kill these, the grade does):
- dropping the operand: SURVIVED the test; the grade is DIFFER(3534v3534), 37 lines;
- `||` -> `&&`: SAME_CODE (RELOCATION_MATCH);
- `"SCRIPT_NAME_HERO"` -> `""`: SAME_CODE;
- `return _Fail_Val.Return();` -> `;`: SAME_CODE.

## Entry for docs/journal/FINDINGS_LOG.md (addendum to the 0x00E72630 entry)
2026-11-20 (p25B, 0x00E54E90) BookCollecting CBS_Teacher::Main.
- It lands with the same fitted dead conditional temporary as 0x00E72630, an extra operand on the dead post-loop check. The inline-body probe gives
  the same result: the flag init lands at entry, and the list of expanded bodies is clean.
- The draft's other residue, the exit-block order, was the plain `return;`. `return _Fail_Val.Return();` fixes it.

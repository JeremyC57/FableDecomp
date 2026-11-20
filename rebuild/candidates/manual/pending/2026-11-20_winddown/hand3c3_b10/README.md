# b10 (hand3c3, Account B, 2026-11-20): CTCHeroCentre::FrameUpdate -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x0077D5B0 | CTCHeroCentre::FrameUpdate() | 1491 | RELOCATION_MATCH (`// Flags: /GS`) | PASS (60,000 cases) | 139 / 141 (2 equivalent) |

Strict vland (LANDVERIFY_STRICT_HEADERS=1): `RELOCATION_MATCH PASS`; two RELOC_NAME warnings, both stale catalog labels (see
NAME_FIXES.tsv: 0x0077EC20 and 0x004AC940). Purity: plain C++, no asm, no shared retail_asm helper. Grades: 4 (baseline, the port,
the commented final, the COMDAT define pair). No local class model: the row is `CTCHeroCentre : CTCBase` of engine/CTCHeroCentre.h (new typed branch) on the
shared CThing / CThingObject / CQuestManager / CTCQuestCard / CGameEvent / CPlayerGui / CWorld / CThingSearchTools headers.

## Source shape
Ego_d.pdb locals and line table (lines 89..236, egod_lines/), one statement per entry; the 132..517-byte entries are debug ASSERT
lines without retail code. Two halves: the tutorial wait (subtitle idle, world not paused -> clear the flag and queue game event
0x1A / 0xB for a hero with TCI_INVENTORY_EXPERIENCE), else the quest card rebuild (kill the listed cards, walk
CQuestManager::GetAvailableQuestCardTypes(), reuse or create a card thing per type on the next table position, feat cards through
OBJECT_FEAT_CARD_DUMMY, kill the leftover card things, stamp the frame, clear the manager's flag).
- `// Inferred:` (1) the two string arguments are implicit `const char*` conversions (retail pushes `lea edx,[temp]` and loads the
  position table after the string constructor; explicit `CCharString("..")` is 1474 bytes). (2) GetTC / PeekTC and the
  CIntelligentPointer copy constructor in their FableWin text (FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN,
  FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN): the other spellings leave swapped registers at 0x0077D8E2 / 0x0077DA2C
  (84 and 73 instructions off, same bytes otherwise). (3) AddULONG rather than AddSLONG (same bytes; the landed OpenTrainingScreen
  0x0077D4B0 spells this event AddULONG).
- `// Fidelity:` the TU instantiates GetAllLeafThings<CThingObject, std::vector<CThingObject*>, CThingFilter_HasTC> (retail
  0x0077EC20, 136 bytes) from the visible noinline body of engine/CThingSearchTools.h. With the two existing opt-ins
  FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY_CASTARG + FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE (the define pair of the landed
  shared-header instance 0x00910A90) that COMDAT is RELOCATION_MATCH against 0x0077EC20 from this file (`tools/gother.py
  src/0077d5b0.cpp 77ec20 136`; it was 126 bytes without the pair: folded `test al,21h` scan test and the argument homed early)
  and FrameUpdate is byte-identical to before. No header change was needed for it. The landed local-model row 0x0077EC20 can be
  re-ported as an explicit instantiation with this row's preamble (backlog).
  The row needs the visible body (the vector's size is cached across the calls in the last loop, 0x0077D886).

## Header overlay (hdr/, hdr.diff; strictly additive: 0 removed lines; copied from today's rebuild/include)
- engine/CTCHeroCentre.h: `FABLE_RETAIL_CTCHEROCENTRE_CLASS` -- the polymorphic class (CTCBase, slots 8 / 9, GetQuestCards,
  RemoveQuestCard, OpenTrainingScreen, the six Ego_r members with retail's 12-byte vectors, sizeof 0x34) as `#ifdef .. #else <the
  existing flat layout> #endif`; the flat layout stays the default.
- engine/CTCQuestCard.h: `FABLE_RETAIL_CTCQUESTCARD_FEATACCESSORS` -- GetFeatIndex / SetFeatIndex / SetQuestName /
  SetScriptQuestName, inline (FableWin names in the comment).
- engine/CQuestManager.h: `FABLE_RETAIL_CQUESTMANAGER_QUESTCARDTYPES` -- GetAvailableQuestCardTypes / GetQuestCardsNeedUpdating,
  inline (inside the existing FAILUREBOOKKEEP block, which already has SetQuestCardsNeedUpdating).
- engine/CGameEvent.h: `FABLE_RETAIL_CGAMEEVENT_INLINE_ADDULONG` -- `void AddULONG(unsigned long value) { AddData(sizeof(value), &value); }`.
No new header file: no landed local-model row is newly flagged. Re-port backlog (landed rows of this class still on local models):
OpenTrainingScreen 0x0077D4B0, RemoveQuestCard 0x0077D400, GetQuestCards 0x0077DBC0, HasFrameUpdate 0x0077EB90.

## Test (test/0077d5b0.cpp)
Links the real row object. No class: the row is entered and its 31 out-of-line callees are bound through `/alternatename`
(`std::__find` as an explicit specialisation: its alias name is too long for the linker option), vtables are hand-built, operator
new / delete are the test's own. An independent model (RunModel) writes the expected event list (K kill, R reserve, G def lookup,
C create with position and right-handed set, = string assign, A game event with the player byte, E erase) and the expected centre /
card / manager state; both are compared with the row's after every case. 60,000 seeded cases; the run prints its path counters
(tutorial wait 8347, event 3318, no inventory 3245; list skipped 9898; reused 12453, created 30689 (null 3696), feat card reused
1975 / created 33321, no dummy def 8516, last position 46541, leftovers killed 77513, list regrown 3038) and fails if one is zero.

## Mutants (mutants/)
141 one-decision mutants: 139 KILLED. The two survivors are equivalent:
- `SetFeatIndex(..); SetQuestName(..);` swapped -- two independent member stores of the same card component;
- `continue;` removed in the no-dummy-def branch -- pquest_card is still NULL there, so the `if (pquest_card)` block that follows is
  skipped either way (the `break` mutant of the same line is killed).

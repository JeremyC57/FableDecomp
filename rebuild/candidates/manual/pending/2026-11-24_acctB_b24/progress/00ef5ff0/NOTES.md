# 0x00EF5FF0 CPersonalScript_EmptyScript::CCinemaMan::Main -- B24A12 (2026-11-24)

## Lessons applied
- Model rows (CTB_Bard 0x00ED8680): TRetVoid _Fail_Val + `return _Fail_Val.Return();`, `if (false) {} else for (;; Game->NewScriptFrame())`
  with the terminate check first (FableWin 0x03AEBB60 shows exactly this: `xor ; je ; jmp` before the main loop, TRetVoid::Return at
  every exit, GetHealth(seh_me.GetScriptThing()) > 0 guard before each of the 8 Speak calls, final IsPerformingScriptTask wait).
- Same-TU siblings landed with per-row T flags: CMeanBandit::Main /d2inlT28, CTestPerson::Main /d2inlT36.

## State
- differ/00ef5ff0.cpp = v/c1.cpp: the draft ported to the model idioms (TRetVoid, for loop) on header FABLE_RETAIL_CPREDICATE_AND_INLINE_DTOR
  (the draft's FABLE_RETAIL_CPREDICATE_AND_IMPLICIT_DTOR no longer exists; without an inline dtor the draft grades 3106v3265).
  Stock: DIFFER(3265v3265) 24 hunks; construction (ITA ctors expanded, And ctors called) matches to 0x00EF61E4.
- c2 trace (tools in ../../b24e/00daadd0/tr/, trace v/../tr/b1.cpp.trace.txt): heuristic adj of the And dtors in Main:
  ~And6 d1 27 (accepted), nested ~And5 d2 85 (rejected, = retail); ~And5 temp d1 82 (REJECTED, retail expands); ~And4 d1 64,
  nested ~And3 d2 52, ~And2 d3 30 (all accepted, retail calls the nested ones); ~And2 temp d1 27. ~CScriptThing adj 60.
- /d2inlT28..30: whole predicate teardown = retail (first diff 0x00EF6334) but the FIRST terminate exit then calls ~CScriptThing
  (retail expands 5 ~CScriptThing there, the other exits call 0x004AA840) -> DIFFER(3090v3265) b1 / (3231v3265) c1. ~CScriptThing's 133 decisions (adj 60) never reach the score compare
  (no Q/R line): trace the branch between 0x1078D2BE and 0x1078D2F5 at stock and at /d2inlT29 to see what decides exit 1.
- Probe d1 (`#pragma inline_depth(0)` at TU END = instantiation state of the class-template dtors): teardown = retail except the
  ~CPredicateBase vptr reset is called instead of expanded (19 hunks, 3282v3265); forceinline ~CPredicateBase does not pass depth 0.
  Not a landable device; it shows retail's shape = "temp dtor inline, its nested template dtors called, ~CPredicateBase inline".
- Next idea: find why retail's depth-2 ~CPredicate_And are rejected at T74 while depth-1 accepted: (a) the out-of-line And dtors'
  compiled sizes in retail's object (compile order: the ??_G / Clone users of each instantiation in the TU), (b) a per-row
  explicit specialization of the five ~CPredicate_And under inline_depth(0) WITH the ~CPredicateBase body visible (0x00ED5F50 device)
  -- needs the trace proof the brief asks for; (c) T29 + an evidenced reason for exit 1's expansion.
- Heuristic detail (trace tr/c1.cpp.trace.txt): every And-dtor decision in Main has freq 1e8 (score = adj), none takes the
  few-call-site reduction (0x107CF94E: adj/3 for one site); adj = the callee's compiled size. Retail's out-of-line ~And5 0x00EF6DE0
  fully expands (5 ~ITA calls, nested vptr stores) like ours (adj 82..85), yet retail's Main expands the And5 temp dtor at depth 1
  and calls ~And4 0x00EF6D90 at depth 2. With equal sizes the stock heuristic cannot do that -> the depth-1 decisions in retail's Main
  saw smaller callee sizes than the final out-of-line bodies: SPECULATION (conflicts with row 1's PIPE order, where c2 compiled every inline callee before the caller): Main compiled before the
  out-of-line dtors. Not evidence.
- FIRST next step: the landed same-TU sibling 0x00EF87D0 (CUndeadFellow::Main, 3-term predicate) documents an explicit-specialization
  / inline_depth device at its lines 32-34 and 87-158 -- read it and grade once on c1 (probe d1 left only the ~CPredicateBase vptr store).
- Unread this session: docs/pipeline/INLINE_DEPTH_REVISIT.md, $S/w22/b24a/SHARED_FORM.md, $S/w22/b24b/SHARED_FORM.md (required before exit work).
- Port debt in differ/: local enums EScriptAIPriority / ETextGroupSelectionMethod (use engine/ headers) and the local inline ~CScriptThing.

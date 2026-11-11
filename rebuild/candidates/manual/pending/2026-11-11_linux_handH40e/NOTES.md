# Chunk H40e (Fable 5.2, fifth pass) -- 23 of the longest parked rows: hand-back notes

Every grade and probe was made with `FABLE_HDR_OVERLAY=$A/hdr` (the H40d overlay, no file changed; hdr/README.md). Result: **no row
matched**. One row worked with grades (0x00E50FD0: 224 -> 99 masked lines, 2 grades, 6 of 10 used in all), three rows read with
no-grade inline traces only (0x00E5FD40, 0x00E33EA0, 0x00CF68C0), 19 rows not reached (STATUS.tsv). Nothing was written under the
repo; no git command was run; src/ is empty (gall.sh has nothing to grade), so payload.json carries 0 rows.
The brief's order (the four never-graded rows first) was NOT followed: the first trace I ran on the family frequency question paid
off at once on 0x00E50FD0 and the pass went into that row. 0x0061E6D0 / 0x00603000 (header debt), 0x0078FD60 / 0x00507C30 (no
source) are exactly as H40d left them.

## 0x00E50FD0 NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Main -- DIFFER(9850v9885), 99 lines, 6 grades used
Identity: Ego_r.pdb + FableWin.pdb `?Main@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@EAEXXZ` (as chunk 815).
What closed (differ/00e50fd0.cpp):
- **The 0.5 H40d asked for is the main-loop condition**: `for (; !IsActiveThreadTerminating() && true; Game->NewScriptFrame())`.
  c2 gives a `for` body 5x the entry frequency, 2.5x when the condition is an `&&` (hand890 TRACE S6.1), and keeps that when the
  second operand folds. Trace before / after (movie ctor c1 20, dtor c1 19): attack ctor 125 -> 62.5 EXP, exits 62.5 / 31.2 ->
  31.2 / 15.6 EXP, 7.8 EXP -> 3.9 CALL, 3.9 -> 2.0 CALL, scene end 19.5 -> 9.8 EXP, phase-0 ctor 7.0 EXP -> 3.5 CALL: retail's
  split at every movie site, and the two expanded ~STB sites (loop exit, first attack exit) stay. `true && !T`, and
  `for (;; NSF) { if (T || false) break; ...}` give the same trace and the same allocator table. Precedent for the spelling:
  the landed 0x0056AD40 (`questCardThing == 0 && true`, hand890 S7.5). First grade: 10022 -> 9882 v 9885, 224 -> 128 lines.
- Two source errors that the big residue had hidden: the phase-0 intro test is `if (!ParentClass.IntroComplete) { INTRO1;
  IntroComplete = true; } else { INTRO_REMINDER }` (retail 0x00E51665 `jne 0x00E51725` to the reminder arm), and the random-speech
  turn is `Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false)` -- retail pushes `false`, then `&Me`, then calls
  GetHero and pushes its result (0x00E51370..0x00E51383), so the hero is the first argument (four landed scripts spell it that way).
  Second grade: 9850 v 9885, 99 lines; everything between 0x00E51308 and 0x00E534DC now differs only by frame displacements and
  alignment pads.
What is left (one cause each, measured):
1. **ebp: vtable 0x0126008C in retail, const 0 in ours.** Retail loads `mov ebp,0126008Ch` in front of the loop test (0x00E510FE)
   and at the loop bottom (0x00E534DC) and stores it at the scene end, the loop-exit ~STB and the two expanded exit pads
   (0x00E512F7, 0x00E5354D, 0x00E53580, 0x00E535B0); zero is `xor eax,eax` for the two movie-ctor stores (0x00E51139), `xor
   esi,esi` in the exit block (0x00E5351E) and `push 0` elsewhere. Ours: c2prio --trace gives ebp to const 0's piece #45 (prio 0,
   allowed ebp only) and #28 (-24), the vtable constant (`const 0xfe328c`, 5 refs) is split into pieces of one reference and
   dropped. The exit-pad hunks follow: our exit 2 is cross-jumped into exit 1's tail (retail keeps both, the store being 7 bytes
   from ebp instead of 11), and the two seh_me dtor copies come in the other order. With the body at 5x (plain `for (;; NSF) { if
   (T) return; }` / `break`) the vtable constant has 8 refs and DOES take an ebp piece, but then three movie sites expand that
   retail calls -- so the reference count, not the loop shape, is what moved it.
2. **Frame slot order + a second entry store.** Retail: `xor ebx,ebx ; xor ebp,ebp ; mov [esp+30h],ebx ; push edi ; lea ecx,
   [esp+14h] ; mov [esp+34h],ebp` -- the same slot (+0x34 after the push) is zero-stored twice, from the flag-word register and
   from the zero register; ours stores once (`mov [esp+48h],ebx`; /FA names it $T10695, the conditional-temporary flag word's
   home, sharing its slot with the two CCharString temporaries). Retail's slot sits in front of `speech` (+0x38) and the phase
   movies (+0x3C), ours behind (+0x48); the frame size is equal (0x210). So retail has one more compiler variable set to 0 at
   entry that leaves no other code and shares that slot. A scan of every large-frame body in 0x00CE0000..0x00F30000 finds this
   double store only here (0x00E4E800 is the one other body with two zero registers, storing to different slots).
   Micro probes (work/micro/m1..m9, tools/mc.py / fa.py; not row builds): a new-expression with a conditional temporary gives
   exactly one such store per function (two new-expressions: still one; an inlined callee's conditional temporary takes a bit of
   the SAME flag word); `x(temp) && true` leaves real flag code; `false && f(temp)` leaves nothing; a never-set `long done = 0`
   in the loop condition and an unused zero-initialised struct local are removed completely.
   This variable is the best lead for residue 1 as well: one more zero reference in the entry block changes K there.
Not tried (4 grades left): the phase / attack tests with an own folded operand instead of the loop condition (changes which
arm is hot: attack arm 0.25 would keep the body at 5x, where the vtable constant wins ebp -- needs the phase-0 site handled
separately, e.g. `Me.MsgIsTalkedToBy(...)` nested under a folded test; f16 in work/micro/m4.cpp shows `if (g && true) { if
(U(temp)) ...}` is code-free).

## Rows read with inline traces only (no grade, drafts unchanged)
- **0x00E5FD40** CBodyGuard::Main: `&& true` on the main loop halves every ~CScriptThing site (306 -> 153, the 9.9 group -> 4.9);
  no site crosses its threshold, so the 2-of-8 expanded split is not a loop-factor question.
- **0x00E33EA0** CAC_Owner::Main: movie ctor / dtor are called at all six retail sites (tools/rc.sh) and out of line in the draft:
  the loop lever has nothing to move; the residue is the frame / spawn-ctor one in its line 2.
- **0x00CF68C0** CEntranceGuard::Main: retail expands all four movie ctors and all six dtor sites; the draft's sites run at
  f 0.1..25 (expanded only above ~5). No loop-condition factor (2.5x / 5x / 7.5x) reaches that; it is the "no proper loop ->
  every site expanded" regime of GOTCHAS line 513 or a forced-inline declaration, as its line 2 says.

## Family reading for the next pass
- The loop-condition factor is a third frequency lever beside the for / while choice and the `&&` / `||` branch split:
  `for (; a && b; inc)` body 2.5x, `for (; a || b; inc)` 7.5x, plain 5x -- and the folded operand costs no code. Check it with
  tools/iv.sh before anything else on a row whose movie / STB / CScriptThing sites are all off by one uniform factor.
- "zero register v vtable-constant register" is now the open residue of two rows (0x00E50FD0, 0x00DB97A0). In both retail pushes
  immediates where ours pushes the zero register.
- 0x00D0F640: the wanted header factor is 0.2 (H40d). The available code-free factors are 0.5 (for-`&&`), 0.5 (`||` on a
  return test: fall-through 0.25 instead of 0.5) and 1.5 (a 0.5 branch that becomes a 0.75 fall-through); 0.5 x 0.5 x 0.75 =
  0.1875 is inside H40d's 0.185..0.225 band. Not traced here.

## Header debt
Unchanged from H40d: the screen-builder / CWorldMap overlays of hand729 / 879 / 882 / 883 are not re-based (0x00603000, 0x00502E90,
0x0061E6D0, 0x006096E0); 0x0078FD60 and 0x00507C30 have no source.

## Tools added (tools/)
`bbv.sh` (0x00E50FD0 variant: movie split pattern + the const 0 / vtable-constant allocator rows), `rc.sh` (retail call-or-expand
sites of the movie / STB special members of a script Main, by address), `mc.py` (compile and disassemble a MICRO probe TU, no
oracle), `fa.py` (the same with an /FA listing, which names the frame slots).

## Dead end added after the notes above (micro probe work/micro/m10.cpp)
`bool` / `char` / `int` / enum zero arguments all share one zero register with the int zero stores of the same loop (9 register
pushes, 0 immediates in every variant): the "bool argument constants are a different constant class" idea of 0x00DB97A0's line 3
is wrong, and it does not explain retail's `push 0` in the attack scene of 0x00E50FD0 either. Retail's immediates there are
what is left when the zero piece that crosses the calls loses ebp to the vtable constant.

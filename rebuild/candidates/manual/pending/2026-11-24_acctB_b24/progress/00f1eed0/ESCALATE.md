# 0x00F1EED0 CQ_ArenaScript::PlayWave -- escalation state (B24B2, 2026-11-24)

Residue: ONE raw byte, the SIB of `lea eax,[edi+eax+0x30]` at 0x00F1FFBA (ours `[eax+edi+0x30]`), the HUD statement
`CreatureCounter[i] = Game->AddQuestInfoCounterList(Rounds[ArenaRound].Waves[ArenaRoundWave].Creatures[i].HUDType, ...)`.
Every other byte, incl. all branch displacements (rawcmp.py), equals retail with source differ/00f1eed0.cpp (= b3.cpp, overlay hdr2).

Mechanism (c2 13.10.3077, measured with lk/lkx): the two address operands are ordered by c2's 16-bit key; the creature start
pointer is an inliner-imported symbol L (s2862), the i*0x38 strength-reduction temp is T (t7465, kind 15). Retail order iff
T mod 2048 in 1024..2047 AND 2*(T mod 1024) > (L mod 2048) (now 594 < 814). L = allocation counter at the HUD import
(c1 IL of the whole function ~2213 + a 470-temp per-symbol burst + ~146 inline-phase numbers, recycled numbers reused);
T = (counter after inlining rounded to 32) + 2921. With L += a, T += b the order flips iff 2b - a > ~220 (b <= 726, a >= -814).

What is pinned (so the body text is not the lever): 67 retail T calls v 69 source `if (T)` = 2 c2-hoisted per-arm checks;
exits cost ~1.9 numbers per live destructible object (pe1/pe2), every exit/object is fixed by the code; plain `return;` (no
TRetVoid) 9326; no for-scope macro 11061; macro after the includes 9374; donor gift loop+break raw-clean but L +1 / T 0;
Game pointer/reference, in-row v header bodies: L/T unchanged.

Not landable without evidence: any text added/removed only to move a or b ("never fit text to a record count", lk README).
Open evidence questions that could move a/b with identical code:
 1. the exact CTimer inline texts (FableWin: each of the four bodies is one /Od instruction longer than ours); they also
    decide the two cold-return inline_depth(0) devices (NOTES: device trace);
 2. the ~MovieBase / GROBase header form (B24A family root cause) -- late-site inline costs (b side);
 3. which headers the VC6 for-scope macro wrapped in the original TU (it changes c2 record ids, GOTCHAS hand3c).

Escalation candidates (fitted, NOT landable; recorded for the owner):
- d1b.cpp: b3 + `inline CTimer::~CTimer() { CGameScriptInterfaceBase* game = pDSTGame; game->DeregisterTimer(TimerIndex); }`
  -> retail SIB order, but 53 raw-differing lines (resync compare); first at the countdown exits. No evidence for that body (FableWin /Od 15i; this body 16i) and it makes ~CTimer always-inline, which retail is not.
- e1.cpp: d1 + inline_depth(0) on three 2nd/3rd-countdown returns, cold regions dropped -> retail SIB order, 87 raw lines.

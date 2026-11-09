#!/usr/bin/env python3
"""fmodel.py: analytic model of c2's static frequencies in CTCBoastUI::Activate (rules measured in work/s6/fq: an if gives its
then-path p (0.5 plain, 0.75 `a || b`, 0.25 `a && b`), a `for` body runs at 5x its entry and the code after it stays at the body's
frequency, a `while` body at 9x and the code after it drops back to the entry). Enumerates loop forms and per-check splits and keeps
the assignments that satisfy every call/expand choice retail shows (c1 values from work/inl/s5_l1.json)."""
import itertools
def score(c, f):
    fi = int(round(f * 1e8)); return (c * 100000000 // fi + 6 * c) // 7
def exp(c, f): return score(c, f) < 74 or c < 14
P = {'plain': 0.5, 'or': 0.75, 'and': 0.25}
res = []
for l1, l2, l3 in itertools.product(('for', 'while'), repeat=3):
    for top in (0.25, 0.5, 0.75):
     for act, nz, hs, cd, em, s1, gp in itertools.product(P, repeat=7):
        f = top                        # after `if (IsDisplayingText() || IsInCutscene()) return;`
        m1 = f                         # BoastGroups.clear
        f = f * (1 - P[act])           # if (Active) return  -> fall-through
        body1 = f * (5 if l1 == 'for' else 9)
        m2 = body1 * 0.5               # loop-1 boasts list (inside `if HasScript`)
        f = body1 if l1 == 'for' else f
        f = f * (1 - P[nz])            # if (n == 0) return
        E = f
        B = E * (5 if l2 == 'for' else 9)
        F = B * (1 - P[hs]) * (1 - P[cd])
        m3 = F * P[em]; m4 = F * (1 - P[em])
        m5 = m4 * (5 if l3 == 'for' else 9)
        m6 = m5 * P[s1] * P[gp]
        m7 = m5 if l3 == 'for' else m4
        m8 = B if l2 == 'for' else E
        ok = [exp(48, m1), exp(19, m2), exp(21, F), exp(19, m3), not exp(18, m4), not exp(48, m4),
              exp(39, m5), not exp(47, m5), not exp(59, m5), exp(18, m5),
              not exp(17, m6), not exp(18, m6), not exp(22, m6), not exp(47, m5 * P[s1]),
              exp(19, m7), not exp(47, m7), not exp(60, m7), exp(42, m8)]
        nonplain = sum(1 for x in (act, nz, hs, cd, em, s1, gp) if x != 'plain') + (l1 != 'while') + (l2 != 'for') + (l3 != 'for')
        res.append((sum(ok), -nonplain, (str(top), l1, l2, l3, act, nz, hs, cd, em, s1, gp), (m1, m2, F, m3, m4, m5, m6, m7, m8), ok))
res.sort(key=lambda r: (-r[0], -r[1]))
print('checks:', 18)
for r in res[:25]:
    print(r[0], -r[1], ' '.join(r[2]), ' '.join(f'{x:.4f}' for x in r[3]), ''.join('.' if o else 'X' for o in r[4]))

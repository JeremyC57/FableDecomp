#!/usr/bin/env python3
"""t3r2.py <accepted.txt> <held_with_source.tsv>: per-address round-2 status of every row round 2 touched (T3 lines:
source<TAB>address<TAB>unit<TAB>reason) -> r2/handback/T3_r2.txt."""
import sys
from collections import Counter
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
acc = set(open(sys.argv[1]).read().split())
held = {l.split("\t")[0]: l.rstrip("\n").split("\t", 1)[1] for l in open(sys.argv[2]) if "\t" in l}
diag = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(A + "r2/work/diag.tsv")}
gt = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(A + "r2/work/gen_thin.tsv")}
s0 = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(A + "r2/work/s0cls.tsv")}
t3 = [l.rstrip("\n").split("\t") for l in open(A + "T3.txt") if l.startswith("nf424\t")]
holdc = set(l.split("\t")[0] for l in open(A + "work/hold_c.tsv"))
holdc_why = {"008c9670": "Clone probe against the shared header DIFFER(85v71): the header's copy differs from retail's inline copy",
             "008c96d0": "Clone probe against the shared header DIFFER(85v71): the header's copy differs from retail's inline copy",
             "008cecc0": "Clone probe against the shared header DIFFER(83v59): the header's copy differs from retail's inline copy",
             "0097d130": "Clone probe against the shared header DIFFER(83v59): the header's copy differs from retail's inline copy",
             "008ca960": "no Clone for CCreatureAction_NymphTeleportLimboBase (header declares none, no landed row): cannot be settled by the Clone test"}
out = []; c = Counter()
for q in t3:
    a = q[1][2:].lower(); shape = q[2]
    if a in acc: st, why = "ACCEPT", "handed back in round 2 (see RESULTS)"
    elif a in held: st, why = "HOLD", held[a]
    elif a in diag:
        d = diag[a]; g = gt[a]
        if d[3] == "NOLEAF":
            st, why = "r2 ctor-unit: no leaf", "ctor-owned unit tried (owner 0x%s, %s): the owner TU emits no %s (its model gives the class no virtual destructor / vtable, or another spelling: %s); needs a polymorphic model with retail's slots first" % (g[2].upper(), g[3], g[4], d[5][:120] or "no ??_G at all")
        else:
            st, why = "r2 ctor-unit: body differs", "ctor-owned unit tried (owner 0x%s, %s): the owner TU emits %s but %s against retail: the model's destructor chain differs (member / base destructor expansion)" % (g[2].upper(), g[3], g[4], d[2])
        if a in s0 and "CActiveEntityScript" in s0[a][5]: st, why = "HOLD R-NF424-2", "CActiveEntityScript<T>: name proven (V+P+C); ~CActiveEntityScriptBase not expanded, RULINGS_NEEDED R-NF424-2"
    elif shape.startswith("S0"):
        if a in s0 and s0[a][5] == "-": st, why = "S0 no name", "vftable slot (UAE form) in an RTTI-less vftable of the libpk / CODA or DirectShow region: no PDB, no RTTI, no landed non-placeholder name (evidence/s0_vt.tsv)"
        elif a not in s0: st, why = "S0 not in a vftable", "not a vftable slot (evidence/s0_nonvt.tsv): unreferenced, a QAE `push 0; call` target (R-NF424-3) or a thunk target"
        else: st, why = "S0", "named but not compiled"
    elif shape.startswith("G1 hold (batches/g1_r1") and a not in holdc:
        st, why = "landed", "R-NF424-1 (a): landed at FullDecomp e6af6da0e9"
    elif a in holdc:
        st, why = "HOLD R-NF424-1 (b)", holdc_why.get(a, "copy-ctor COMDAT not settled")
    else:
        st, why = "unchanged", q[3]
    c[st] += 1
    out.append("\t".join(["nf424", q[1], shape + " | " + st, why]))
hdr = ["T3 lines (NF424 round 2, 2026-11-25): status after round 2 of every address in pending/2026-11-25_nf424_handback/T3.txt.",
       "Columns: source<TAB>address<TAB>shape | round-2 status<TAB>reason. Counts: " + ", ".join("%s %d" % kv for kv in c.most_common()), ""]
open(A + "r2/handback/T3_r2.txt", "w").write("\n".join(hdr + out) + "\n")
print(c)

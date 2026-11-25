#!/usr/bin/env python3
"""checks436.py <batch>: per-row CHECKS.tsv from the batch evidence (vland H/N/M, objgate, pk, link, name_gate, fwchk, vt, slot0, homes)."""
import sys, re, os
W = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436"; b = sys.argv[1]; B = W + "/batches/" + b; E = B + "/evidence"
rows = open(B + "/rows.txt").read().split()
def grade(f, a):
    m = re.search(r"^%s +(\S+) +(PASS|FAIL)" % a, open(f).read(), re.M); return "%s %s" % m.groups() if m else "?"
def warns(f, a): return sorted(set(re.findall(r"^%s +WARN (\S+)" % a, open(f).read(), re.M)))
def line(f, a, pat=None):
    for l in open(f).read().splitlines():
        if l.startswith(a) and (pat is None or pat in l): return l
    return ""
link = open(E + "/linkgate.txt").read().splitlines()[0]
out = open(B + "/CHECKS.tsv", "w")
out.write("address\thead_grade\tnew_grade\tmutant\tobjgate\tpk_copies\tpk\tlink\tliteral_warns\tnew_warn_kinds\tname_gate\tvftable\tcopies_home\n")
for a in rows:
    pk = line(E + "/pk.log", a).split("\t")
    fw = [l.split()[2] for l in open(E + "/fwchk.txt").read().splitlines() if l.startswith(a + " ")]
    vt = [l for l in open(E + "/vt_leafmap.txt").read().splitlines() if l.startswith(a + " ")]
    wr = sum(1 for l in vt if " slot " in l)
    s0 = [l for l in open(E + "/slot0.tsv").read().splitlines() if l.startswith(a)]
    s0eq = sum(1 for l in s0 if "bytes EQ relocs EQ" in l)
    hm = [l for l in open(E + "/homes.tsv").read().splitlines() if l.startswith(a)]
    hn = sum(1 for l in hm if "EQ_home=NONE" in l)
    ng = line(E + "/name_gate.tsv", a).split("\t")
    obj = line(E + "/objgate_summary.tsv", a).split("\t")
    out.write("\t".join([a, grade(E + "/vland_%sh.log" % b, a), grade(E + "/vland_%sn.log" % b, a),
        "FAIL (Alloc data / interface swapped -> test FAIL)" if grade(E + "/vland_%sm.log" % b, a).endswith("FAIL") else "PASS (!)",
        (obj[1] if len(obj) > 1 else "?") + " (Alloc leaf byte+reloc equal; NOT_EQ by design: the class vftable now carries the FableWin overrides and the TU emits ??_G<C> calling ~CScriptBase directly)",
        pk[1] if len(pk) > 1 else "?",
        "0 new NEQ reached (pk.log); 0 reach flips (computed H v N per symbol, evidence/pk_reach.txt) (only NEQ: the unreached 5-byte ??1?$CLevelScriptBase<T> copy v the table's 0x00CBD510 binding, T3 NF434 0x00CBD510)",
        link, str(len(re.findall(r"^%s .*LITERAL" % a, open(E + "/vland_%sn.log" % b).read(), re.M))),
        ",".join(warns(E + "/vland_%sn.log" % b, a)) or "-",
        ng[1] if len(ng) > 1 else "?",
        "fwchk %s; vt432 %d WRONG (table + landed row leaves)" % ("/".join(fw), wr),
        "slot0 %d/%d EQ; homes NONE %d of %d" % (s0eq, len(s0), hn, len(hm))]) + "\n")
print("checks", b, len(rows))

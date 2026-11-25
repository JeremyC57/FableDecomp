#!/usr/bin/env python3
"""mkbatch2.py <batch> <addrfile> <ACCEPT|HOLD> [reasons.tsv]: assemble r2/handback/<batch>/ for the lander (NF424 round 2):
rows/<addr>.cpp + tests/<addr>_test.cpp (r2/final, else r2/rows), orig/rows + orig/tests (HEAD copies), head/payload.json (HEAD payload
subset of the round-2 vland HEAD runs), RESULTS.tsv (col 4 = the row's own ??_G self symbol), CHECKS.tsv, UNITS.tsv, TABLE_CHANGES.tsv,
SELF_SYMBOLS.tsv (status from the vland NEW run), CATALOG_CHANGES.tsv (address field old new reason), logs/vland_lines.txt."""
import os, sys, re, json, shutil, glob
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
R = "/home/user/FableDecomp"
sys.path.insert(0, A + "tools"); import nf
b, lst, verdict = sys.argv[1], sys.argv[2], sys.argv[3]
reasons = {}
if len(sys.argv) > 4:
    for l in open(sys.argv[4]):
        p = l.rstrip("\n").split("\t", 1)
        if len(p) == 2: reasons[p[0]] = p[1]
addrs = sorted(open(lst).read().split())
B = A + "r2/handback/" + b + "/"
for d in ("rows", "tests", "orig/rows", "orig/tests", "head", "logs"): os.makedirs(B + d, exist_ok=True)
H = [l.split("\t") for l in open(A + "work/hist.tsv").read().splitlines()]; hist = {r[0]: dict(zip(H[0], r)) for r in H[1:]}
def tsv(p, skip=0):
    d = {}
    if os.path.exists(p):
        for l in open(p).read().splitlines()[skip:]:
            q = l.split("\t"); d[q[0]] = q
    return d
gate = tsv(A + "r2/work/gate.tsv", 1); cal = tsv(A + "r2/work/calleechk.tsv"); cen = tsv(A + "r2/work/census_thin.tsv"); s0 = tsv(A + "r2/work/s0cls.tsv")
selfs = {}
for l in open(R + "/rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    q = l.split("\t"); selfs[q[0]] = "|".join(q[1:])
vl = {}
for p in glob.glob(A + "r2/work/vland_rp*.log"):
    side = p[-5]
    for l in open(p):
        m = re.match(r"\s*([0-9a-f]{8})\s", l)
        if m: vl.setdefault((m.group(1), side), []).append(l.rstrip("\n"))
def grade(a, side):
    for l in vl.get((a, side), []):
        m = re.match(r"\s*[0-9a-f]{8}\s+(RELOCATION_MATCH\S*|MATCH\S*|DIFFER\S*|SRC_FAIL|TCC_FAIL)\s+(PASS|FAIL|LINK_FAIL|TCC_FAIL)", l)
        if m: return m.group(1), m.group(2)
    return "-", "-"
def warns(a, side): return set(re.sub(r"\?A0x[0-9a-f]{8}", "?A0x", l.split("WARN ", 1)[1].split(" (")[0])[:160] for l in vl.get((a, side), []) if " WARN " in l)
pays = []
for p in glob.glob(A + "r2/work/p_rp*h/payload.json"): pays += json.load(open(p))["result"]["authored"]
pay = [x for x in pays if x["address"] in addrs]
json.dump({"result": {"authored": pay}}, open(B + "head/payload.json", "w"), indent=1)
open(B + "head/HEAD_PAYLOAD_NOTE.txt", "w").write("HEAD payload of the round-2 vland HEAD runs (repo recheck.py); oracle.tsv (retail bytes) stays in work/, never in a hand-back.\n")
def pksum(a):
    p = A + "evidence/pk/R2N_%s.pkall.tsv" % a
    if not os.path.exists(p): return "-"
    from collections import Counter
    c = Counter(l.split("\t")[0] for l in open(p) if l.strip() and not l.startswith("#"))
    return " ".join("%s=%d" % kv for kv in sorted(c.items())) or "none"
SCAF9 = {"008cd280", "008cee50", "008f5700", "00925a50", "0062d7d0", "0062d870", "0084ab80", "0084b4a0", "0087fe80"}
res = ["address\tverdict\told\tnew\treason"]
chk = ["address\thead_grade\thead_test\tnew_grade\tnew_test\twarn_removed\twarn_added\tidentity\towner_root\troot_sha1\tflags\tcensus_definers\tpk_new_object\tcallee_binding"]
units = ["address\tunit\tkind"]; ss = ["address\tsymbol\tstatus\tleaf_given\tcurrent_line"]; cat = ["address\tfield\told\tnew\treason"]
logs = []
for a in addrs:
    h = hist[a]
    src = A + "r2/final/%s.cpp" % a
    if not os.path.exists(src): src = A + "r2/rows/%s.cpp" % a
    tst = src[:-4] + "_test.cpp"
    text = open(src).read()
    leaf = re.search(r"^// Leaf:\s*(\S+)", text, re.M).group(1)
    shutil.copy(src, B + "rows/%s.cpp" % a); shutil.copy(tst, B + "tests/%s_test.cpp" % a)
    hs, ht = nf.src_of(a), nf.test_of(a)
    shutil.copy(hs, B + "orig/rows/%s.cpp" % a)
    if ht and ht.exists(): shutil.copy(ht, B + "orig/tests/%s_test.cpp" % a)
    hg, htp = grade(a, "h"); ng, ntp = grade(a, "n")
    wr = sorted(warns(a, "h") - warns(a, "n")); wa = sorted(warns(a, "n") - warns(a, "h"))
    g = gate.get(a); c = cen.get(a)
    if a in SCAF9:
        ident = "round-1 G1 (RTTI + PDB name set)"; ownr = "-"; sha = "-"; flg = "SAME"; cd = "see README (R-NF424-1)"
    else:
        ident = g[3]; ownr = "%s (%s)" % (g[5], g[6]); sha = g[7]; flg = g[10]; cd = "%s %s %s" % (c[2], c[3], c[4]) if c else "-"
    chk.append("\t".join([a, hg, htp, ng, ntp, ";".join(wr) or "-", ";".join(wa) or "-", ident, ownr, sha, flg, cd, pksum(a), (cal[a][1] + " " + cal[a][2] if a in cal else "-")]))
    old = (selfs[a].split("|")[0] if a in selfs else "") or h["self_leaf"]
    if verdict == "ACCEPT":
        if a in SCAF9:
            why = reasons.get(a, "")
        else:
            why = ("NF424 r2 ctor-owned thin row, nf159 TU-emitter fold / nf251 pattern (%s): %s is emitted as a pick-any COMDAT by the landed owner TU 0x%s %s (it stores vftable "
                   "0x%s), included with no code of its own. Identity: %s. Gates: vland HEAD %s/%s -> new %s/%s; census of every other storer of "
                   "the vftable + round-1 census: %s; pk on the new object: %s; owner not locked / claimed / queued; every leaf relocation bound at retail's target (calleechk); flags %s.") % (
                   g[1], leaf, g[5].upper(), g[6], (h["retail_vt"].split("/")[0] if g[1] != "S0" else s0[a][2]).upper(), g[3][3:],
                   hg, htp, ng, ntp, c[2] if c else "-", pksum(a), flg)
    else:
        why = reasons.get(a, "held")
    res.append("\t".join([a, verdict, old, leaf, why]))
    if a in SCAF9: units.append("%s\t%s\t-" % (a, a))
    else: units.append("%s\t%s\tfold" % (a, g[5]))
    if verdict == "ACCEPT":
        ss.append("\t".join([a, leaf, ng, leaf, selfs.get(a, "-")]))
        catn = (nf.catalog().get(a) or {}).get("name", "")
        if "`scalar_deleting_destructor'" in catn:
            cat.append("\t".join([a, "name", catn, catn.replace("`scalar_deleting_destructor'", "`scalar deleting destructor'"),
                                  "standin_leaf.py decodes `scalar deleting destructor' (spaces) as ??_G; the underscore label makes vland print WARN STANDIN_LEAF on the real ??_G leaf"]))
        mod = nf.field(a, "Module") or ""
        if mod.startswith("CVtable_") or mod.startswith("CLandscapeBackgroundPatch") or mod.startswith("CThingSwitch"):
            nm = catn.rsplit("::`", 1)[0]
            cat.append("\t".join([a, "Module", mod, nm, "the row's class (S0 identity); the module was the vftable placeholder"]))
        if flg.startswith("CHANGE"):
            ofl = flg.split("owner=")[1]
            cat.append("\t".join([a, "CompilerFlags", nf.field(a, "CompilerFlags") or "(none)", "/O2 /Oy /W3 " + ofl,
                                  "thin row: the object is the owner TU, compiled with the owner's flags (the row also carries `// Flags: %s`)" % ofl]))
    for side in ("h", "n"): logs += ["[%s] %s" % (side.upper(), l) for l in vl.get((a, side), [])]
open(B + "RESULTS.tsv", "w").write("\n".join(res) + "\n")
open(B + "CHECKS.tsv", "w").write("\n".join(chk) + "\n")
open(B + "UNITS.tsv", "w").write("\n".join(units) + "\n")
open(B + "TABLE_CHANGES.tsv", "w").write("action\tsymbol\ttarget\twitness\tnote\n")
open(B + "SELF_SYMBOLS.tsv", "w").write("\n".join(ss) + "\n")
open(B + "CATALOG_CHANGES.tsv", "w").write("\n".join(cat) + "\n")
open(B + "rows.txt", "w").write("\n".join(addrs) + "\n")
open(B + "logs/vland_lines.txt", "w").write("\n".join(logs) + "\n")
print(b, verdict, len(addrs))

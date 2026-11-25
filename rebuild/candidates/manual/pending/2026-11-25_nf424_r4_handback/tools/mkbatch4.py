#!/usr/bin/env python3
"""mkbatch4.py <batch> <addrfile> <ACCEPT|HOLD> [reasons.tsv]: NF424 round-4 hand-back batch r4/handback/<batch>/ (rows/, tests/, orig/,
head/payload.json, RESULTS.tsv (col 4 = the row's own self symbol), SELF_SYMBOLS.tsv (with current_line), CATALOG_CHANGES.tsv
(address field old new reason), CHECKS.tsv, UNITS.tsv, TABLE_CHANGES.tsv, logs/). Rows: thin ??_G rows (r4/final/<a>.cpp) and owner edits
(r4/final/<owner>.cpp, an existing row: tests kept at HEAD)."""
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
B = A + "r4/handback/" + b + "/"
for d in ("rows", "tests", "orig/rows", "orig/tests", "head", "logs"): os.makedirs(B + d, exist_ok=True)
H = [l.split("\t") for l in open(A + "work/hist.tsv").read().splitlines()]; hist = {r[0]: dict(zip(H[0], r)) for r in H[1:]}
def tsv(p):
    d = {}
    if os.path.exists(p):
        for l in open(p).read().splitlines():
            q = l.split("\t"); d[q[0]] = q
    return d
aes = {}; ownerof = {}
for l in open(A + "r3/work/aes_cand.tsv"):
    a, o, s = l.rstrip("\n").split("\t"); aes[a] = (o, s); ownerof[o] = a
gate = tsv(A + "r4/work/aes_gate4.tsv"); s0 = tsv(A + "r2/work/s0cls.tsv"); g48 = tsv(A + "r3/work/gate48.tsv")
selfs = {}
for l in open(R + "/rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    q = l.split("\t"); selfs[q[0]] = "|".join(q[1:])
vl = {}
for p in glob.glob(A + "r4/work/vland_ab*.log"):
    side = p[-5]
    for l in open(p):
        m = re.match(r"\s*([0-9a-f]{8})\s", l)
        if m: vl.setdefault((m.group(1), side), []).append(l.rstrip("\n"))
def grade(a, side):
    for l in vl.get((a, side), []):
        m = re.match(r"\s*[0-9a-f]{8}\s+(RELOCATION_MATCH\S*|MATCH\S*|DIFFER\S*|SRC_FAIL|TCC_FAIL)\s+(PASS|FAIL|LINK_FAIL|TCC_FAIL)\b", l)
        if m: return m.group(1), m.group(2)
    return "-", "-"
def warns(a, side): return set(l.split("WARN ", 1)[1][:160] for l in vl.get((a, side), []) if " WARN " in l)
pays = []
for p in glob.glob(A + "r4/work/p_ab*h/payload.json"): pays += json.load(open(p))["result"]["authored"]
json.dump({"result": {"authored": [x for x in pays if x["address"] in addrs]}}, open(B + "head/payload.json", "w"), indent=1)
open(B + "head/HEAD_PAYLOAD_NOTE.txt", "w").write("HEAD payload of the round-4 vland HEAD runs (repo recheck.py); oracle.tsv stays in work/.\n")
def pksum(a):
    p = A + "evidence/pk/R4N_%s.pkall.tsv" % (ownerof.get(a, a))
    if not os.path.exists(p): return "-"
    from collections import Counter
    c = Counter(l.split("\t")[0] for l in open(p) if l.strip() and not l.startswith("#"))
    return " ".join("%s=%d" % kv for kv in sorted(c.items())) or "none"
FL = "/O2 /Oy /W3 /d2inlS20"
res = ["address\tverdict\told\tnew\treason"]
chk = ["address\trole\thead_grade\thead_test\tnew_grade\tnew_test\twarn_removed\twarn_added\tgates\tpk_new_object"]
units = ["address\tunit\tkind"]; ss = ["address\tsymbol\tstatus\tleaf_given\tcurrent_line"]; cat = ["address\tfield\told\tnew\treason"]
logs = []
for a in addrs:
    src = A + "r4/final/%s.cpp" % a
    text = open(src).read()
    leaf = re.search(r"^// Leaf:\s*(\S+)", text, re.M).group(1)
    shutil.copy(src, B + "rows/%s.cpp" % a)
    tst = src[:-4] + "_test.cpp"
    if os.path.exists(tst): shutil.copy(tst, B + "tests/%s_test.cpp" % a)
    hs, ht = nf.src_of(a), nf.test_of(a)
    shutil.copy(hs, B + "orig/rows/%s.cpp" % a)
    if ht and ht.exists(): shutil.copy(ht, B + "orig/tests/%s_test.cpp" % a)
    hg, htp = grade(a, "h"); ng, ntp = grade(a, "n")
    wr = sorted(warns(a, "h") - warns(a, "n")); wa = sorted(warns(a, "n") - warns(a, "h"))
    cur = selfs.get(a, "-")
    old = cur.split("|")[0] if a in selfs and cur.split("|")[0] else (hist[a]["self_leaf"] if a in hist else "-")
    if a in ownerof and old == "-": old = leaf   # owner: symbol unchanged (no self line at HEAD)
    if a in ownerof:
        role = "owner edit"; t = ownerof[a]
        gates = "the owner TU of thin row 0x%s; object diff HEAD v edited: %s" % (t.upper(), " ".join(gate[t][5:8]))
        why = reasons.get(a) or ("NF424 r4 owner edit (R-NF424-5 ruling, 2026-11-08 owner rule: inline-budget residue): `// Flags: /d2inlS20` + `// Inline-budget:` marker + FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY (existing opt-in), "
               "so the TU's pick-any ??_G?$CActiveEntityScript<T> (thin row 0x%s) expands ~CActiveEntityScriptBase as retail. Own leaf unchanged: vland HEAD %s/%s -> new %s/%s; "
               "object diff HEAD v edited: only the unreferenced NOADDR COMDATs ??1CSpawnedFuncBase / ??1CCountedPointer<const CPredicateBase> leave, "
               "??1CActiveEntityScriptBase (unbound, NOADDR) and the thin row's leaf change; the leaf %s itself is byte-identical. No other row includes this source." % (t.upper(), hg, htp, ng, ntp, leaf))
        units.append("%s\t%s\towner" % (a, a))
    else:
        role = "thin row"
        o = aes[a][0] if a in aes else (g48[a][2] if a in g48 else "?")
        if a in aes:
            g = gate[a]
            gates = "identity V+P+C (%s); stores=1 (owner only); callee %s" % (s0[a][6], g[4])
        else:
            g = g48.get(a, ["-"] * 6)
            gates = " ".join(g[3:])
        why = reasons.get(a) or ("NF424 r4 ctor-owned thin row (nf159 TU-emitter fold, nf251 pattern): %s is the pick-any COMDAT of the owner TU 0x%s, included with no code of its own. "
               "Gates: %s; vland HEAD %s/%s -> new %s/%s (strict leaf); pk %s." % (leaf, o.upper(), gates, hg, htp, ng, ntp, pksum(a)))
        units.append("%s\t%s\tfold" % (a, o))
    chk.append("\t".join([a, role, hg, htp, ng, ntp, ";".join(wr) or "-", ";".join(wa) or "-", gates, pksum(a)]))
    res.append("\t".join([a, verdict, old, leaf, why]))
    if verdict == "ACCEPT":
        ss.append("\t".join([a, leaf, ng.split("[")[0], leaf, cur]))
        catn = (nf.catalog().get(a) or {}).get("name", "")
        if "`scalar_deleting_destructor'" in catn:
            cat.append("\t".join([a, "name", catn, catn.replace("`scalar_deleting_destructor'", "`scalar deleting destructor'"), "standin_leaf.py decodes the spaced form as ??_G"]))
        mcls = re.match(r"\?\?_G\?\$CActiveEntityScript@V(\w+)@(\w+)@@@@", leaf)
        if catn.startswith("CVtable_") and mcls:
            cls = "CActiveEntityScript<%s::%s>" % (mcls.group(2), mcls.group(1))
            cat.append("\t".join([a, "name", catn, cls + "::`scalar deleting destructor'", "the row's class (S0 identity V+P+C, leaf %s); the name was the vftable placeholder (vland STANDIN_LEAF)" % leaf]))
            cat.append("\t".join([a, "Module", nf.field(a, "Module") or "", cls, "the row's class; the module was the vftable placeholder"]))
        mod = nf.field(a, "Module") or ""
        if catn.startswith("CVtable_") and mcls: pass
        elif mod.startswith("CVtable_") or mod.startswith("CLandscapeBackgroundPatch") or mod.startswith("CThingSwitch"):
            cat.append("\t".join([a, "Module", mod, catn.rsplit("::`", 1)[0], "the row's class (S0 identity); the module was the vftable placeholder"]))
        if "// Flags: /d2inlS20" in text:
            cat.append("\t".join([a, "CompilerFlags", nf.field(a, "CompilerFlags") or "(none)", FL,
                                  "R-NF424-5 inline control (2026-11-08 owner rule): /d2inlS20 (smallest N: 17-19 fail; no /Og needed) expands the implicit ~CActiveEntityScriptBase in the pair's ??_G as retail; the row carries `// Flags: /d2inlS20`; old (none) on both rows of the pair, nothing replaced"]))
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

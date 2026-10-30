#!/usr/bin/env python3
"""gen_rows7.py: write $S/mig8/ROWS.tsv (every edited row: addr, file, families, edit notes, mcheck grade before->after,
regrade parity/behaviour) and REVERTED.tsv (deduplicated work/rev.tsv, rows that are not edited at the end)."""
import os, re, glob, subprocess, collections
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]; W = S + "/mig8/work"; L = S + "/mig8/logs"
FAM = ["LSB", "GI", "SBF", "LSBC", "SB48", "ITC", "ST", "ESB"]
DESC = {"LSB": "non-template CLevelScriptBase -> CLevelScriptBase<T> (lsb_nt.py)", "GI": "CGameScriptInterface* Game -> CGameScriptInterfaceBase* (gi.py)",
        "SBF": "full-member CScriptBase + local CBaseClassNonCopyable -> headers (sb_full.py)", "LSBC": "template CLevelScriptBase inline ctor -> header (sb_full.py)",
        "SB48": "0x48 / methods-only CScriptBase stand-in -> headers (sb48.py)", "ITC": "CPredicateBase / CIsThingAlive / CIsThingConscious -> headers (mig_itc.py)",
        "ST": "local CScriptThing -> engine/CScriptThing.h (mig_st.py)", "ESB": "local CEntityScriptBase -> engine/CEntityScriptBase.h (mig_esb.py)"}
def addr(f): return re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1)
edited = {addr(f): f for f in subprocess.run(["git", "-C", R, "diff", "--name-only"], capture_output=True, text=True).stdout.split() if f.startswith("rebuild/src/compiled/")}
fams = collections.defaultdict(list); notes = collections.defaultdict(list)
for fm in FAM:
    p = W + "/fam_%s.txt" % fm
    if not os.path.exists(p): continue
    for l in open(p):
        if l.strip(): a = addr(l.strip()); fams[a].append(fm) if fm not in fams[a] else None
for lg in glob.glob(L + "/apply_*.txt") + [L + "/lsb_apply.txt", L + "/gi_apply.txt", L + "/sbfull_apply.txt", L + "/sbfull_apply2.txt", L + "/lsbctor_apply.txt"]:
    if not os.path.exists(lg): continue
    fm = os.path.basename(lg).replace("apply_", "").replace(".txt", "")
    for l in open(lg):
        if l.startswith("edit "):
            q = l.split(None, 2); a = addr(q[1]); notes[a].append(q[2].strip() if len(q) > 2 else "")
mc = {}
for l in open(W + "/mstat_all.tsv"):
    q = l.rstrip("\n").split("\t"); mc[q[0]] = (q[1], q[2] if len(q) > 2 else "")
rg = {}
for lg in sorted(glob.glob(L + "/recheck*.log"), key=lambda p: int(re.search(r"(\d+)", os.path.basename(p)).group(1))):
    for l in open(lg):
        m = re.match(r"^([0-9a-f]{8})\s+(\S+)\s+(\S+)\s", l)
        if m and m.group(2) != "WARN": rg[m.group(1)] = m.group(2) + "+" + m.group(3)
out = ["addr\tfile\tfamilies\tchange\tmcheck (HEAD grade -> edited grade)\tregrade"]
for a, f in sorted(edited.items()):
    st, d = mc.get(a, ("?", ""))
    g = re.search(r"(MATCH|RELOCATION_MATCH)->(MATCH|RELOCATION_MATCH)", d)
    gr = g.group(0) if g else st + (" " + d[:120] if d else "")
    fl = fams.get(a, ["?"])
    out.append("\t".join([a, f, ",".join(fl), "; ".join(DESC.get(x, x) for x in fl) + " | " + " / ".join(dict.fromkeys(notes.get(a, [])))[:300], gr, rg.get(a, "NOT_REGRADED")]))
open(S + "/mig8/ROWS.tsv", "w").write("\n".join(out) + "\n")
rv = {}
for l in open(W + "/rev.tsv"):
    q = l.rstrip("\n").split("\t")
    if len(q) >= 3 and q[1] not in fams.get(q[0], []): rv[(q[0], q[1])] = q[2] + ("" if q[0] not in edited else "  [row keeps its other family edit: " + ",".join(fams.get(q[0], [])) + "]")
with open(S + "/mig8/REVERTED.tsv", "w") as o:
    o.write("addr\tfamily\treason\n")
    for (a, fm), why in sorted(rv.items()): o.write(f"{a}\t{fm}\t{why}\n")
c = collections.Counter(x.split("\t")[5] for x in out[1:])
print(len(out) - 1, "rows", dict(c)); print(len(rv), "revert entries")

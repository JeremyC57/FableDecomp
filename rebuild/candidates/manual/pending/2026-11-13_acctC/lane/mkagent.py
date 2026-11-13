#!/usr/bin/env python3
"""mkagent.py <agent-name> <addr8> [<addr8> ...]   (acctC lane coordinator; run from the repo root with the lane env sourced)

Builds $S/w22/<agent-name>/ for an author agent: per row ref/<addr>/ (draft.cpp from RESERVED.tsv's closest_version with the
notes of its folder, the acctB dossier / outline / stale header overlay when present, retail_dis.txt cut at the scanned extent),
rows.tsv, the grader oracle $S/batch/<agent-name>_oracle.tsv (scratch only, never committed) and BRIEF.md from BRIEF_TEMPLATE.md.
Lengths come from lane_tools/bounds/extent.py: the brief tells the agent to confirm them. `--hdr <dir>` seeds hdr/ from an earlier
agent's overlay; `--len <addr>=<n>` overrides a length."""
import csv, os, shutil, subprocess, sys
ROOT = "/home/user/FableDecomp"; S = os.environ["S"]
sys.path.insert(0, ROOT + "/tools/decomp_pipeline/linux_lane/lane_tools/bounds")
import extent
HERE = os.path.dirname(os.path.abspath(__file__)); PEND = os.path.dirname(os.path.dirname(HERE))
a = sys.argv[1:]; hdr = None; lens = {}
while "--hdr" in a: i = a.index("--hdr"); hdr = a[i + 1]; del a[i:i + 2]
while "--len" in a:
    i = a.index("--len"); k, v = a[i + 1].split("="); lens[k.lower()] = int(v); del a[i:i + 2]
name, addrs = a[0], [x.lower().replace("0x", "") for x in a[1:]]
res = {r["address"]: r for r in csv.DictReader(open(PEND + "/2026-11-13_acctC/RESERVED.tsv"), delimiter="\t")}
A = f"{S}/w22/{name}"
for d in ("src", "test", "differ", "tmp", "probe", "ref"): os.makedirs(f"{A}/{d}", exist_ok=True)
if hdr and not os.path.isdir(f"{A}/hdr"): shutil.copytree(hdr, f"{A}/hdr")
os.makedirs(f"{A}/hdr", exist_ok=True)
E_ = extent.Exe(); rows = []; lines = []
for n, ad in enumerate(addrs, 1):
    r = res[ad]; R = f"{A}/ref/{ad}"; os.makedirs(R, exist_ok=True)
    ln = lens.get(ad) or E_.scan(int(ad, 16))["extent"]
    mod, nm = r["catalog_module"], r["catalog_name"]
    if nm.startswith(mod + "::"): nm = nm[len(mod) + 2:]
    rows.append((ad, mod, nm, ln)); have = []
    cv = r["closest_version"]
    if cv and os.path.isfile(f"{ROOT}/{cv}"):
        shutil.copy(f"{ROOT}/{cv}", f"{R}/draft.cpp"); have.append("draft.cpp (best earlier source; grade / residue in its first lines)")
        fold = f"{ROOT}/" + "/".join(cv.split("/")[:5])
        t = cv[:-4] + ".test.cpp"
        if os.path.isfile(f"{ROOT}/{t}"): shutil.copy(f"{ROOT}/{t}", f"{R}/draft.test.cpp"); have.append("draft.test.cpp")
        if "acctB" not in fold:
            os.makedirs(f"{R}/draft_folder", exist_ok=True)
            for f in os.listdir(fold):
                if f.endswith((".md", ".tsv")) and os.path.getsize(f"{fold}/{f}") < 400000: shutil.copy(f"{fold}/{f}", f"{R}/draft_folder/{f}")
            have.append("draft_folder/ (NOTES / LESSONS / STATUS of the chunk that wrote the draft: grep them for this address)")
            if os.path.isdir(f"{fold}/hdr") and not os.path.isdir(f"{R}/hdr_STALE"):
                shutil.copytree(f"{fold}/hdr", f"{R}/hdr_STALE"); have.append("hdr_STALE/")
    for sub, dst, what in (("notes", "notes", "notes/ (NOTES, DOSSIER, OUTLINE, HINTS, ATTEMPTS of the acctB lane)"), ("hdr", "hdr_STALE", "hdr_STALE/")):
        p = f"{PEND}/2026-11-13_acctB_parked/{sub}/{ad}"
        if os.path.isdir(p) and not os.path.isdir(f"{R}/{dst}"): shutil.copytree(p, f"{R}/{dst}"); have.append(what)
    out = subprocess.run(["python3", f"{S}/tools/xdis.py", ad, hex(ln)], capture_output=True, text=True).stdout
    open(f"{R}/retail_dis.txt", "w").write(out); last = out.strip().split("\n")[-1][:60]
    lines.append(f"{n}. 0x{ad.upper()} {mod}::{nm} ({ln:,} bytes by extent scan; last instruction `{' '.join(last.split()[2:])}`; {r['kind']}"
                 + (f"; {r['note'][:150]}" if r["note"] else "") + f")\n   ref/{ad}/: retail_dis.txt" + "".join(", " + h for h in have))
with open(f"{A}/rows.tsv", "w") as f:
    f.write("address\tmodule\tname\tlength\n"); [f.write("\t".join(map(str, x)) + "\n") for x in rows]
subprocess.run(["python3", ROOT + "/tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/oracle_from_exe.py", f"{A}/rows.tsv", name], stdout=subprocess.DEVNULL)
block = ("## Your rows, in this order (ONE AT A TIME; do not open the next until the current one grades MATCH / RELOCATION_MATCH)\n"
         + "\n".join(lines) + """

Catalog names above are labels, not proof: prove the row's real name (two sources) before writing line 1. Lengths come from a
recursive-descent extent scan: confirm the first and last instruction (a row may own cold code after its ret, or be followed by an
uncatalogued function); if the true length differs, rebuild your oracle row (rows.tsv + chunk_pool/oracle_from_exe.py with your
agent name) and say so in NOTES.md. A row with no draft is authored from the disassembly: for a long generated function (script
Main, Register / Transfer table) build a machine-made draft from the call sites first (lessons f4001 / f4003). `hdr_STALE/` is an
OLD header overlay: never use its files as they are; re-apply only the opt-in blocks you need to today's header in your hdr/.
""" + (f"Your hdr/ starts as a copy of an earlier agent's overlay ({hdr}): never edit the body of an opt-in already in it.\n" if hdr else "") + "\n")
t = open(HERE + "/BRIEF_TEMPLATE.md").read().replace("@@ROWS@@\n", block).replace("@@A@@", A).replace("@@NAME@@", name).replace("@@S@@", S)
open(f"{A}/BRIEF.md", "w").write(t)
print("\n".join(lines)); print("wrote", f"{A}/BRIEF.md")

#!/usr/bin/env python3
"""Build the parked-row history index over every rebuild/candidates/manual/pending/*/differ/*.cpp.

    S=<lane scratch> python3 parked_index.py [OUT.tsv]   (default $S/w22/diag/INDEX.tsv;
    the committed copy is rebuild/candidates/manual/PARKED_INDEX.tsv)
"""
import os, re, glob, csv, subprocess, collections
import sys
S = os.environ["S"]; R = subprocess.check_output(["git", "rev-parse", "--show-toplevel"], text=True).strip()
OUT = sys.argv[1] if len(sys.argv) > 1 else f"{S}/w22/diag/INDEX.tsv"
KEEP_BATCHES = {"2026-10-24_debakeA", "2026-10-24_debakeB2", "2026-10-24_bounds_landed_differ"}

landed = {}
for r in csv.DictReader(open(f"{R}/rebuild/oracles/auto-re-candidates.tsv", encoding="utf-8-sig"), delimiter="\t"):
    landed[r["address"].lower().replace("0x", "")] = (r["name"], int(r["length"]))

bounds = {}
for r in csv.DictReader(open(f"{S}/bounds/AUDIT.tsv"), delimiter="\t"):
    bounds[r["address"].lower()] = (r["class"], r["row_len"], r["extent"], r["corrected_start"], r["corrected_len"])

retry = collections.defaultdict(list)
for name in ("retry11", "retry12", "retry10", "debakeA", "debakeB", "debakeB2"):
    p = f"{S}/w22/{name}/STATUS.tsv"
    if not os.path.exists(p): continue
    for r in csv.DictReader(open(p), delimiter="\t"):
        a = (r.get("addr") or r.get("address") or "").lower().replace("0x", "")
        if a: retry[a].append(f"{name}:{r.get('status','')}:{r.get('grade','')}")

skipped = {}
for r in csv.DictReader(open(f"{R}/rebuild/candidates/manual/SKIPPED_ROWS.tsv"), delimiter="\t"):
    skipped[r["address"].lower()] = f"{r['kind']}/{r['status']}: {r['note']}"
rebounds = {}
for r in csv.DictReader(open(f"{R}/rebuild/oracles/boundary-rebounds.tsv"), delimiter="\t"):
    rebounds[r["address"].lower()] = f"{r['old_length']}->{r['new_length']}"

findings = open(f"{R}/docs/journal/FINDINGS_LOG.md", encoding="utf-8", errors="replace").read().lower()

# git history over differ paths (one log call)
gl = subprocess.run(["git", "log", "--oneline", "--name-only", "--", "rebuild/candidates/manual/pending"],
                    cwd=R, capture_output=True, text=True).stdout
gitc = collections.defaultdict(set); cur = None
for line in gl.splitlines():
    if re.match(r"^[0-9a-f]{7,} ", line): cur = line.split()[0]; continue
    m = re.search(r"/differ/([0-9a-fA-F]{8})\.cpp$", line)
    if m and cur: gitc[m.group(1).lower()].add(cur)

rows = collections.defaultdict(list)
for f in sorted(glob.glob(f"{R}/rebuild/candidates/manual/pending/*/differ/*.cpp")):
    a = os.path.basename(f)[:8].lower(); batch = f.split("/")[-3]
    txt = open(f, encoding="utf-8", errors="replace").read(6000)
    lines = txt.splitlines()
    grade = ""; res = []
    for i, l in enumerate(lines[:40]):
        if "Grade:" in l:
            grade = l.split("Grade:", 1)[1].strip()
            res.append(grade)
            for l2 in lines[i+1:i+6]:
                if l2.startswith("//") and not re.match(r"//\s*(Leaf|Retail|Callees|Layout|Flags|Tried|Model|Source|Residue|Calls):", l2):
                    res.append(l2.lstrip("/ ").strip())
                else: break
            break
    m = re.search(r"(\d+)\s*v\s*(\d+)", grade)
    lens = f"{m.group(1)}v{m.group(2)}" if m else ""
    rows[a].append((batch, lens, " ".join(res)[:600].replace("\t", " "), f))

def classify(text):
    t = text.lower()
    rules = [
        ("ltcg/crt/asm-impossible", r"asm_bake|inline asm|inline-asm|fistp|fnstcw|fldcw|rdtsc|\bsse\b|ltcg|lane gap|__asm|cpuid|security_check|seh_prolog|movaps|retail_asm|crt "),
        ("incomplete-model", r"first-pass|first pass|stub only|not reconstructed|coarse|out of budget|one attempt|one grade|skeleton|not modelled|unmodelled|not yet"),
        ("bounds", r"catalog row (spans|stops|is|holds|cut)|row (is|stops|cut|ends)|jump table|next function|two functions|truncat|overlong|midstart|row extent|the row (includes|contains)"),
        ("eh/unwind", r"unwind|eh state|\beh\b|exception|funclet|__ehhandler|try/catch|catch block"),
        ("wrong-flags/pragma", r"/gs\b|/o1\b|/ot\b|/os\b|cookie"),
        ("x87-order", r"x87|fpu|\bfld|fmul|fadd|fsub|fdiv|fucom|fxch|fstp|reciprocal|fcom|st\(\d\)|\bfst\b"),
        ("register-tie", r"register|reg alloc|colour|swap|esi/edi|edi/esi|ebx/ebp|ebp/ebx|enregister|permutation"),
        ("stack-slot/frame", r"stack slot|frame|slot order|spill|home[sd]? |sub esp|\[esp\+|\[ebp-"),
        ("block/arm-layout", r"block|\barm\b|arms\b|layout|early return|tail|fall.?through|branch order|exit order|hoist|epilogue|after the ret"),
        ("load/schedule-order", r"schedul|load order|loads? .* before|reload|before the push|operand order|sib|order only|instruction order|ordering|store order|interleav"),
        ("inline-vs-call", r"inline|out-of-line|out of line|noinline|forceinline|not inlined|inlined|expanded|call(s|ed)? instead"),
        ("wrong-model/layout", r"struct|member|offset|vtable|model|type|size"),
    ]
    for cls, rx in rules:
        if re.search(rx, t): return cls
    return "unknown"

with open(OUT, "w") as o:
    w = csv.writer(o, delimiter="\t", lineterminator="\n")
    w.writerow(["addr", "status", "batches", "attempts", "last_lens", "bounds_class", "bounds_detail", "retries", "git_commits",
                "skipped", "rebound", "in_findings", "residue_class", "residue", "last_path", "diagnosis", "work_status"])
    for a in sorted(rows):
        rs = rows[a]
        batches = [b for b, _, _, _ in rs]
        status = "parked"
        if a in landed:
            status = "landed-special" if any(b in KEEP_BATCHES for b in batches) else "landed"
        last = rs[-1]
        lens = next((l for _, l, _, _ in reversed(rs) if l), "")
        bc = bounds.get(a, ("?", "", "", "", ""))
        residue = last[2]
        w.writerow([a, status, ";".join(batches), len(rs), lens, bc[0], f"row={bc[1]} ext={bc[2]} corr={bc[3]}:{bc[4]}",
                    ";".join(retry.get(a, [])), ",".join(sorted(gitc.get(a, []))[:6]), skipped.get(a, ""), rebounds.get(a, ""),
                    "y" if a in findings else "", classify(residue), residue, last[3], "", "todo"])
print("rows", len(rows), "landed", sum(1 for a in rows if a in landed))

#!/usr/bin/env python3
# prep_bwf.py -- build $S/w22/bwf{1..4}/<addr>/ row directories (main coordinator's author_rows layout) for every remaining
# acctB reserved row, from the earlier agent directories. Run with S exported, lane env sourced, cwd = repo.
import csv, glob, json, os, re, shutil, subprocess, sys
S = os.environ["S"]; ROOT = "/home/user/FableDecomp"; W22 = S + "/w22"
BASE = "6abff6d47"   # rebuild/include state the old overlays were copied from
sys.path.insert(0, S + "/coord")
import importlib.util
spec = importlib.util.spec_from_file_location("pd", S + "/coord/prep_drafts.py")
src = open(S + "/coord/prep_drafts.py").read()
# reuse the family() hint texts of prep_drafts.py without running its main part
ns = {"re": re}
exec(src.split("def sh(cmd):")[0].split("print(len(rows), \"rows\")")[1], ns)
family = ns["family"]

res = {r["address"].lower(): r for r in csv.DictReader(open(ROOT + "/rebuild/candidates/manual/pending/2026-11-11_acctB/RESERVED.tsv"), delimiter="\t")}
NS = [4008, 4009, 4010, 2056, 2057, 2060, 2061, 2062, 2063] + list(range(2064, 2100)) + list(range(4011, 4031))

def git_show(rev, path):
    p = subprocess.run(["git", "show", f"{rev}:{path}"], cwd=ROOT, capture_output=True)
    return p.stdout if p.returncode == 0 else None

def rebase_overlay(old, new, log):
    n_same = n_merge = n_conf = n_new = 0
    for f in glob.glob(old + "/**/*", recursive=True):
        if os.path.isdir(f): continue
        rel = os.path.relpath(f, old); dst = os.path.join(new, rel); os.makedirs(os.path.dirname(dst), exist_ok=True)
        if not rel.endswith((".h", ".inl", ".hpp")):
            continue
        base = git_show(BASE, "rebuild/include/" + rel); head_p = ROOT + "/rebuild/include/" + rel
        head = open(head_p, "rb").read() if os.path.exists(head_p) else None
        if base is None and head is None:
            shutil.copy(f, dst); n_new += 1; continue
        if base is None and head is not None:
            shutil.copy(f, dst); log.append(f"STALE {rel}: new in the old overlay, but the repo now has its own engine header of this name; this is the OLD overlay copy verbatim -- re-base onto the repo's header before the hand-back"); n_conf += 1; continue
        if head is None:
            log.append(f"CONFLICT {rel}: header no longer in the repo"); n_conf += 1; continue
        if base == head:
            shutil.copy(f, dst); n_same += 1; continue
        tb = dst + ".base"; th = dst + ".head"
        open(tb, "wb").write(base); open(th, "wb").write(head); shutil.copy(f, dst)
        p = subprocess.run(["git", "merge-file", dst, tb, th], capture_output=True)
        os.remove(tb); os.remove(th)
        if p.returncode == 0:
            n_merge += 1; log.append(f"merged {rel}: repo header changed since the overlay was made; three-way merge clean")
            continue
        shutil.copy(f, dst); n_conf += 1
        log.append(f"STALE {rel}: the repo header changed and the three-way merge conflicts; this is the OLD overlay copy verbatim (so the draft still compiles) -- re-base it by hand onto the current rebuild/include/{rel} (current text + your additions only) before the hand-back")
    return n_same, n_merge, n_conf, n_new

rows = []
for n in NS:
    A = f"{W22}/agent{n}"
    if not os.path.exists(A + "/dis_all.txt"): print("missing", n); continue
    first = open(A + "/dis_all.txt").readline()
    m = re.match(r"#### (\w{8}) .*len=(\d+)", first)
    a = m.group(1).lower(); ext = int(m.group(2)); r = res[a]
    # best draft
    draft = None; why = ""
    d1 = f"{A}/differ/{a}.cpp"
    if os.path.exists(d1): draft = d1; why = "differ/"
    else:
        best = (-1, None)
        gl = A + "/grades.log"
        if os.path.exists(gl):
            for l in open(gl, errors="replace"):
                mm = re.search(r"(?:eq|equal-instr)\s+(\d+)", l); f = re.search(r"\b([\w.\-]+\.cpp)\b", l)
                if mm and f:
                    for c in (f"{A}/cand/{f.group(1)}", f"{A}/{f.group(1)}", f"{A}/differ/{f.group(1)}"):
                        if os.path.exists(c) and int(mm.group(1)) > best[0]: best = (int(mm.group(1)), c)
        if best[1]: draft = best[1]; why = f"best of grades.log ({best[0]} equal instructions)"
        else:
            c = sorted(glob.glob(A + "/differ/*.cpp") + glob.glob(A + "/cand/*.cpp"), key=os.path.getmtime)
            if c: draft = c[-1]; why = "newest candidate"
    rows.append(dict(n=n, a=a, ext=ext, r=r, A=A, draft=draft, why=why))

with_draft = [x for x in rows if x["draft"]]; no_draft = sorted([x for x in rows if not x["draft"]], key=lambda x: x["ext"])
wfs = {"bwf1": with_draft, "bwf2": no_draft[0::3], "bwf3": no_draft[1::3], "bwf4": no_draft[2::3]}
for wf, lst in wfs.items():
    base = f"{W22}/{wf}"; os.makedirs(base, exist_ok=True); args = []; tsv = ["address\tmodule\tname\tlength\tsource"]
    for x in lst:
        a, A, r = x["a"], x["A"], x["r"]; W = f"{base}/{a}"
        for d in ("differ", "hdr", "prior"): os.makedirs(f"{W}/{d}", exist_ok=True)
        mod = r["catalog_module"]; nm = r["catalog_name"]
        if nm.startswith(mod + "::"): nm = nm[len(mod) + 2:]
        log = []; st = "no draft yet"
        if x["draft"]:
            shutil.copy(x["draft"], f"{W}/differ/{a}.cpp"); shutil.copy(x["draft"], f"{W}/draft_original.cpp")
            st = f"draft from {x['draft']} ({x['why']})"
        if os.path.isdir(A + "/hdr") and any(f.endswith(".h") for _, _, fs in os.walk(A + "/hdr") for f in fs):
            if not os.path.exists(W + "/hdr_prior"): shutil.copytree(A + "/hdr", W + "/hdr_prior")
            if x["draft"]:
                s_, m_, c_, n_ = rebase_overlay(A + "/hdr", W + "/hdr", log)
                log.insert(0, f"overlay re-based onto the current repo headers by the coordinator: {s_} unchanged-base copies, {m_} three-way merges, {n_} new headers, {c_} conflicts")
                open(W + "/hdr/README.md", "w").write("# hdr/ overlay (re-based by the coordinator from the earlier agent's overlay; the verbatim old overlay is ../hdr_prior, its README is ../hdr_prior/README.md)\n" + "\n".join("- " + l for l in log) + "\n")
        for f in ("NOTES.md", "LESSONS.md", "STATUS.tsv", "grades.log", "NAME_FIXES.tsv", "LABELS.tsv"):
            if os.path.exists(f"{A}/{f}"): shutil.copy(f"{A}/{f}", f"{W}/prior/{f}")
        if os.path.exists(f"{A}/test/{a}.cpp"): shutil.copy(f"{A}/test/{a}.cpp", f"{W}/prior/test.cpp")
        if os.path.isdir(A + "/scout"):
            os.makedirs(W + "/prior/scout", exist_ok=True)
            for f in glob.glob(A + "/scout/*"):
                if os.path.isfile(f) and os.path.getsize(f) < 600000 and f.endswith((".md", ".tsv", ".txt", ".cpp", ".sh", ".py")): shutil.copy(f, W + "/prior/scout/")
        if os.path.isdir(A + "/prior"):
            for d in glob.glob(A + "/prior/*"):
                t = W + "/prior/earlier_" + os.path.basename(d)
                if os.path.isdir(d) and not os.path.exists(t): shutil.copytree(d, t)
        shutil.copy(A + "/dis_all.txt", W + "/dis_all.txt")
        hint = family(mod, r["catalog_name"])
        open(W + "/HINTS.md", "w").write(
            f"# Coordinator's pointers for 0x{a.upper()} ({mod}::{nm}, {x['ext']} bytes)\n\n- State: {st}.\n"
            f"- Earlier work on this row (if any) is in $W/prior/: NOTES.md, grades.log (one line per earlier grade), scout/ (earlier Opus scouts' outline.md / headers.md / probes), "
            f"earlier_*/ (older chunks' notes), test.cpp (an earlier test draft). The earlier agent's full directory is {A} (read-only; its cand/ holds every graded candidate). "
            f"$W/dis_all.txt is the full retail listing of the function.\n"
            f"- $W/hdr_prior/ (if present) is the header overlay earlier agents used, copied from an OLDER repo state; $W/hdr/ holds the coordinator's re-base of it onto the current "
            f"repo headers (see hdr/README.md for conflicts to re-base by hand). Overlay rule: full copy of the CURRENT repo header plus `#ifdef FABLE_RETAIL_*` additions only.\n"
            f"- Family pointers: {hint}\n"
            f"- This lane's extra lessons and tools (read when a residue resists): /opt/fable_lane/w22/agent2050/CHUNK_BRIEF.md, section 'Lessons' (FableWin debug-size oracle "
            f"`python3 /opt/fable_lane/tools/fwsize.py <src.cpp> [name-regex]` with FW_EXTRA=/EHsc for rows: an inline helper whose /Od size differs from FableWin's has the wrong TEXT; "
            f"operand-order residues follow compiler record ids: name the intermediates the logic already has; call-v-expand decisions follow static frequency: look for one more branch level).\n"
            + ("".join(f"- {l}\n" for l in log)))
        open(W + "/rows.tsv", "w").write(tsv[0] + f"\n{a}\t{mod}\t{nm}\t{x['ext']}\t{x['draft'] or ''}\n")
        tsv.append(f"{a}\t{mod}\t{nm}\t{x['ext']}\t{x['draft'] or ''}")
        args.append({"addr": a, "module": mod, "name": nm, "length": x["ext"], "has_draft": bool(x["draft"]), "has_dossier_material": os.path.exists(W + "/prior/scout/outline.md")})
        print(wf, a, x["ext"], f"{mod}::{nm}"[:50], "| draft:", (x["why"] or "-"), "|", (log[0][60:] if log else ""))
    open(base + "/rows.tsv", "w").write("\n".join(tsv) + "\n")
    json.dump({"scratch": S, "wf": wf, "rows": args}, open(base + "/args.json", "w"))
    print(wf, len(args), "rows")

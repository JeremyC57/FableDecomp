#!/usr/bin/env python3
"""mkbatch430.py <tag> <lines.tsv> : table-only batches. lines.tsv = reloc_bindings lines (sym, va, witness) to DELETE.
Rows (the witnesses) are unchanged HEAD copies. Writes batches/<tag>{a,b,c}/ ."""
import sys, os, json, re
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF430"
R = "/home/user/FableDecomp/"
tag, lf = sys.argv[1], sys.argv[2]
lines = [l.rstrip("\n").split("\t") for l in open(lf) if l.strip()]
pay = json.load(open(A + "/work/p_c1/payload.json"))
byaddr = {x["address"]: x for x in pay["result"]["authored"]}
real = {l.split("\t")[0]: l.split("\t")[2] for l in open(A + "/vtpdb_cls.tsv")}
fw = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(A + "/vtpdb_cls.tsv")}
src2 = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(A + "/src2.tsv")}
ss = {}
for l in open(R + "rebuild/oracles/self_symbols.tsv"):
    f = l.rstrip("\n").split("\t"); ss[f[0]] = (f, l.rstrip("\n"))
lines.sort(key=lambda x: x[2])
chunks = [lines[i:i + 34] for i in range(0, len(lines), 34)]
for k, ch in enumerate(chunks):
    b = "%s%s" % (tag, "abcdefgh"[k]); B = A + "/batches/" + b
    for d in ("rows", "tests", "orig/rows", "orig/tests", "head"): os.makedirs(B + "/" + d, exist_ok=True)
    res = ["address\tverdict\told\tnew\treason"]; sel = ["address\tsymbol\tstatus\tleaf_given\tcurrent_line"]
    tc = ["action\tsymbol\ttarget\twitness\tnote"]; rows = []
    sub = {"result": {"authored": []}}
    for k2 in pay:
        if k2 != "result": sub[k2] = pay[k2]
    for sym, va, w in ch:
        x = byaddr[w]; rows.append(w); sub["result"]["authored"].append(x)
        for d in ("rows", "orig/rows"): open("%s/%s/%s.cpp" % (B, d, w), "w", encoding="utf-8").write(x["source_cpp"])
        for d in ("tests", "orig/tests"): open("%s/%s/%s_test.cpp" % (B, d, w), "w", encoding="utf-8").write(x["test_cpp"])
        f, cur = ss.get(w, (None, ""))
        self_sym = f[1] if f else x.get("leaf", "")
        status = f[2] if f else "RELOCATION_MATCH"
        res.append("%s\tACCEPT\t%s\t%s\ttable-only (NF430): delete the stale placeholder line `%s` -> %s (witness %s); the row is unchanged and its object emits %s at that relocation (HEAD census); class from two sources: FableWin.pdb %s with %s slots agreeing with retail 0x%s (unnamed shared base stubs neutral), and the vptr-storing row's PDB leaf (%s)"
                   % (w, self_sym, self_sym, sym, va, w, real[va], real[va], fw[va][3], va.upper(), src2[va][3].split(" | ")[0][:200]))
        sel.append("%s\t%s\t%s\t%s\t%s" % (w, self_sym, status, x.get("leaf", ""), cur))
        tc.append("delete\t%s\t%s\t%s\tstale placeholder (NF430): every landed binder emits %s (HEAD census)" % (sym, va, w, real[va]))
    open(B + "/RESULTS.tsv", "w").write("\n".join(res) + "\n")
    open(B + "/SELF_SYMBOLS.tsv", "w").write("\n".join(sel) + "\n")
    open(B + "/TABLE_CHANGES.tsv", "w").write("\n".join(tc) + "\n")
    open(B + "/HEADERS.tsv", "w").write("address\tsymbol\tstatus\tleaf_given\n")
    open(B + "/UNITS.tsv", "w").write("address\tunit\tkind\n")
    open(B + "/RENAMES.tsv", "w").write("address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence\told_source\tnew_source\tnew_test_source\n")
    open(B + "/CATALOG_CHANGES.tsv", "w").write("address\tfield\told\tnew\tnote\n")
    open(B + "/rows.txt", "w").write("\n".join(rows) + "\n")
    json.dump(sub, open(B + "/head/payload.json", "w"))
    print(b, len(ch))

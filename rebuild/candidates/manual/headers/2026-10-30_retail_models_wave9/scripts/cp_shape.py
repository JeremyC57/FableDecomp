#!/usr/bin/env python3
"""cp_shape.py <dry.txt>: feature-tag the CCountedPointer block of every `edit` row: min (data only), dtor-inline, dtor-ool
(declared only), dtor-force, dtor-noinline, cc (copy ctor), op= (inline/ool), share, tctor (T* ctor), force (any
__forceinline). Prints tag counts and writes work/cp_tags.tsv."""
import sys, re, os, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]
rows = [l.split()[1] for l in open(sys.argv[1]) if l.startswith("edit ")]
cnt = collections.Counter(); out = []
for f in rows:
    t = open(os.path.join(R, f), encoding="utf-8").read()
    b = find_block(t, "CCountedPointer"); blk = norm(t[b[0]:b[1]])
    tags = []
    dt = re.search(r"((?:__forceinline|__declspec\(noinline\)|inline)\s+)?~CCountedPointer\s*\(\s*\)\s*(;|\{)", blk)
    if dt:
        k = "dtor-ool" if dt.group(2) == ";" else "dtor"
        if dt.group(1) and "force" in dt.group(1): k += "-force"
        if dt.group(1) and "noinline" in dt.group(1): k += "-noinline"
        tags.append(k)
    if re.search(r"CCountedPointer\s*\(\s*const\s+CCountedPointer", blk): tags.append("cc" + ("-ool" if re.search(r"CCountedPointer\s*\(\s*const\s+CCountedPointer[^)]*\)\s*;", blk) else ""))
    if re.search(r"operator\s*=", blk): tags.append("op=" + ("-ool" if re.search(r"operator\s*=\s*\([^)]*\)\s*;", blk) else ""))
    if "ShareData" in blk: tags.append("share")
    if re.search(r"CCountedPointer\s*\(\s*(const\s+)?\w+\s*\*", blk): tags.append("tctor")
    if "__forceinline" in blk: tags.append("force")
    if "noinline" in blk: tags.append("noinline")
    if not tags: tags = ["min" if not re.search(r"\(", blk) else "other"]
    tg = " ".join(tags); cnt[tg] += 1; out.append((re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1), f, tg))
open(S + "/mig9/work/cp_tags.tsv", "w").write("".join("\t".join(x) + "\n" for x in out))
for k, v in cnt.most_common(): print(v, k)

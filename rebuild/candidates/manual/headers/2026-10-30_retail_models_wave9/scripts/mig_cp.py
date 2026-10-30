#!/usr/bin/env python3
"""mig_cp.py [--dry] [--keep-pi] <file>...: replace a row-local global-scope `template <class T> class CCountedPointer {..};`
(and its local CCPPointerInfo block) with #include "engine/CCountedPointer.h" (wave 9). Skips (with a reason) every shape
the shared header cannot stand in for: non-template / namespace-local / several blocks / out-of-class members / member
names the header lacks that the row uses / CCPPointerInfo fields or ctor the header spells differently."""
import sys, re, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
from cputil import strip_comments, depth_at, pp_depth, items, member_name
INC = '#include "engine/CCountedPointer.h"'
CP_OK = {"CCountedPointer", "~CCountedPointer", "operator=", "operator->", "operator*", "ReleaseData", "ShareData", "Reset",
         "IsNull", "Get", "get", "reset", "DeleteData", "GetReferenceCount", "SetStaticCast"}
def fptr_name(h):
    m = re.search(r"\(\s*(?:__\w+\s*)?\*\s*(\w+)\s*\)\s*\(", h)
    return m.group(1) if m else None
def analyse(t):
    c = strip_comments(t)
    if INC in t: return None, "already"
    if re.search(r"_{2}asm\b|Retail-asm|retail_asm|asm_bake", t): return None, "asm row (not ours)"
    blocks = []; pos = 0
    while True:
        b = find_block(c, "CCountedPointer", pos)
        if not b: break
        blocks.append(b); pos = b[1]
    if not blocks: return None, "no block"
    if len(blocks) > 1: return None, "several blocks"
    b0, b1 = blocks[0]
    head = c[b0:c.find("{", b0)]
    if not re.search(r"template\s*<\s*(class|typename)\s+\w+\s*>", head): return None, "non-template"
    d, st = depth_at(c, b0)
    if d != 0: return None, "nested (" + (st[-1][:40] if st else "?") + ")"
    if pp_depth(c, b0) != 0: return None, "inside #if"
    tp = re.search(r"template\s*<\s*(?:class|typename)\s+(\w+)\s*>", head).group(1)
    body = c[c.find("{", b0) + 1:c.rfind("}", b0, b1)]
    data = []; funcs = []
    for kind, h in items(body):
        if kind == "nested": return None, "nested type in block"
        name, isf = member_name(h)
        if re.match(r"\s*(typedef|enum|friend|using)\b", h): return None, "typedef/friend in block"
        if isf: funcs.append((name, h))
        else:
            if re.search(r"\bstatic\b", h): return None, "static data"
            data.append((name, re.sub(r"\s+", " ", h).strip()))
    dn = [n for n, _ in data]
    if dn != ["Data", "Info"]: return None, "data members " + ",".join(dn)
    if not re.fullmatch(r"(const\s+)?%s\s*\*\s*Data" % tp, data[0][1]): return None, "Data type " + data[0][1]
    if not re.fullmatch(r"(CCPPointerInfo|void|struct CCPPointerInfo|class CCPPointerInfo)\s*\*\s*Info", data[1][1]): return None, "Info type " + data[1][1]
    rest = c[:b0] + c[b1:]
    if re.search(r"\bCCountedPointer\s*<\s*(const\s+)?void\s*>", rest): return None, "CCountedPointer<void>"
    if re.search(r"\bCCountedPointer\s*<[^<>;{}()]*(?:<[^<>;{}()]*(?:<[^<>;{}()]*>[^<>;{}()]*)*>[^<>;{}()]*)*>\s*::\s*~?\w", rest): return None, "out-of-class member"
    if re.search(r"template\s*<\s*>\s*(class|struct)\s+CCountedPointer\b", rest): return None, "specialisation"
    for n, h in funcs:
        if n in CP_OK:
            if n == "ShareData" and not re.search(r"ShareData\s*\(\s*(const\s+)?%s\s*\*\s*\w*\s*,\s*(CCPPointerInfo|void)\s*\*" % tp, h):
                return None, "ShareData signature"
            if n == "Reset" and not re.search(r"Reset\s*\(\s*(const\s+)?%s\s*\*\s*\w*\s*\)" % tp, h): return None, "Reset signature"
            if n == "operator=" and not re.search(r"operator\s*=\s*\(\s*const\s+CCountedPointer\b", h): return None, "operator= signature"
            if n == "CCountedPointer":
                a = re.search(r"CCountedPointer\s*\(([^)]*)\)", h).group(1).strip()
                if a not in ("", "void") and not re.fullmatch(r"(const\s+)?CCountedPointer\s*(<\s*%s\s*>|<\s*\w+\s*>)?\s*&\s*\w*" % tp, a) \
                   and not re.fullmatch(r"(const\s+)?%s\s*\*\s*\w*" % tp, a): return None, "ctor signature (%s)" % a[:40]
            continue
        if n.startswith("operator"): return None, "invented operator " + n
        if re.search(r"\b%s\b" % re.escape(n.lstrip("~")), rest): return None, "used member " + n
    # the CCPPointerInfo block
    pi = find_block(c, "CCPPointerInfo")
    pinfo = None
    if pi:
        if find_block(c, "CCPPointerInfo", pi[1]): return None, "several CCPPointerInfo"
        if depth_at(c, pi[0])[0] != 0 or pp_depth(c, pi[0]) != 0: return None, "CCPPointerInfo nested"
        pb = c[c.find("{", pi[0]) + 1:c.rfind("}", pi[0], pi[1])]
        fl = []
        for kind, h in items(pb):
            name, isf = member_name(h)
            if name == "CCPPointerInfo": continue
            fp = fptr_name(h)
            if fp: fl.append(fp); continue
            if isf:
                if name != "CCPPointerInfo": return None, "CCPPointerInfo member " + name
                continue
            fl.append(name)
        if fl not in (["RefCount", "DeleteFunc", "Data"], ["RefCount"], ["RefCount", "DeleteFunc"]): return None, "CCPPointerInfo fields " + ",".join(fl)
        rest2 = c[:min(b0, pi[0])] + c[min(b1, pi[1]):max(b0, pi[0])] + c[max(b1, pi[1]):]
        if re.search(r"\bCCPPointerInfo\s*\(", rest2): return None, "CCPPointerInfo ctor used"
        if re.search(r"\bCCPPointerInfo\s*::", rest2): return None, "CCPPointerInfo member def"
        pinfo = pi
    else:
        # a forward-declared-only CCPPointerInfo with a struct key would clash in mangling; record
        if re.search(r"\bstruct\s+CCPPointerInfo\b", c): return None, "struct CCPPointerInfo key"
    if pinfo and re.match(r"\s*struct\b", c[pinfo[0]:pinfo[1]]):
        pass  # the definition goes away with the block; the header's class key is the PDB's (V)
    if re.search(r"\bstruct\s+CCPPointerInfo\s*;", c): return None, "struct CCPPointerInfo forward"
    return (blocks[0], pinfo), "ok"
def apply(t, res):
    (b0, b1), pi = res
    spans = sorted([(b0, b1)] + ([pi] if pi else []))
    out = t
    for s, e in reversed(spans):
        first = (s, e) == spans[0]
        rep = INC + "\n" if first else ""
        # keep the line structure: a block that started mid-line keeps its newline
        if first and s > 0 and out[s - 1] != "\n": rep = "\n" + rep
        out = out[:s] + rep + out[e:]
    return out
if __name__ == "__main__":
    dry = "--dry" in sys.argv; why = collections.Counter()
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        res, w = analyse(t); why[w.split(" (")[0] if not w.startswith("data members") else w] += 1
        if res is None: print("skip", f, w); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(apply(t, res))
        print("edit", f, w)
    for k, v in why.most_common(): print("#", v, k, file=sys.stderr)
